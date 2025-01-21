// Fill out your copyright notice in the Description page of Project Settings.


#include "Fly.h"

AFly::AFly() {
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	_skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	_skeletalMesh->SetupAttachment(RootComponent);

	_triggerShape = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//_triggerShape->OnComponentBeginOverlap.AddDynamic(this, &ATongue::OnOverlapBegin);
	//_triggerShape->OnComponentEndOverlap.AddDynamic(this, &ATongue::OnOverlapEnd);
	_triggerShape->SetupAttachment(RootComponent);
	_triggerShape->SetSimulatePhysics(false);
	_triggerShape->SetMobility(EComponentMobility::Movable);
	_triggerShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_triggerShape->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	_triggerShape->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	_triggerShape->SetGenerateOverlapEvents(true);

	protein = 1.f;
}

void AFly::BeginPlay() {
	Super::BeginPlay();

	_triggerShape->OnComponentBeginOverlap.AddDynamic(this, &AFly::OnOverlapBegin);
}

void AFly::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AFly::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
