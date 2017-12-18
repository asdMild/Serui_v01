// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "enemy.generated.h"

UCLASS()
class SERUI_V01_API Aenemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aenemy();


	// ----- hited place of the body
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int attacked_head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int attacked_body_up;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int attacked_body_down;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int attacked_back;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION()
		void mesh_hit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
