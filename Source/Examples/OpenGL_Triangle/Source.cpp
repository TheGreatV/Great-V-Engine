#include <APIs/OpenGL/OpenGL.hpp>

namespace GVE = GreatVEngine;

using namespace GVE;
using namespace GVE::Graphics;


void func()
{
	auto instance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference(new GreatVEngine::WinAPI::WindowClass(instance, "class"));
	auto window = MakeReference(new GreatVEngine::WinAPI::Window(windowClass, "window"));
	auto deviceContext = MakeReference(new GreatVEngine::WinAPI::DeviceContext(window));
	{
		deviceContext->SetPixelFormat();
	}

	auto graphicContext = MakeReference(new OpenGL::WinAPI::ExtendedContext(deviceContext));
	{
		graphicContext->Set();
	}

	auto source1 = "#version 330\n in vec2 vPos; void main(){ gl_Position = vec4(vPos,0,1); }";
	auto source2 = "#version 330\n out vec4 oColor; void main(){ oColor = vec4(0,1,0,1); }";

	auto shader1 = MakeReference(new OpenGL::Shader(
		OpenGL::Shader::Type::Vertex,
		Vector<UInt8>(source1, source1 + strlen(source1) + 1)));

	auto shader2 = MakeReference(new OpenGL::Shader(
		OpenGL::Shader::Type::Fragment,
		Vector<UInt8>(source2, source2 + strlen(source2) + 1)));

	auto program = MakeReference(new OpenGL::Program(
		graphicContext.get(), {
		shader1.get(),
		shader2.get()}));
	{
		program->Set();
	}

	Vector<Float32> dataVertices = {-0.5f, -0.5f, +0.5f, -0.5f, +0.0f, +0.5f};
	Vector<UInt32> dataIndices = {0, 1, 2};
		
	auto vertices = MakeReference(new OpenGL::Buffers::Array(graphicContext.get(), dataVertices));
	{
		vertices->Set();
	}
	auto indices = MakeReference(new OpenGL::Buffers::Index(graphicContext.get(), dataIndices));
	{
		indices->Set();
	}

	auto attributes = MakeReference(new OpenGL::Buffers::Attribute(
		graphicContext.get(),
		program.get(),
		vertices.get(),
		indices.get(), {
		OpenGL::Buffers::Attribute::Data(
		program->GetAttribute("vPos"),
		OpenGL::Buffers::Attribute::Data::Type::Float32, 2, false,
		sizeof(Float32)*2, 0)
	}));


	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		window->Loop();

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f); OpenGL::DebugTest();
		glClear(GL_COLOR_BUFFER_BIT); OpenGL::DebugTest();

		glViewport(0, 0, 800, 600); OpenGL::DebugTest();

		program->Set();
		attributes->Set();
		vertices->Set();
		indices->Set();

		// glDrawArrays(GL_TRIANGLES, 0, 3); OpenGL::DebugTest();
		glDrawElements(GL_TRIANGLES, dataIndices.size(), GL_UNSIGNED_INT, 0); OpenGL::DebugTest();

		glFlush(); OpenGL::DebugTest();

		deviceContext->SwapBuffers();
	}
}

int WINAPI				WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
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
	catch(GVE::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "GVE Exception", MB_OK);
	}

	return 0;
}
