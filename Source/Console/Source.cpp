#include <iostream>
using namespace std;

#include <Logic/Logic.hpp>
#include <Infrastructure/Infrastructure.hpp>
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

	auto material = MakeReference(new OpenGL::Material);
	{
		material->technique(Material::TechniqueType::Basic) = MakeReference(new OpenGL::Technique);
		material->map(Material::MapType::Color) = MakeReference(new OpenGL::Map);
	}

	Helper::Transformation::Dimension3::ViewProjectionMatrix camera(
		Vec3(0.0f, 8.0f, -20.0f),
		Vec3(20.0f, 0.0f, 0.0f),
		Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, 800.0f/600.0f, 0.1f, 1000.0f));
	Helper::Transformation::Dimension3::ModelMatrix model(
		Vec3(0.0f),
		Vec3(0.0f),
		Vec3(1.0f));

	auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(50));


	auto shader1 = MakeReference(new OpenGL::Shader(
		OpenGL::Shader::Type::Vertex,
		"#version 330\n uniform mat4 modelViewProjectionMatrix; in vec3 vPos; void main(){ gl_Position = modelViewProjectionMatrix * vec4(vPos,1); }"));

	auto shader2 = MakeReference(new OpenGL::Shader(
		OpenGL::Shader::Type::Fragment,
		"#version 330\n out vec4 oColor; void main(){ oColor = vec4(0,1,0,1); }"));

	auto program = MakeReference(new OpenGL::Program(
		graphicContext.get(),{
		shader1.get(),
		shader2.get()}));
	{
		program->Set();
	}

	auto dataVertices = geometry->PackVertices();
	auto dataIndices = geometry->PackIndices();

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
			sizeof(Float32)*(3 + 3*3 + 2), 0)
		}));


	// auto shape = MakeReference(new OpenGL::Shape(geometry));
	// 
	// auto model = MakeReference(new OpenGL::Model(shape, material));
	// 
	// auto object = MakeReference(new OpenGL::Object);
	// {
	// 	object->SetModel(model);
	// }
	// 
	// auto light = MakeReference(new Light);
	// 
	// auto camera = MakeReference(new Camera);
	// 
	// auto scene = MakeReference(new OpenGL::Scene(graphicContext));
	// {
	// 	// scene->Add(object);
	// 	// scene->Add(light);
	// }

	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		window->Loop();
		
		// scene->Render(camera);

		model.SetAngle(Vec3(0.0f, model.GetAngle().y + 1.0f, 0.0f));
		program->SetMat4("modelViewProjectionMatrix", camera.GetVPMat() * model.GetMMat());

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f); OpenGL::DebugTest();
		glClear(GL_COLOR_BUFFER_BIT); OpenGL::DebugTest();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

void main()
{
	try
	{
		func();
	}
	catch(GVE::WinAPI::Exception e)
	{
		cout << e.GetCode() << ": " << e.GetText() << endl;
	}
	catch(GVE::OpenGL::Exception e)
	{
		cout << e.GetCode() << ": " << e.GetText() << endl;
	}
	catch(GVE::Exception e)
	{
		cout << e.GetText() << endl;
	}

	system("pause");
}