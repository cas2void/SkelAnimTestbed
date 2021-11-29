#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "StaticDataDynamicMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class STATICDATADYNAMICMESHCOMPONENT_API UStaticDataDynamicMeshComponent : public UMeshComponent
{
	GENERATED_BODY()

public:
	UStaticDataDynamicMeshComponent();

	//
	// UActorComponent Interfaces
	//
public:
	virtual void SendRenderDynamicData_Concurrent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
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

	UPROPERTY(Transient)
	TArray<FVector> QuadPoints;
};
