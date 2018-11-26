// Fill out your copyright notice in the Description page of Project Settings.

#include "ZamerSDKVerifyDemo.h"
#include "Windows/MinWindows.h"

// Sets default values
AZamerSDKVerifyDemo::AZamerSDKVerifyDemo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#if SDK_OUTLOG_
	if (!mlogFile.is_open())
	{
		std::string logpth = TCHAR_TO_ANSI(*FPaths::GameDir());
		logpth.append("\\Saved\\Logs\\log.txt\0");
		mlogFile.open(logpth, std::ofstream::out);
	}
#endif
}

void AZamerSDKVerifyDemo::SetZamerSdkParams(const FString& pDevID, const FString& pContentID, const FString& pToken, const FString& pCheckCode)
{
	mDevelop_ID=FString(pDevID);
	mContent_ID=FString(pContentID);
	mToken=FString(pToken);
	mCheckCode=FString(pCheckCode);
}

ESDKVerifyResault AZamerSDKVerifyDemo::Exe_SDKVerify()
{
	ESDKVerifyResault local_RetResault=ESDKVerifyResault::E_NONE;

	E_ZamerSdkVerifyResault local_ExeResault=Exe_ZamerSDKVerify(mDevelop_ID,mContent_ID,mToken,mCheckCode);
	if (E_ZamerSdkVerifyResault::E_VERIFY_SUCCESS == local_ExeResault)
	{
		local_RetResault=ESDKVerifyResault::E_VERIFY_SUCCESS;
		Event_SDK_Verify_Success.Broadcast();
	}
	else{
		local_RetResault=ESDKVerifyResault::E_VERIFY_FAILURE;
		Event_SDK_Verify_Faild.Broadcast(TEXT("E_VERIFY_FAILURE"));
	}
	return local_RetResault;
}

E_ZamerSdkVerifyResault AZamerSDKVerifyDemo::Exe_ZamerSDKVerify(const FString& pDevID,
													 const FString& pContentID,
													 const FString& pToken,
													 const FString& pCheckCode)
{
	std::string local_DevID=FStringToCHAR(pDevID);
	std::string local_ContentID=FStringToCHAR(pContentID);
	std::string local_Token=FStringToCHAR(pToken);
	std::string local_CheckCode=FStringToCHAR(pCheckCode);

	E_ZamerSdkVerifyResault local_VerifyResault = this->ZamerSDKVerify(const_cast<char*>(local_DevID.c_str()),
																	   const_cast<char*>(local_ContentID.c_str()),
																	   const_cast<char*>(local_Token.c_str()),
															 		   const_cast<char*>(local_CheckCode.c_str())
															  		  );

	return local_VerifyResault;
}

// Called when the game starts or when spawned
void AZamerSDKVerifyDemo::BeginPlay()
{
	Super::BeginPlay();
	if (bAutoVerify)
	{
		this->Exe_SDKVerify();
	}
}

// Called every frame
void AZamerSDKVerifyDemo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AZamerSDKVerifyDemo::GetSDKLibraryPath()const
{
#if PLATFORM_64BITS
	#define SDK_PLATFROM TEXT("Win64")
#else
	#define SDK_PLATFROM TEXT("Win32")
#endif
	  // Get ZamerSDK CheckToken.dll Path
	  FString DLLPath=FPaths::Combine(
                      *FPaths::GameDir(),
                      TEXT("Plugins/ZamerSDK/ThirdParty/Libraries"),
		 			  SDK_PLATFROM,
                      TEXT("ZamerSDK_CheckToken.dll"));
	  return DLLPath;
}

//The API to be called, will return a checkcode.
typedef char* (*ZamerSDK_Func_CheckToken)(char *, char *, char *);

E_ZamerSdkVerifyResault AZamerSDKVerifyDemo::ZamerSDKVerify(char* pDevID,char* pContentID,char* pToken,char* pCheckCode)
{
#if SDK_OUTLOG_
	if (mlogFile.is_open())
	{
		mlogFile << "DevID: "<< pDevID << std::endl
			<< "ContentID: " << pContentID << std::endl
			<< "Token: " << pToken << std::endl
			<< "CheckCode: " << pCheckCode << std::endl;
	}
#endif

	E_ZamerSdkVerifyResault local_VerifyResault = E_ZamerSdkVerifyResault::E_NONE;

	// Invariant condition
	if (!(pDevID && pContentID && pToken && pCheckCode))
		return local_VerifyResault;

	std::wstring local_DLLPath= *(this->GetSDKLibraryPath());
	local_DLLPath.push_back('\0');

#if SDK_OUTLOG_
	if (mlogFile.is_open())
		mlogFile << "DLL Path: "<<local_DLLPath.c_str() << std::endl;
#endif

	HINSTANCE hdll = LoadLibrary(local_DLLPath.c_str());
	if (!hdll)
	{
#if SDK_OUTLOG_
		if (mlogFile.is_open())
			mlogFile << "Run Resault: E_Dll_LOAD_FAILD" << std::endl;
#endif
		FreeLibrary(hdll);
		local_VerifyResault = E_ZamerSdkVerifyResault::E_Dll_LOAD_FAILD;
		return local_VerifyResault;
	}

	ZamerSDK_Func_CheckToken TokenVerifyFunc_CheckToken = (ZamerSDK_Func_CheckToken)GetProcAddress(hdll, "CheckToken");
	if (!TokenVerifyFunc_CheckToken)
	{
#if SDK_OUTLOG_
		if (mlogFile.is_open())
			mlogFile << "Run Resault: E_CHECKTOKEN_FPTR_INVALID" << std::endl;
#endif
		FreeLibrary(hdll);
		local_VerifyResault = E_ZamerSdkVerifyResault::E_CHECKTOKEN_FPTR_INVALID;
		return local_VerifyResault;
	}

	char* SDKVerify_checkCode = TokenVerifyFunc_CheckToken(pDevID, pContentID, pToken);

#if SDK_OUTLOG_
	if (mlogFile.is_open())
		mlogFile << "Call CheckToken Return: "<< SDKVerify_checkCode << std::endl;
#endif

	if (!SDKVerify_checkCode)
	{
#if SDK_OUTLOG_
		if (mlogFile.is_open())
			mlogFile << "Run Resault: E_CHECKTOUKEN_RET_INVALID" << std::endl;
#endif
		FreeLibrary(hdll);
		local_VerifyResault = E_ZamerSdkVerifyResault::E_CHECKTOUKEN_RET_INVALID;
		return local_VerifyResault;
	}
	if (std::strcmp(SDKVerify_checkCode, pContentID) > 0)
	{
#if SDK_OUTLOG_
	if (mlogFile.is_open())
		mlogFile << "Run Resault: CheckCode Verify Success" << std::endl;
#endif
		local_VerifyResault = E_ZamerSdkVerifyResault::E_VERIFY_SUCCESS;
	}
	else
	{
#if SDK_OUTLOG_
	if (mlogFile.is_open())
		mlogFile << "Run Resault: CheckCode Verify Faild" << std::endl;
#endif
		FreeLibrary(hdll);
		local_VerifyResault = E_ZamerSdkVerifyResault::E_VERIFY_FAILURE;
	}
	return local_VerifyResault;
}
