
#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;
UCLASS()
class UE5COURSE_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const;
	void PlayEquipSound() const;
	void DisableSphereCollision() const;
	void DisableEmbers() const;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay() override;
	void ExecuteGetHit(const FHitResult& BoxHit);
	bool IsSameActor(AActor* OtherActor);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:

	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category= "Weapon Properties ")
	FVector BoxTracerExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category= "Weapon Properties ")
	bool bShowBoxDebug = false;
	
	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	USoundBase* EquipSound;
	UPROPERTY(VisibleAnywhere, Category= "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	float Damage = 20.f;


public:
	FORCEINLINE UBoxComponent* GetWeaponBox(){ return WeaponBox;}
};
