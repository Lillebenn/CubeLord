// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
struct Achievement_t
{
	int m_eAchievementID;
	const char* m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

// Defining our achievements
enum EAchievements
{
	ACH_BEAT_THE_GAME = 0,
	ACH_LEVEL_1_MASTER = 1,
	ACH_LEVEL_2_MASTER = 2,
	ACH_LEVEL_3_MASTER = 3,
	ACH_LEVEL_4_MASTER = 4,
	ACH_LEVEL_5_MASTER = 5,
	ACH_LEVEL_6_MASTER = 6,
	ACH_LEVEL_7_MASTER = 7,
	ACH_LEVEL_8_MASTER = 8,
	ACH_LEVEL_9_MASTER = 9,
	ACH_LEVEL_10_MASTER = 10,
	ACH_ALL_LEVELS_MASTER = 11,
	ACH_SKIP_CUTSCENE = 12,
};

// Achievement array which will hold data about the achievements and their state
Achievement_t g_Achievements[] =
{
	_ACH_ID(ACH_BEAT_THE_GAME, "Cat Saviour"),
	_ACH_ID(ACH_LEVEL_1_MASTER, "Level 1 Master"),
	_ACH_ID(ACH_LEVEL_2_MASTER, "Level 2 Master"),
	_ACH_ID(ACH_LEVEL_3_MASTER, "Level 3 Master"),
	_ACH_ID(ACH_LEVEL_4_MASTER, "Level 4 Master"),
	_ACH_ID(ACH_LEVEL_5_MASTER, "Level 5 Master"),
	_ACH_ID(ACH_LEVEL_6_MASTER, "Level 6 Master"),
	_ACH_ID(ACH_LEVEL_7_MASTER, "Level 7 Master"),
	_ACH_ID(ACH_LEVEL_8_MASTER, "Level 8 Master"),
	_ACH_ID(ACH_LEVEL_9_MASTER, "Level 9 Master"),
	_ACH_ID(ACH_LEVEL_10_MASTER, "Level 10 Master"),
	_ACH_ID(ACH_ALL_LEVELS_MASTER, "Master of the Mansion"),
	_ACH_ID(ACH_SKIP_CUTSCENE, "You're a mean one"),
};

