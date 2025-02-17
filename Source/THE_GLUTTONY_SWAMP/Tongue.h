// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <chrono>
#include <vector>
#include <deque>

#include "Tongue.generated.h"

class ABasicFrog;
class AEatable;

struct ParabolaParams {
	float a;
	float b;
	float c;
};

struct ThrowData {
	//trajectory of throw
	ParabolaParams parabolaParams;
	//x-coord of intersection point
	float x_intersect;
	//y-coord of intersection point
	float y_intersect;
	//angle of line, casted from tongue center, that intersect perpendicular from center of screen (camera)
	float verticalAngle;
	//angle of line, casted from tongue center, that intersect perpendicular from center of screen (camera)
	float horizontalAngle;
	//literaly
	float maxPossibleLength;
	float timeToGround;
	//of tongue
	float horizontalVelocity;
	//of tongue
	float verticalVelocity;
};

enum class TONGUE_STATE : uint8 {
	Thrown = 0,
	Returning,
	Idle
};

UCLASS()
class THE_GLUTTONY_SWAMP_API ATongue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATongue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tongue")
	FVector TonguePos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tongue")
	FRotator TongueRot;

	void AttackPressed();

	void AttackReleased();

	void Attack();

	void AttackSetup();

	bool IsAttackShouldEnd();

	void AttackEnd();

	void AttackTick(float DeltaTime);

	void AttackProbe();

	void UpdateAttached();

	void ReturnTongueSetup();

	void ReturnTongueTick(float DeltaTime);

	bool IsTongueReturned();

	void TongueReturnEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* triggerShape;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void Setup();

	FVector cameraPos;
	FVector tongueCenter;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tongue")
	float maxTongueLength = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tongue")
	float maxTongueSpeed = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tongue")
	float tongueFullfilBarTime = 3.0f;

	float tongueFullfilSpeed = maxTongueLength / tongueFullfilBarTime;

	//height difference between camera and tongue
	float heightDiff;
	//distance difference (Y projection) between camera and tongue
	float distanceDiff;

	ThrowData probeData;
	ThrowData fixedData;
	//current x-coord of parabola
	float current_x_2d;
	//current y-coord of parabola
	float current_y_2d;

	//literaly
	float timeInAir;
	//Whether the last probe function succeeded
	bool lastProbeSucceed;
	//time elapsed since last attack pressed
	float attackHoldTimer;
	//time shift from factor sets to 0
	float shiftTimer;

	//owner frog
	ABasicFrog* _Frog;

	//pointer array with attachables
	std::vector<AEatable*> AttachedEatable;
	//maximum attached flies
	uint8 cap;

	//for return tongue
	FVector prevPos;
	//progress of tongue to the destination point
	float tongueFactor;
	//interpolating return factor
	float tongueReturnFactor;
	//for interpolating return factor
	float fixedTongueFactor;
	//fixed destination point
	float initDist;
	//it is what it is
	float returnTime;
	//it is what it is
	float returnVelocity;

	bool _isPressed;

	TONGUE_STATE state;

private:
	UPROPERTY(EditAnywhere, Category = "Tongue", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* _tongueSkeletalMesh;
};
