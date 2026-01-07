// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_FireDamage.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UGEEC_FireDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEEC_FireDamage();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomData")
	float MinimumDamage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomData")
	float CriticalDamageMultiplier = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CustomData")
	TObjectPtr<UCurveTable> DamageTable = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CustomData")
	TObjectPtr<UCurveTable> DamageMultiplierTable = nullptr;

private:
	FGameplayTag Tag_DebuffBurn;
	FGameplayTag Tag_ElementFire;

	FGameplayTag Tag_DebuffFrozen;
	FGameplayTag Tag_ElementIce;
};
