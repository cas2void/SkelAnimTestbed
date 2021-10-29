#include "NaiveSkinnedMeshComponent.h"
#include "NaiveSkeletalMeshSceneProxy.h"

FBoxSphereBounds UNaiveSkinnedMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	float MinValue = Width * -0.5f;
	float MaxValue = Width * 0.5f;
	FBox BoundingBox(FVector(MinValue, MinValue, 0.0f), FVector(MaxValue, MaxValue, 0.0f));
	BoundingBox = BoundingBox.TransformBy(LocalToWorld);

	return FBoxSphereBounds(BoundingBox);
}

int32 UNaiveSkinnedMeshComponent::GetNumMaterials() const
{
	return 1;
}

FPrimitiveSceneProxy* UNaiveSkinnedMeshComponent::CreateSceneProxy()
{
	return new FNaiveSkeletalMeshSceneProxy(this);
}
