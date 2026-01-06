// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/EffectTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include <AbilitySystemBlueprintLibrary.h>

// Sets default values
AEffectTrigger::AEffectTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MyMesh->SetupAttachment(GetRootComponent());

	TrigerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TrigerArea"));
	TrigerArea->SetupAttachment(MyMesh);
	TrigerArea->InitBoxExtent(FVector(500.f, 500.f, 100.f));
}

// Called when the game starts or when spawned
void AEffectTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (TrigerArea)
	{
		TrigerArea->OnComponentBeginOverlap.AddDynamic(this, &AEffectTrigger::StartEffect);
		TrigerArea->OnComponentEndOverlap.AddDynamic(this, &AEffectTrigger::EndEffect);
	}
}

void AEffectTrigger::StartEffect(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("StartEffect"));

	if (!TrigeredEffectClass || !OtherActor) return;

	TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	if (!TargetASC) return;

	FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
	Context.AddInstigator(this, this);

	FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(TrigeredEffectClass, 1.f, Context);
	if (Spec.IsValid())
	{
		EffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void AEffectTrigger::EndEffect(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("EndEffect"));
	if (EffectHandle.IsValid() && TargetASC)
	{
		TargetASC->RemoveActiveGameplayEffect(EffectHandle);
	}
}


