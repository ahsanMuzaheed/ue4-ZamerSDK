// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealString.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSDKVerify.generated.h"

UENUM(BlueprintType)
enum class ESDKVerifyResault:uint8
{
	E_NONE				UMETA(DisplayName = "E_NONE"),
	E_VERIFY_FAILURE 	UMETA(DisplayName = "E_VERIFY_FAILURE"),
	E_VERIFY_SUCCESS 	UMETA(DisplayName = "E_VERIFY_SUCCESS")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEvent_SDKVerifySuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEvent_SDKVerifyFaild, FString,Reason);


UCLASS()
class ZAMERSDK_API ABaseSDKVerify : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSDKVerify();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable,DisplayName="Exe_SDKVerify")
	virtual ESDKVerifyResault Exe_SDKVerify();
public:
	UPROPERTY(BlueprintAssignable)
		FEvent_SDKVerifySuccess Event_SDK_Verify_Success;
	UPROPERTY(BlueprintAssignable)
		FEvent_SDKVerifyFaild Event_SDK_Verify_Faild;
};
