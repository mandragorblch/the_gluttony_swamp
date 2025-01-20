// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TGSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THE_GLUTTONY_SWAMP_API ATGSGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ATGSGameMode();

protected:
	void virtual BeginPlay() override;
};
