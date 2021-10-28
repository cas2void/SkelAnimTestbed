#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "NaiveSkinnedMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class NAIVESKELETALMESHCOMPONENT_API UNaiveSkinnedMeshComponent : public UMeshComponent
{
	GENERATED_BODY()
	
	//
	// USceneComponent Interfaces
	//
public:
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	//
	// UPrimitiveComponent Intefaces
	//
public:
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
};
