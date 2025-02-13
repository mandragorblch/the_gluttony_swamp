// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackWidget.h"
#include "BasicFrog.h"
#include "Tongue.h"

void UAttackWidget::SetProgress(float progress)
{
	AttackProgressBar->SetPercent(progress);
}

void UAttackWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AttackProgressBar->SetPercent(1.f);

	_Frog = Cast<ABasicFrog>(GetOwningPlayerPawn());
}

void UAttackWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	SetProgress(_Frog->_Tongue->tongueFactor);

	if (_Frog->_Tongue->lastProbeSucceed) {
		AttackProgressBar->SetFillColorAndOpacity(ableColor);
	} else {
		AttackProgressBar->SetFillColorAndOpacity(unableColor);
	}
}
