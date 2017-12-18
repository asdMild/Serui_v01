// Fill out your copyright notice in the Description page of Project Settings.

#include "Serui_v01.h"
#include "ball.h"


// Sets default values
Aball::Aball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	body = CreateDefaultSubobject<USphereComponent>(TEXT("sss"));
	body->bGenerateOverlapEvents = true;

	body->OnComponentBeginOverlap.AddDynamic(this,&Aball::hit);

}

void Aball::hit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GEngine->AddOnScreenDebugMessage(1,1,FColor::Blue,"ball_hit");
}

// Called when the game starts or when spawned
void Aball::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

