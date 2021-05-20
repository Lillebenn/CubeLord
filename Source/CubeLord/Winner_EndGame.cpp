// Fill out your copyright notice in the Description page of Project Settings.


#include "Winner_EndGame.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AWinner_EndGame::AWinner_EndGame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = Box;
}

// Called when the game starts or when spawned
void AWinner_EndGame::BeginPlay()
{
	Super::BeginPlay();

	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void AWinner_EndGame::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor == Player && OtherComponent->IsA(UCapsuleComponent::StaticClass()))
	{
		EndGame();
	}
}

// Called every frame
void AWinner_EndGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

