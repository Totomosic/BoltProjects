#include "dndpch.h"
#include "GameData.h"
#include "../GlobalState.h"

#include "../Players/Characters/Entities/TileMotion.h"
#include "../Players/Characters/Entities/SpriteAnimator.h"
#include "../Players/Characters/Entities/CharacterAnimator.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb_image_write.h"

namespace DND
{

	void CreateCharacterPrefabs(const ResourcePack& resources)
	{
		ResourcePtr<Texture2D> blueWizardUp = ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("WizardStaticBackward"));
		ResourcePtr<Texture2D> blueWizardDown = ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("WizardStaticForward"));
		ResourcePtr<Texture2D> blueWizardLeft = ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("WizardStaticLeft"));
		ResourcePtr<Texture2D> blueWizardRight = ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("WizardStaticRight"));

		Mesh blueWizardMesh;
		blueWizardMesh.Models.push_back({ BasicModels::Get().Square(), Matrix4f::Translation(0, GlobalState::MapManager.TileHeight() * 0.35f, 0) 
			* Matrix4f::Scale(GlobalState::MapManager.TileWidth() * 0.9f, GlobalState::MapManager.TileHeight() * 1.5f, 1), { 0 } });
		blueWizardMesh.Materials.push_back(ResourceManager::Get().Materials().Texture(blueWizardDown));
		blueWizardMesh.Materials[0]->SetIsTransparent(true);

		ObjectPrefab blueWizardPrefab;
		blueWizardPrefab.Components().AddComponent<MeshRenderer>(std::move(blueWizardMesh));
		blueWizardPrefab.Components().AddComponent<SpriteAnimator>();
		blueWizardPrefab.Components().AddComponent<CharacterAnimator>(Direction::Down, CharacterTextureSet{ blueWizardUp, blueWizardDown, blueWizardLeft, blueWizardRight });

		GlobalState::Prefabs.BlueWizardCharacter = GlobalState::Factory.AddPrefab(std::move(blueWizardPrefab));
	}

	void WriteImage(const Image& image, const blt::string& filename)
	{
		stbi_write_jpg(filename.c_str(), image.Width, image.Height, image.Components, image.Pixels, 1);
	}

	void CreateTilemaps(const ResourcePack& resources)
	{
		ResourcePtr<Texture2D> tileset = ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("Tileset"));

		TilemapManager& mapManager = GlobalState::MapManager;
		id_t grassTileId = mapManager.TileImages().AddTile(tileset->GetImage(0, 0, 32, 32));
		id_t pathTileId = mapManager.TileImages().AddTile(tileset->GetImage(0, 32, 32, 32));

		Tilemap map0(50, 50);
		TilemapLayer& map0Layer = map0.AddLayer(0.5f);
		map0Layer.SetRegion(0, 0, map0.Width(), map0.Height(), grassTileId);
		map0Layer.SetRegion(22, 0, 6, map0.Height(), pathTileId);
		mapManager.AddMap(std::move(map0));
	}

}