#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequenceBase.h"
#include "DistanceMatchingAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DISTANCEMATCHING_API UDistanceMatchingAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	//
	// UAnimInstance Interfaces
	//
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//
	// Distance Matching
	//
public:
	UFUNCTION(BlueprintCallable)
	float GetTimeKey(UAnimSequenceBase* AnimSequence, FName CurveName, float Value) const;

	UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction, DeprecationMessage = "Not optimized as an intermediate curve resource created inside"))
	float GetTimeKeyFromCurve(UAnimSequenceBase* AnimSequence, FName CurveName, float Value) const;

	UFUNCTION(BlueprintCallable)
	bool PredictStopLocation(FVector& StopLocation) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsAccelerating;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsMoving;

	UPROPERTY()
	bool bWasAccelerating;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsDistanceMatching;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector StopLocation;

	UPROPERTY(BlueprintReadOnly)
	float DistanceMatchingStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeKeyAtDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 0))
	float JoggingRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase* JogStartAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase* JogStopAnimation;
};
