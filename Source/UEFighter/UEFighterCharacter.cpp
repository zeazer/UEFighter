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

AUEFighterCharacter::AUEFighterCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	LoadHurtbox();

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	mWasAttackUsed.Init(false, (int)EAttack::VE_COUNT);

	mHurtbox = nullptr;
	mPlayerHealth = 1.f;
	mCanCombo = false;
}

void AUEFighterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Cast<UUEFighterGameInstance>(GetGameInstance())->PushMenu(EMenuType::InGameHUD);
	SpawnHurtbox();
}


void AUEFighterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUEFighterCharacter::MoveRight);

	PlayerInputComponent->BindAction("Attack1", IE_Pressed, this, &AUEFighterCharacter::StartAttack1);
	PlayerInputComponent->BindAction("Attack2", IE_Pressed, this, &AUEFighterCharacter::StartAttack2);
	PlayerInputComponent->BindAction("Attack3", IE_Pressed, this, &AUEFighterCharacter::StartAttack3);
	PlayerInputComponent->BindAction("Attack4", IE_Pressed, this, &AUEFighterCharacter::StartAttack4);


	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUEFighterCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUEFighterCharacter::TouchStopped);
}

void AUEFighterCharacter::TakeDamage(float damageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("We took %f damage"), damageAmount);
	mPlayerHealth -= damageAmount;
	if (mPlayerHealth < 0.0f)
	{
		mPlayerHealth = 0.0f;
	}
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
	UE_LOG(LogTemp, Warning, TEXT("Light Attack"));
	mWasAttackUsed[(int)EAttack::VE_LightAttack] = true;
}

void AUEFighterCharacter::StartAttack2()
{
	UE_LOG(LogTemp, Warning, TEXT("Medium Attack"));
	mWasAttackUsed[(int)EAttack::VE_MediumAttack] = true;
}

void AUEFighterCharacter::StartAttack3()
{
	UE_LOG(LogTemp, Warning, TEXT("Heavy Attack"));
	mWasAttackUsed[(int)EAttack::VE_HeavyAttack] = true;
}

void AUEFighterCharacter::StartAttack4()
{
	UE_LOG(LogTemp, Warning, TEXT("Special Attack"));
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

	if (Value > 0.2f)
	{
		mDirectionalInput = EDirectionalInput::VE_MovingRight;
	}
	else if (Value < -0.2f)
	{
		mDirectionalInput = EDirectionalInput::VE_MovingLeft;
	}
	else
	{
		mDirectionalInput = EDirectionalInput::VE_Default;
	}

	if (Value >= 1 || Value <= -1)
	{
		FlipCharacter(Value);
	}
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AUEFighterCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	Jump();
}

void AUEFighterCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

