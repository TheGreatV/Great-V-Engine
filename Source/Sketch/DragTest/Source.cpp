#include <Infrastructure/OpenGL.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <Input/Input.hpp>
namespace GVE = GreatVEngine;
using namespace GVE;

GreatVEngine::OpenIL::Initer GreatVEngine::OpenIL::Initer::initer;


class Node:
	public This<Node>,
	public Helper::Transformation::Dimension2::Position
{
public:
	bool isStatic;
public:
	inline Node(const Reference<Node>& this_, const Vec2& pos_, const bool& isStatic_):
		This(this_),
		Position(pos_),
		isStatic(isStatic_)
	{
	}
};
class Spring:
	public This<Spring>
{
public:
	const Reference<Node> a, b;
	Float32 power;
public:
	inline Spring(const Reference<Spring>& this_, const Reference<Node>& a_, const Reference<Node>& b_, const Float32& power_):
		This(this_),
		a(a_),
		b(b_),
		power(power_)
	{
	}
};
class Engine:
	public This<Engine>
{
public:
	Vector<const Reference<Node>> nodes;
	Vector<const Reference<Spring>> springs;
public:
	inline Engine(const Reference<Engine>& this_):
		This(this_)
	{
	}
public:
	inline void Add(const Reference<Node>& node_)
	{
		nodes.push_back(node_);
	}
	inline void Add(const Reference<Spring>& spring_)
	{
		springs.push_back(spring_);
	}
	inline void Render(const Reference<Graphics::OpenGL::Scene>& scene_)
	{
		for(auto &node : nodes)
		{
			scene_->DrawCircle(
				node->GetPosition(),
				8.0f,
				Vec4(1.0f, 1.0f, 0.0f, 1.0f)
			);
		}

		for(auto &spring : springs)
		{
			scene_->DrawLine(
				spring->a->GetPosition(),
				spring->b->GetPosition(),
				Vec4(1.0f, 1.0f, 0.0f, 1.0f)
			);
		}
	}
};


void func()
{
	using namespace glm;

	WinAPI::Window::Size resolution(800, 600);
	// WinAPI::Window::Size resolution(1280, 600);
	// WinAPI::Window::Size resolution(1920, 1080);

	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference<GreatVEngine::WinAPI::WindowClass>(instance, "class");
	auto window = MakeReference<GreatVEngine::WinAPI::Window>(windowClass, "window", resolution);
	auto deviceContext = MakeReference<GreatVEngine::WinAPI::DeviceContext>(window);
	{
		deviceContext->SetPixelFormat();
	}


	auto graphicsEngine = MakeReference<Graphics::OpenGL::Engine>(deviceContext);
	auto scene = MakeReference<Graphics::OpenGL::Scene>(graphicsEngine, window->GetSize());
	auto camera = MakeReference<Graphics::Camera>();


	auto engine = Make<Engine>();

	auto nodeA = Make<Node>(Vec2(100.0f, 100.0f), true);
	auto nodeB = Make<Node>(Vec2(600.0f, 400.0f), false);
	auto spring = Make<Spring>(nodeA, nodeB, 1.0f);

	engine->Add(nodeA);
	engine->Add(nodeB);
	engine->Add(spring);

	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();

		auto mouse = window->ToLocalPosition(Input::Mouse::GetPosition());

		scene->DrawCircle(mouse, 10.0f, Vec4(0.0f, 1.0f, 0.0f, 1.0f));

		engine->Render(scene);

		scene->Render(camera);

		Sleep(1000 / 60);

		deviceContext->SwapBuffers();
	}
}

void main()
{
	try
	{
		func();
	}
	catch(GVE::WinAPI::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "WinAPI Exception", MB_OK);
	}
	catch(GVE::OpenGL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenGL Exception", MB_OK);
	}
	catch(GVE::OpenIL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenIL Exception", MB_OK);
	}
	catch(GVE::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "GVE Exception", MB_OK);
	}

	return;
}