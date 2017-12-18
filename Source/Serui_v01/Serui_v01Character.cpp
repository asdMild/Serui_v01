// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Serui_v01.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Serui_v01Character.h"

//////////////////////////////////////////////////////////////////////////
// ASerui_v01Character

ASerui_v01Character::ASerui_v01Character()
{
	
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	
	switch_bool = false;
	jump_twice = 0;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASerui_v01Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Switch",IE_Pressed,this,&ASerui_v01Character::Switch);
	

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASerui_v01Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASerui_v01Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASerui_v01Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASerui_v01Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASerui_v01Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASerui_v01Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASerui_v01Character::OnResetVR);

}

void ASerui_v01Character::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASerui_v01Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	time += DeltaTime;
	//if(GetCharacterMovement()->Velocity.Size() > 600)
	camera_smooth_reset();
	//GEngine->AddOnScreenDebugMessage(0,0,FColor::Blue,FString::SanitizeFloat( time));
}

void ASerui_v01Character::Jump()
{

	if (GetCharacterMovement() && jump_twice <= 1)
	{
		if(jump_twice==1)
           GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, "jumptwice");
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, "jumponce");
			
		}

	    GetCharacterMovement()->Velocity.Z +=GetCharacterMovement()->JumpZVelocity;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		++jump_twice;
    }

}

void ASerui_v01Character::StopJumping()
{
}

void ASerui_v01Character::Landed(const FHitResult & Hit)
{
	//OnLanded(Hit);

	//LandedDelegate.Broadcast(Hit);

	jump_twice = 0;
	jump_duration = time - jump_duration;
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, FString::SanitizeFloat(jump_duration));

	if (jump_duration >= 1.5) 
	{
	land_mode = 1;
	delay_active_movement(1.2);
    }
	else
	{
		land_mode = 0;
	}


	GEngine->AddOnScreenDebugMessage(1,1,FColor::Blue,"onland");
}

void ASerui_v01Character::Falling()
{
	jump_duration = time;
}


void ASerui_v01Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASerui_v01Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASerui_v01Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASerui_v01Character::Switch()//the func called when th shift pressed;
{


	if ((forward||right)&&switch_lock)
	{ 
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, FString::SanitizeFloat(forward));
	GEngine->AddOnScreenDebugMessage(2, 1, FColor::Blue, FString::SanitizeFloat(right));
    GEngine->AddOnScreenDebugMessage(3,1,FColor::Blue,"Switch");

	switch_bool = true;

	GetCharacterMovement()->AirControl = 1;

	GetCharacterMovement()->MaxWalkSpeed = switch_speed;//increase the max speed to have a fast speed;
	GetWorld()->GetTimerManager().SetTimer(switch_timer, this, &ASerui_v01Character::reset_maxspeed,switch_time);//timer set;

	switch_lock = false;
	GetWorld()->GetTimerManager().SetTimer(switch_cd_timer,this,&ASerui_v01Character::reset_switch,switch_cd);
	}
}

void ASerui_v01Character::reset_maxspeed()//reset the charactermovement's max walk speed; called when the end of the timeline;
{
	GEngine->AddOnScreenDebugMessage(5,1,FColor::Green,"resetspeed");
	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->AirControl = 0.2;

	switch_bool = false;

	GetWorld()->GetTimerManager().ClearTimer(switch_timer);
}

void ASerui_v01Character::reset_active_movement()
{
	this->EnableInput(GetWorld()->GetFirstPlayerController());
}

void ASerui_v01Character::reset_switch()
{
	switch_lock = true;
}

void ASerui_v01Character::delay_active_movement(float delay_time)
{
	//GetCharacterMovement()->DisableMovement();
	this->DisableInput( GetWorld()->GetFirstPlayerController());
	//GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "sss");
	GetWorld()->GetTimerManager().SetTimer(movement_active_timer, this, &ASerui_v01Character::reset_active_movement, delay_time);
}

void ASerui_v01Character::camera_smooth_reset()
{
    FVector velocity=	GetCharacterMovement()->Velocity;
	velocity.Z = 0;

	camera_offset+=velocity*velocity.Size()/-1000000*camera_offset_speed-camera_offset/60*camera_offset_resetspeed;
	CameraBoom->TargetOffset = camera_offset;

}


float ASerui_v01Character::call_curve_fl(UCurveFloat curve)
{
return	curve.GetFloatValue(time);
}

FVector ASerui_v01Character::call_curve_v3(UCurveVector curve)
{
return	curve.GetVectorValue(time);
}

void ASerui_v01Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASerui_v01Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASerui_v01Character::MoveForward(float Value)
{
	forward = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASerui_v01Character::MoveRight(float Value)
{
	right = Value;
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
