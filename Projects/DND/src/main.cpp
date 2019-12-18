#include "dndpch.h"

#include "GlobalState.h"

#include "Scenes/CharacterScene.h"
#include "Scenes/GameData.h"
#include "Scenes/GameScene.h"
#include "Scenes/ServerScene.h"
#include "Scenes/LoadingScene.h"
#include "Scenes/TitleScene.h"

#include "NetworkManager.h"

namespace DND
{

	class App : public Application
	{
	public:
		void Init() override
		{
			GetWindow().CentreOnMonitor(Monitor::Primary());
			ResourceManager::Get().LoadPack("res/resources.pack", [](const ResourcePack& resources)
				{
					CreateTilemaps(resources);
					CreateCharacterPrefabs(resources);

					Scene& loadingScene = CreateLoadingScene(resources);
					Scene& titleScene = CreateTitleScene(resources);
					Scene& characterScene = CreateCharacterScene(resources);
					Scene& serverScene = CreateServerScene(resources);
					Scene& gameScene = CreateGameScene(resources);

					SceneManager::Get().SetCurrentScene(titleScene);

					NetworkManager::Get().Initialize([](bool initialized)
						{

						});
				});				
		}

		void Tick() override
		{
			GetWindow().SetTitle("RPG " + std::to_string((int)Time::Get().FramesPerSecond()));
		}

		void Update() override
		{
			NetworkManager::Get().Update();
			if (Input::Get().KeyPressed(Keycode::Esc))
			{
				SceneManager::Get().SetCurrentSceneByName("Title");
			}
			if (Input::Get().KeyPressed(Keycode::R))
			{
				NetworkManager::Get().Reconnect([](bool initialized)
				{

				});
			}
		}

		void Render() override
		{
			Graphics::Get().RenderScene();
		}

		void Exit() override
		{
			NetworkManager::Get().Terminate([this]()
			{
				GlobalState::Factory.ClearPrefabs();
				Application::Exit();
			});
		}

	};

}

int main()
{
	EngineCreateInfo info;
	auto& window = info.WindowInfo;
	window.Title = "RPG";
	window.Width = 1280;
	window.Height = 720;
	Engine e(info);	
	e.SetApplication<DND::App>();
	e.Run();
	return 0;
}