#include "dndpch.h"
#include "CharacterScene.h"

namespace DND
{

	Scene& CreateCharacterScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::Get().CreateScene(12, "Character");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);

		scene.OnLoad.AddEventListener([](Event<SceneLoadedEvent>& e)
		{
			SceneManager::Get().SetCurrentSceneByName("Server");
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::Get().AddRenderSchedule(schedule);
		return scene;
	}

}