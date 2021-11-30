#include "HandEditSkinnedMeshComponent.h"
#include "HandEditSkinnedMeshSceneProxy.h"

FBoxSphereBounds UHandEditSkinnedMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	float MinValue = Width * -0.5f;
	float MaxValue = Width * 0.5f;
	FBox BoundingBox(FVector(MinValue, MinValue, 0.0f), FVector(MaxValue, MaxValue, 0.0f));
	BoundingBox = BoundingBox.TransformBy(LocalToWorld);

	return FBoxSphereBounds(BoundingBox);
}

int32 UHandEditSkinnedMeshComponent::GetNumMaterials() const
{
	return 1;
}

FPrimitiveSceneProxy* UHandEditSkinnedMeshComponent::CreateSceneProxy()
{
	return new FHandEditSkinnedMeshSceneProxy(this);
}
