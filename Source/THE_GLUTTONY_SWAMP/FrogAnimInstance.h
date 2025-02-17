// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FrogAnimInstance.generated.h"

class ABasicFrog;

UENUM(BlueprintType)
enum class ELegsState : uint8 {
	Idle = 0,
	Swim,
	Jump
};

UENUM(BlueprintType)
enum class EMouthState : uint8 {
	Idle = 0,
	Eat
};

UCLASS()
class THE_GLUTTONY_SWAMP_API UFrogAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	void virtual NativeInitializeAnimation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog Params")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog Params")
	ELegsState legsState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog Params")
	EMouthState mouthState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog Params")
	ABasicFrog* OwnedPawn;

	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();
};
