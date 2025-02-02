// Fill out your copyright notice in the Description page of Project Settings.

#include "Tongue.h"
#include "TongueAnimInstance.h"
#include "Components/SphereComponent.h"
#include "BasicFrog.h"
#include "Eatable.h"
#include <cmath>
#include <algorithm>
#include <numeric>

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

	cap = 5;
	AttachedEatable.reserve(cap);

	returnTime = 1.f;

	_isThrown = false;
	_isPressed = false;
}

// Called when the game starts or when spawned
void ATongue::BeginPlay()
{
	Super::BeginPlay();

	tongueCenter = _tongueSkeletalMesh->GetSocketTransform(TEXT("tongueCenter"), ERelativeTransformSpace::RTS_Actor).GetLocation();
	//triggerShape->AttachToComponent(_tongueSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("tongueCenter"));

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

	if (_isPressed) {
		AttackProbe();
	}
	if(_isThrown) {
		AttackTick(DeltaTime);
		UpdateAttached();
	}
	if(_isReturning) {
		ReturnTongueTick(DeltaTime);
		UpdateAttached();
	} 
}

void ATongue::AttackPressed()
{
	if(!_isPressed && !_isThrown && !_isReturning){
		attackTimer = 0.f;
		_isPressed = true;
	}
}

void ATongue::AttackReleased()
{
	if (_isPressed && !_isReturning) {
		_isPressed = false;
		Attack();
	}
}

void ATongue::Attack()
{
	if (!_isThrown) {
		_isThrown = true;
		AttackSetup();
	}
}

void ATongue::AttackSetup()
{
	TonguePos = FVector(0.0f);
	triggerShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	triggerShape->Activate();
	current_x_2d = 0.0f;
	current_y_2d = 0.0f;
	timer = 0.0f;
}

bool ATongue::IsAttackShouldEnd()
{
	return _isThrown && (timer >= timeToGround);
}

void ATongue::AttackEnd()
{
	_isThrown = false;
	triggerShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	triggerShape->Deactivate();
}

void ATongue::AttackTick(float DeltaTime)
{
	if (IsAttackShouldEnd()) {
		AttackEnd();
		ReturnTongueSetup();
		return;
	}
	timer += DeltaTime;
	current_x_2d = horizontalVelocity * timer;
	current_y_2d = a * (current_x_2d - x_intersect) * (current_x_2d - x_intersect) + y_intersect;
	TonguePos = FVector(
		current_x_2d * cos(horizontalAngle),
		current_x_2d * sin(horizontalAngle),
		current_y_2d
	);
	triggerShape->SetRelativeLocation(TonguePos + tongueCenter);
}

void ATongue::AttackProbe()
{
	lastProbeSucceed = false;
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
				y_intersect_test > 0.001f
				&& y_intersect_test / x_intersect_test <= tanf(PI / 4)
				) {
				x_intersect = x_intersect_test;
				y_intersect = y_intersect_test;
				verticalAngle = atanf(y_intersect / x_intersect);
				horizontalAngle = horizontalAngle_test;

				a = -y_intersect / (x_intersect * x_intersect);
				horizontalVelocity = tongueSpeed * cosf(verticalAngle);
				timeToGround = max_length / horizontalVelocity;
				horizontalAngle -= PI / 2.0f;//spring arm rotation
				lastProbeSucceed = true;
			}
		}
	}
}

void ATongue::UpdateAttached()
{
	for (uint8 it = 0; it < AttachedEatable.size(); ++it) {
		AttachedEatable[it]->SetActorRelativeLocation(TonguePos + tongueCenter);
	}
}

void ATongue::ReturnTongueSetup()
{
	_isReturning = true;
	prevPos = TonguePos;
	initDist = (TonguePos - tongueCenter).Length();
	factor = 1.0f;
	returnVelocity = initDist / returnTime;
}

void ATongue::ReturnTongueTick(float DeltaTime)
{
	factor -= (returnVelocity * DeltaTime) / initDist;
	if (IsTongueReturned()) {
		TongueReturnEnd();
		factor = 0.f;
		TonguePos = FVector(0.f, 0.f, 0.f);
		return;
	}
	TonguePos = factor * prevPos;
}

bool ATongue::IsTongueReturned()
{
	return (_isReturning && factor <= 0.f);
}

void ATongue::TongueReturnEnd()
{
	_isReturning = false;
	//TODO SCORE
	//TODO unused fly array
	for (uint8 it = 0; it < AttachedEatable.size(); ++it) {
		AttachedEatable[it]->SetActorHiddenInGame(true);
	}
	AttachedEatable.clear();
	_Frog->mouthClosing = true;
}

void ATongue::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEatable* eatable = Cast<AEatable>(OtherActor); eatable != nullptr) {
		eatable->_triggerShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		eatable->_triggerShape->Deactivate();
		eatable->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		AttachedEatable.push_back(eatable);
	}
}

void ATongue::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}