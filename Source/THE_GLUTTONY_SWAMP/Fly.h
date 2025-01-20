// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Eatable.h"
#include "Fly.generated.h"

/**
 * 
 */
UCLASS()
class THE_GLUTTONY_SWAMP_API AFly : public AEatable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFly();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;

};
