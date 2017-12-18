// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Serui_v01.h"
#include "Serui_v01GameMode.h"
#include "Serui_v01Character.h"

ASerui_v01GameMode::ASerui_v01GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
