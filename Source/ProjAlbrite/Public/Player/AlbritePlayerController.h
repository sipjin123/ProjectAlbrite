// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlbritePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJALBRITE_API AAlbritePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	UUserWidget* CursorWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CursorWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FVector Offset;
};