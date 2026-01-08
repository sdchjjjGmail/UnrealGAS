// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GA_SuperJump.h"
#include "AbilitySystemComponent.h"
#include "Character/TestCharacter.h"

UGA_SuperJump::UGA_SuperJump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_SuperJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bOk = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	UE_LOG(LogTemp, Log, TEXT("CanActivateAbility, %d"), bOk);
	return bOk;
}

void UGA_SuperJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("ActivateAbility"));

	//if (!EffectClass)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("No class"));

	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	//	return;
	//}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Log, TEXT("Ability Fail"));

		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_SuperJump::InputReleased(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	UE_LOG(LogTemp, Log, TEXT("UGA_SuperJump::InputReleased"));

	//UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	//FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	//EffectContext.AddSourceObject(this);

	//FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
	//	EffectClass, GetAbilityLevel(), EffectContext);

	//if (SpecHandle.IsValid())
	{
		//ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		if (ActorInfo && ActorInfo->AvatarActor.IsValid())
		{
			if (ACharacter* Character = Cast<ATestCharacter>(ActorInfo->AvatarActor.Get()))
			{
				const FVector Forward = Character->GetActorForwardVector();
				const FVector LaunchVel = Forward * 2500.f + FVector(0.f, 0.f, 2000.f);
				Character->LaunchCharacter(LaunchVel, true, true);
			}
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
