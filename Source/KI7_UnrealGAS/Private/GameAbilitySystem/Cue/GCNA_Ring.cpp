// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Cue/GCNA_Ring.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/TestCharacter.h"

AGCNA_Ring::AGCNA_Ring()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Actor"));
	bAutoDestroyOnRemove = true;
}

bool AGCNA_Ring::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("OnActive_Implementation"));

	if (SpawnedVfs.IsValid())
	{
		SpawnedVfs->Deactivate();
		SpawnedVfs = nullptr;
	}

	if (MyTarget)
	{
		SpawnedVfs = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			RingVfx,
			MyTarget->GetActorLocation(),
			MyTarget->GetActorRotation());
		return true;
	}

	if (MyTarget)
	{
		ATestCharacter* TC = Cast<ATestCharacter>(MyTarget);
		if (!TC)
		{
			UE_LOG(LogTemp, Warning, TEXT("No TC"));

			return false;
		}

		USkeletalMeshComponent* Mesh = TC->GetMySkeletalMesh();
		if (!Mesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Mesh"));

			return false;
		}

		static const FName SocketName(TEXT("Head"));
		if (!Mesh->DoesSocketExist(SocketName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Socket not found: %s"), *SocketName.ToString());
			return false;
		}

		SpawnedVfs = UNiagaraFunctionLibrary::SpawnSystemAttached(
			RingVfx,
			Mesh,
			SocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false
		);
		return true;
	}

	return false;
}

bool AGCNA_Ring::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (SpawnedVfs.IsValid())
	{
		SpawnedVfs->Deactivate();
		SpawnedVfs = nullptr;
		return true;
	}
	return false;
}
