// Fill out your copyright notice in the Description page of Project Settings.


#include "NewActorComponent.h"
#include "RTMMessages.h"
#include "RTMChat.h"
#include "RTMData.h"
#include "RTMFiles.h"
#include "RTMFriends.h"
#include "RTMGroups.h"
#include "RTMLogin.h"
#include "RTMHistories.h"
#include "RTMRooms.h"
#include "RTMRTC.h"
#include "RTMSystem.h"
#include "RTMUsers.h"
#include "RTMControlCenter.h"
#include "RTMAudio.h"
#include "AudioRecord.h"
#if PLATFORM_IOS
#import <RTMNative/FPNNBridge.h>
#endif

using namespace rtm;
// Sets default values for this component's properties
UNewActorComponent::UNewActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	capture = nullptr;
}

UNewActorComponent::~UNewActorComponent()
{
	delete tester;
	if (capture != nullptr)
		delete capture;
}

static void PlayFinish()
{

}
// Called when the game starts
void UNewActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//client = make_shared<RTMClient>(11000001, 7654321, "161.189.171.91:13321");
	//bool ok = false;
	//int32_t errorCode = client->Login(ok, "33C43A1787B2609513EB83CEC4BBC011");
	//UE_LOG(LogTemp, Warning, TEXT("errorCode = %d, ok = %d"), errorCode, ok);
	//bool status = client->Login([](int64_t pid, int64_t uid, bool ok, int32_t errorCode) {
	//	UE_LOG(LogTemp, Warning, TEXT("errorCode = %d, ok = %d"), errorCode, ok);
	//	}, "33C43A1787B2609513EB83CEC4BBC011");
	int64_t pid = 80000071;
	int64_t uid = 7654321;
	string endpoint = "rtm-nx-front.ilivedata.com:13321";
	string rtcEndpoint = "161.189.171.91:13702";
	string token = "4ED39E112EE531B3013E48CCF92A0A791B5E6255F55D4FF0561B953";
	string token2 = "0701DF36AEBB2237D25BA8E25200BA4D";

	RTMControlCenter::Init();

	// FString gameDir = FPaths::ProjectDir() + "test.amr";
	// UE_LOG(LogTemp, Warning, TEXT("gameDir = %s"), *gameDir);
	// FString str;
	// FFileHelper::LoadFileToString(str, *gameDir);
	// std::string amr(TCHAR_TO_UTF8(*str));
	// #if PLATFORM_IOS
	// 	startPlay(amr.c_str(), (int32_t)amr.size(), PlayFinish);
	// #endif


	// capture = new AudioRecord(new TestAudioCaptureMonitor);
	// UE_LOG(LogTemp, Warning, TEXT("Start"));
	// capture->StartCapture();
	// FPlatformProcess::Sleep(10.0f);
	// UE_LOG(LogTemp, Warning, TEXT("Stop"));
	// capture->StopCapture();

	tester = new RTMAudio(endpoint, pid, uid, token);
	//tester = new RTMChat(endpoint, pid, uid, token);
	//tester = new RTMData(endpoint, pid, uid, token);
	//tester = new RTMFiles(endpoint, pid, uid, token);
	//tester = new RTMFriends(endpoint, pid, uid, token);
	//tester = new RTMGroups(endpoint, pid, uid, token);
	//tester = new RTMHistories(endpoint, pid, uid, token);
	//tester = new RTMLogin(endpoint, pid, uid, token);
	//tester = new RTMRooms(endpoint, pid, uid, token);
	//tester = new RTMRTC(endpoint, rtcEndpoint, pid, uid, token, token2);
	//tester = new RTMMessages(endpoint, pid, uid, token);
	//tester = new RTMSystem(endpoint, pid, uid, token);
	//tester = new RTMUsers(endpoint, pid, uid, token);
	tester->Start();
	tester->Stop();
}


// Called every frame
void UNewActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	static int count = 0;
	count += 1;
	if (count == 150)
	{
		UE_LOG(LogTemp, Warning, TEXT("record start"));
		AudioRecord::StartRecord();
	}
	if (count == 450)
	{
		UE_LOG(LogTemp, Warning, TEXT("record stop"));
		AudioRecord::StopRecord();
	}
}

