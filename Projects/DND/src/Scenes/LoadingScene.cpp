#include "dndpch.h"
#include "LoadingScene.h"

namespace DND
{

	Scene& CreateLoadingScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::Get().CreateScene(12, "Loading");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);

		id_t loadingIconId = resources.GetResourceId("loadingSymbol");
		UIRectangle& background = layer.UI().Root().CreateRectangle(camera->ViewWidth(), camera->ViewHeight(), Color::White, Transform({ camera->ViewWidth() / 2, camera->ViewHeight() / 2, -50 }));

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::Get().AddRenderSchedule(schedule);
		return scene;
	}

}