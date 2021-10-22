// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEFighterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UEFighterGameInstance.h"
#include "HitboxActor.h"
#include "UObject/ConstructorHelpers.h"
#include "GAS/GASComponent.h"
#include "GAS/GASAttributeSet.h"
#include "GAS/GASGameplayAbility.h"
#include <GameplayEffect.h>
#include "TimerManager.h"
#include "Animation/AnimMontage.h"

AUEFighterCharacter::AUEFighterCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	LoadHurtbox();

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetUsingAbsoluteRotation(true);
	//CameraBoom->bDoCollisionTest = false;
	//CameraBoom->TargetArmLength = 500.f;
	//CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	//CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	//
	//SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	//SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//SideViewCameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	//Ability Creation
	mAbilitySystemComponent = CreateDefaultSubobject<UGASComponent>("AbilitySystemComp");
	mAbilitySystemComponent->SetIsReplicated(true);
	mAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	mAttributes = CreateDefaultSubobject<UGASAttributeSet>("Attributes");

	mHurtbox = nullptr;
	mPlayerHealth = 1.f;
	mMaxDistanceApart = 800.f;
	mPlayerNumber = 0;
	mCanMove = true;
	mStunTime = 0.f;
	mGravityScale = GetCharacterMovement()->GravityScale;
}

void AUEFighterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Cast<UUEFighterGameInstance>(GetGameInstance())->PushMenu(EMenuType::InGameHUD);
	if (mAbilitySystemComponent)
	{
		mAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	SpawnHurtbox();
}

void AUEFighterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (auto* gameInstance = Cast<UUEFighterGameInstance>(GetGameInstance()))
	{
		//if (gameInstance->mPlayer1 == this)
		//{
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUEFighterCharacter::StartJump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &AUEFighterCharacter::StopJumping);
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AUEFighterCharacter::StartCrouch);
		PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AUEFighterCharacter::StopCrouch);
		PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AUEFighterCharacter::StartBlocking);
		PlayerInputComponent->BindAction("Block", IE_Released, this, &AUEFighterCharacter::StopBlocking);

		PlayerInputComponent->BindAxis("MoveRight", this, &AUEFighterCharacter::MoveRight);
		//PlayerInputComponent->BindAxis("MoveRightController", this, &AUEFighterCharacter::MoveRight);

		PlayerInputComponent->BindAction("Attack1", IE_Pressed, this, &AUEFighterCharacter::StartAttack1);
		PlayerInputComponent->BindAction("Attack2", IE_Pressed, this, &AUEFighterCharacter::StartAttack2);
		PlayerInputComponent->BindAction("Attack3", IE_Pressed, this, &AUEFighterCharacter::StartAttack3);
		PlayerInputComponent->BindAction("Attack4", IE_Pressed, this, &AUEFighterCharacter::StartAttack4);
		//}
		/*else if (gameInstance->mPlayer2 == this)
		{
			PlayerInputComponent->BindAction("P2Jump", IE_Pressed, this, &ACharacter::Jump);
			PlayerInputComponent->BindAction("P2Jump", IE_Released, this, &ACharacter::StopJumping);
			PlayerInputComponent->BindAxis("P2MoveRight", this, &AUEFighterCharacter::MoveRight);

			PlayerInputComponent->BindAction("P2Attack1", IE_Pressed, this, &AUEFighterCharacter::StartAttack1);
			PlayerInputComponent->BindAction("P2Attack2", IE_Pressed, this, &AUEFighterCharacter::StartAttack2);
			PlayerInputComponent->BindAction("P2Attack3", IE_Pressed, this, &AUEFighterCharacter::StartAttack3);
			PlayerInputComponent->BindAction("P2Attack4", IE_Pressed, this, &AUEFighterCharacter::StartAttack4);
		}*/

	}
}

void AUEFighterCharacter::StartJump()
{
	if (mCanMove)
	{
		mCharacterState = ECharacterState::VE_Jumping;
	}
}

void AUEFighterCharacter::Jump()
{
	ACharacter::Jump();
}

void AUEFighterCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void AUEFighterCharacter::Landed(const FHitResult& Hit)
{
	if (mCharacterState == ECharacterState::VE_Jumping || mCharacterState == ECharacterState::VE_Launched)
	{
		ACharacter::Landed(Hit);
		GetCharacterMovement()->GravityScale = mGravityScale;
		mCharacterState = ECharacterState::VE_Default;
	}
}

void AUEFighterCharacter::BeginStun()
{
	GetWorldTimerManager().SetTimer(mStunTimerHandle, this, &AUEFighterCharacter::EndStun, mStunTime, false);
	mCanMove = false;
}

void AUEFighterCharacter::EndStun()
{
	mCanMove = true;
}

void AUEFighterCharacter::StartBlocking()
{
	mCharacterState = ECharacterState::VE_Blocking;
}

void AUEFighterCharacter::StopBlocking()
{
	mCharacterState = ECharacterState::VE_Default;
}

void AUEFighterCharacter::StartCrouch()
{
	mCharacterState = ECharacterState::VE_Crouching;
}

void AUEFighterCharacter::StopCrouch()
{
	mCharacterState = ECharacterState::VE_Default;
}

void AUEFighterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	mAbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GrantStartAbilities();
}

void AUEFighterCharacter::InitializeAttributes()
{
	if (mAbilitySystemComponent && mAttributeEffect)
	{
		FGameplayEffectContextHandle effectContect = mAbilitySystemComponent->MakeEffectContext();
		effectContect.AddSourceObject(this);

		FGameplayEffectSpecHandle speccHandle = mAbilitySystemComponent->MakeOutgoingSpec(mAttributeEffect, 1, effectContect);

		if (speccHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = mAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*speccHandle.Data.Get());
		}
	}
}

void AUEFighterCharacter::GrantStartAbilities()
{
	if (mAbilitySystemComponent)
	{
		for (int32 Index = 0; Index < mAbilityTemplates.Num(); Index++)
		{
			if (auto Object = mAbilityTemplates[Index].GetDefaultObject())
			{
				AddAbility(Object);
				AddAbilityToSlot(Object->AbilitytID, Index);
			}
		}
	}
}

void AUEFighterCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	mAbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
}

UAbilitySystemComponent* AUEFighterCharacter::GetAbilitySystemComponent() const
{
	return mAbilitySystemComponent;
}

void AUEFighterCharacter::TakeAbilityDamage(AUEFighterCharacter* damageInstigator, const float damageAmount, const float stunTime, const float blockstunTime, const float pushbackAmount, const float launchAmount)
{
	if (mCharacterState != ECharacterState::VE_Blocking)
	{
		mPlayerHealth -= damageAmount;
		mStunTime = stunTime;

		if (mStunTime > 0.f)
		{
			mCharacterState = ECharacterState::VE_Stunned;
			BeginStun();
		}
	}
	else
	{
		float reducedDamage = damageAmount * 0.5f;
		mPlayerHealth -= reducedDamage;
		mStunTime = blockstunTime;

		if (stunTime > 0.f)
		{
			BeginStun();
		}
		else
		{
			mCharacterState = ECharacterState::VE_Default;
		}
	}

	float direction = -GetActorForwardVector().Y;
	if (damageInstigator)
	{
		damageInstigator->PerformPushback(pushbackAmount, 0.f, direction, mCharacterState == ECharacterState::VE_Blocking);
		direction = damageInstigator->GetActorForwardVector().Y;
	}
	PerformPushback(pushbackAmount, launchAmount, direction, mCharacterState == ECharacterState::VE_Blocking);

	if (mPlayerHealth < 0.0f)
	{
		mPlayerHealth = 0.0f;
	}
}

void AUEFighterCharacter::PerformPushback(const float pushbackAmount, const float launchAmount, const float direction, bool hasBlocked)
{
	if (hasBlocked)
	{
		LaunchCharacter(FVector(0.f, direction * pushbackAmount * 2.f, 0.f), false, false);
	}
	else
	{
		LaunchCharacter(FVector(0.f, direction * pushbackAmount, launchAmount), false, false);
	}
}

void AUEFighterCharacter::LockMovement()
{
	mCanMove = false;
}

void AUEFighterCharacter::UnlockMovement()
{
	mCanMove = true;
}

void AUEFighterCharacter::UnlockAnimation()
{
	mIsAnimationLocked = false;
}

void AUEFighterCharacter::AddAbilityToSlot(const FName& Ability, int32 SlotIndex)
{
	if (auto Spec = mAbilitySpecs.Find(Ability))
	{
		mAbilitySlots.Add(SlotIndex, *Spec);
	}
}

void AUEFighterCharacter::AddAbility(UGASGameplayAbility* Ability)
{
	FGameplayAbilitySpec Spec(Ability, 0, -1);
	mAbilitySpecs.Add(Ability->AbilitytID, mAbilitySystemComponent->GiveAbility(Spec));
}

void AUEFighterCharacter::RemoveAbility(UGASGameplayAbility* Ability)
{
	RemoveAbilityByName(Ability->AbilitytID);
}

void AUEFighterCharacter::RemoveAbilityByName(const FName& AbilityName)
{
	if (auto Spec = mAbilitySpecs.Find(AbilityName))
	{
		mAbilitySystemComponent->ClearAbility(*Spec);
	}
}

UAnimMontage* AUEFighterCharacter::GetAnimation(const FName& AbilityName)
{
	if (mAbilityAnimations.Contains(AbilityName))
	{
		return mAbilityAnimations[AbilityName];
	}
	return nullptr;
}

void AUEFighterCharacter::LockAnimation()
{
	mIsAnimationLocked = true;
}

void AUEFighterCharacter::FlipCharacter(int scaleValue)
{
	if (auto* capsuleComponent = GetCapsuleComponent()->GetChildComponent(1))
	{
		mFaceDirection = scaleValue;
		auto transform = capsuleComponent->GetRelativeTransform();
		auto scale = transform.GetScale3D();
		scale.Y = scaleValue;
		transform.SetScale3D(scale);
		capsuleComponent->SetRelativeTransform(transform);
	}
}

void AUEFighterCharacter::StartAttack1()
{
	if (mAbilitySystemComponent)
	{
		if (auto Spec = mAbilitySlots.Find(0))
		{
			mAbilitySystemComponent->TryActivateAbility(*Spec);
		}
	}
}

void AUEFighterCharacter::StartAttack2()
{
	if (mAbilitySystemComponent)
	{
		if (auto Spec = mAbilitySlots.Find(1))
		{
			mAbilitySystemComponent->TryActivateAbility(*Spec);
		}
	}
}

void AUEFighterCharacter::StartAttack3()
{
	if (mAbilitySystemComponent)
	{
		if (auto Spec = mAbilitySlots.Find(2))
		{
			mAbilitySystemComponent->TryActivateAbility(*Spec);
		}
	}
}

void AUEFighterCharacter::StartAttack4()
{
	if (mAbilitySystemComponent)
	{
		if (auto Spec = mAbilitySlots.Find(3))
		{
			mAbilitySystemComponent->TryActivateAbility(*Spec);
		}
	}
}

void AUEFighterCharacter::LoadHurtbox()
{
	ConstructorHelpers::FClassFinder<AActor> hurtboxActorClass(TEXT("Blueprint'/Game/UEFighter/Blueprints/Abilities/Hurtbox_BP.Hurtbox_BP_C'"));
	if (hurtboxActorClass.Succeeded())
	{
		mHurtboxClass = hurtboxActorClass.Class;
	}
}

void AUEFighterCharacter::SpawnHurtbox()
{
	if (mHurtboxClass.Get()->IsValidLowLevel())
	{
		auto& transform = GetCharacterMovement()->GetPawnOwner()->GetActorTransform();
		FActorSpawnParameters spawnInfo;
		spawnInfo.Instigator = this;
		mHurtbox = GetWorld()->SpawnActor<AActor>(mHurtboxClass.Get(), transform, spawnInfo);

		mHurtbox->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

		mHurtbox->SetOwner(this);
	}
}

void AUEFighterCharacter::MoveRight(float Value)
{
	if (mCanMove && mCharacterState != ECharacterState::VE_Crouching && mCharacterState != ECharacterState::VE_Blocking)
	{
		if (mDirectionalInput != ECharacterState::VE_Jumping)
		{
			if (Value > 0.2f)
			{
				mDirectionalInput = ECharacterState::VE_MovingRight;
			}
			else if (Value < -0.2f)
			{
				mDirectionalInput = ECharacterState::VE_MovingLeft;
			}
			else
			{
				mDirectionalInput = ECharacterState::VE_Default;
			}

			if (mCharacterState != ECharacterState::VE_Jumping && mCharacterState != ECharacterState::VE_Launched)
			{
				if (Value > 0.2f)
				{
					mCharacterState = ECharacterState::VE_Moving;
				}
				else if (Value < -0.2f)
				{
					mCharacterState = ECharacterState::VE_Moving;
				}
				else
				{
					mCharacterState = ECharacterState::VE_Default;
				}
			}

			if (Value >= 1 || Value <= -1)
			{
				FlipCharacter(Value);
			}
			AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
		}
	}
}

void AUEFighterCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	Jump();
}

void AUEFighterCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}
