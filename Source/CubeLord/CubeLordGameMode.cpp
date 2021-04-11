// Copyright Epic Games, Inc. All Rights Reserved.

#include "CubeLordGameMode.h"
#include "CubeLordCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACubeLordGameMode::ACubeLordGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }


}

void ACubeLordGameMode::TitleScreen(bool bTitleScreen) 
{
	UE_LOG(LogTemp, Warning, TEXT("Titlescreen"));

	
	if (bTitleScreen)
	{
		if (World)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Going to titlescreen!!"));
			UGameplayStatics::OpenLevel(World, "TitleScreen", false);
			// GoToTitleScreen(true);
			// EnableMouseControl(true);
			return;
		}
	}
}

void ACubeLordGameMode::StartGame() 
{
	UE_LOG(LogTemp, Warning, TEXT("Start the game"));
	// TitleScreen(false);
	if (PC)
	{
		EnableMouseControl(false);
	}
	if (World)
	{
		// UGameplayStatics::OpenLevel(World, "CharacterMovement_Testing_Map");	//	Testing Purposes Only
		UGameplayStatics::OpenLevel(World, "Level_1_E");
	}
}

void ACubeLordGameMode::OptionsMenu() 
{
	UE_LOG(LogTemp, Display, TEXT("Go to options menu"));
}

void ACubeLordGameMode::PauseGameFunc() 
{

	if (!bGamePaused)
	{
		UGameplayStatics::SetGamePaused(World, true);
		bGamePaused = true;
		PauseGameMenu(bGamePaused);
		if(PC)
		{
			EnableMouseControl(true);
		}
		return;
	}
	else
	{
		UGameplayStatics::SetGamePaused(World, false);
		bGamePaused = false;
		PauseGameMenu(bGamePaused);
		if(PC)
		{
			EnableMouseControl(false);
		}
		return;
	}
}

void ACubeLordGameMode::ResetLevelFunc() 
{
	
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

void ACubeLordGameMode::EnableMouseControl(bool bEnableMouse) 
{
	PC->bShowMouseCursor = bEnableMouse;
	PC->bEnableClickEvents = bEnableMouse;
	PC->bEnableMouseOverEvents = bEnableMouse;
	UE_LOG(LogTemp, Warning, TEXT("Mouse enabled is: %i"), bEnableMouse);
}

void ACubeLordGameMode::SetupGame() 
{
	World = GetWorld();
	PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(World, 0));

	UE_LOG(LogTemp, Warning, TEXT("Game is setup"));
}
