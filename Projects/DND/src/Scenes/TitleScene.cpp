#include "dndpch.h"
#include "TitleScene.h"

namespace DND
{

	Scene& CreateTitleScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::Get().CreateScene(12, "Title");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);

		std::unique_ptr<Material> material = ResourceManager::Get().Materials().Texture(ResourceManager::Get().GetResource<Texture2D>(resources.GetResourceId("dndBackground")));

		UIRectangle& background = layer.UI().Root().CreateRectangle(camera->ViewWidth(), camera->ViewHeight(), std::move(material), Transform({ camera->ViewWidth() / 2, camera->ViewHeight() / 2, -50 }));
		UIRectangle& titleBack = background.CreateRectangle(600, 800, Color(30, 30, 30, 240), Transform({ 0, 0, 1 }));
		UIText& title = titleBack.CreateText("RPG", Color::White, Transform({ 0, 250, 1 }));

		UIRectangle& playButton = titleBack.CreateRectangle(500, 200, Color::Green, Transform({ 0, 0, 1 }));
		playButton.CreateText("Play", Color::Black, Transform({ 0, 0, 1 }));
		playButton.Events().OnClick.AddEventListener([](Event<UIClickedEvent>& e)
			{
				SceneManager::Get().SetCurrentSceneByName("Character");
			});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::Get().AddRenderSchedule(schedule);
		return scene;
	}

}