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


	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SetupGame();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PauseGameMenu(bool GamePause);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void EnableMouseControl(bool bEnableMouse);

public:
	ACubeLordGameMode();

	UFUNCTION(BlueprintImplementableEvent)
	void GoToTitleScreen(bool bGoToTitleScreen);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void TitleScreen(bool bTitleScreen);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void StartGame();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void OptionsMenu();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void PauseGameFunc();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void ResetLevelFunc();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void QuitGameFunc();

};



