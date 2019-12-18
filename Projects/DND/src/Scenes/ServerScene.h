#pragma once
#include "dndpch.h"
#include "../Network/AddressPair.h"
#include "../Players/Characters/Entities/EntityNetworkData.h"

namespace DND
{

	class GameStartData
	{
	public:
		bool IsHosting;
		AddressPair Address;
		EntityNetworkData CharacterData;
	};

	Scene& CreateServerScene(const ResourcePack& resources);

}