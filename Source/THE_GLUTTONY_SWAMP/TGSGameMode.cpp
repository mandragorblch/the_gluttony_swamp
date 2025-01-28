// Fill out your copyright notice in the Description page of Project Settings.


#include "TGSGameMode.h"
#include "FrogPlayerController.h"
#include "Fly.h"

ATGSGameMode::ATGSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> FrogPawnBPClass(TEXT("/Game/BP/BP_Frog.BP_Frog_C"));
	//check(FrogPawnBPClass.Class != nullptr);
	DefaultPawnClass = FrogPawnBPClass.Class;

	PlayerControllerClass = AFrogPlayerController::StaticClass();

	//static ConstructorHelpers::FClassFinder<APawn> SpectatorPawnBPClass(TEXT("/Game/BP/BP_Spectator.BP_Spectator_C"));
	//SpectatorPawn = SpectatorPawnBPClass.Class;
}

void ATGSGameMode::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<AActor> BP_Fly = LoadObject<UClass>(nullptr, TEXT("Blueprint'/Game/BP/BP_Fly.BP_Fly_C'"));
	GetWorld()->SpawnActor<AFly>(BP_Fly, FVector(0.f, 350.f, 250.f), FRotator(0.f, 0.f, 0.f));
}
