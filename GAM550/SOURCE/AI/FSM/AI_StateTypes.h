/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef AI_STATETYPE_H
#define AI_STATETYPE_H

// Macro trick to make NPC state enums from the file NPCStateNames.h
#define REGISTER_STATE(x) x,
typedef enum
{
#include "AI_StateTypeNames.h"
	NUM_AI_STATES
} AIStateType;
#undef REGISTER_STATE

#define REGISTER_STATE(x) #x,
static const char* StateText[] =
{
#include "AI_StateTypeNames.h"
	"Invalid"
};
#undef REGISTER_STATE


#endif
