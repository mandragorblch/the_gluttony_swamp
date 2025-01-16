// Fill out your copyright notice in the Description page of Project Settings.

#include "Bush.h"
#include "BasicFrog.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/Actor.h"
#include <cmath>

// Sets default values
ABush::ABush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void ABush::BeginPlay()
{
	Super::BeginPlay();
	_Frog = Cast<ABasicFrog>(UGameplayStatics::GetActorOfClass(GetWorld(), ABasicFrog::StaticClass()));
	_Spectator = Cast<ASpectatorPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpectatorPawn::StaticClass()));
}

// Called every frame
void ABush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector direction = _Spectator->GetActorLocation() - GetActorLocation();
	direction.Normalize();
	float theta = std::atan2f(direction.Y, direction.X) - PI / 2;
	SetActorRotation(FRotator(0.f, theta * 180.f / PI, 0.f));
}

