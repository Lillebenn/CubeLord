// Fill out your copyright notice in the Description page of Project Settings.


#include "Albert_Foot_AnimNotify.h"
#include "Engine.h"
#include "Albert_Character.h"
#include "Kismet/GameplayStatics.h"

#define COLLISION_FLOOR ECC_GameTraceChannel3

void UAlbert_Foot_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
    if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
    {
        Albert = Cast<AAlbert_Character>(MeshComp->GetOwner());
        if (Albert != NULL)
        {
            FName HitResult = Albert->RayTraceFromSocket(10.0f, SocketName);
            UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.ToString());
            if (HitResult == "Block")
            {
                PlaySoundnEffects(MeshComp, FootStep_Stone, Particle_Stone);
            }
            if (HitResult == "None")
            {
                PlaySoundnEffects(MeshComp, FootStep_Wood, Particle_Wood);
            }
        }
    }
}

void UAlbert_Foot_AnimNotify::PlaySoundnEffects(USkeletalMeshComponent* MeshComp, USoundBase* Sound, UParticleSystem* Particle) 
{
    UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), Sound, MeshComp->GetComponentLocation());   //  Sound
    UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), Particle, MeshComp->GetSocketLocation(SocketName));  //  Particle
}

