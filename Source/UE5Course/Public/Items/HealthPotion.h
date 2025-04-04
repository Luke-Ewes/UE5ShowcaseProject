// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class UE5COURSE_API AHealthPotion : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category="Health Properties")
	int32 Health;

public:
	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(int32 value) { Health = value; }
};
