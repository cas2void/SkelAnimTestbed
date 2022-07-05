#include "SequencingGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void ASequencingGameMode::PressKey(FName Name)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->InputKey(FKey(Name), IE_Pressed, 1, false);
    }
}

void ASequencingGameMode::ReleaseKey(FName Name)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->InputKey(FKey(Name), IE_Released, 0, false);
    }
}