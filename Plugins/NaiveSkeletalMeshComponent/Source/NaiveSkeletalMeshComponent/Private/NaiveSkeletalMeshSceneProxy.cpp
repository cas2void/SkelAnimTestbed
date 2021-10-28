#include "NaiveSkeletalMeshSceneProxy.h"

#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"

FNaiveSkeletalMeshSceneProxy::FNaiveSkeletalMeshSceneProxy(const UNaiveSkinnedMeshComponent* Component)
    : FPrimitiveSceneProxy(Component)
    , VertexFactory(GetScene().GetFeatureLevel(), "FNaiveSkeletalMeshSceneProxy")
{
    //StaticVertexBuffers.StaticMeshVertexBuffer.Init(3, 3);

    TArray<FVector> Positions{ FVector(0.0f, 0.0f, 100.0f), FVector(100.0f, 0.0f, 100.0f), FVector(0.0f, 100.0f, 100.0f) };
    StaticVertexBuffers.PositionVertexBuffer.Init(Positions);

    TArray<FColor> Colors{ FColor::White, FColor::White, FColor::White };
    StaticVertexBuffers.ColorVertexBuffer.InitFromColorArray(Colors);

    TArray<FStaticMeshBuildVertex> Vertices;
    Vertices.AddDefaulted(3);
    const FVector Edge01 = (Positions[1] - Positions[0]);
    const FVector Edge02 = (Positions[2] - Positions[0]);

    const FVector TangentX = Edge01.GetSafeNormal();
    const FVector TangentZ = (Edge02 ^ Edge01).GetSafeNormal();
    const FVector TangentY = (TangentX ^ TangentZ).GetSafeNormal();

    Vertices[0].TangentX = TangentX;
    Vertices[0].TangentY = TangentY;
    Vertices[0].TangentZ = TangentZ;

    Vertices[1].TangentX = TangentX;
    Vertices[1].TangentY = TangentY;
    Vertices[1].TangentZ = TangentZ;

    Vertices[2].TangentX = TangentX;
    Vertices[2].TangentY = TangentY;
    Vertices[2].TangentZ = TangentZ;

    Vertices[0].UVs[0] = FVector2D(0.0f, 0.0f);
    Vertices[1].UVs[0] = FVector2D(1.0f, 0.0f);
    Vertices[2].UVs[0] = FVector2D(0.0f, 1.0f);

    StaticVertexBuffers.StaticMeshVertexBuffer.Init(Vertices, 1);

    TArray<uint32> Indices{ 0, 1, 2 };
    IndexBuffer.AppendIndices(Indices.GetData(), Indices.Num());

    ENQUEUE_RENDER_COMMAND(NaiveSkeletalMeshVertexFactoryInit)(
        [this](FRHICommandListImmediate& RHICmdList)
        {
            StaticVertexBuffers.StaticMeshVertexBuffer.InitResource();
            StaticVertexBuffers.PositionVertexBuffer.InitResource();
            StaticVertexBuffers.ColorVertexBuffer.InitResource();
            IndexBuffer.InitResource();

            FLocalVertexFactory::FDataType Data;
            StaticVertexBuffers.PositionVertexBuffer.BindPositionVertexBuffer(&VertexFactory, Data);
            StaticVertexBuffers.StaticMeshVertexBuffer.BindTangentVertexBuffer(&VertexFactory, Data);
            StaticVertexBuffers.StaticMeshVertexBuffer.BindPackedTexCoordVertexBuffer(&VertexFactory, Data);
            StaticVertexBuffers.ColorVertexBuffer.BindColorVertexBuffer(&VertexFactory, Data);
            VertexFactory.SetData(Data);

            VertexFactory.InitResource();
        });

    Material = Component->GetMaterial(0);
    if (!Material)
    {
        Material = UMaterial::GetDefaultMaterial(MD_Surface);
    }
}

FNaiveSkeletalMeshSceneProxy::~FNaiveSkeletalMeshSceneProxy()
{
    StaticVertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
    StaticVertexBuffers.PositionVertexBuffer.ReleaseResource();
    StaticVertexBuffers.ColorVertexBuffer.ReleaseResource();
    IndexBuffer.ReleaseResource();
    VertexFactory.ReleaseResource();
}

void FNaiveSkeletalMeshSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
    for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
    {
        if (VisibilityMap & (1 << ViewIndex))
        {
            FMeshBatch& Mesh = Collector.AllocateMesh();
            Mesh.MaterialRenderProxy = Material->GetRenderProxy();
            Mesh.VertexFactory = &VertexFactory;
            Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
            Mesh.Type = PT_TriangleList;
            Mesh.DepthPriorityGroup = SDPG_World;
            Mesh.bCanApplyViewModeOverrides = true;
            //Mesh.bWireframe = 

            FMeshBatchElement& BatchElement = Mesh.Elements[0];
            BatchElement.IndexBuffer = &IndexBuffer;

            bool bHasPrecomputedVolumetricLightmap;
            FMatrix PreviousLocalToWorld;
            int32 SingleCaptureIndex;
            bool bOutputVelocity;
            GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld, SingleCaptureIndex, bOutputVelocity);

            FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
            DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap, DrawsVelocity(), false);
            BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;

            BatchElement.FirstIndex = 0;
            BatchElement.NumPrimitives = IndexBuffer.GetNumIndices() / 3;
            BatchElement.MinVertexIndex = 0;
            BatchElement.MaxVertexIndex = StaticVertexBuffers.PositionVertexBuffer.GetNumVertices() - 1;

            Collector.AddMesh(ViewIndex, Mesh);
        }
    }
}

FPrimitiveViewRelevance FNaiveSkeletalMeshSceneProxy::GetViewRelevance(const FSceneView* View) const
{
    FPrimitiveViewRelevance Result;
    Result.bDynamicRelevance = true;
    Result.bDrawRelevance = IsShown(View);
    Result.bShadowRelevance = IsShadowCast(View);
    return Result;
}
