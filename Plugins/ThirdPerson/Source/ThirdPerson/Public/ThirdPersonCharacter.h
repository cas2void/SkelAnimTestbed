#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

UCLASS()
class THIRDPERSON_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();

	//
	// AActor Interfaces
	//
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	//
	// APawn Interfaces
	//
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//
	// Camera
	//
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;

	//
	// Movement
	//
protected:
	void MoveForward(float Value);

	void MoveRight(float Value);
};
