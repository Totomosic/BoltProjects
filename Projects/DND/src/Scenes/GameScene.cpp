#include "dndpch.h"
#include "GameScene.h"
#include "ServerScene.h"

#include "../GlobalState.h"
#include "../DndInstance.h"

namespace DND
{

	Scene& CreateGameScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::Get().CreateScene(12, "Game");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& tilemapLayer = scene.CreateLayer(camera);
		Layer& entityLayer = scene.CreateLayer(camera);
		camera->transform().Translate(0, 0, 50);

		GlobalState::MapManager.SetMapLayer(tilemapLayer);
		GlobalState::Factory.SetCurrentLayer(entityLayer);

		scene.OnLoad.AddEventListener([](Event<SceneLoadedEvent>& e)
			{
				GameStartData& data = std::any_cast<GameStartData>(e.Data.LoadData);

				GameObject* dndInstance = GlobalState::Factory.Instantiate();
				DndInstance& instance = dndInstance->Components().AddComponent<DndInstance>(GlobalState::MapManager, GlobalState::Factory);
				if (data.IsHosting)
				{
					instance.StartGame(data.CharacterData);
				} 
				else
				{
					instance.StartGame(data.Address, data.CharacterData, [addr = data.Address]()
					{
						BLT_ERROR("CONNECTION TO {} TIMED OUT", addr.PrivateEndpoint);
						SceneManager::Get().SetCurrentSceneByName("Server");
					});
				}
			});

		scene.OnUnload.AddEventListener([&entityLayer](Event<SceneUnloadedEvent>& e)
			{
				entityLayer.Clear();
			});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::Get().AddRenderSchedule(schedule);
		return scene;
	}

}