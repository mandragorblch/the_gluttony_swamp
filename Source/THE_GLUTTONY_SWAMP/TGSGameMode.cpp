// Fill out your copyright notice in the Description page of Project Settings.


#include "TGSGameMode.h"
#include "FrogPlayerController.h"

ATGSGameMode::ATGSGameMode()
{

	PlayerControllerClass = AFrogPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> FrogPawnBPClass(TEXT("/Game/BP/BP_Frog.BP_Frog_C"));
	//check(FrogPawnBPClass.Class != nullptr);
	DefaultPawnClass = FrogPawnBPClass.Class;
}
