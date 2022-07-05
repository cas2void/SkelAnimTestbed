#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ThirdPersonAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class THIRDPERSON_API UThirdPersonAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

    //
    // UAnimInstance Interfaces
    //
public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 0, UIMax = 375))
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsInAir;
};
