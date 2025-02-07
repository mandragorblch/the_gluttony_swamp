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

	state = TONGUE_STATE::Idle;
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

	//TODO read from file
	//max_length_x = maxTongueSpeed * maxTongueTime;
}

void ATongue::Setup()
{
	cameraPos = _Frog->_SpringArmComponent->GetRelativeTransform().GetLocation();
	distanceDiff = cameraPos.Y - tongueCenter.Y;
	heightDiff = cameraPos.Z - tongueCenter.Z;
}

// Called every frame
void ATongue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_isPressed) {
		attackHoldTimer += DeltaTime;
		AttackProbe();
	}

	switch (state) {
		case TONGUE_STATE::Returning:
			ReturnTongueTick(DeltaTime);
			UpdateAttached();
			break;
		case TONGUE_STATE::Thrown:
			AttackTick(DeltaTime);
			UpdateAttached();
			break;
	}
}

void ATongue::AttackPressed()
{
	if(!_isPressed){
		attackHoldTimer = 0.f;
		_isPressed = true;
	}
}

void ATongue::AttackReleased() {
	if (_isPressed) {
		_isPressed = false;

		if (lastProbeSucceed) {
			if (state == TONGUE_STATE::Idle) {
				Attack();
			}
		} else {
			_Frog->mouthClosing = (state == TONGUE_STATE::Idle);
		}
	}
}

void ATongue::Attack()
{
	AttackSetup();
}

void ATongue::AttackSetup()
{
	state = TONGUE_STATE::Thrown;
	TonguePos = FVector(0.0f);
	triggerShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	triggerShape->Activate();
	fixedData = probeData;
	timeInAir = 0.0f;
}

bool ATongue::IsAttackShouldEnd()
{
	return (state == TONGUE_STATE::Thrown) && (timeInAir >= fixedData.timeToGround);
}

void ATongue::AttackEnd()
{
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
	timeInAir += DeltaTime;
	current_x_2d = fixedData.horizontalVelocity * timeInAir;
	current_y_2d = fixedData.parabolaParams.a * (current_x_2d - fixedData.x_intersect) * (current_x_2d - fixedData.x_intersect) + fixedData.y_intersect;
	TonguePos = FVector(
		current_x_2d * sin(fixedData.horizontalAngle),
		-current_x_2d * cos(fixedData.horizontalAngle),
		current_y_2d
	);
	triggerShape->SetRelativeLocation(TonguePos + tongueCenter);
}

void ATongue::AttackProbe()
{
	lastProbeSucceed = false;
	probeData.maxPossibleLength = probeData.maxPossibleLength < maxTongueLength ? tongueFullfilSpeed * (attackHoldTimer - shiftTimer) : maxTongueLength;
	if (state == TONGUE_STATE::Idle) {
		tongueFactor = probeData.maxPossibleLength / maxTongueLength;
	}

	float alpha = _Frog->_horizontalRotation * (PI / 180.f);
	float beta = _Frog->_verticalRotation * (PI / 180.f);

	probeData.x_intersect = 0.5f * probeData.maxPossibleLength;
	if (std::abs(alpha) < PI / 2
		&& probeData.x_intersect / distanceDiff >= std::abs(tanf(alpha))//horizontalAngle of tongue must be less than 90 degrees
		)
	{
		if (float sin_required = sinf(alpha) * distanceDiff / probeData.x_intersect;
			std::abs(sin_required) <= PI / 2) //this is bunch of conclusions from stereometry
		{
			probeData.horizontalAngle = alpha + asinf(sin_required);
			if (probeData.y_intersect = probeData.x_intersect * sinf(probeData.horizontalAngle) / sinf(alpha) * tanf(beta) + heightDiff;
				probeData.y_intersect > 0.001f
				&& probeData.y_intersect / probeData.x_intersect <= tanf(PI / 4)
				) 
			{
				probeData.verticalAngle = atanf(probeData.y_intersect / probeData.x_intersect);
				//probeData.verticalAngle = asinf(probeData.y_intersect / (0.5f * maxTongueSpeed * probeData.timeToGround));

				probeData.parabolaParams.a = -probeData.y_intersect / (probeData.x_intersect * probeData.x_intersect);
				probeData.horizontalVelocity = maxTongueSpeed * cosf(probeData.verticalAngle);
				probeData.timeToGround = probeData.maxPossibleLength / probeData.horizontalVelocity;

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
	state = TONGUE_STATE::Returning;
	prevPos = TonguePos;
	initDist = (TonguePos - tongueCenter).Length();
	tongueReturnFactor = 1.0f;
	returnVelocity = initDist / returnTime;
	fixedTongueFactor = tongueFactor;
}

void ATongue::ReturnTongueTick(float DeltaTime)
{
	tongueReturnFactor -= (returnVelocity * DeltaTime) / initDist;
	if (IsTongueReturned()) {
		TongueReturnEnd();
		return;
	}
	TonguePos = tongueReturnFactor * prevPos;
	tongueFactor = fixedTongueFactor * tongueReturnFactor;
}

bool ATongue::IsTongueReturned()
{
	return (state == TONGUE_STATE::Returning) && (tongueFactor <= 0.f);
}

void ATongue::TongueReturnEnd()
{
	state = TONGUE_STATE::Idle;
	tongueFactor = 0.f;
	TonguePos = FVector(0.f, 0.f, 0.f);
	probeData.maxPossibleLength = 0.0f;
	//TODO SCORE
	//TODO unused fly array
	for (uint8 it = 0; it < AttachedEatable.size(); ++it) {
		AttachedEatable[it]->SetActorHiddenInGame(true);
	}
	AttachedEatable.clear();

	if (!_isPressed) {
		_Frog->mouthClosing = true;
		shiftTimer = 0.f;
	} else {
		shiftTimer = attackHoldTimer;
	}
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