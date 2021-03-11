// Copyright Epic Games, Inc. All Rights Reserved.

#include "CubeLordGameMode.h"
#include "CubeLordCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ACubeLordGameMode::ACubeLordGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACubeLordGameMode::ResetLevel() 
{
	World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, *World->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Level Reset"));
	}
}

void ACubeLordGameMode::PauseGame() 
{
	World = GetWorld();
	
	if (!bGamePaused)
	{
		UGameplayStatics::SetGamePaused(World, true);
		bGamePaused = true;
		UE_LOG(LogTemp, Warning, TEXT("Pause Game"));
		return;
	}
	else
	{
		UGameplayStatics::SetGamePaused(World, false);
		bGamePaused = false;
		UE_LOG(LogTemp, Warning, TEXT("Resume Game"));
		return;
	}
}
