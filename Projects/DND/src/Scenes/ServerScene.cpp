#include "dndpch.h"
#include "ServerScene.h"
#include "NetworkManager.h"

#include "../GlobalState.h"

namespace DND
{

	void GetHostList(UIRectangle& region, id_t loadingIconId, EntityNetworkData myCharacter)
	{
		region.Clear();
		NetworkManager::Get().GetAllHosts(5.0f, [&region, myCharacter](std::vector<AddressPair> hosts)
		{
			region.Clear();
			float yOffset = 300;
			if (hosts.size() > 0)
			{
				for (const AddressPair& host : hosts)
				{
					UIRectangle& joinButton = region.CreateRectangle(800, 150, Color::Black, Transform({ 0, yOffset, 1 }));
					joinButton.CreateText(host.PublicEndpoint.ToString(), Color::White, Transform({ 0, 0, 1 }));
					joinButton.Events().OnClick.AddEventListener([myCharacter, host](Event<UIClickedEvent>& e)
					{
						GameStartData data;
						data.CharacterData = myCharacter;
						data.IsHosting = false;
						data.Address = host;
						SceneManager::Get().SetCurrentSceneByName("Game", std::move(data));
					});	
					yOffset -= 175;
				}
			}
			else
			{
				region.CreateText("No active games found.", Color::Black, Transform({ 0, 300, 1 }));
			}
		},
		[&region]()
		{
			region.Clear();
			region.CreateText("Unable to connect to server...", Color::Black, Transform({ 0, 300, 1 }));
		});
	}

	Scene& CreateServerScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::Get().CreateScene(12, "Server");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);

		id_t loadingIconId = resources.GetResourceId("loadingSymbol");
		
		UIElement& sceneElements = layer.UI().Root().CreateElement();
		UIRectangle& joinableGamesBackground = layer.UI().Root().CreateRectangle(camera->ViewWidth() * 0.75f, camera->ViewHeight() * 0.75f, Color(200, 200, 200, 230), Transform({ camera->ViewWidth() / 2, camera->ViewHeight() * 0.6f, -50 }));

		scene.OnLoad.AddEventListener([&sceneElements, &joinableGamesBackground, loadingIconId, camera](Event<SceneLoadedEvent>& e)
		{
			EntityNetworkData myCharacter;
			myCharacter.MapId = 0;
			myCharacter.NetworkId = 0;
			myCharacter.PrefabId = GlobalState::Prefabs.BlueWizardCharacter;
			myCharacter.TilesPerSecondSpeed = 8;
			myCharacter.TransformData.BottomLeftTile = Tile(0, 0);
			myCharacter.TransformData.Width = 1;
			myCharacter.TransformData.Height = 1;
			
			GetHostList(joinableGamesBackground, loadingIconId, myCharacter);

			UIRectangle& hostButton = sceneElements.CreateRectangle(600, 200, Color::Green, Transform({ camera->ViewWidth() / 2, 125, -40 }));
			hostButton.CreateText("Host", Color::Black, Transform({ 0, 0, 1 }));
			hostButton.Events().OnClick.AddEventListener([myCharacter](Event<UIClickedEvent>& e)
			{
				// START GAME
				GameStartData data;
				data.CharacterData = myCharacter;
				data.IsHosting = true;				
				SceneManager::Get().SetCurrentSceneByName("Game", std::move(data));
			});

			UIRectangle& refreshButton = sceneElements.CreateRectangle(50, 50, Color::White, Transform({ camera->ViewWidth() / 2 + 335, 200, -40 }));
			refreshButton.Events().OnClick.AddEventListener([&joinableGamesBackground, loadingIconId, camera, myCharacter](Event<UIClickedEvent>& e) mutable
			{
				GetHostList(joinableGamesBackground, loadingIconId, std::move(myCharacter));
			});
		});

		scene.OnUnload.AddEventListener([&sceneElements](Event<SceneUnloadedEvent>& e)
		{
			sceneElements.Clear();
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::Get().AddRenderSchedule(schedule);
		return scene;
	}

}