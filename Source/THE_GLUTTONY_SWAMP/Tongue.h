// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <chrono>
#include <vector>

#include "Tongue.generated.h"

class ABasicFrog;
class AEatable;

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
	FVector TongueVel;

	void Attack();

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
	float max_length = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tongue")
	float timeToGround = 3.f;

	float horizontalVelocity;
	float verticalVelocity;

	float b;
	float a;
	float current_x_2d;
	float current_y_2d;
	float verticalAngle;
	float horizontalAngle;
	float timer;
	float distanceToCamera;

	float x_intersect;
	float y_intersect;

	ABasicFrog* _Frog;

	std::vector<AEatable*> AttachedEatable;

private:
	std::chrono::high_resolution_clock::time_point _startTime;

	UPROPERTY(EditAnywhere, Category = "Tongue", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* _tongueSkeletalMesh;

	bool _isThrown;
};
