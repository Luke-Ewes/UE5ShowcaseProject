// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "CourseAnimInstance.generated.h"

		
class ACourseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class UE5COURSE_API UCourseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	ACourseCharacter* CourseCharacter;

	UPROPERTY(BlueprintReadOnly, Category= Movement)
	UCharacterMovementComponent* CourseCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category= Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category= Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category= "Movement | CharacterState")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category= "Movement | CharacterState")
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category= "Movement | CharacterState")
	TEnumAsByte<EDeathPose> DeathPose;
};
