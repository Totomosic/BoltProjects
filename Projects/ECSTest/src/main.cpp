#include "Bolt.h"

class App : public Application
{
private:
	EntityHandle m_Camera;
	EntityHandle m_Planet;

public:
	void Init() override
	{
		GetWindow().DisableVSync();
		Scene& scene = SceneManager::Get().AddScene();
		Layer& layer = scene.AddLayer();

		EntityFactory factory = layer.GetFactory();
		m_Camera = factory.Camera(Matrix4f::Perspective(PI / 3.0f, GetWindow().Aspect(), 0.1f, 100.0f));

		EntityHandle sphere = factory.Sphere(1, Color::White, Transform({ 0, 0, -10 }));
		auto material = ResourceManager::Get().Materials().PBR();
		material->LinkMetallic(1.0f);
		material->LinkRoughness(0.5f);
		m_Planet = factory.Sphere(1, std::move(material));

		EntityHandle emerald = factory.Sphere(1, Color::White, Transform({ 10, 0, 0 }));
		DefaultLightingMaterial& em = *(DefaultLightingMaterial*)emerald.GetComponent<Mesh>()->Materials[0].get();
		em.LinkColor(Color::FromFloats(0.07568f, 0.61424f, 0.07568f));
		em.LinkSpecularColor(Color::FromFloats(0.633, 0.727811, 0.633));
		em.LinkShininess(0.6f);
		em.LinkShineDamper(32);

		for (int i = 0; i < 10; i++)
		{
			EntityHandle moon = factory.Sphere(0.1f, Color::White, Transform({ Random::NextFloat(-15, 15), 0, Random::NextFloat(-15, 15) }));
			ComponentHandle<LightSource> light = moon.Assign<LightSource>();
			light->Intensity = 5.0f;
			light->Color = Color::Random();
			light->Ambient = 0.02f;
			moon.GetComponent<Transform>()->SetParent(m_Planet.GetComponent<Transform>().Get());
		}
	}

	void Tick() override
	{
		BLT_INFO(Time::Get().FramesPerSecond());
		//const RendererStats& stats = Graphics::Get().GetRenderer().GetStats();
		//BLT_INFO("Scenes {0} RenderGroups {1} DrawCalls {2}", stats.SceneCount, stats.RenderGroupCount, stats.DrawCalls);
	}
	
	void Update() override
	{
		float speed = 3;
		float deltaTime = Time::Get().RenderingTimeline().DeltaTime();

		Transform& planetTransform = *m_Planet.GetComponent<Transform>();
		planetTransform.Rotate(2 * PI / 4.0f * deltaTime, Vector3f::Up());
		
		Transform& t = *m_Camera.GetComponent<Transform>();
		if (Input::Get().KeyDown(Keycode::A))
		{
			t.Translate(t.Right() * -speed * deltaTime);
		}
		if (Input::Get().KeyDown(Keycode::D))
		{
			t.Translate(t.Right() * speed * deltaTime);
		}
		if (Input::Get().KeyDown(Keycode::W))
		{
			t.Translate(t.Forward() * speed * deltaTime);
		}
		if (Input::Get().KeyDown(Keycode::S))
		{
			t.Translate(t.Forward() * -speed * deltaTime);
		}
		if (Input::Get().MouseButtonDown(MouseButton::Middle))
		{
			Vector3f movement = Input::Get().RelMousePosition();
			t.Rotate(-movement.x * 0.005f, Vector3f::Up(), Space::World);
			t.Rotate(movement.y * 0.005f, Vector3f::Right(), Space::Local);
		}
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