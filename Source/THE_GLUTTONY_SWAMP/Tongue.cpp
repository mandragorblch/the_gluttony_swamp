// Fill out your copyright notice in the Description page of Project Settings.

#include "Tongue.h"
#include "TongueAnimInstance.h"
#include "Components/SphereComponent.h"
#include "BasicFrog.h"
#include "Eatable.h"
#include <cmath>
#include <algorithm>

// Sets default values
ATongue::ATongue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	_tongueSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Tongue"));
	_tongueSkeletalMesh->SetupAttachment(RootComponent);
	_tongueSkeletalMesh->BoundsScale = 50.f;

	triggerShape = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	triggerShape->SetupAttachment(RootComponent);
	triggerShape->SetSimulatePhysics(false);
	triggerShape->SetMobility(EComponentMobility::Movable);
	triggerShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	triggerShape->Deactivate();

	AttachedEatable.resize(5);
	std::fill(AttachedEatable.begin(), AttachedEatable.end(), nullptr);

	_isThrown = false;
}

// Called when the game starts or when spawned
void ATongue::BeginPlay()
{
	Super::BeginPlay();

	_startTime = std::chrono::high_resolution_clock::now();
	tongueCenter = _tongueSkeletalMesh->GetSocketTransform(TEXT("tongueCenter"), ERelativeTransformSpace::RTS_Actor).GetLocation();
	triggerShape->AttachToComponent(_tongueSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("tongueCenter"));

	triggerShape->OnComponentBeginOverlap.AddDynamic(this, &ATongue::OnOverlapBegin);
	triggerShape->OnComponentEndOverlap.AddDynamic(this, &ATongue::OnOverlapEnd);
}

void ATongue::Setup()
{
	cameraPos = _Frog->_SpringArmComponent->GetRelativeTransform().GetLocation();
	distanceToCamera = cameraPos.Y - tongueCenter.Y;
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

	{
		auto iter = AttachedEatable.begin();
		while (*iter != nullptr) {
			(*iter)->SetActorRelativeLocation(TonguePos + tongueCenter);
		}
	}
}

void ATongue::Attack()
{
	if (!_isThrown) {
		float x_intersect_test = 0.5f * max_length;
		float alpha = _Frog->_horizontalRotation * (PI / 180.f);
		float beta = _Frog->_verticalRotation * (PI / 180.f);
		if (
			std::abs(alpha) < PI / 2
			&& x_intersect_test / distanceToCamera >= std::abs(tanf(alpha))//horizontalAngle of tongue must be less than 90 degrees
			) {
			if (float sin_required = sinf(alpha) * distanceToCamera / x_intersect_test; std::abs(sin_required) <= PI / 2) {
				float horizontalAngle_test = alpha + asinf(sin_required);
				if (float y_intersect_test = x_intersect_test * sinf(horizontalAngle_test) / sinf(alpha) * tanf(beta) + b;
					y_intersect_test > 0.01f
					&& y_intersect_test / x_intersect_test <= tanf(PI / 4)
					) {
					x_intersect = x_intersect_test;
					y_intersect = y_intersect_test;
					verticalAngle = atanf(y_intersect / x_intersect);
					horizontalAngle = horizontalAngle_test;
					_isThrown = true;
					triggerShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					triggerShape->Activate();
					current_x_2d = 0.0f;
					current_y_2d = 0.0f;
					timer = 0.0f;

					a = y_intersect / (x_intersect * x_intersect);
					horizontalVelocity = max_length / timeToGround;
					verticalVelocity = horizontalVelocity * tan(verticalAngle);
					horizontalAngle -= PI / 2.0f;//spring arm rotation
				}
			}
		}
	}
}

void ATongue::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlapped with: %s"), *OtherActor->GetName()));
}

void ATongue::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}