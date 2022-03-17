// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <mutex>

/**
 * 
 */
namespace rtm
{
	//using namespace fpnn;
	using namespace std;
	class RTMMidGenerator
	{
	public:
		RTMMidGenerator(){}
		~RTMMidGenerator(){}

		static int64_t Gen()
		{
			int64_t time = FDateTime::Now().GetTimeOfDay().GetTotalMilliseconds();
			time = time << 16;
			{
				lock_guard<mutex> lck(_mutexMidGenrator);
				return time + ++_orderMideGenertor;
			}
		}

	private:
		static int _orderMideGenertor;
		static mutex _mutexMidGenrator;
	};
}

