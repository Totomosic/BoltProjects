#include "Bolt.h"

class App : public Application
{
public:
	void Init() override
	{
		Scene& scene = SceneManager::Get().AddScene();
		Layer& layer = scene.AddLayer();

		EntityHandle camera = layer.Entities().Create();
		camera.Assign<Transform>();
		camera.Assign<Camera>(Matrix4f::Orthographic(-5, 5, -5, 5, -10, 10));
		layer.SetCamera(camera);

		EntityHandle entity = layer.Entities().Create();
		Mesh mesh;
		mesh.Models.push_back({ ResourceManager::Get().Models().Square(), Matrix4f::Identity(), { 0 } });
		mesh.Materials.push_back(ResourceManager::Get().Materials().Default());
		entity.Assign<Mesh>(std::move(mesh));
		entity.Assign<Transform>();
	}
	
	void Update() override
	{
	
	}

	void Render() override
	{
		Graphics::Get().RenderScene();
	}
};

int main()
{
	EngineCreateInfo info;
	Engine e(info);
	e.SetApplication<App>();
	e.Run();
	return 0;
}