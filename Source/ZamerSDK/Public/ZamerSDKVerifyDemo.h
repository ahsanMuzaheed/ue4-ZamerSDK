
#pragma once

#pragma warning(disable:4668)
#pragma warning(disable:4005)
#pragma warning(disable:4191)

#include "BaseSDKVerify.h"

#include <fstream>
#include <cstring>
#include <string>
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealString.h"
#include "Paths.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "ZamerSDKVerifyDemo.generated.h"

#define SDK_OUTLOG_ 0

//api parameter
#define DEVELOP_ID	TEXT("252")
#define CONTENT_ID	TEXT("1766")
#define TOKEN	TEXT("3503cd17128847d997dc5ab9aafa5a56")
#define CHECKCODE	TEXT("90c7ad8ea63c4abea923c10510fef4dd")

#define FStringToCHAR(pFStr) TCHAR_TO_ANSI(*pFStr)

UENUM(BlueprintType)
enum class E_ZamerSdkVerifyResault:uint8
{
	E_NONE				UMETA(DisplayName = "E_NONE"),
	E_Dll_LOAD_FAILD 	UMETA(DisplayName = "E_Dll_LOAD_FAILD"),
	E_CHECKTOKEN_FPTR_INVALID 	UMETA(DisplayName = "E_CHECKTOKEN_FUNC_INVALID"),
	E_CHECKTOUKEN_RET_INVALID	UMETA(DisplayName = "E_CHECKTOUKEN_RET_INVALID"),
	E_VERIFY_FAILURE 	UMETA(DisplayName = "E_VERIFY_FAILURE"),
	E_VERIFY_SUCCESS 	UMETA(DisplayName = "E_VERIFY_SUCCESS")
};


UCLASS(BlueprintType)
class ZAMERSDK_API AZamerSDKVerifyDemo : public ABaseSDKVerify
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZamerSDKVerifyDemo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable,DisplayName="Set_ZamerSDK_Params")
		void SetZamerSdkParams(const FString& pDevID, const FString& pContentID, const FString& pToken, const FString& pCheckCode);
	virtual ESDKVerifyResault Exe_SDKVerify();

private:
	E_ZamerSdkVerifyResault Exe_ZamerSDKVerify(const FString& pDevID, const FString& pContentID, const FString& pToken, const FString& pCheckCode);

	// from project folder get DLL file path.
	FString GetSDKLibraryPath()const;
	// call ZamerSDK CheckToken function
	E_ZamerSdkVerifyResault ZamerSDKVerify(char* pDevID, char* pContentID, char* pToken, char* pCheckCod);

#if SDK_OUTLOG_
private:
	std::fstream mlogFile;
#endif

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = ZamerSDK)
		FString mDevelop_ID = DEVELOP_ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = ZamerSDK)
		FString mContent_ID = CONTENT_ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = ZamerSDK)
		FString mToken = TOKEN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = ZamerSDK)
		FString mCheckCode = CHECKCODE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = ZamerSDK)
		bool bAutoVerify=true;
};

