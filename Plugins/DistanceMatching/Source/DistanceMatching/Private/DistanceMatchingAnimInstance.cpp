#include "DistanceMatchingAnimInstance.h"

#include "Animation/Skeleton.h"
#include "Stats/Stats2.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

DECLARE_STATS_GROUP(TEXT("Curve"), STATGROUP_Curve, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Recreate Curve"), STAT_Recreate_Curve, STATGROUP_Curve);
DECLARE_CYCLE_STAT(TEXT("Evaluate Curve"), STAT_Evaluate_Curve, STATGROUP_Curve);

static FName InverseDistanceCurveName(TEXT("InverseDistanceCurve"));

void UDistanceMatchingAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    bWasAccelerating = bIsAccelerating;

    ACharacter* CharacterOwner = Cast<ACharacter>(TryGetPawnOwner());
    if (CharacterOwner)
    {
        bIsMoving = CharacterOwner->GetVelocity().SizeSquared() > 0.0f;

        UCharacterMovementComponent* MovementComponent = CharacterOwner->GetCharacterMovement();
        if (MovementComponent)
        {
            bIsAccelerating = MovementComponent->GetCurrentAcceleration().SizeSquared() > 0.0f;
        }

        if (bIsAccelerating)
        {
            JoggingRate = CharacterOwner->GetVelocity().Size() / 360.0f;
        }

        if (bWasAccelerating != bIsAccelerating)
        {
            if (bIsAccelerating)
            {
                StartLocation = CharacterOwner->GetActorLocation();
            }
            else
            {
                FVector TempLocation;
                if (PredictStopLocation(TempLocation))
                {
                    StopLocation = TempLocation;
                }
            }

            bIsDistanceMatching = true;
            DistanceMatchingStartTime = GetWorld()->GetTimeSeconds();

            // Debug
            {
                if (bIsAccelerating)
                {
                    UKismetSystemLibrary::DrawDebugSphere(GetWorld(), StartLocation, 20.0f, 12, FLinearColor::Green, 1.0f);
                }
                else
                {
                    UKismetSystemLibrary::DrawDebugSphere(GetWorld(), StopLocation, 20.0f, 12, FLinearColor::Red, 1.0f);
                }
            }
        }

        if (bIsDistanceMatching)
        {
            if (bIsAccelerating)
            {
                float Distance = FVector::Distance(StartLocation, CharacterOwner->GetActorLocation());
                TimeKeyAtDistance = GetTimeKey(JogStartAnimation, InverseDistanceCurveName, Distance);
            }
            else
            {
                float Distance = FVector::Distance(StopLocation, CharacterOwner->GetActorLocation()) * -1.0f;
                TimeKeyAtDistance = GetTimeKey(JogStopAnimation, InverseDistanceCurveName, Distance);
            }
        }
    }
}

float UDistanceMatchingAnimInstance::GetTimeKey(UAnimSequenceBase* AnimSequence, FName CurveName, float Value) const
{
    float Result = -1.0f;

    if (AnimSequence)
    {
        USkeleton* Skeleton = AnimSequence->GetSkeleton();
        if (Skeleton)
        {
            FSmartName SmartName;
            if (Skeleton->GetSmartNameByName(USkeleton::AnimCurveMappingName, CurveName, SmartName))
            {
                FFloatCurve* ProcessedCurve = AnimSequence->RawCurveData.FloatCurves.FindByPredicate(
                    [SmartName](const FFloatCurve& Curve)
                    {
                        return Curve.Name == SmartName;
                    }
                );
                if (ProcessedCurve)
                {
                    Result = ProcessedCurve->Evaluate(Value);
                }
            }
        }
    }

    return Result;
}

float UDistanceMatchingAnimInstance::GetTimeKeyFromCurve(UAnimSequenceBase* AnimSequence, FName CurveName, float Value) const
{
    float Result = -1.0f;

    if (AnimSequence)
    {
        USkeleton* Skeleton = AnimSequence->GetSkeleton();
        if (Skeleton)
        {
            FSmartName SmartName;
            if (Skeleton->GetSmartNameByName(USkeleton::AnimCurveMappingName, CurveName, SmartName))
            {
                FFloatCurve* OriginalCurve = AnimSequence->RawCurveData.FloatCurves.FindByPredicate(
                    [SmartName](const FFloatCurve& Curve)
                    {
                        return Curve.Name == SmartName;
                    }
                );
                if (OriginalCurve)
                {
                    FFloatCurve ValueToTimeCurve;

                    {
                        SCOPE_CYCLE_COUNTER(STAT_Recreate_Curve);
                        TArray<float> OutTimes;
                        TArray<float> OutValues;

                        // This function outputs time and value arrays with the size
                        OriginalCurve->GetKeys(OutTimes, OutValues);
                        for (int32 Index = 0; Index < OutTimes.Num(); Index++)
                        {
                            // This function implicitly make curve monotonic
                            ValueToTimeCurve.UpdateOrAddKey(OutTimes[Index], OutValues[Index]);
                        }
                    }

                    {
                        SCOPE_CYCLE_COUNTER(STAT_Evaluate_Curve);
                        Result = ValueToTimeCurve.Evaluate(Value);
                    }
                }
            }
        }
    }

    return Result;
}

bool UDistanceMatchingAnimInstance::PredictStopLocation(FVector& OutStopLocation) const
{
    ACharacter* CharacterOwner = Cast<ACharacter>(TryGetPawnOwner());
    if (CharacterOwner)
    {
        UCharacterMovementComponent* MovementComponent = CharacterOwner->GetCharacterMovement();
        if (MovementComponent)
        {
            const float FrictionFactor = FMath::Max(0.0f, MovementComponent->BrakingFrictionFactor);
            const float Friction = FMath::Max(0.0f, MovementComponent->GroundFriction * FrictionFactor);
            const float BrakingDeceleration = MovementComponent->BrakingDecelerationWalking;
            const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
            const bool bZeroFriction = (Friction == 0.0f);
            const bool bZeroBraking = (BrakingDeceleration == 0.0f);

            if (!bZeroFriction || !bZeroBraking)
            {
                float Velocity = MovementComponent->Velocity.Size2D();

                // Decelerate to brake to a stop
                const float RevAccel = -BrakingDeceleration;
                float StopDistance = 0.0f;

                while (Velocity > 1.0f)
                {
                    Velocity = Velocity + (-Friction * Velocity + RevAccel) * DeltaSeconds;
                    StopDistance += Velocity * DeltaSeconds;
                }

                FVector VelocityDirection = MovementComponent->Velocity.GetSafeNormal2D();
                OutStopLocation = CharacterOwner->GetActorLocation() + VelocityDirection * StopDistance;
                return true;
            }
        }
    }

    return false;
}