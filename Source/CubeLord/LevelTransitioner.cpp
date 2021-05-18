// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitioner.h"
#include "Components/BoxComponent.h"
#include "Albert_Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALevelTransitioner::ALevelTransitioner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = TriggerVolume;
}

// Called when the game starts or when spawned
void ALevelTransitioner::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitioner::OnOverlapBegin);

	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void ALevelTransitioner::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor == Player && OtherComponent->IsA(UCapsuleComponent::StaticClass()))
	{
		BeginLevelTransition();
	}
}

// Called every frame
void ALevelTransitioner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

