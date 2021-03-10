// Fill out your copyright notice in the Description page of Project Settings.


#include "Albert_Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AAlbert_Character::AAlbert_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	GetCapsuleComponent()->SetupAttachment(Root);
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 92.0f);
	GetCapsuleComponent()->SetWorldLocation(FVector(0.0f, 0.0f, 92.0f));

	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(Root);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(CameraRoot);
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//	Don't rotate when the controller rotates. 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//	Kopiert rett fra UE sin default 3rd person character
	GetCharacterMovement()->bOrientRotationToMovement = true;	//	Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);	// ... at this rotation rate
}

// Called when the game starts or when spawned
void AAlbert_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlbert_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCamera();
	RotateCamera();

	CameraParentRotation = CameraRoot->GetComponentRotation();
}

// Called to bind functionality to input
void AAlbert_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AAlbert_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAlbert_Character::MoveRight);
}

void AAlbert_Character::MoveForward(float Value) 
{
	if ((Controller != NULL) && (Value != 0.f))
	{
		//	Finds out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw + CameraParentRotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAlbert_Character::MoveRight(float Value) 
{
	if ((Controller != NULL) && (Value != 0.f))
	{
		//	Finds out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw + CameraParentRotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


void AAlbert_Character::RotateCamera() 
{
	FRotator Rotation = CameraRoot->GetComponentRotation();
	Rotation.Yaw = TargetYaw;
	CameraRoot->SetWorldRotation(Rotation);
}


void AAlbert_Character::MoveCamera() 
{
	CameraRoot->SetRelativeLocation(CamLocation);
}
