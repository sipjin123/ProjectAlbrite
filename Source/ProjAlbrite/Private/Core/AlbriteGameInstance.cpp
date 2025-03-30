// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AlbriteGameInstance.h"
#include "SocketSubsystem.h"

void UAlbriteGameInstance::ClientJoinHostLobby_Implementation(const FString& ServerAddress)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	FString Command = FString::Printf(TEXT("open %s"), *ServerAddress);
	PlayerController->ClientTravel(Command, TRAVEL_Absolute);
}

FString UAlbriteGameInstance::GetLocalIPAddress()
{
	bool bCanBind = false;
	TSharedPtr<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);

	if (LocalAddr.IsValid())
	{
		return LocalAddr->ToString(false); // false removes port number
	}

	return TEXT("Unable to get Local IP");
}
