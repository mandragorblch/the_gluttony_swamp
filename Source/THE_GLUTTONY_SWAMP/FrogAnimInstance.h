// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FrogAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EFrogState : uint8 {
	Idle = 0,
	Eat,
	Swim,
	Jump
};

UCLASS()
class THE_GLUTTONY_SWAMP_API UFrogAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog Params")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frog Params")
	EFrogState State;

	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();
};
