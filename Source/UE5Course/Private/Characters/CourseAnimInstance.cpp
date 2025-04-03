// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CourseAnimInstance.h"
#include "Characters/CourseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

void UCourseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CourseCharacter = Cast<ACourseCharacter>(TryGetPawnOwner());
	if (CourseCharacter)
	{
		CourseCharacterMovement = CourseCharacter->GetCharacterMovement();
	}
	
}

void UCourseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CourseCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CourseCharacterMovement->Velocity);
		IsFalling = CourseCharacterMovement->IsFalling();
		CharacterState = CourseCharacter->GetCharacterState();
		ActionState = CourseCharacter->GetActionState();
		DeathPose = CourseCharacter->GetDeathPose();
	}
}	
