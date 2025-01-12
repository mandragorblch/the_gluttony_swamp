// Fill out your copyright notice in the Description page of Project Settings.

#include "Tongue.h"
#include "TongueAnimInstance.h"
#include "Components/SphereComponent.h"
#include "BasicFrog.h"
#include <cmath>

// Sets default values
ATongue::ATongue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_tongueSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Tongue"));
	_tongueSkeletalMesh->SetupAttachment(RootComponent);
	_tongueSkeletalMesh->BoundsScale = 50.f;

	triggerShape = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	triggerShape->OnComponentBeginOverlap.AddDynamic(this, &ATongue::OnOverlapBegin);
	triggerShape->OnComponentEndOverlap.AddDynamic(this, &ATongue::OnOverlapEnd);
	triggerShape->SetupAttachment(RootComponent);
	triggerShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	triggerShape->Deactivate();

	_isThrown = false;
}

// Called when the game starts or when spawned
void ATongue::BeginPlay()
{
	Super::BeginPlay();

	_startTime = std::chrono::high_resolution_clock::now();

	tongueCenter = _tongueSkeletalMesh->GetSocketTransform(TEXT("tongueCenter"), ERelativeTransformSpace::RTS_Actor).GetLocation();
}

void ATongue::Setup()
{
	cameraPos = _Frog->_SpringArmComponent->GetRelativeTransform().GetLocation();
	distanceToCamera = cameraPos.Y - tongueCenter.Y;
	x_intersect = 0.5f * max_length;
	b = cameraPos.Z - tongueCenter.Z;
}

// Called every frame
void ATongue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (timer >= timeToGround) {
		_isThrown = false;
		TonguePos = FVector(0.0f);
		triggerShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		triggerShape->Deactivate();
	} else {
		timer += DeltaTime;
		current_x_2d = horizontalVelocity * timer;
		current_y_2d = -a * (current_x_2d - x_intersect) * (current_x_2d - x_intersect) + y_intersect;
		TonguePos = FVector(
			current_x_2d * cos(horizontalAngle),
			current_x_2d * sin(horizontalAngle),
			current_y_2d
		);
	}
}

void ATongue::Attack()
{
	if (!_isThrown) {
		float alpha = _Frog->_horizontalRotation * (PI / 180.f);
		float beta = _Frog->_verticalRotation * (PI / 180.f);
		horizontalAngle = alpha + asin(sin(alpha) * distanceToCamera / x_intersect);
		y_intersect = abs(x_intersect * sin(horizontalAngle) / sin(alpha)) * tan(beta) + b;
		verticalAngle = atan(y_intersect / x_intersect);
		a = y_intersect / (x_intersect * x_intersect);
		if (!std::isnan(verticalAngle)) {
			if(!std::isnan(horizontalAngle)) {
				_isThrown = true;
				triggerShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				triggerShape->Activate();
				current_x_2d = 0.0f;
				current_y_2d = 0.0f;
				timer = 0.0f;

				horizontalVelocity = max_length / timeToGround;
				verticalVelocity = horizontalVelocity * tan(verticalAngle);
				horizontalAngle -= PI / 2.0f;//spring arm rotation
			}
		}
	}
}

void ATongue::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ATongue::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}