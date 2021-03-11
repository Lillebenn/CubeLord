// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CubeLordGameMode.generated.h"

UCLASS(minimalapi)
class ACubeLordGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UWorld* World;
	bool bGamePaused{ false };


public:
	ACubeLordGameMode();

	void ResetLevel();
	void PauseGame();
};



