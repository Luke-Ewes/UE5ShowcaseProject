#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "Characters/BaseCharacter.h"
#include "Enemy.generated.h"


class ASoul;
class UPawnSensingComponent;
class AAIController;
class UHealthBarComponent;;

UCLASS()
class UE5COURSE_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();
	/** AActor */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/** AActor */

	/** IHitInterface */  
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** IHitInterface */  
	
protected:
	/** AActor*/
	virtual void BeginPlay() override;
	void SpawnSoul();
	/** AActor*/

	/**ABaseCharacter*/

	virtual void Die_Implementation() override;
	virtual void HandleDamage(float DamageAmount) const override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void Attack() override;
	/**ABaseCharacter*/
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* IdleMontage;
private:
	/** AI behaviour */
	void InitializeEnemy();
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void PatrolTimerFinished() const;
	void SetHealthBarVisibility(bool Visible) const;
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsInAttackRadius();
	bool IsInCombatRadius();
	bool IsChasing() const;
	bool IsAttacking() const;
	bool IsDead() const;
	bool IsEngaged() const;
	void StartAttackTimer();
	AActor* ChoosePatrolTarget();
	bool InTargetRange(const AActor* Target, const double Radius) const;
	void SpawnDefaultWeapon();
	void MoveToTarget(const AActor* Target) const;
	UFUNCTION(BlueprintCallable)
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;
	
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AWeapon> WeaponClass;
	

	UPROPERTY(EditAnywhere,Category = Combat )
	double CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere,Category = Combat )
	double PatrolRadius = 200.f;
	UPROPERTY(EditAnywhere,Category = Combat )
	double AttackRadius = 150.f;
	UPROPERTY(EditAnywhere,Category = Combat )
	double AcceptanceRadius = 50.f;

	UPROPERTY()
	AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category= "AI_Navigation")
	AActor* PatrolTarget;
	
	UPROPERTY(EditInstanceOnly, Category= "AI_Navigation")
	TArray<AActor*> PatrolTargets;

	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere, Category= "AI Navigation")
	float PatrolWaitMin = 8.f;
	UPROPERTY(EditAnywhere, Category= "AI Navigation")
	float PatrolWaitMax = 10.f;
	UPROPERTY(EditAnywhere, Category=Combat)
	float PatrolSpeed;

	
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackMax = 1.f;
	UPROPERTY(EditAnywhere, Category=Combat)
	float ChasingSpeed;
	UPROPERTY(EditAnywhere, Category=Combat)
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category=Combat)
	TSubclassOf<ASoul> SoulClass;
	

protected:
	

	

};
