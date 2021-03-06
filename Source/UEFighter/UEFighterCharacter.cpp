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

	mWasAttackUsed.Init(false, (int)EAttack::VE_COUNT);

	mHurtbox = nullptr;
	mPlayerHealth = 1.f;
	mMaxDistanceApart = 800.f;
	mCanCombo = false;
	mPlayerNumber = 0;
	mCanMove = true;
	mStunTime = 0.f;
	mGravityScale = GetCharacterMovement()->GravityScale;
}

void AUEFighterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Cast<UUEFighterGameInstance>(GetGameInstance())->PushMenu(EMenuType::InGameHUD);
	SpawnHurtbox();
}

<<<<<<< Updated upstream
=======
<<<<<<< HEAD
void AUEFighterCharacter::StartJump()
{
	if (mCanMove)
	{
		mDirectionalInput = ECharacterState::VE_Jumping;
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
	ACharacter::Landed(Hit);
	mDirectionalInput = ECharacterState::VE_Default;
}

void AUEFighterCharacter::StartCrouch()
{
	mIsCrouching = true;
}

void AUEFighterCharacter::StopCrouch()
{
	mIsCrouching = false;
}


=======
>>>>>>> caa8d9145f9686832c81c070633b4420a76bc72d
>>>>>>> Stashed changes
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
	/*if (mAbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		mAbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, binds);
	}*/
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

void AUEFighterCharacter::ResetAttacks()
{
	for (auto& attackUsed : mWasAttackUsed)
	{
		attackUsed = false;
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
	GiveAbilities();
}

void AUEFighterCharacter::InitializeAttributes()
{
	if (mAbilitySystemComponent && mDefaultAttributeEffect)
	{
		FGameplayEffectContextHandle effectContect = mAbilitySystemComponent->MakeEffectContext();
		effectContect.AddSourceObject(this);

		FGameplayEffectSpecHandle speccHandle = mAbilitySystemComponent->MakeOutgoingSpec(mDefaultAttributeEffect, 1, effectContect);

		if (speccHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = mAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*speccHandle.Data.Get());
		}
	}
}

void AUEFighterCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	mAbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	/*if (mAbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		mAbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, binds);
	}*/
}

void AUEFighterCharacter::GiveAbilities()
{
	if (HasAuthority() && mAbilitySystemComponent)
	{
		for (auto& startupAbility : mDefaultAbilities)
		{
			int32 id = static_cast<int32>(startupAbility.GetDefaultObject()->mAbilityInputID);
			mAbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(startupAbility, 1, id, this));
		}
	}
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
	mWasAttackUsed[(int)EAttack::VE_LightAttack] = true;
}

void AUEFighterCharacter::StartAttack2()
{
	mWasAttackUsed[(int)EAttack::VE_MediumAttack] = true;
}

void AUEFighterCharacter::StartAttack3()
{
	mWasAttackUsed[(int)EAttack::VE_HeavyAttack] = true;
}

void AUEFighterCharacter::StartAttack4()
{
	mWasAttackUsed[(int)EAttack::VE_SpecialAttack] = true;
}

void AUEFighterCharacter::LoadHurtbox()
{
	ConstructorHelpers::FClassFinder<AActor> hurtboxActorClass(TEXT("Blueprint'/Game/UEFighter/Blueprints/Hurtbox_BP.Hurtbox_BP_C'"));
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
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
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
=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
>>>>>>> caa8d9145f9686832c81c070633b4420a76bc72d
>>>>>>> Stashed changes
			}
		}

		if (Value >= 1 || Value <= -1)
		{
			FlipCharacter(Value);
		}
		AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
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

