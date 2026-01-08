// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"

/**
 * 
 */

 // 어트리뷰트 Getter/Setter/Initializer를 쉽게 만들어주는 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

