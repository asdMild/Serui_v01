// Fill out your copyright notice in the Description page of Project Settings.

#include "Serui_v01.h"
#include "Sword_man.h"


// Sets default values
ASword_man::ASword_man()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCharacterMovement()->MaxAcceleration = 60000;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	sword01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sword"));
	sword01->SetupAttachment(GetMesh(),FName("hand_rSocket"));

	sword = CreateDefaultSubobject<UChildActorComponent>(TEXT("sword2"));
	sword->SetupAttachment(sword01);


	sword01->OnComponentBeginOverlap.AddDynamic(this,&ASword_man::sword_hit);
}

void ASword_man::MoveForward(float Value)
{
	forwardValue = Value;
	if (Value)
		if(FMath::Sign(Value)*FMath::Sign(forward)>=0)
		forward += Value* 2 *GetWorld()->DeltaTimeSeconds;
		else forward = 0;
	else forward = FMath::LerpStable(forward, 0.0f, 0.1f);

	forward=FMath::Clamp(forward,-1.0f,1.0f);
	if ((Controller != NULL) && (forward != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, forward);
	}
}

void ASword_man::MoveRight(float Value)
{
	rightValue = Value;
	if (Value)
		if (FMath::Sign(Value) * FMath::Sign(right)>=0)
		right += Value*2*GetWorld()->DeltaTimeSeconds;
		else right = 0;
	else right =FMath::LerpStable(right,0.0f,0.1f);
	
	right = FMath::Clamp(right,-1.0f,1.0f);
	if ((Controller != NULL) && (right != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, right);
	}
}

void ASword_man::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddActorWorldRotation(FRotator(0,0,Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()));
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}

void ASword_man::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Called when the game starts or when spawned
void ASword_man::BeginPlay()
{
	Super::BeginPlay();

}

void ASword_man::Jump()
{
	if (GetCharacterMovement() && jump_twice <= 1)
	{
		if (jump_twice == 1)
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, "jumptwice");
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, "jumponce");
		}

		montage_ready = false;
		GetWorld()->GetTimerManager().ClearTimer(montage_timer);

		GetCharacterMovement()->Velocity.Z += GetCharacterMovement()->JumpZVelocity;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		++jump_twice;
	}
}

void ASword_man::StopJumping()
{
}

void ASword_man::Landed(const FHitResult & Hit)
{
	jump_twice = 0;

	reset_montage();
}

void ASword_man::sword_hit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	sword_overlap_result = SweepResult;
	GEngine->AddOnScreenDebugMessage(25,1,FColor::Green, SweepResult.BoneName.ToString());
}

void ASword_man::shift()
{
	if ((rightValue || forwardValue)&&montage_ready)
	{
		if(rightValue==1)
		GetMesh()->GetAnimInstance()->Montage_Play(dodge_right);
		else if(rightValue==-1)
		GetMesh()->GetAnimInstance()->Montage_Play(dodge_left);
		else if(forwardValue==1)
		GetMesh()->GetAnimInstance()->Montage_Play(dodge_forward);
		else
		GetMesh()->GetAnimInstance()->Montage_Play(dodge_back);

		montage_ready = false;
		GetWorld()->GetTimerManager().ClearTimer(montage_timer);
		GetWorld()->GetTimerManager().SetTimer(montage_timer,this,&ASword_man::reset_montage,montage_cd);
	}
}

void ASword_man::reset_montage()
{
	montage_ready = true;
	GetWorld()->GetTimerManager().ClearTimer(montage_timer);
}

void ASword_man::reset_mouse_left_combo()
{
	mouse_left_mun = 0;
	GetWorld()->GetTimerManager().ClearTimer(mouse_left_combo);
	GEngine->AddOnScreenDebugMessage(11, 1, FColor::Green, "comboend_left");
}

void ASword_man::reset_mouse_right_combo()
{
	mouse_right_mun = 0;
	GetWorld()->GetTimerManager().ClearTimer(mouse_right_combo);
	GEngine->AddOnScreenDebugMessage(11, 1, FColor::Green, "comboend_right");
}

void ASword_man::mouseLeft()
{
	if (montage_ready)
	{
		switch (mouse_left_mun) {
		case 0:
			GetMesh()->GetAnimInstance()->Montage_Play(attack1); break;
		case 1:
			GetMesh()->GetAnimInstance()->Montage_Play(attack2); break;
		case 2:
			GetMesh()->GetAnimInstance()->Montage_Play(attack3); mouse_left_mun = -1; break;
		default:break;
		}

		montage_ready = false;
		GetWorld()->GetTimerManager().ClearTimer(montage_timer);
		GetWorld()->GetTimerManager().SetTimer(montage_timer, this, &ASword_man::reset_montage, montage_cd);

		mouse_left_mun++;
		GetWorld()->GetTimerManager().ClearTimer(mouse_left_combo);
		GetWorld()->GetTimerManager().SetTimer(mouse_left_combo, this, &ASword_man::reset_mouse_left_combo, combo_lastTime);
	}
}

void ASword_man::mouseRight()
{
	if (montage_ready) {
		switch (mouse_right_mun) {
		case 0:
			GetMesh()->GetAnimInstance()->Montage_Play(attack4); break;
		case 1:
			GetMesh()->GetAnimInstance()->Montage_Play(attack5); break;
		case 2:
			GetMesh()->GetAnimInstance()->Montage_Play(attack6); mouse_right_mun = -1; break;
		default:break;
		}

		montage_ready = false;
		GetWorld()->GetTimerManager().ClearTimer(montage_timer);
		GetWorld()->GetTimerManager().SetTimer(montage_timer, this, &ASword_man::reset_montage, montage_cd);


		mouse_right_mun++;
		GetWorld()->GetTimerManager().ClearTimer(mouse_right_combo);
		GetWorld()->GetTimerManager().SetTimer(mouse_right_combo, this, &ASword_man::reset_mouse_right_combo, combo_lastTime);
	}
}

// Called every frame
void ASword_man::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	time += DeltaTime;
}

// Called to bind functionality to input
void ASword_man::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &ASword_man::shift);

	PlayerInputComponent->BindAction("mouse_left", IE_Pressed, this, &ASword_man::mouseLeft);
	PlayerInputComponent->BindAction("mouse_right", IE_Pressed, this, &ASword_man::mouseRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASword_man::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASword_man::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASword_man::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASword_man::LookUpAtRate);

}