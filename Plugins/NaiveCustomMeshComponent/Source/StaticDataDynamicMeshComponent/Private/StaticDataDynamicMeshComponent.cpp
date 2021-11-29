#include "StaticDataDynamicMeshComponent.h"
#include "StaticDataDynamicMeshSceneProxy.h"

UStaticDataDynamicMeshComponent::UStaticDataDynamicMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;

	QuadPoints.Init(FVector::ZeroVector, 4);
}

void UStaticDataDynamicMeshComponent::SendRenderDynamicData_Concurrent()
{
	FStaticDataDynamicMeshSceneProxy* MeshSceneProxy = StaticCast<FStaticDataDynamicMeshSceneProxy*>(SceneProxy);
	if (MeshSceneProxy)
	{
		ENQUEUE_RENDER_COMMAND(FSendData)(
			[MeshSceneProxy, this](FRHICommandListImmediate& RHICmdList)
			{
				MeshSceneProxy->SetData_RenderThread(QuadPoints);
			});
	}
}

void UStaticDataDynamicMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float ScaleValue = FMath::Abs(FMath::Sin(GetOwner()->GetGameTimeSinceCreation()));
	float MinValue = Width * -0.5f * ScaleValue;
	float MaxValue = Width * 0.5f * ScaleValue;

	QuadPoints[0].Set(MaxValue, MinValue, 0.0f);
	QuadPoints[1].Set(MinValue, MinValue, 0.0f);
	QuadPoints[2].Set(MinValue, MaxValue, 0.0f);
	QuadPoints[3].Set(MaxValue, MaxValue, 0.0f);

	UpdateBounds();
	
	MarkRenderDynamicDataDirty();
}

FBoxSphereBounds UStaticDataDynamicMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	//float MinValue = Width * -0.5f;
	//float MaxValue = Width * 0.5f;
	//FBox BoundingBox(FVector(MinValue, MinValue, 0.0f), FVector(MaxValue, MaxValue, 0.0f));
	FBox BoundingBox(QuadPoints);
	BoundingBox = BoundingBox.TransformBy(LocalToWorld);

	return FBoxSphereBounds(BoundingBox);
}

int32 UStaticDataDynamicMeshComponent::GetNumMaterials() const
{
	return 1;
}

FPrimitiveSceneProxy* UStaticDataDynamicMeshComponent::CreateSceneProxy()
{
	return new FStaticDataDynamicMeshSceneProxy(this);
}
