// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrogPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ABasicFrog;

UCLASS()
class THE_GLUTTONY_SWAMP_API AFrogPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFrogPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressTreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestintationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetClickAction;

	void OnSpectatorSwitched();

	bool isSpectator = false;

	ASpectatorPawn* SpectatorPawn;

	void Attack();
protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

private:
	ABasicFrog* _Frog;
};
