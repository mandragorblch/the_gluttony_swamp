// Fill out your copyright notice in the Description page of Project Settings.


#include "TGSGameMode.h"
#include "FrogPlayerController.h"
//#include "PhysXPublic.h"
//#include "PhysXIncludes.h"
//#include "PxPhysicsAPI.h"
#include "Fly.h"

ATGSGameMode::ATGSGameMode()
{

	PlayerControllerClass = AFrogPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> FrogPawnBPClass(TEXT("/Game/BP/BP_Frog.BP_Frog_C"));
	//check(FrogPawnBPClass.Class != nullptr);
	DefaultPawnClass = FrogPawnBPClass.Class;

	//physx::PxFoundation* Foundation = physx::PxCreateFoundation(physx::PX_PHYSICS_VERSION, physx::GPhysXAllocator, physx::GPhysXErrorCallback);
	//physx::PxPvd* Pvd = physx::PxCreatePvd(*Foundation);
	//physx::PxPvdTransport* Transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	//Pvd->connect(*Transport, physx::PxPvdInstrumentationFlag::eALL);

}

void ATGSGameMode::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<AActor> BP_Fly = LoadObject<UClass>(nullptr, TEXT("Blueprint'/Game/BP/BP_Fly.BP_Fly_C'"));
	GetWorld()->SpawnActor<AFly>(BP_Fly, FVector(0.f, 350.f, 150.f), FRotator(0.f, 0.f, 0.f));
}
