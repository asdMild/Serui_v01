// Fill out your copyright notice in the Description page of Project Settings.


#include "Serui_v01.h"
#include "Engine.h"
#include "enemy.h"


// Sets default values
Aenemy::Aenemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
}

// Called when the game starts or when spawned
void Aenemy::BeginPlay()
{
	Super::BeginPlay();
	USkeletalMeshComponent *self_mesh;
	self_mesh = GetMesh();

	UAnimInstance *animinstance_self;
	if (self_mesh->GetAnimInstance())
	{
		GetCapsuleComponent()->bHiddenInGame = false;
		animinstance_self=self_mesh->GetAnimInstance();
		self_mesh->bGenerateOverlapEvents = true;
		self_mesh->OnComponentBeginOverlap.AddDynamic(this,&Aenemy::mesh_hit);
    }
}


void Aenemy::mesh_hit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (attacked_head==0&& SweepResult.BoneName != FName("None"))
	{
		attacked_head = 1;

		GEngine->AddOnScreenDebugMessage(21, 1, FColor::Red, SweepResult.BoneName.ToString());
		//GEngine->AddOnScreenDebugMessage(20, 1, FColor::Blue, );
	}
}


// Called every frame
void Aenemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Aenemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

