// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AlbritePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/PlayerSpawnPoint.h"
#include "ProjAlbrite/ProjAlbriteGameMode.h"

AAlbritePlayerController::AAlbritePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
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

void AAlbritePlayerController::Client_CanSpawnAvatar_Implementation()
{
	if (SpawnCharacterPair.IsEmpty())
	{
		return;
	}

	UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI)
	{
		return;
	}

	Server_ReqSpawnAvatar(1, 1, "");
}

void AAlbritePlayerController::Server_ReqSpawnAvatar_Implementation(const int32 SpawnPosIndex, const int32 AvatarIndex, const FString& ID)
{
	AllSpawnPoints.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnPointClass, AllSpawnPoints);
	if (AllSpawnPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("%s(): %d can not find APlayerSpawnPoint"), *FString(__FUNCTION__), __LINE__);
		return;
	}

	APlayerSpawnPoint* SpawnPoint = nullptr;
	for (auto OnePoint : AllSpawnPoints)
	{
		SpawnPoint = Cast<APlayerSpawnPoint>(OnePoint);
		if (!SpawnPoint)
		{
			continue;
		}

		if (SpawnPoint->SpawnIndex == SpawnPosIndex)
		{
			break;
		}
	}

	if (!SpawnPoint)
	{
		return;
	}
	
	const FVector TempSpawnLocation = SpawnPoint->GetActorLocation();
	const FRotator TempSpawnRotation = SpawnPoint->GetActorRotation();

	AProjAlbriteCharacter* ControlledCharacter = Cast<AProjAlbriteCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
		GetWorld(),
		SpawnCharacterPair[AvatarIndex],
		FTransform(TempSpawnRotation, TempSpawnLocation),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
		this));

	if (!ControlledCharacter)
	{
		return;
	}

	UGameplayStatics::FinishSpawningActor(ControlledCharacter, FTransform(TempSpawnRotation, TempSpawnLocation));
	Possess(ControlledCharacter);
}

void AAlbritePlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	OnPossessAcknowledge.Broadcast();

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

void AAlbritePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Enable Mouse Cursor
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(nullptr); // Set to your UI widget if needed

	SetInputMode(InputMode);
	bShowMouseCursor = true; // Hide system cursor
}

void AAlbritePlayerController::Client_JoinLobby_Implementation(const FString& ServerAddress)
{
	UE_LOG(LogTemp, Warning, TEXT("Player wants to login to address %s"), *ServerAddress);
	if (!ServerAddress.IsEmpty())
	{
		FString Command = FString::Printf(TEXT("open %s:7777"), *ServerAddress);
		GetWorld()->GetFirstPlayerController()->ConsoleCommand(Command);
	}
}

void AAlbritePlayerController::ServerRequestSpawn_Implementation()
{
	AProjAlbriteGameMode* GM = Cast<AProjAlbriteGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->RequestSpawn(this);
	}
}

bool AAlbritePlayerController::ServerRequestSpawn_Validate()
{
	return true;
}

void AAlbritePlayerController::Client_JoinAsCharacter_Implementation()
{
	ServerRequestSpawn();
}
