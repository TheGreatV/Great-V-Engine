#include <Logic/Logic.hpp>
#include <APIs/OpenGL/OpenGL.hpp>
#include <Utilities/OpenIL/Functions.hpp>

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

	Helper::Transformation::Dimension3::ViewProjectionMatrix camera(
		Vec3(0.0f, 8.0f, -20.0f),
		Vec3(20.0f, 0.0f, 0.0f),
		Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, 800.0f / 600.0f, 0.1f, 1000.0f));
	Helper::Transformation::Dimension3::ModelMatrix model(
		Vec3(0.0f),
		Vec3(0.0f),
		Vec3(1.0f));

	auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(50));

	auto image = OpenIL::Image::Load2D("../../../../../Media/Images/image.png");

	auto texture = MakeReference(new OpenGL::Texture(
		graphicContext.get(),
		image->GetWidth(), image->GetHeight(), image->GetDepth(),
		OpenGL::Texture::Type::D2, OpenGL::Texture::InternalFormat::RGBA8,
		OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())), 
		OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
		OpenGL::Texture::Wrap::Repeat, OpenGL::Texture::Filter::Linear, image->GetRawData()));
	{
		texture->Set(0);
	}

	auto source1 =
		"#version 330\n\
		uniform mat4 modelViewProjectionMatrix;\
		in vec3 vPos;\
		in vec2 vTex;\
		out vec2 pTex;\
		void main(){\
		pTex = vTex;\
		gl_Position = modelViewProjectionMatrix * vec4(vPos,1); }";
	auto source2 =
		"#version 330\n\
		uniform sampler2D textureColor;\
		in vec2 pTex;\
		out vec4 oColor;\
		void main(){ oColor = texture(textureColor, pTex); }";

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

	auto dataVertices = geometry->GetVertices();
	auto dataIndices = geometry->GetIndices();

	auto vertices = MakeReference(new OpenGL::Buffers::Array(graphicContext.get(), *dataVertices.get()));
	{
		vertices->Set();
	}
	auto indices = MakeReference(new OpenGL::Buffers::Index(graphicContext.get(), *dataIndices.get()));
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
			OpenGL::Buffers::Attribute::Data::Type::Float32, 3, false,
			sizeof(Float32)*(3 + 3 * 3 + 2), 0),
		OpenGL::Buffers::Attribute::Data(
			program->GetAttribute("vTex"),
			OpenGL::Buffers::Attribute::Data::Type::Float32, 2, false,
			sizeof(Float32)*(3 + 3 * 3 + 2), sizeof(Float32)*(3 + 3*3))
	}));

	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		window->Loop();

		model.SetAngle(Vec3(0.0f, model.GetAngle().y + 1.0f, 0.0f));
		program->SetMat4("modelViewProjectionMatrix", camera.GetVPMat() * model.GetMMat());

		glDisable(GL_DEPTH_TEST); OpenGL::DebugTest();
		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE); glCullFace(GL_BACK); OpenGL::DebugTest(); //glDisable(GL_CULL_FACE);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f); OpenGL::DebugTest();
		glClear(GL_COLOR_BUFFER_BIT); OpenGL::DebugTest();
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glViewport(0, 0, 800, 600); OpenGL::DebugTest();

		program->Set();
		attributes->Set();
		vertices->Set();
		indices->Set();

		// glDrawArrays(GL_TRIANGLES, 0, 3); OpenGL::DebugTest();
		glDrawElements(GL_TRIANGLES, geometry->indices.size(), GL_UNSIGNED_INT, 0); OpenGL::DebugTest();

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
	catch(GVE::OpenIL::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "OpenIL Exception", MB_OK);
	}
	catch(GVE::Exception e)
	{
		MessageBoxA(NULL, e.GetText().c_str(), "GVE Exception", MB_OK);
	}

	return 0;
}
