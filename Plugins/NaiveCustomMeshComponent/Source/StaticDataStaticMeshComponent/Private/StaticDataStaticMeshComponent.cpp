#include "StaticDataStaticMeshComponent.h"
#include "StaticDataStaticMeshSceneProxy.h"

FBoxSphereBounds UStaticDataStaticMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	float MinValue = Width * -0.5f;
	float MaxValue = Width * 0.5f;
	FBox BoundingBox(FVector(MinValue, MinValue, 0.0f), FVector(MaxValue, MaxValue, 0.0f));
	BoundingBox = BoundingBox.TransformBy(LocalToWorld);

	return FBoxSphereBounds(BoundingBox);
}

int32 UStaticDataStaticMeshComponent::GetNumMaterials() const
{
	return 1;
}

FPrimitiveSceneProxy* UStaticDataStaticMeshComponent::CreateSceneProxy()
{
	return new FStaticDataStaticMeshSceneProxy(this);
}
