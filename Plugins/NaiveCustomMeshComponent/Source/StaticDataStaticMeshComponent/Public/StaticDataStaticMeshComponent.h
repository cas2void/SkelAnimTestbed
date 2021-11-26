#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "StaticDataStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class STATICDATASTATICMESHCOMPONENT_API UStaticDataStaticMeshComponent : public UMeshComponent
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
	// Enable material slots in detail panel
	virtual int32 GetNumMaterials() const override;

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

public:
	// Quad width
	UPROPERTY(EditAnywhere)
	float Width = 100.0f;
};
