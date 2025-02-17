// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"
#include "Temp_Door.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "CubePawn.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	TriggerVolume->SetupAttachment(RootComponent);
	TriggerVolume->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	TriggerVolume->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

	Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
	Ground->SetupAttachment(RootComponent);

	Plate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate"));
	Plate->SetupAttachment(Ground);
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	PlateZ = PlateZMax;
	PlayerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapEnd);
	
	if (AffectingActor)
	{
		Door = Cast<ATemp_Door>(AffectingActor);
	}
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	Animerer plata på pressure plate
	PlateAnimation(DeltaTime);
}

void APressurePlate::PlateAnimation(float Deltatime) 
{
	PlateCurrentZ = FMath::FInterpTo(PlateCurrentZ, PlateZ, Deltatime, 10.0f);
	FVector PlateLocation{ 0.0f, 0.0f, 0.0f };
	PlateLocation.Z = PlateCurrentZ;
	Plate->SetRelativeLocation(PlateLocation);
}

void APressurePlate::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor == PlayerActor && OtherComp->IsA(UCapsuleComponent::StaticClass()) || OtherActor->IsA(ACubePawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Collision"));
		if (AffectingActor)
		{
			Door->PressurePlateEffectStart();
			PlateZ = PlateZMin;
			UE_LOG(LogTemp, Warning, TEXT("Start"));
			PlayPressurePlateFX();
		}
	}
}

void APressurePlate::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (OtherActor == PlayerActor && OtherComp->IsA(UCapsuleComponent::StaticClass()) || OtherActor->IsA(ACubePawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Collision END"));
		if (AffectingActor)
		{
			Door->PressurePlateEffectEnd();
			PlateZ = PlateZMax;
		}
	}
}

