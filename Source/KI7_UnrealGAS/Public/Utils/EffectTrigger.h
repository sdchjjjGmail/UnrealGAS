// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "EffectTrigger.generated.h"

UCLASS()
class KI7_UNREALGAS_API AEffectTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEffectTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartEffect(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void EndEffect(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<class UGameplayEffect> TrigeredEffectClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> MyMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> TrigerArea = nullptr;

private:
	TMap<class UAbilitySystemComponent*, FActiveGameplayEffectHandle> ActiveEffectHandles;
};
