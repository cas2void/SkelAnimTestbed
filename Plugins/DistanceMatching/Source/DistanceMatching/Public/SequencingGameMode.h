#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SequencingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DISTANCEMATCHING_API ASequencingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	//
	// Virtual Input
	//
public:
	UFUNCTION(BlueprintCallable)
	void PressKey(FName Name);

	UFUNCTION(BlueprintCallable)
	void ReleaseKey(FName Name);
};
