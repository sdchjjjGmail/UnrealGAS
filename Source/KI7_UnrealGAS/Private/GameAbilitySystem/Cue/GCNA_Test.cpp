// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/Cue/GCNA_Test.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AGCNA_Test::AGCNA_Test()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.TestActor"));
	bAutoDestroyOnRemove = true;
}

bool AGCNA_Test::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (SpawnedVfs.IsValid())
	{
		SpawnedVfs->Deactivate();
		SpawnedVfs = nullptr;
	}

	if (MyTarget)
	{
		SpawnedVfs = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TestVfx,
			MyTarget->GetActorLocation(),
			MyTarget->GetActorRotation());
		return true;
	}
	return false;
}

bool AGCNA_Test::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (SpawnedVfs.IsValid())
	{
		SpawnedVfs->Deactivate();
		SpawnedVfs = nullptr;
		return true;
	}
	return false;
}
