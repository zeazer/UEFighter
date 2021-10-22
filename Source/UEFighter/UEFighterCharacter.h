// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UEFighterGameInstance.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UEFighterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	VE_Default			UMETA(DisplayName = "NOT_MOVING"),
	VE_MovingRight		UMETA(DisplayName = "MOVING_RIGHT"),
	VE_MovingLeft		UMETA(DisplayName = "MOVING_LEFT"),
	VE_Jumping			UMETA(DisplayName = "JUMPING"),
	VE_Stunned			UMETA(DisplayName = "STUNNED"),
	VE_Blocking			UMETA(DisplayName = "BLOCKING"),
	VE_Moving			UMETA(DisplayName = "MOVING"),
	VE_Crouching		UMETA(DisplayName = "CROUCHING"),
	VE_Launched			UMETA(DisplayName = "LAUNCHED")
};

UCLASS(config = Game)
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
	void BeginStun();

	UFUNCTION(BlueprintCallable)
	void EndStun();

	UFUNCTION(BlueprintCallable)
	void StartBlocking();

	UFUNCTION(BlueprintCallable)
	void StopBlocking();

	UFUNCTION(BlueprintCallable)
	void StartCrouch();

	UFUNCTION(BlueprintCallable)
	void StopCrouch();

	UFUNCTION(BlueprintCallable)
	void TakeAbilityDamage(AUEFighterCharacter* damageInstigator, const float damageAmount, const float stunTime, const float blockstunTime, const float pushbackAmount, const float launchAmount);

	void PerformPushback(const float pushbackAmount, const float launchAmount, const float direction, bool hasBlocked);

	UFUNCTION(BlueprintCallable)
	void LockMovement();

	UFUNCTION(BlueprintCallable)
	void UnlockMovement();

	UFUNCTION(BlueprintCallable)
	void LockAnimation();

	UFUNCTION(BlueprintCallable)
	void UnlockAnimation();

	UFUNCTION(BlueprintCallable)
	void AddAbilityToSlot(const FName& Ability, int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void AddAbility(class UGASGameplayAbility* Ability);

	UFUNCTION(BlueprintCallable)
	void RemoveAbility(class UGASGameplayAbility* Ability);

	UFUNCTION(BlueprintCallable)
	void RemoveAbilityByName(const FName& AbilityName);

	class UAnimMontage* GetAnimation(const FName& AbilityName);

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

#pragma region Getters/Setters
	UFUNCTION(BlueprintCallable)
	const ECharacterClass& GetCharacterClass() { return mCharacterClass; }

	UFUNCTION(BlueprintCallable)
	const ECharacterState& GetCharacterState() { return mCharacterState; }

	UFUNCTION(BlueprintCallable)
	float GetPlayerHealth() { return mPlayerHealth; }
	
#pragma endregion

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character")
	TSubclassOf<class ACaptureModel> mCaptureModel;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Game, meta = (AllowPrivateAccess = "true"))
	class UGASComponent* mAbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ECharacterClass mCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	ECharacterState mCharacterState;
	ECharacterState mDirectionalInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float mStunTime;
	FTimerHandle mStunTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float mGravityScale;

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

	void GrantStartAbilities();

	void LoadHurtbox();
	void SpawnHurtbox();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox")
	AActor* mHurtbox;

	TSubclassOf<class AActor> mHurtboxClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player")
	TMap<FName, class UAnimMontage*> mAbilityAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mPlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mMaxDistanceApart;

	float mFaceDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int mPlayerNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool mCanMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player")
	TSubclassOf<class UGameplayEffect> mAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player")
	TArray<TSubclassOf<class UGASGameplayAbility>> mAbilityTemplates;

	TMap<FName, FGameplayAbilitySpecHandle> mAbilitySpecs;

	TMap<int32, FGameplayAbilitySpecHandle> mAbilitySlots;

	UPROPERTY()
	class UGASAttributeSet* mAttributes;
#pragma endregion
};