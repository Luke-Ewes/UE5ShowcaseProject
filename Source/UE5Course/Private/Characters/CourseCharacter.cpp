

#include "Characters/CourseCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GroomComponent.h"

#include "Items/Weapons/Weapon.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/AtributeComponent.h"
#include "HUD/MainHUD.h"
#include "HUD/MainOverlay.h"
#include "Items/HealthPotion.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

ACourseCharacter::ACourseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,400.f,0 );

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	Camera->SetupAttachment(SpringArm);	

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair-> SetupAttachment(GetMesh());
	Hair-> AttachmentName = FString("head");

	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ACourseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent  = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent-> BindAction(MovementAction, ETriggerEvent::Triggered, this, &ACourseCharacter::Move);
		EnhancedInputComponent-> BindAction(LookAction, ETriggerEvent::Triggered, this, &ACourseCharacter::Look);
		EnhancedInputComponent-> BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACourseCharacter::Jump);
		EnhancedInputComponent-> BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACourseCharacter::Equip);
		EnhancedInputComponent-> BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACourseCharacter::Attack);
		EnhancedInputComponent-> BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ACourseCharacter::Dodge);
	}
}

void ACourseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Attributes)
	{
		Attributes->RegenStamina(DeltaSeconds);
		MainOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercentage());
	}
}


void ACourseCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ACourseCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && MainOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		MainOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ACourseCharacter::AddGold(ATreasure* Gold)
{
	if (Attributes && MainOverlay)
	{
		Attributes->AddGold(Gold->GetGold());
		MainOverlay->SetGold(Attributes->GetGold());
	}
}

void ACourseCharacter::AddHealth(AHealthPotion* Health)
{
	if (Attributes && MainOverlay)
	{
		Attributes->RecieveHealth(Health->GetHealth());
		SetHUDHealth();
	}
}

bool ACourseCharacter::IsUnoccupied() const
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ACourseCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

void ACourseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	if (Attributes && Attributes->GetHealthPercentage()> 0.f)
	{
		ActionState = EActionState::EAS_HitReacting;
	}
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ACourseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	
	return DamageAmount;
}

void ACourseCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngageableTarget"));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer:: GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CourseContext,0);
		}
		InitializeMainOverlay(PlayerController);
	}
}

void ACourseCharacter::InitializeMainOverlay(const APlayerController* PlayerController)
{
	if (AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD()))
	{
		MainOverlay = MainHUD->GetMainOverlay();
		if (MainOverlay && Attributes)
		{
			MainOverlay->SetHealthBarPercent(Attributes->GetHealthPercentage());
			MainOverlay->SetStaminaBarPercent(1.f);
			MainOverlay->SetGold(0);
			MainOverlay->SetSouls(0);
		}
	}
}

void ACourseCharacter::SetHUDHealth() const
{
	if (MainOverlay && Attributes)
	{
		MainOverlay->SetHealthBarPercent(Attributes->GetHealthPercentage());
	}
}

void ACourseCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied){ return; }
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
	
}

void ACourseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookDirection = Value.Get<FVector2D>();

	AddControllerYawInput(LookDirection.X);
	AddControllerPitchInput(LookDirection.Y);
}

void ACourseCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingItem)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void ACourseCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool ACourseCharacter::HasEnoughStamina(const float StaminaCost) const
{
	return Attributes && Attributes->GetStamina() > StaminaCost;
}

void ACourseCharacter::Dodge()
{
	if (!Attributes && !IsUnoccupied() || !HasEnoughStamina(Attributes->GetDodgeCost())){return;}
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;

	Attributes->UseStamina(Attributes->GetDodgeCost());

	if (MainOverlay)
	{
		MainOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercentage());
	}
	
}	

void ACourseCharacter::EquipWeapon(AWeapon* Weapon)
{

	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr; 
	EquippedWeapon = Weapon;
	
}

void ACourseCharacter::PlayEquipMontage(const FName SectionName) const
{
	UAnimInstance* AnimInstance= GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ACourseCharacter::AttachWeaponToBack() const
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("spine_05Socket"));
	}
}

void ACourseCharacter::AttachWeaponToHand() const
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ACourseCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ACourseCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ACourseCharacter::CanDisarm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ACourseCharacter::CanArm() const
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void ACourseCharacter::Disarm()
{
	PlayEquipMontage(FName("UnEquip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ACourseCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

bool ACourseCharacter::CanAttack()
{
	return ActionState ==  EActionState::EAS_Unoccupied &&
			CharacterState != ECharacterState::ECS_Unequipped;
}

void ACourseCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();

}

void ACourseCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ACourseCharacter::DodgeEnd()
{
	Super::DodgeEnd();

	ActionState = EActionState::EAS_Unoccupied;
}












