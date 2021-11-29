#include "StaticDataDynamicMeshSceneProxy.h"

#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "..\Public\StaticDataDynamicMeshSceneProxy.h"

FStaticDataDynamicMeshSceneProxy::FStaticDataDynamicMeshSceneProxy(const UStaticDataDynamicMeshComponent* Component)
    : FPrimitiveSceneProxy(Component)
    , VertexFactory(GetScene().GetFeatureLevel(), "FStaticDataDynamicMeshSceneProxy")
{
    // Generate verices of a quad
    //float MinValue = Component->Width * -0.5f;
    //float MaxValue = Component->Width * 0.5f;
    //TArray<FVector> Positions{ 
    //    FVector(MaxValue, MinValue, 0.0f),
    //    FVector(MinValue, MinValue, 0.0f),
    //    FVector(MinValue, MaxValue, 0.0f),
    //    FVector(MaxValue, MaxValue, 0.0f)
    //};
    TArray<FVector> Positions;
    Positions.AddDefaulted(4);
    StaticVertexBuffers.PositionVertexBuffer.Init(Positions);

    TArray<FStaticMeshBuildVertex> Vertices;
    Vertices.AddDefaulted(4);
    Vertices[0].TangentZ = FVector::UpVector;
    Vertices[1].TangentZ = FVector::UpVector;
    Vertices[2].TangentZ = FVector::UpVector;
    Vertices[3].TangentZ = FVector::UpVector;
    Vertices[0].UVs[0] = FVector2D(0.0f, 0.0f);
    Vertices[1].UVs[0] = FVector2D(0.0f, 1.0f);
    Vertices[2].UVs[0] = FVector2D(1.0f, 1.0f);
    Vertices[3].UVs[0] = FVector2D(1.0f, 0.0f);
    StaticVertexBuffers.StaticMeshVertexBuffer.Init(Vertices, 1);

    TArray<FColor> Colors{ FColor::Red, FColor::Green, FColor::White, FColor::Blue };
    StaticVertexBuffers.ColorVertexBuffer.InitFromColorArray(Colors);

    TArray<uint32> Indices{ 
        0, 1, 2,
        2, 3, 0
    };
    IndexBuffer.AppendIndices(Indices.GetData(), Indices.Num());

    // Initialize render resources and bind data
    ENQUEUE_RENDER_COMMAND(NaiveSkeletalMeshVertexFactoryInit)(
        [this](FRHICommandListImmediate& RHICmdList)
        {
            StaticVertexBuffers.StaticMeshVertexBuffer.InitResource();
            StaticVertexBuffers.PositionVertexBuffer.InitResource();
            StaticVertexBuffers.ColorVertexBuffer.InitResource();
            IndexBuffer.InitResource();

            FLocalVertexFactory::FDataType Data;
            StaticVertexBuffers.StaticMeshVertexBuffer.BindTangentVertexBuffer(&VertexFactory, Data);
            StaticVertexBuffers.StaticMeshVertexBuffer.BindPackedTexCoordVertexBuffer(&VertexFactory, Data);
            StaticVertexBuffers.PositionVertexBuffer.BindPositionVertexBuffer(&VertexFactory, Data);
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

FStaticDataDynamicMeshSceneProxy::~FStaticDataDynamicMeshSceneProxy()
{
    StaticVertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
    StaticVertexBuffers.PositionVertexBuffer.ReleaseResource();
    StaticVertexBuffers.ColorVertexBuffer.ReleaseResource();
    IndexBuffer.ReleaseResource();
    VertexFactory.ReleaseResource();
}

void FStaticDataDynamicMeshSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
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

            FMeshBatchElement& BatchElement = Mesh.Elements[0];
            BatchElement.IndexBuffer = &IndexBuffer;

            // Set uniform buffer
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

FPrimitiveViewRelevance FStaticDataDynamicMeshSceneProxy::GetViewRelevance(const FSceneView* View) const
{
    FPrimitiveViewRelevance Result;
    Result.bDynamicRelevance = true;
    Result.bDrawRelevance = IsShown(View);
    Result.bShadowRelevance = IsShadowCast(View);
    return Result;
}

void FStaticDataDynamicMeshSceneProxy::SetData_RenderThread(const TArray<FVector>& Points)
{
    check(IsInRenderingThread());

    check(Points.Num() == 4);

    auto& VertexBuffer = StaticVertexBuffers.PositionVertexBuffer;
    void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
    FMemory::Memcpy(VertexBufferData, Points.GetData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
    RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
}
