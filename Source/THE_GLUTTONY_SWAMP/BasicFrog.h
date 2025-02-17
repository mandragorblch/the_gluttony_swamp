// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasicFrog.generated.h"

class ATongue;
class AFrogView;
class UFrogAnimInstance;

UCLASS()
class THE_GLUTTONY_SWAMP_API ABasicFrog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicFrog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Turn(float delta);

	void LookUp(float delta);

	void JumpPressed();

	void JumpReleased();

	UFUNCTION(BlueprintCallable)
	void AttackPressed();

	UFUNCTION(BlueprintCallable)
	void AttackReleased();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tongue", meta = (AllowPrivateAccess = "true"))
	ATongue* _Tongue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float _mouseSensivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float _horizontalRotation = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float _verticalRotation = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* _SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UFrogAnimInstance* _AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _FixForRotation;

	void StartEatAnimation();

	void UpdateEatAnimation(float DeltaTime);

	void EndEatAnimation();

	//per second
	float EatAnimationSpeed = 3.f;

	//animation flag
	bool mouthClosing;
	//animation flag
	bool mouthOpening;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float EatStateFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float EatAnimationLength = 0.25f;
};
