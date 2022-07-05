#include "ParagonAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UParagonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    ACharacter* CharacterOwner = Cast<ACharacter>(TryGetPawnOwner());
    if (CharacterOwner)
    {
        UCharacterMovementComponent* MovementComponent = CharacterOwner->GetCharacterMovement();
        if (MovementComponent)
        {
            bIsAccelerating = MovementComponent->GetCurrentAcceleration().Size() > 0;
            bIsInAir = MovementComponent->IsFalling();
        }
    }
}