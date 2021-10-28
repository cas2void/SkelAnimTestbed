#include "NaiveSkinnedMeshComponent.h"
#include "NaiveSkeletalMeshSceneProxy.h"

FBoxSphereBounds UNaiveSkinnedMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
    TArray<FVector> Positions{ FVector(0.0f, 0.0f, 100.0f), FVector(100.0f, 0.0f, 100.0f), FVector(0.0f, 100.0f, 100.0f) };

	FBox BoundingBox(ForceInit);

	// Bounds are tighter if the box is generated from pre-transformed vertices.
	for (int32 Index = 0; Index < Positions.Num(); ++Index)
	{
		BoundingBox += LocalToWorld.TransformPosition(Positions[Index]);
		BoundingBox += LocalToWorld.TransformPosition(Positions[Index]);
		BoundingBox += LocalToWorld.TransformPosition(Positions[Index]);
	}

	FBoxSphereBounds NewBounds;
	NewBounds.BoxExtent = BoundingBox.GetExtent();
	NewBounds.Origin = BoundingBox.GetCenter();
	NewBounds.SphereRadius = NewBounds.BoxExtent.Size();

	return NewBounds;
}

FPrimitiveSceneProxy* UNaiveSkinnedMeshComponent::CreateSceneProxy()
{
	return new FNaiveSkeletalMeshSceneProxy(this);
}
