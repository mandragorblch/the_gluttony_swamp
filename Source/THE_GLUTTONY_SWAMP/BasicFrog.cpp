// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicFrog.h"
#include "FrogAnimInstance.h"
#include "Tongue.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABasicFrog::ABasicFrog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	_FixForRotation = CreateDefaultSubobject<USceneComponent>(TEXT("FixForRotation"));
	_FixForRotation->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_FixForRotation->SetRelativeLocation(FVector(0.f, 12.f, 0.f));
	_FixForRotation->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	GetCapsuleComponent()->SetupAttachment(RootComponent);
	GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	GetMesh()->AttachToComponent(_FixForRotation, FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	auto frogMovement = GetCharacterMovement();
	frogMovement->MovementMode = EMovementMode::MOVE_Falling;

	_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_SpringArmComponent->SetupAttachment(_FixForRotation);
	_SpringArmComponent->TargetArmLength = 0.f;
	_SpringArmComponent->SetRelativeLocation(FVector(0.0f, 100.0f, 50.0f));
	_SpringArmComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_CameraComponent->SetupAttachment(_SpringArmComponent, USpringArmComponent::SocketName);

	_mouseSensivity = 10.0f;
}

// Called when the game starts or when spawned
void ABasicFrog::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<AActor> BP_Tongue = LoadObject<UClass>(nullptr, TEXT("Blueprint'/Game/BP/BP_Tongue.BP_Tongue_C'"));
	_Tongue = GetWorld()->SpawnActor<ATongue>(BP_Tongue, FVector(0.f), FRotator(0.f));
	_Tongue->AttachToComponent(_FixForRotation, FAttachmentTransformRules::KeepRelativeTransform);
	_Tongue->_Frog = this;
	_Tongue->Setup();

	_AnimInstance = Cast<UFrogAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void ABasicFrog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_CameraComponent->SetRelativeRotation(FRotator(_verticalRotation, _horizontalRotation, 0.f));

	UpdateEatAnimation(DeltaTime);
}

// Called to bind functionality to input
void ABasicFrog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ABasicFrog::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasicFrog::LookUp);
}

void ABasicFrog::Turn(float delta)
{
	_horizontalRotation += delta * _mouseSensivity * GetWorld()->GetDeltaSeconds();
	if (_horizontalRotation >= 360.f) _horizontalRotation -= 360.f;
}

void ABasicFrog::LookUp(float delta)
{
	if (abs(_verticalRotation) <= 90.f) _verticalRotation += delta * _mouseSensivity * GetWorld()->GetDeltaSeconds();
	else _verticalRotation = std::signbit(_verticalRotation) ? -90.f : 90.f;
}

void ABasicFrog::AttackPressed()
{
	_Tongue->AttackPressed();

	if (EatStateFactor <= 0.f) {
		StartEatAnimation();
	} else if (mouthClosing) {
		mouthClosing = false;
		mouthOpening = true;
	}
}

void ABasicFrog::AttackReleased()
{
	_Tongue->AttackReleased();

	if (_Tongue->state == TONGUE_STATE::Idle) {
		mouthClosing = true;
	}
}

void ABasicFrog::StartEatAnimation()
{
	_AnimInstance->State = EFrogState::Eat;
	EatStateFactor = 0.f;
	mouthOpening = true;
}

void ABasicFrog::UpdateEatAnimation(float DeltaTime)
{
	if (mouthOpening) {
		EatStateFactor += EatAnimationSpeed * DeltaTime;
		mouthOpening = EatStateFactor < 1.0f;
		if (mouthOpening == false) EatStateFactor = 1.0f;
	} else if (mouthClosing) {
		EatStateFactor -= EatAnimationSpeed * DeltaTime;
		mouthClosing = EatStateFactor > 0.f;
		if (mouthClosing == false) EndEatAnimation();
	}
}

void ABasicFrog::EndEatAnimation()
{
	_AnimInstance->State = EFrogState::Idle;
}
