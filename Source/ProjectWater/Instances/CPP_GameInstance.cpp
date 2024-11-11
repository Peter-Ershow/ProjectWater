// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GameInstance.h"

void UCPP_GameInstance::Init()
{
	Super::Init();

	StartTime = FDateTime::Now();
}

void UCPP_GameInstance::SetCurrentState(EGroup NewState)
{
	MainEGroup = NewState;
}


