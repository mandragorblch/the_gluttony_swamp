// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bush.generated.h"

class ABasicFrog;

UCLASS()
class THE_GLUTTONY_SWAMP_API ABush : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABush();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ABasicFrog* _Frog = nullptr;
	ASpectatorPawn* _Spectator = nullptr;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _StaticMesh;

};
