//
//  FPNNBridge.h
//  RTMNative
//
//  Created by Sun on 2022/1/17.
//

#ifndef FPNNBridge_h
#define FPNNBridge_h
#import <Foundation/Foundation.h>

typedef void(*NetworkStatusCallback)(int);
typedef void(*VolumnCallback)(float);
typedef void(*StartRecordCallback)();
typedef void(*PlayFinishCallback)();
typedef void(*StopRecordCallback)(const char*, int, long long);

void initNetworkStatusChecker(NetworkStatusCallback callback);
void startRecord(VolumnCallback callback, StartRecordCallback startCallback);
void stopRecord(StopRecordCallback callback);
void startPlay(const char* data, int size, PlayFinishCallback callback);
void stopPlay();

#endif /* FPNNBridge_h */
