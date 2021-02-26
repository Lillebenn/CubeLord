// Copyright Epic Games, Inc. All Rights Reserved.

#include "CubeLordGameMode.h"
#include "CubeLordCharacter.h"
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
