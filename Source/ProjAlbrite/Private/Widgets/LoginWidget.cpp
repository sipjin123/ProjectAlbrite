// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LoginWidget.h"
#include "Player/AlbritePlayerController.h"
#include "ProjAlbrite/ProjAlbriteGameMode.h"

void ULoginWidget::HostButtonPressed()
{
	AProjAlbriteGameMode* GM = Cast<AProjAlbriteGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		UE_LOG(LogTemp, Warning, TEXT("Host was launched"));
		GM->HostGame();
	}
}

void ULoginWidget::LoginButtonPressed(int32 CharacterChoice)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Button was Pressed"));
		Cast<AAlbritePlayerController>(PC)->Client_JoinLobby(TEXT("127.0.0.1"));
	}
}

void ULoginWidget::SpawnPlayerPressed(int32 CharacterChoice)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AAlbritePlayerController* CustomRef = Cast<AAlbritePlayerController>(PC);

		if (CustomRef)
		{
			//CustomRef->ServerRequestSpawn();
			CustomRef->Client_JoinAsCharacter();
			UE_LOG(LogTemp, Warning, TEXT("Spawn Button was Pressed 222"));
		}
	}
}
