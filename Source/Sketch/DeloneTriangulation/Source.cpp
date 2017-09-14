#include <Infrastructure/OpenGL.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <Input/Input.hpp>
namespace GVE = GreatVEngine;
using namespace GVE;

GreatVEngine::OpenIL::Initer GreatVEngine::OpenIL::Initer::initer;


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


	auto engine = MakeReference<Graphics::OpenGL::Engine>(deviceContext);
	auto scene = MakeReference<Graphics::OpenGL::Scene>(engine, window->GetSize());
	auto camera = MakeReference<Graphics::Camera>();


	bool click = false;
	bool showCircles = true;
	Size mode = 0;
	Vector<Vec2> points;


	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();

		auto m = Input::Mouse::GetPosition() - Vec2(window->GetPosition());

		scene->DrawCircle(m, 10.0f, Vec4(0.0f, 1.0f, 0.0f, 1.0f));

		if(Input::Mouse::GetButtonState(Buttons::Right))
		{
			points.clear();
		}
		if(KeyState(Keys::PLUS))
		{
			showCircles = true;
		}
		if(KeyState(Keys::MINUS))
		{
			showCircles = false;
		}

		if(KeyState(Keys::DIGIT1))
		{
			mode = 0;
		}
		if(KeyState(Keys::DIGIT2))
		{
			mode = 1;
		}

		if(Input::Mouse::GetButtonState(Buttons::Left))
		{
			if(!click)
			{
				points.push_back(m);
			}
			click = true;
		}
		else
		{
			click = false;
		}

		for(auto &i : points)
		{
			scene->DrawCircle(i, 2.0f, Vec4(0.0f, 1.0f, 0.0f, 1.0f));
		}

		auto isPointInsideTriangleCircle = [](const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d){
			auto mat = glm::transpose(Mat3(
				a.x - d.x, a.y - d.y, (a.x*a.x - d.x*d.x) + (a.y*a.y - d.y*d.y),
				b.x - d.x, b.y - d.y, (b.x*b.x - d.x*d.x) + (b.y*b.y - d.y*d.y),
				c.x - d.x, c.y - d.y, (c.x*c.x - d.x*d.x) + (c.y*c.y - d.y*d.y)
			));

			auto det = glm::determinant(mat);

			return (Rotate2(-GetAngle(b - a)) * (c - a)).x < 0.0f ? (det > 0.0f) : (det < 0.0f); // check if points order is clokwise/counterclockwise
			// return det > 0.0f;
		};

		struct Triangle
		{
			Size a, b, c;
			inline bool operator == (const Triangle& i) const
			{
				return
					(i.a == a && i.b == b && i.c == c) ||
					(i.a == b && i.b == c && i.c == a) ||
					(i.a == c && i.b == a && i.c == b);
			}
		};

		Vector<Triangle> triangles;
		{
			auto isTriangleExist = [&](const Size& a, const Size& b, const Size& c)
			{
				for(auto &i : triangles)
				{
					if(i == Triangle{a, b, c})
					{
						return true;
					}
					// if(	(i.a == a && i.b == b && i.c == c) ||
					// 	(i.a == b && i.b == c && i.c == a) ||
					// 	(i.a == c && i.b == a && i.c == b))
					// {
					// 	return true;
					// }
				}
				return false;
			};

			switch(mode)
			{
				case 1: {
					if(points.size() >= 3)
					{
						auto isIndexUsed = [&](const Size& i)
						{
							for(auto &t : triangles)
							{
								if(t.a == i || t.b == i || t.c == i)
								{
									return true;
								}
							}

							return false;
						};
						triangles.push_back({0, 1, 2});
						{
							indices.erase(0);
							indices.erase(1);
							indices.erase(2);
						}

						for(Size i = 0; i < points.size(); ++i)
						{
							for(auto it = triangles.begin(); it != triangles.end(); ++it)
							{

							}
						}

						// while(true)
						/*{
							Set<Size> indices({0, 1, 2});
							triangles.push_back({0, 1, 2});

							for(Size i = 0; i < points.size(); ++i)
							for(Size j = 0; j < points.size(); ++j)
							for(Size k = 0; k < points.size(); ++k)
							if(i != j && i != k && j != k && !isTriangleExist(i, j, k) && !isBanned({i,j,k}))
							{
								if([&]()
								{
									for(auto &triangle : triangles)
									{
										if(	isPointInsideTriangleCircle(points[triangle.a], points[triangle.b], points[triangle.c], points[i]) ||
											isPointInsideTriangleCircle(points[triangle.a], points[triangle.b], points[triangle.c], points[j]) ||
											isPointInsideTriangleCircle(points[triangle.a], points[triangle.b], points[triangle.c], points[k]) ||
											isPointInsideTriangleCircle(points[i], points[j], points[k], points[triangle.a]) ||
											isPointInsideTriangleCircle(points[i], points[j], points[k], points[triangle.b]) ||
											isPointInsideTriangleCircle(points[i], points[j], points[k], points[triangle.c]))
										{
											return false;
										}
									}

									return true;
								}())
								{
									triangles.push_back({i, j, k});
									if(indices.find(i) == indices.end())
									{
										indices.insert(i);
									}
									if(indices.find(j) == indices.end())
									{
										indices.insert(j);
									}
									if(indices.find(k) == indices.end())
									{
										indices.insert(k);
									}
								}
							}
						}*/
					}
				} break;
				default: {
					for(int i = 0; i < (int)points.size() - 2; ++i)
					{
						triangles.push_back({i, i + 1, i + 2});
					}
				} break;
			}
		}

		// draw triangles
		for(auto &triagle : triangles)
		{
			auto	A = points[triagle.a],
					B = points[triagle.b],
					C = points[triagle.c];
			auto	c = glm::distance(A, B),
					b = glm::distance(B, C),
					a = glm::distance(C, A);

			scene->DrawCircle(A, 5.0f, Vec4(1.0f, 1.0f, 0.0f, 1.0f));
			scene->DrawCircle(B, 5.0f, Vec4(1.0f, 1.0f, 0.0f, 1.0f));
			scene->DrawCircle(C, 5.0f, Vec4(1.0f, 1.0f, 0.0f, 1.0f));

			scene->DrawLine(A, B, Vec4(1.0f, 1.0f, 0.0f, 1.0f));
			scene->DrawLine(B, C, Vec4(1.0f, 1.0f, 0.0f, 1.0f));
			scene->DrawLine(C, A, Vec4(1.0f, 1.0f, 0.0f, 1.0f));

			if(showCircles)
			{
				auto p = 0.5f * (a + b + c); // half-perimeter
				auto s = glm::sqrt(p * (p - a) * (p - b) * (p - c)); // square
				auto r = (a * b * c) / (4.0f * s); // radius

				auto o = Vec2(
					-(A.y*(dot(B,B) - dot(C,C)) + B.y*(dot(C,C) - dot(A,A)) + C.y*(dot(A,A) - dot(B,B))),
					+(A.x*(dot(B,B) - dot(C,C)) + B.x*(dot(C,C) - dot(A,A)) + C.x*(dot(A,A) - dot(B,B)))
				) * 0.5f / (A.x*(B.y - C.y) + B.x*(C.y - A.y) + C.x*(A.y - B.y));

				scene->DrawCircle(o, r, Vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
		}

		// mark invalid points
		for(auto &point : points)
		for(auto &triangle : triangles)
		{
			if(isPointInsideTriangleCircle(points[triangle.a], points[triangle.b], points[triangle.c], point))
			{
				scene->DrawCircle(point, 8.0f, Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}

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