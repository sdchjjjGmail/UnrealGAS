// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeAccessors.h"
#include "StatusAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALGAS_API UStatusAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UStatusAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, MaxMana)
};
