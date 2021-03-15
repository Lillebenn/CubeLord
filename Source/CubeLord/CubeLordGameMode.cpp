// Copyright Epic Games, Inc. All Rights Reserved.

#include "CubeLordGameMode.h"
#include "CubeLordCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACubeLordGameMode::ACubeLordGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void ACubeLordGameMode::PauseGameFunc() 
{
	World = GetWorld();
	PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(World, 0));

	
	if (!bGamePaused)
	{
		UGameplayStatics::SetGamePaused(World, true);
		bGamePaused = true;
		// UE_LOG(LogTemp, Warning, TEXT("Pause Game: %i"), bGamePaused);
		PauseGameMenu(bGamePaused);
		if(PC)
		{
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
		}
		return;
	}
	else
	{
		UGameplayStatics::SetGamePaused(World, false);
		bGamePaused = false;
		// UE_LOG(LogTemp, Warning, TEXT("Pause Game: %i"), bGamePaused);
		PauseGameMenu(bGamePaused);
		if(PC)
		{
			PC->bShowMouseCursor = false;
			PC->bEnableClickEvents = false;
			PC->bEnableMouseOverEvents = false;
		}
		return;
	}
}

void ACubeLordGameMode::ResetLevelFunc() 
{
	World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, *World->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Level Reset"));
	}
}

void ACubeLordGameMode::QuitGameFunc() 
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

