// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/StatAttributeSet.h"
#include "GameplayEffectExtension.h"

UStatAttributeSet::UStatAttributeSet()
{
	InitStrengh(BasicStatValue);
	InitAgility(BasicStatValue);
	InitInteligence(BasicStatValue);

	InitMoveSpeed(500.0f);
	InitJumpPower(1000.0f);
}

void UStatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetStrenghAttribute())
	{

	}
	else if (Attribute == GetAgilityAttribute())
	{

	}
	else if (Attribute == GetInteligenceAttribute())
	{

	}
}

void UStatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetStrenghAttribute())
	{

	}
	else if (Data.EvaluatedData.Attribute == GetAgilityAttribute())
	{

	}
	else if (Data.EvaluatedData.Attribute == GetInteligenceAttribute())
	{

	}
}
