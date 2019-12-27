#include "mcpch.h"
#include "World/Chunks/Block.h"
#include "World/Chunks/BlockDatabase.h"
#include "World/Chunks/ChunkManager.h"

namespace Minecraft
{

	float BLOCK_SIZE = 1.0f;
	
	class App : public Application
	{
	private:
		EntityHandle m_Camera;
		std::unique_ptr<ChunkManager> manager;

	public:
		void Init() override
		{		
			GetWindow().SetClearColor(Color::CornflowerBlue);
			GetWindow().DisableVSync();
			Scene& s = SceneManager::Get().AddScene();			
			Layer& l = s.AddLayer();

			EntityFactory factory = l.GetFactory();
			m_Camera = factory.Camera(Matrix4f::Perspective(PI / 3, GetWindow().Aspect(), 0.1f, 5000.0f));

			EntityHandle sun = factory.CreateTransform(Transform({ 0, 1000, 0 }));
			ComponentHandle<LightSource> light = sun.Assign<LightSource>();
			light->Ambient = 0.5f;

			Layer& uiLayer = s.AddLayer();
			EntityFactory uiFactory = uiLayer.GetFactory();
			EntityHandle uiCamera = uiFactory.Camera(Matrix4f::Orthographic(0, Width(), 0, Height(), -100, 100));
			
			m_Camera.GetTransform()->Translate(32, 200, 32);
			m_Camera.GetTransform()->Rotate(-PI / 2, Vector3f::Right());

			ResourceManager::Get().LoadPack("res/resources.pack", [&l, this](const ResourcePack& pack)
				{
					ResourceExtractor resources(pack);
					TextureAtlas atlas(resources.GetResourcePtr<Texture2D>("DefaultBlockFaces"), 16, 16);
					BlockDatabase::Initialize(atlas);
					EntityFactory factory = l.GetFactory();

					manager = std::make_unique<ChunkManager>(factory, atlas.GetTexture(), 4, 4);
					ChunkRegion& chunk = manager->GetChunkRegion();
					TaskManager::Run([&chunk]()
						{
							SimplexNoise noise;
							Vector2f seed(10, 10);
							for (int x = 0; x < chunk.GetWidthInBlocks(); x++)
							{
								for (int z = 0; z < chunk.GetHeightInBlocks(); z++)
								{
									float height = noise.Generate(8, x / 150.0f + seed.x, z / 150.0f + seed.y);
									int blockHeight = 65 + height * 10;
									for (int y = 0; y < chunk.GetDepthInBlocks(); y++)
									{
										if (y < blockHeight - 5)
										{
											chunk.SetBlockId(x, y, z, BlockId::Stone);
										}
										else if (y < blockHeight)
										{
											chunk.SetBlockId(x, y, z, BlockId::Dirt);
										}
										else if (y == blockHeight)
										{
											chunk.SetBlockId(x, y, z, BlockId::Grass);
										}
										else
										{
											chunk.SetBlockId(x, y, z, BlockId::Air);
										}
									}
								}
							}
						}).ContinueWithOnMainThread([manager{ manager.get() }]()
						{
							manager->BuildAllChunks();
						});
				});
		}

		void Tick() override
		{
			BLT_INFO(Time::Get().FramesPerSecond());
		}

		void Update() override
		{
			Transform& t = *m_Camera.GetTransform();
			static float speed = 15;
			if (Input::Get().KeyDown(Keycode::W))
			{
				t.Translate(t.Forward() * speed * Time::Get().RenderingTimeline().DeltaTime());
			}
			if (Input::Get().KeyDown(Keycode::S))
			{
				t.Translate(-t.Forward() * speed * Time::Get().RenderingTimeline().DeltaTime());
			}
			if (Input::Get().KeyDown(Keycode::D))
			{
				t.Translate(t.Right() * speed * Time::Get().RenderingTimeline().DeltaTime());
			}
			if (Input::Get().KeyDown(Keycode::A))
			{
				t.Translate(-t.Right() * speed * Time::Get().RenderingTimeline().DeltaTime());
			}
			if (Input::Get().MouseButtonDown(MouseButton::Left) || Input::Get().MouseButtonDown(MouseButton::Middle))
			{
				float x = Input::Get().RelMousePosition().x;
				float y = Input::Get().RelMousePosition().y;
				t.Rotate(-x * 0.003f, Vector3f::Up(), Space::World);
				t.Rotate(y * 0.003f, Vector3f::Right(), Space::Local);
			}
		}

		void Render() override
		{
			Graphics::Get().RenderScene();
		}

	};

}

int main()
{
	EngineCreateInfo e;
	Engine eng(e);
	eng.SetApplication<Minecraft::App>();
	eng.Run();
}