// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSDKVerify.h"


// Sets default values
ABaseSDKVerify::ABaseSDKVerify()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseSDKVerify::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseSDKVerify::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ESDKVerifyResault ABaseSDKVerify::Exe_SDKVerify()
{
	return ESDKVerifyResault::E_NONE;
}