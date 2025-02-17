// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Temp_Door.generated.h"

UCLASS()
class CUBELORD_API ATemp_Door : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;

	void OpenDoor(int32 Counter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Open", meta = (AllowPrivateAccess = "true"));
	int32 PressurePlatesNeededToOpen;

	int32 DoorCounter{ 0 };

	float DoorYaw{ 0.0f };
	float CurrentYaw;

public:	
	// Sets default values for this actor's properties
	ATemp_Door();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PressurePlateEffectStart();
	void PressurePlateEffectEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
