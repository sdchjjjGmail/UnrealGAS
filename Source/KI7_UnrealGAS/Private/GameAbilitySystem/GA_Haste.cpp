// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GA_Haste.h"
#include "AbilitySystemComponent.h"

UGA_Haste::UGA_Haste()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Haste::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (!EffectClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
		EffectClass, GetAbilityLevel(), EffectContext);

	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
