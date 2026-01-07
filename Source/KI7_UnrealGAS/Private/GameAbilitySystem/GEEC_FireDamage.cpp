// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GEEC_FireDamage.h"
#include "GameAbilitySystem/StatusAttributeSet.h"

// Tag_ElementFire 계산에 필요한 어트리뷰트를 캡처하기 위한 구조체(이 계산이 어디에 영향을 줄것인가)
struct FFireDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FFireDamageStatics()
	{
		// UStatusAttributeSet의 Health를 캡쳐하는데, Target으로부터 캡쳐. 스냅샷은 사용하지 않음
		DEFINE_ATTRIBUTE_CAPTUREDEF(UStatusAttributeSet, Health, Target, false);
	}
};

static FFireDamageStatics& FireDamageStatics()
{
	static FFireDamageStatics Statics; // FFireDamageStatics의 싱글톤 인스턴스를 반환하는 함수
	return Statics;
}

UGEEC_FireDamage::UGEEC_FireDamage()
{
	RelevantAttributesToCapture.Add(FireDamageStatics().HealthDef); // 캡쳐할 어트리뷰트 목록에 저장

	Tag_DebuffBurn = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Burn")); // 태그 미리 저장
	Tag_ElementFire = FGameplayTag::RequestGameplayTag("Element.Fire");
	Tag_DataDamage = FGameplayTag::RequestGameplayTag("Effect.Damage");
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

		// 커브테이블에서 값 가져오기
		if (DamageTable)
		{
			float EffectLevel = Spec.GetLevel();
			if (FRealCurve* DamageCurve = DamageTable->FindCurve(FName("Damage"), TEXT("UGEEC_FireDamage")))
			{
				Damage = DamageCurve->Eval(EffectLevel);
			}
		}

		//float Damage = Spec.GetSetByCallerMagnitude(Tag_DataDamage, false, 1.f); // Tag_DataDamage에 설정된 값을 가져옴
		//if (Damage <= 0.f)
		//{
		//	Damage = 1.f;
		//}

		if (SourceTags && SourceTags->HasTag(Tag_ElementFire) &&
			TargetTags && TargetTags->HasTag(Tag_DebuffBurn))
		{
			Damage *= 2.f;
		}

		if (Damage > 0.f)
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
				FireDamageStatics().HealthProperty, // Health Attribute를 변경한다
				EGameplayModOp::Additive, // 더해줄거다
				-Damage // -Damage 만큼 처리
			));
		}
	}
}
