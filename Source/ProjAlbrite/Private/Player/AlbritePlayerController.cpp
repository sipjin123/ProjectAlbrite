// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AlbritePlayerController.h"
#include "Blueprint/UserWidget.h"

void AAlbritePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Enable Mouse Cursor
	bShowMouseCursor = false; // Hide system cursor

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Create and Add Widget Cursor
		if (CursorWidgetClass)
		{
			CursorWidget = CreateWidget<UUserWidget>(this, CursorWidgetClass);
			if (CursorWidget)
			{
				CursorWidget->AddToViewport();
			}
		}
	}
}

void AAlbritePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CursorWidget || !GetWorld()) return;

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Get Camera Location & Convert to Screen Space
		FVector CameraLocation;
		FRotator CameraRotation;
		GetPlayerViewPoint(CameraLocation, CameraRotation);
		CameraLocation += Offset;
		FVector2D ScreenPosition;
		ProjectWorldLocationToScreen(CameraLocation + (CameraRotation.Vector() * 5000.0f), ScreenPosition);
		
		// Set Widget Position
		CursorWidget->SetPositionInViewport(ScreenPosition);
	}
}