// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjAlbriteGameMode.h"
#include "ProjAlbriteCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AProjAlbriteGameMode::AProjAlbriteGameMode()
{
	DefaultPawnClass = nullptr;
	bStartPlayersAsSpectators = true;
}

void AProjAlbriteGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AProjAlbriteGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AProjAlbriteGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);

	// Check if the game is running as a listen server
	if (GetNetMode() == NM_ListenServer)
	{
		// Check if this joining player is the listen server host
		if (NewPlayerController->IsLocalController() || bIsAutoSpawn) 
		{
			UE_LOG(LogTemp, Warning, TEXT("The joining player is the host!"));
			NewPlayerController->SetShowMouseCursor(false);
			NewPlayerController->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			AAlbritePlayerController* CustomController = Cast<AAlbritePlayerController>(NewPlayerController);
			SpawnCharacter(CustomController);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Player joined, waiting for manual spawn."));
}

void AProjAlbriteGameMode::HostGame()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FString MapName = TEXT("AlbriteSandbox");

	// Open map as a listen server
	FString Command = FString::Printf(TEXT("%s?listen"), *MapName);
	World->ServerTravel(Command);
	
	UE_LOG(LogTemp, Warning, TEXT("Host has Initialized!"));
}

void AProjAlbriteGameMode::RequestSpawn(AAlbritePlayerController* PlayerController)
{
	if (HasAuthority()) // Ensure we're on the server
	{
		SpawnCharacter(PlayerController);
	}
	else
	{
		ServerRequestSpawn(PlayerController);
	}
}

void AProjAlbriteGameMode::ServerRequestSpawn_Implementation(AAlbritePlayerController* PlayerController)
{
	SpawnCharacter(PlayerController);
}

void AProjAlbriteGameMode::SpawnCharacter(AAlbritePlayerController* NewPlayerController)
{
	// Spawn the custom character
	FVector SpawnLocation;
	FRotator SpawnRotation;
	AActor* PlayerSpawnPt = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	if (PlayerSpawnPt)
	{
		SpawnLocation = PlayerSpawnPt->GetActorLocation();
		SpawnRotation = PlayerSpawnPt->GetActorRotation();
	}

	// Spawn Player Character
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACharacter* SpawnedCharacter = GetWorld()->SpawnActor<AProjAlbriteCharacter>(LobbyCharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (SpawnedCharacter)
	{
		NewPlayerController->Possess(SpawnedCharacter);
		
		AllControllers.Add(Cast<AAlbritePlayerController>(NewPlayerController));
		UE_LOG(LogTemp, Warning, TEXT("Player Joined! Total Players: %d"), AllControllers.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Failed to Join Lobby!"));
	}
}

void AProjAlbriteGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AAlbritePlayerController* PS = Cast<AAlbritePlayerController>(Exiting);
	if (!PS)
	{
		return;
	}

	AllControllers.Remove(PS);
}
