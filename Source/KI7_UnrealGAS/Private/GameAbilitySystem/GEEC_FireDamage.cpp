// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GEEC_FireDamage.h"
#include "GameAbilitySystem/StatusAttributeSet.h"
#include "GameAbilitySystem/StatAttributeSet.h"

// Tag_ElementFire 계산에 필요한 어트리뷰트를 캡처하기 위한 구조체(이 계산이 어디에 영향을 줄것인가)
struct FFireDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);

	FFireDamageStatics()
	{
		// UStatusAttributeSet의 Health를 캡쳐하는데, Target으로부터 캡쳐. 스냅샷은 사용하지 않음
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStatusAttributeSet, Health, Target, false);

		// UStatAttributeSet의 AttackPower를 캡쳐, 공격자로부터 캡쳐. 공격 시점의 값을 가져오기
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, AttackPower, Source, true);

		// UStatAttributeSet의 CriticalRate를 캡쳐, 공격자로부터 캡쳐. 공격 시점의 값을 가져오기
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStatAttributeSet, CriticalRate, Source, true);
	}
};

static FFireDamageStatics& FireDamageStatics()
{
	static FFireDamageStatics Statics; // FFireDamageStatics의 싱글톤 인스턴스를 반환하는 함수
	return Statics;
}

UGEEC_FireDamage::UGEEC_FireDamage()
{
	// 캡쳐할 어트리뷰트 목록에 저장
	RelevantAttributesToCapture.Add(FireDamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(FireDamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(FireDamageStatics().CriticalRateDef);

	Tag_DebuffBurn = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Burn")); // 태그 미리 저장
	Tag_ElementFire = FGameplayTag::RequestGameplayTag("Element.Fire");

	Tag_DebuffFrozen = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Frozen"));
	Tag_ElementIce = FGameplayTag::RequestGameplayTag("Element.Ice");
}

void UGEEC_FireDamage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (TargetASC && SourceASC)
	{
		const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

		const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags(); // 소스의 태그 가져오기
		const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags(); // 타깃의 태그 가져오기

		float Damage = MinimumDamage;
		float DamageMultiplier = 1.f;

		// 커브테이블에서 값 가져오기
		if (DamageTable)
		{
			float EffectLevel = Spec.GetLevel();
			if (FRealCurve* DamageCurve = DamageTable->FindCurve(FName("Damage"), TEXT("UGEEC_FireDamage")))
			{
				Damage = DamageCurve->Eval(EffectLevel);
			}
		}
		if (DamageMultiplierTable)
		{
			float EffectLevel = Spec.GetLevel();
			if (FRealCurve* DamageCurve = DamageMultiplierTable->FindCurve(FName("Multiplier"), TEXT("UGEEC_FireDamage")))
			{
				DamageMultiplier = DamageCurve->Eval(EffectLevel);
			}
		}

		FAggregatorEvaluateParameters EvaluateParams;
		EvaluateParams.SourceTags = SourceTags;
		EvaluateParams.TargetTags = TargetTags;

		float AttackPower = 0.f;
		bool GetAttackPowerResult = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			FireDamageStatics().AttackPowerDef,
			EvaluateParams,
			AttackPower);

		if (GetAttackPowerResult)
		{
			// 정상적으로 값을 가지고 있다
			Damage += AttackPower;
		}
		else
		{
			// 값을 가져오지 못했다
		}

		//float Damage = Spec.GetSetByCallerMagnitude(Tag_DataDamage, false, 1.f); // Tag_DataDamage에 설정된 값을 가져옴
		//if (Damage <= 0.f)
		//{
		//	Damage = 1.f;
		//}

		if (SourceTags && SourceTags->HasTag(Tag_ElementFire) &&
			TargetTags && TargetTags->HasTag(Tag_DebuffBurn))
		{
			Damage *= DamageMultiplier;
		}

		else if (SourceTags && SourceTags->HasTag(Tag_ElementIce) &&
			TargetTags && TargetTags->HasTag(Tag_DebuffFrozen))
		{
			UE_LOG(LogTemp, Log, TEXT("Ice Attack!!"));
			Damage *= DamageMultiplier;
		}

		UE_LOG(LogTemp, Log, TEXT("Current Damage : %.2f"), Damage);

		float CriticalRate = 0.f;
		bool GetCriticalRateResult = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			FireDamageStatics().CriticalRateDef,
			EvaluateParams,
			CriticalRate);

		if (GetCriticalRateResult)
		{
			// 정상적으로 값을 가지고 있다
			CriticalRate += CriticalRate;
		}
		else
		{
			// 값을 가져오지 못했다
		}

		if (FMath::FRandRange(0.f, 99.f) >= 50.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("Critical!!"));
			Damage *= CriticalDamageMultiplier;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Normal Attack"));
		}
		UE_LOG(LogTemp, Log, TEXT("Final Damage : %.2f"), Damage);

		if (Damage > 0.f)
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
				FireDamageStatics().HealthProperty,
				EGameplayModOp::Additive,
				-Damage
			));
		}
	}
}
