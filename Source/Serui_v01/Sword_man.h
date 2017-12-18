// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Engine.h"
#include "Sword_man.generated.h"

UCLASS()
class SERUI_V01_API ASword_man : public ACharacter
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *dodge_forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *dodge_back;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *dodge_left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *dodge_right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *attack1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *attack2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *attack3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *attack4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *attack5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim_Montage)
		UAnimMontage *attack6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* sword01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UChildActorComponent* sword;
	

	// Sets default values for this character's properties
	ASword_man();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	//-------hit result
	FHitResult sword_overlap_result;

	//--------timer of montage
	FTimerHandle montage_timer;
	bool montage_ready=true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float montage_cd=0.5f;

	//--------timers of the combons
	FTimerHandle mouse_left_combo;
	FTimerHandle mouse_right_combo;
	float combo_lastTime=1;
	

	//--------timers of the cd_mouseclick
	float mouseClick_cd=0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int mouse_left_mun = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int mouse_right_mun=0;
	//--------porperty of man movement(self set)
UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int jump_twice;
	float time;
	int forwardValue;
	int rightValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float right;


protected:

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
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;


	virtual void Jump() override;

	virtual void StopJumping() override;

	virtual void Landed(const FHitResult& Hit) override;

	//-----sword overlap

	UFUNCTION()
		void sword_hit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	//-----shift
	void shift();
	//------reset montage
	UFUNCTION()
	void reset_montage();

	//----mouse combon reset
	UFUNCTION()
		void reset_mouse_left_combo();
	UFUNCTION()
		void reset_mouse_right_combo();
	//----mouse click function
	void mouseLeft();
	void mouseRight();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
