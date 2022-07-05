// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DistanceMatchingHUD.generated.h"

/**
 * 
 */
UCLASS()
class DISTANCEMATCHING_API ADistanceMatchingHUD : public AHUD
{
	GENERATED_BODY()

	//
	// AHUD Interfaces
	//
public:
	virtual void ShowDebugInfo(float& YL, float& YPos) override;

	virtual void DrawHUD() override;

	//
	// Panel
	//
public:
	UFUNCTION(exec)
	void ShowMovementPanel();

	UFUNCTION(BlueprintCallable)
	void SetMovementPanelVisibility(bool bVisible) { bShowMovementPanel = bVisible; }

protected:
	void DrawRadialPanel(const FVector& VectorValue, float MaxMag, const FVector2D Center, float Radius, const FString& Caption,
		const FVector& North = FVector::ForwardVector, const FVector& East = FVector::RightVector);

	UPROPERTY()
	bool bShowMovementPanel;

	UPROPERTY(EditAnywhere)
	float PanelRadius = 80.0f;
};
