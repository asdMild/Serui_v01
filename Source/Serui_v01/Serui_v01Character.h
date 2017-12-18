// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Engine.h"
#include "Serui_v01Character.generated.h"

UCLASS(config=Game)
class ASerui_v01Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


	UPROPERTY()
	class UTimelineComponent *switch_timeline_component;
public:
	ASerui_v01Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere)
		UCurveFloat *switch_curve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int jump_twice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float switch_cd=2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int land_mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float jump_duration;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool switch_bool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = player_settings)
		float switch_speed = 6000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = player_settings)
		float switch_time = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = player_settings)
		float camera_offset_speed=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = player_settings)
		float camera_offset_resetspeed=5;


	FTimerHandle switch_timer;
	FTimerHandle movement_active_timer;
	FTimerHandle switch_cd_timer;

	float time;
	FVector switch_direction;
	FVector camera_offset;

	bool switch_lock=true;

	float forward;
	float right;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void Switch();

	UFUNCTION()
	void reset_maxspeed();

	UFUNCTION()
	void reset_active_movement();

	UFUNCTION()
		void reset_switch();

	UFUNCTION()
		void delay_active_movement(float delay_time);

	void camera_smooth_reset();


	float call_curve_fl(UCurveFloat curve);
	FVector call_curve_v3(UCurveVector curve);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	virtual void Jump() override;

	virtual void StopJumping() override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual void Falling() override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

