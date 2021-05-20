// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitioner.generated.h"

UCLASS()
class CUBELORD_API ALevelTransitioner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransitioner();

	UFUNCTION(BlueprintImplementableEvent)
	void BeginLevelTransition();

	UFUNCTION(BlueprintImplementableEvent)
	void EndGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level to open", meta = (AllowPrivateAccess = "true"))
	FName LevelToOpen;

	class UBoxComponent* TriggerVolume;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	AActor* Player;

	
};
