// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "TestCharacter.generated.h"

class UStatusAttributeSet;
class UStatAttributeSet;
class UWidgetComponent;
class UGameplayEffect;

UCLASS()
class KI7_UNREALGAS_API ATestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComponent;
	};

	UFUNCTION(BlueprintCallable)
	void TestHealthChange(float Amount);

	UFUNCTION(BlueprintCallable)
	void TestSetByCaller(float InAmount);

	UFUNCTION(BlueprintCallable)
	void TestAddInfiniteEffect();

	UFUNCTION(BlueprintCallable)
	void TestRemoveInfiniteEffect();

	UFUNCTION(BlueprintCallable)
	void TestSetMoveSpeedEffect();

	UFUNCTION(BlueprintCallable)
	void TestSetJumpPowerEffect();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnHealthChange(const FOnAttributeChangeData& InData);
	void OnManaChange(const FOnAttributeChangeData& InData);
	void OnMaxHealthChange(const FOnAttributeChangeData& InData);

	void OnMoveSpeedChange(const FOnAttributeChangeData& InData);
	void OnJumpPowerChange(const FOnAttributeChangeData& InData);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float TestValue = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<class UGameplayEffect> TestEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<class UGameplayEffect> TestInfiniteEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<class UGameplayEffect> MoveSpeedEffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	TSubclassOf<class UGameplayEffect> JumpPowerEffectClass = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UWidgetComponent> BarWigetComponent = nullptr;
	
private:
	UPROPERTY()
	TObjectPtr<UStatusAttributeSet> StatusAttributeSet = nullptr;

	UPROPERTY()
	TObjectPtr<UStatAttributeSet> StatAttributeSet = nullptr;

	FGameplayTag Tag_EffectDamage;

	FActiveGameplayEffectHandle TestInfinite;
};
