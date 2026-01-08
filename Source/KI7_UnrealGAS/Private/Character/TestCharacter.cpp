// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystem/StatusAttributeSet.h"
#include "GameAbilitySystem/StatAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/TwinResource.h"
#include "GameplayEffect.h"
#include "EnhancedInputComponent.h"
#include "Utils/AbilityEnums.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BarWigetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	BarWigetComponent->SetupAttachment(RootComponent);

	// 컴포넌트 생성
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// 어트리뷰트 셋 생성
	StatusAttributeSet = CreateDefaultSubobject<UStatusAttributeSet>(TEXT("Status"));
	StatAttributeSet = CreateDefaultSubobject<UStatAttributeSet>(TEXT("Stat"));
}

void ATestCharacter::TestHealthChange(float Amount)
{
	if (StatusAttributeSet)
	{
		float CurrentValue = StatusAttributeSet->GetHealth();
		StatusAttributeSet->SetHealth(CurrentValue + Amount);
	}
}

void ATestCharacter::TestSetByCaller(float InAmount)
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(TestEffectClass, 0, EffectContext);

		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(Tag_EffectDamage, InAmount);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATestCharacter::TestAddInfiniteEffect()
{
	if (TestInfiniteEffectClass && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddInstigator(this, this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(TestInfiniteEffectClass, 0, EffectContext);
		if (SpecHandle.IsValid())
		{
			TestInfinite = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATestCharacter::TestRemoveInfiniteEffect()
{
	if (TestInfinite.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(TestInfinite);
	}
}

void ATestCharacter::TestSetMoveSpeedEffect()
{
	if (MoveSpeedEffectClass && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddInstigator(this, this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(MoveSpeedEffectClass, 0, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATestCharacter::TestSetJumpPowerEffect()
{
	if (JumpPowerEffectClass && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddInstigator(this, this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(JumpPowerEffectClass, 0, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATestCharacter::TestAbility()
{
	if (AbilitySystemComponent && HasteClass)
	{
		// 클래스로 어빌리티 발동시키기
		AbilitySystemComponent->TryActivateAbilityByClass(HasteClass);
	}
}

void ATestCharacter::TestSuperJumpAbility()
{
	if (AbilitySystemComponent && SuperJumpClass)
	{
		// 클래스로 어빌리티 발동시키기
		AbilitySystemComponent->TryActivateAbilityByClass(SuperJumpClass);
	}
}

USkeletalMeshComponent* ATestCharacter::GetMySkeletalMesh()
{
	return GetMesh();
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	// 어빌리티 시스템 컴포넌트 초기화

		if (HasteClass)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					HasteClass, // 어빌리티 클래스
					1, // 레벨
					static_cast<int32>(EAbilityInputID::Haste), // 입력 ID
					this // 소스
				)
			);
		}

		if (SuperJumpClass)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					SuperJumpClass, // 어빌리티 클래스
					1, // 레벨
					static_cast<int32>(EAbilityInputID::SuperJump), // 입력 ID
					this // 소스
				)
			);
		}

		if (ChargeDashClass)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					ChargeDashClass, // 어빌리티 클래스
					1, // 레벨
					static_cast<int32>(EAbilityInputID::ChargeDash), // 입력 ID
					this // 소스
				)
			);
		}

		// 초기화 이후에만 가능
		FOnGameplayAttributeValueChange& onHealthChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetHealthAttribute());
		onHealthChange.AddUObject(this, &ATestCharacter::OnHealthChange);	// Health가 변경되었을 때 실행될 함수 바인딩

		FOnGameplayAttributeValueChange& onManaChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetManaAttribute());
		onManaChange.AddUObject(this, &ATestCharacter::OnManaChange);	// Mana가 변경되었을 때 실행될 함수 바인딩

		FOnGameplayAttributeValueChange& onMaxHealthChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatusAttributeSet::GetMaxHealthAttribute());
		onMaxHealthChange.AddUObject(this, &ATestCharacter::OnMaxHealthChange);	// Mana가 변경되었을 때 실행될 함수 바인딩

		FOnGameplayAttributeValueChange& onMoveSpeedChange =
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetMoveSpeedAttribute());
		onMoveSpeedChange.AddUObject(this, &ATestCharacter::OnMoveSpeedChange);

		//FOnGameplayAttributeValueChange& onJumpPowerChange =
		//	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStatAttributeSet::GetJumpPowerAttribute());
		//onJumpPowerChange.AddUObject(this, &ATestCharacter::OnJumpPowerChange);
	}

	if (StatusAttributeSet)
	{
		if (BarWigetComponent && BarWigetComponent->GetWidget())
		{
			if (BarWigetComponent->GetWidget()->Implements<UTwinResource>())
			{
				ITwinResource::Execute_UpdateMaxHealth(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMaxHealth());
				ITwinResource::Execute_UpdateCurrentHealth(BarWigetComponent->GetWidget(), StatusAttributeSet->GetHealth());

				ITwinResource::Execute_UpdateMaxMana(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMaxMana());
				ITwinResource::Execute_UpdateCurrentMana(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMana());

				ITwinResource::Execute_UpdateMaxHealth(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMaxHealth());
			}
		}
		//StatusAttributeSet->Health = 50.0f;	// 절대 안됨
		//StatusAttributeSet->SetHealth(50.0f);	// 무조건 Setter로 변경해야 한다.
	}

	Tag_EffectDamage = FGameplayTag::RequestGameplayTag(FName("Effect.Damage"));
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StatusAttributeSet)
	{
		FString healthString = FString::Printf(TEXT("%.1f / %.1f"),
			StatusAttributeSet->GetHealth(), StatusAttributeSet->GetMaxHealth());
		DrawDebugString(GetWorld(), GetActorLocation(), healthString, nullptr, FColor::White, 0, true);
	}
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(IA_Ability01, ETriggerEvent::Started, this, &ATestCharacter::OnAbility01Press);

		EnhancedInput->BindAction(IA_Ability02, ETriggerEvent::Started, this, &ATestCharacter::OnAbility02Press);
		EnhancedInput->BindAction(IA_Ability02, ETriggerEvent::Completed, this, &ATestCharacter::OnAbility02Release);

		EnhancedInput->BindAction(IA_AbilityDash, ETriggerEvent::Started, this, &ATestCharacter::OnAbilityDashPress);
		EnhancedInput->BindAction(IA_AbilityDash, ETriggerEvent::Completed, this, &ATestCharacter::OnAbilityDashRelease);
	}
}

void ATestCharacter::OnHealthChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("On Health Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateCurrentHealth(BarWigetComponent->GetWidget(), StatusAttributeSet->GetHealth());
}

void ATestCharacter::OnManaChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("On Mana Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateCurrentMana(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMana());
}

void ATestCharacter::OnMaxHealthChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("On Max Health Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
	ITwinResource::Execute_UpdateMaxHealth(BarWigetComponent->GetWidget(), StatusAttributeSet->GetMaxHealth());
	//StatusAttributeSet->SetHealth(StatusAttributeSet->GetHealth());
}

void ATestCharacter::OnMoveSpeedChange(const FOnAttributeChangeData& InData)
{
	UE_LOG(LogTemp, Log, TEXT("On Move Speed Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		// InData.NewValue가 곧 MoveSpeed 어트리뷰트 값
		const float NewSpeed = FMath::Max(0.f, InData.NewValue);
		MoveComp->MaxWalkSpeed = NewSpeed;
	}
}

//void ATestCharacter::OnJumpPowerChange(const FOnAttributeChangeData& InData)
//{
//	UE_LOG(LogTemp, Log, TEXT("On OnJumpPowerChange Change : %.1f -> %.1f"), InData.OldValue, InData.NewValue);
//	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
//	{
//		// JumpPower를 JumpZVelocity로 그대로 사용
//		//MoveComp->JumpZVelocity = FMath::Max(0.f, InData.NewValue);
//	}
//}

void ATestCharacter::OnAbility01Press()
{
	UE_LOG(LogTemp, Log, TEXT("OnAbility01Press"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Haste));
	}
}

void ATestCharacter::OnAbility02Press()
{
	UE_LOG(LogTemp, Log, TEXT("OnAbility02Press"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::SuperJump));
		//TestSuperJumpAbility();
	}
}

void ATestCharacter::OnAbility02Release()
{
	UE_LOG(LogTemp, Log, TEXT("OnAbility02Completed"));

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(EAbilityInputID::SuperJump));
	}
}

void ATestCharacter::OnAbilityDashPress()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::ChargeDash));

	}
}

void ATestCharacter::OnAbilityDashRelease()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(EAbilityInputID::ChargeDash));
	}
}
