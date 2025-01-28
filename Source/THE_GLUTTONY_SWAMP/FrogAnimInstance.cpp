// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAnimInstance.h"
#include "BasicFrog.h"

void UFrogAnimInstance::OnStateAnimationEnds()
{
	if (State != EFrogState::Idle)
	{
		State = EFrogState::Idle;
	}
}