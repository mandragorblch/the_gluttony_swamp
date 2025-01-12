// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogPlayerController.h"
#include "GameFramework/Pawn.h"
#include "BasicFrog.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpectatorPawn.h"

AFrogPlayerController::AFrogPlayerController()
{
	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AFrogPlayerController::BeginPlay()
{
	Super::BeginPlay();

	_Frog = Cast<ABasicFrog>(GetCharacter());

	TSubclassOf<AActor> BP_Spectator = LoadObject<UClass>(nullptr, TEXT("Blueprint'/Game/BP/BP_Spectator.BP_Spectator_C'"));
	SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(BP_Spectator, _Frog->GetActorLocation(), FRotator(_Frog->_verticalRotation, _Frog->_horizontalRotation, 0.f));
}

void AFrogPlayerController::OnSpectatorSwitched()
{
	if (isSpectator) {
		Possess(_Frog);
	}
	else {
		Possess(SpectatorPawn);
	}
	isSpectator = !isSpectator;
}

void AFrogPlayerController::Attack()
{
	_Frog->Attack();
}

void AFrogPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AFrogPlayerController::Attack);

	InputComponent->BindAction(TEXT("SpectatorSwitch"), IE_Pressed, this, &AFrogPlayerController::OnSpectatorSwitched);
}
