// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ChargeDash.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UGA_ChargeDash : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_ChargeDash();

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	) override;

protected:
	// 차징 시간을 업데이트 하는 함수(타이머에서 연결)
	UFUNCTION()
	void UpdateChargingTime();

	// 실제 점프 처리를 하는 함수
	void ExecuteDash(const FGameplayAbilityActorInfo* ActorInfo);

protected:
	// 최소 대시 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|ChargingJump")
	float MinDashDistance = 1000.0f;

	// 최대 대시 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|ChargingJump")
	float MaxDashDistance = 4000.0f;

	// 최대 차징 가능한 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|ChargingJump")
	float MaxChargingTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|ChargeDash")
	TSubclassOf<UGameplayEffect> EffectClass = nullptr;

private:
	// 차징 중에 적용될 이팩트의 핸들(나중에 끄기 위해서)
	FActiveGameplayEffectHandle ChargingEffectHandle;

	// 현재 차징된 시간
	float CurrentChargingTime = 0.0f;

	// 차징 시간 업데이트용 타이머 핸들
	FTimerHandle ChargingTimerHandle;
};
