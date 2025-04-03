#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "Interfaces/PickUpInterface.h"
#include "CourseCharacter.generated.h"

class UMainOverlay;
class UInputMappingContext;
class UInputAction;

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;

UCLASS()
class UE5COURSE_API ACourseCharacter : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	ACourseCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void Jump() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Gold) override;
	virtual void AddHealth(AHealthPotion* Health) override;


protected:
	virtual void BeginPlay() override;
	

	/** input */
	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* CourseContext;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* DodgeAction;
	/** input */

	/** Callbacks for input */	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();
	virtual void Attack() override;
	bool HasEnoughStamina(float StaminaCost) const;
	void Dodge();
	/** Callbacks for input */	

	void PlayEquipMontage(FName SectionName) const;
	void EquipWeapon(AWeapon* Weapon);
	bool CanDisarm() const;
	bool CanArm() const;
	void Disarm();
	void Arm();
	
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack() const;
	
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand() const;

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	//* Combat */
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	virtual void Die_Implementation() override;
	//* Combat */

private:
	bool IsUnoccupied() const;
	void InitializeMainOverlay(const APlayerController* PlayerController);
	void SetHUDHealth() const;
	
	//* Character Components */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category=Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category=Hair)
	UGroomComponent* EyeBrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY()
	UMainOverlay* MainOverlay;
	//* Character Components */
	
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/**
	 *Animation Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category=Montages)
	UAnimMontage* EquipMontage;

public:
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}
	FORCEINLINE EActionState GetActionState() const {return ActionState;}
	FORCEINLINE EDeathPose GetDeathPose() const {return DeathPose;}
};
