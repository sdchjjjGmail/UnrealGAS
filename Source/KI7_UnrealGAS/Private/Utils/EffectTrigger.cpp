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

	UAbilitySystemComponent*  TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	if (TargetASC)
	{
		//UE_LOG(LogTemp, Log, TEXT("ASC OK : %s"), *OtherActor->GetActorLabel());
		FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
		ContextHandle.AddInstigator(this, this);

		// Spec 생성
		FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(TrigeredEffectClass, 1.0f, ContextHandle);

		if (SpecHandle.IsValid())
		{
			// 이펙트 적용 및 핸들 반환
			FActiveGameplayEffectHandle ActiveGEHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			// 나중에 제거하기 위해 맵에 저장
			ActiveEffectHandles.Add(TargetASC, ActiveGEHandle);

			UE_LOG(LogTemp, Log, TEXT("Entered Damage Zone: %s"), *OtherActor->GetName());
		}
	}
}

void AEffectTrigger::EndEffect(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("EndEffect"));

	UAbilitySystemComponent*  TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	if (TargetASC)
	{
		// 이 ASC가 우리 맵에 있는지 확인 (우리가 건 이펙트가 있는지)
		if (ActiveEffectHandles.Contains(TargetASC))
		{
			// 저장해둔 핸들을 찾아서
			FActiveGameplayEffectHandle HandleToRemove = ActiveEffectHandles[TargetASC];

			// 이펙트 제거
			TargetASC->RemoveActiveGameplayEffect(HandleToRemove);

			// 맵에서 삭제
			ActiveEffectHandles.Remove(TargetASC);

			UE_LOG(LogTemp, Log, TEXT("Exited Damage Zone: %s"), *OtherActor->GetName());
		}
	}
}


