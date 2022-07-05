#include "InverseDistanceCurveAnimModifier.h"

#include "AnimationBlueprintLibrary.h"

static FName DistanceCurveName(TEXT("DistanceCurve"));
static FName InverseDistanceCurveName(TEXT("InverseDistanceCurve"));

void UInverseDistanceCurveAnimModifier::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
    if (AnimationSequence)
    {
        if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, DistanceCurveName, ERawCurveTrackTypes::RCT_Float))
        {
            TArray<float> OutTimes;
            TArray<float> OutValues;
            UAnimationBlueprintLibrary::GetFloatKeys(AnimationSequence, DistanceCurveName, OutTimes, OutValues);

            if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, InverseDistanceCurveName, ERawCurveTrackTypes::RCT_Float))
            {
                UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, InverseDistanceCurveName);
            }
            UAnimationBlueprintLibrary::AddCurve(AnimationSequence, InverseDistanceCurveName, ERawCurveTrackTypes::RCT_Float);
            UAnimationBlueprintLibrary::AddFloatCurveKeys(AnimationSequence, InverseDistanceCurveName, OutValues, OutTimes);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Animation sequence %s does not have curve %s"), *AnimationSequence->GetName(), *DistanceCurveName.ToString());
        }
    }
}

void UInverseDistanceCurveAnimModifier::OnRevert_Implementation(UAnimSequence* AnimationSequence)
{
    if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, InverseDistanceCurveName, ERawCurveTrackTypes::RCT_Float))
    {
        UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, InverseDistanceCurveName);
    }
}