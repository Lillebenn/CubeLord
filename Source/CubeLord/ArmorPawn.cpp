// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "CubePawn.h"
#include "Components/CapsuleComponent.h"
#include "SmallGate.h"
#include "Albert_Character.h"

// Sets default values
AArmorPawn::AArmorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	ParticleSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("ParticleSpawn"));
	ParticleSpawn->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AArmorPawn::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AArmorPawn::OnOverlap);

	if (GateActor)
	{
		Gate = Cast<ASmallGate>(GateActor);
	}
}

// Called every frame
void AArmorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArmorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AArmorPawn::HandleDestruction() 
{
	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, ParticleSpawn->GetComponentLocation());
	}
	Mesh->DestroyComponent();

	if (Gate != nullptr)
	{
		Gate->HandleDestruction();
	}
	
	DestroyArmor();
}

void AArmorPawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor->IsA(ACubePawn::StaticClass()))
	{
		HandleDestruction();
	}
	if (OtherActor->IsA(AAlbert_Character::StaticClass()) && OtherComponent->IsA(UCapsuleComponent::StaticClass()))
	{
		Albert = Cast<AAlbert_Character>(OtherActor);
		Albert->Death();
		UE_LOG(LogTemp, Warning, TEXT("Albert will die"));
	}
}
