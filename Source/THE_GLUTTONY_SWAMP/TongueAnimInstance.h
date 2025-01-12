// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TongueAnimInstance.generated.h"

extern class ATongue;

UCLASS()
class THE_GLUTTONY_SWAMP_API UTongueAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void NativeInitializeAnimation() override;

	//virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tongue")
	class ATongue* _Tongue = nullptr;
	
};
