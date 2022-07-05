#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "InverseDistanceCurveAnimModifier.generated.h"

/**
 * 
 */
UCLASS()
class DISTANCEMATCHINGEDITOR_API UInverseDistanceCurveAnimModifier : public UAnimationModifier
{
	GENERATED_BODY()

public:
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;

	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;
};
