#include "ThirdPersonAnimInstance.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"

void UThirdPersonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    APawn* PawnOwner = TryGetPawnOwner();
    if (PawnOwner)
    {
        UPawnMovementComponent* MovementComponent = PawnOwner->GetMovementComponent();
        if (MovementComponent)
        {
            bIsInAir = MovementComponent->IsFalling();
        }

        Speed = PawnOwner->GetVelocity().Size();
    }
}