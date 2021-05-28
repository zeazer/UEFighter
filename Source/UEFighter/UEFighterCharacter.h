// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UEFighterGameInstance.h"
#include "AbilitySystemInterface.h"
#include "UEFighterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	VE_Default			UMETA(DisplayName = "NOT_MOVING"),
	VE_Moving		UMETA(DisplayName = "MOVING"),
	VE_Crouching		UMETA(DisplayName = "CROUCHING"),
	VE_Jumping			UMETA(DisplayName = "JUMPING"),
	VE_Stunned			UMETA(DisplayName = "STUNNED"),
	VE_Blocking			UMETA(DisplayName = "BLOCKING")
};

UENUM(BlueprintType)
enum class EAttack : uint8
{
	VE_LightAttack			UMETA(DisplayName = "LIGHT_ATTACK"),
	VE_MediumAttack		UMETA(DisplayName = "MEDIUM_ATTACK"),
	VE_HeavyAttack		UMETA(DisplayName = "HEAVY_ATTACK"),
	VE_SpecialAttack		UMETA(DisplayName = "SPECIAL_ATTACK"),
	VE_COUNT
};

UCLASS(config=Game)
class AUEFighterCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AUEFighterCharacter();

	virtual void BeginPlay() override;

	void StartJump();

	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void StartCrouch();
	UFUNCTION(BlueprintCallable)
	void StopCrouch();

	UFUNCTION(BlueprintCallable)
	void TakeAbilityDamage(float damageAmount);

	UFUNCTION(BlueprintCallable)
	void LockMovement();

	UFUNCTION(BlueprintCallable)
	void UnlockMovement();

	UFUNCTION(BlueprintCallable)
	void LockAnimation();

	UFUNCTION(BlueprintCallable)
	void UnlockAnimation();

	//FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	//
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class AActor* GetHurtbox() const { return mHurtbox; }

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	float GetFaceDirection() const { return mFaceDirection; }

	void FlipCharacter(int scaleValue);

	///** Side view camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* SideViewCameraComponent;
	//
	///** Camera boom positioning the camera beside the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Game, meta = (AllowPrivateAccess = "true"))
	class UGASComponent* mAbilitySystemComponent;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ECharacterClass mCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	ECharacterState mCharacterState;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();


	void LoadHurtbox();
	void SpawnHurtbox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	AActor* mHurtbox;

	TSubclassOf<class AActor> mHurtboxClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float mPlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float mMaxDistanceApart;

	float mFaceDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int mPlayerNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool mCanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool mIsAnimationLocked;

#pragma region Attacks
	UFUNCTION(BlueprintCallable)
	void StartAttack1();

	UFUNCTION(BlueprintCallable)
	void StartAttack2();

	UFUNCTION(BlueprintCallable)
	void StartAttack3();

	UFUNCTION(BlueprintCallable)
	void StartAttack4();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TArray<bool> mWasAttackUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool mCanCombo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> mDefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGASGameplayAbility>> mDefaultAbilities;

	UPROPERTY()
	class UGASAttributeSet* mAttributes;
#pragma endregion
};