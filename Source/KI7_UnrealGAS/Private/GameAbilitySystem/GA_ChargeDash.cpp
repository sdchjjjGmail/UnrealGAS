// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GA_ChargeDash.h"
#include "AbilitySystemComponent.h"
#include "Character/TestCharacter.h"

UGA_ChargeDash::UGA_ChargeDash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_ChargeDash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_ChargeDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//if (!EffectClass)
	//{
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	//	return;
	//}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CurrentChargingTime = 0.0f;	// 차징 시간 초기화
	GetWorld()->GetTimerManager().SetTimer(
		ChargingTimerHandle,
		this,
		&UGA_ChargeDash::UpdateChargingTime,
		0.01f,
		true
	);	// 타이머를 이용해 차징 시간 업데이트 처리

	// 차징시 적용될 이팩트가 있으면 적용
	if (EffectClass && ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayEffectContextHandle Context = ActorInfo->AbilitySystemComponent->MakeEffectContext();
		Context.AddSourceObject(this);
		FGameplayEffectSpecHandle Spec = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
			EffectClass,
			GetAbilityLevel(),
			Context);
		if (Spec.IsValid())
		{
			ChargingEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
	UE_LOG(LogTemp, Log, TEXT("UGA_ChargingJump : 차징 시작"));
}

void UGA_ChargeDash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ExecuteDash(ActorInfo);	// 점프 실제 실행
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);	// 어빌리티 종료
}

void UGA_ChargeDash::UpdateChargingTime()
{
	CurrentChargingTime += 0.01f;					// 시간 업데이트(타이머가 0.01초 마다 실행)
	if (CurrentChargingTime >= MaxChargingTime)		// 최대치를 넘어서면
	{
		CurrentChargingTime = MaxChargingTime;		// 최대치로 설정
		GetWorld()->GetTimerManager().ClearTimer(ChargingTimerHandle);	// 타이머 중지
		UE_LOG(LogTemp, Log, TEXT("UGA_ChargingJump : 차징 최대치 도달"));
	}
}

void UGA_ChargeDash::ExecuteDash(const FGameplayAbilityActorInfo* ActorInfo)
{
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())	// ActorInfo와 AvatarActor확인
	{
		if (ChargingEffectHandle.IsValid() && ActorInfo->AbilitySystemComponent.IsValid())	// 이팩트가 실행중이면
		{
			ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ChargingEffectHandle);	// 이팩트 정지
			ChargingEffectHandle.Invalidate();	// 핸들 무효화
		}

		ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
		if (Character)
		{
			float ChargeRatio = FMath::Clamp(CurrentChargingTime / MaxChargingTime, 0.0f, 1.0f);	// 차지된 %
			float DashDist = FMath::Lerp(MinDashDistance, MaxDashDistance, ChargeRatio);	// 보간으로 중간값 구하기

			FVector Direction(1, 0, 0);
			Direction = Character->GetActorRotation().RotateVector(Direction);
			Character->LaunchCharacter(Direction * DashDist, false, false);
		}
	}
}