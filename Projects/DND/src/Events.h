#pragma once
#include "dndpch.h"

namespace DND
{

	constexpr uint32_t PACKET_RECEIVED_EVENT = Events::MIN_USER_ID + 0;
	constexpr uint32_t SERVER_SHUTDOWN_EVENT = Events::MIN_USER_ID + 1;

	constexpr uint32_t START_GAME_EVENT = Events::MIN_USER_ID + 50;

}