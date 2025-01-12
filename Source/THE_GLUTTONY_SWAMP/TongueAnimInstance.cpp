// Fill out your copyright notice in the Description page of Project Settings.

#include "TongueAnimInstance.h"
#include "Tongue.h"

void UTongueAnimInstance::NativeInitializeAnimation()
{
	_Tongue = Cast<ATongue>(GetOwningActor());
}

//void UTongueAnimInstance::NativeUpdateAnimation(float DeltaTime)
//{
//
//}
