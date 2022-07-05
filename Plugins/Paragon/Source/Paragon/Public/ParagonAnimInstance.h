#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ParagonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PARAGON_API UParagonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	//
    // UAnimInstance Interfaces
    //
public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsAccelerating;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsInAir;
};
