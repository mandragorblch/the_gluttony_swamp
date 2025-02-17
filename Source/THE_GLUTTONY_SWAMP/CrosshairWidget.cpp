// Fill out your copyright notice in the Description page of Project Settings.

#include "CrosshairWidget.h"
#include "Components/Image.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"

//void UCrosshairWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//    if (GEngine && GEngine->GameViewport)
//    {
//        GEngine->GameViewport->Viewport->ViewportResizedEvent.AddUObject(this, &UCrosshairWidget::OnViewportResized);
//    }
//}
//
//void UCrosshairWidget::OnViewportResized(FViewport* Viewport, uint32)
//{
//    float aspectRatio = (static_cast<float>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y);
//    if (aspectRatio >= 1.f) {
//        CrosshairImage->SetRenderScale(FVector2D(1.0f / aspectRatio, 1.0f));
//    } else {
//        CrosshairImage->SetRenderScale(FVector2D(1.0f, 1.0f * aspectRatio));
//    }
//}
