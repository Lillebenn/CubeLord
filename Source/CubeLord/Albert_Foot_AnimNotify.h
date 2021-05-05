// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Albert_Foot_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class CUBELORD_API UAlbert_Foot_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* Particle_Stone{ nullptr };
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* Particle_Wood{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess))
	USoundBase* FootStep_Stone;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess))
	USoundBase* FootStep_Wood;

	class AAlbert_Character* Albert{ nullptr };

	void PlaySoundnEffects(USkeletalMeshComponent* MeshComp, USoundBase* Sound, UParticleSystem* Particle);

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
