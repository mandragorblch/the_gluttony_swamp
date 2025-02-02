// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAnimInstance.h"
#include "BasicFrog.h"

void UFrogAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Get the pawn that owns this animation instance
    OwnedPawn = Cast<ABasicFrog>(TryGetPawnOwner());
}

void UFrogAnimInstance::OnStateAnimationEnds()
{

}