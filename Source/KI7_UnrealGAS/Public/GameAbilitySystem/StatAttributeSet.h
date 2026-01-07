// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeAccessors.h"
#include "StatAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UStatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UStatAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Strengh;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, Strengh)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, Agility)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Inteligence;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, Inteligence)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CriticalRate;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, CriticalRate)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData JumpPower;
	ATTRIBUTE_ACCESSORS(UStatAttributeSet, JumpPower)

private:
	float BasicStatValue = 5.0f;
};
