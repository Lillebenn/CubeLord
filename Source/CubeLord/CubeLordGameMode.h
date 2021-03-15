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

	APlayerController* PC;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bGamePaused{ false };

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PauseGameMenu(bool GamePause);

public:
	ACubeLordGameMode();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void PauseGameFunc();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void ResetLevelFunc();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void QuitGameFunc();
};



