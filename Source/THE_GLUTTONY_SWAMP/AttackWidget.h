// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "SlateCoreClasses.h"
#include "AttackWidget.generated.h"

class ABasicFrog;

/**
 * 
 */
UCLASS()
class THE_GLUTTONY_SWAMP_API UAttackWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,
		BlueprintReadWrite,
		meta = (BindWidget))
	UProgressBar* AttackProgressBar;

	ABasicFrog* _Frog;

	//[0, 1]
	void SetProgress(float progress);

	FLinearColor unableColor = FLinearColor(1.f, 0.1f, 0.1f, 1.f);
	FLinearColor ableColor = FLinearColor(0.1f, 1.0f, 0.1f, 1.f);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
};
