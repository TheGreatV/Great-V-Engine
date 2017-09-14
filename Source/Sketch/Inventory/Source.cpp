#include <Logic/Logic.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Infrastructure/OpenGL.hpp>
#include <Infrastructure/BulletPhysics.hpp>
#include <Input/Input.hpp>
#include <Geometry/Collision.hpp>

namespace GVE = GreatVEngine;
using namespace GVE;


Vec3 camPos = Vec3(0.0f, 10.0f, 0.0f);
Vec3 camAng = Vec3(30.0f, 45.0f, 0.0f);
Float32 camDist = 50.0f;


namespace Selectable
{
	class Selection
	{
	public:
		static const Float32 NO_SELECTION;
	public:
		virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const
		{
			return NO_SELECTION;
		}
	};
	class Selector
	{
	public:
		List<Reference<Selection>> selections;
	public:
		inline void Add(Reference<Selection> selection_)
		{
			selections.push_back(selection_);
		}
	public:
		Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_) const
		{
			auto t = VecXYZ(camera_->GetPIMat() * Vec4(p_, 1.0f, 1.0f));
			auto p1 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
			auto p2 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));
			auto d = normalize(p2 - p1);

			Float32 minDistance = Selection::NO_SELECTION;
			Reference<Selection> picked = nullptr;

			for(auto &selection : selections)
			{
				auto distance = selection->GetDistance(p1, d);
				if(distance < minDistance)
				{
					minDistance = distance;
					picked = selection;
				}
			}

			return picked;
		}
		template <class Condition_> Reference<Selection> Pick(Reference<Graphics::Camera> camera_, const Vec2& p_, Condition_ condition_) const
		{
			auto t = VecXYZ(camera_->GetPIMat() * Vec4(p_, 1.0f, 1.0f));
			auto p1 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
			auto p2 = VecXYZ(camera_->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));
			auto d = normalize(p2 - p1);

			Float32 minDistance = Selection::NO_SELECTION;
			Reference<Selection> picked = nullptr;

			for(auto &selection : selections)
			{
				if(condition_(selection))
				{
					auto distance = selection->GetDistance(p1, d);
					if(distance < minDistance)
					{
						minDistance = distance;
						picked = selection;
					}
				}
			}

			return picked;
		}
	};

	const Float32 Selection::NO_SELECTION = FLT_MAX;
}


namespace Inventory
{
	enum class Rotation: UInt32
	{	// Values indicates indices in array and must NOT be changed!
		Front	= 0,
		Right	= 1,
		Back	= 2,
		Left	= 3,
	};
	class Slot;
	class Dock;

	class Slot
	{
	public:
		Array<Reference<Slot>, 4>	links;
		Reference<Dock> dock;
	};

	class Dock:
		public std::enable_shared_from_this<Dock>
	{
	public:
		Array<Reference<Dock>, 4>	links;
		Reference<Slot> slot = nullptr;
	protected:
		bool IsConnectPossible(const Reference<Slot>& slot_, const Rotation& rotation_, Set<const Dock*const>& checked_)
		{
			if(!slot_ || slot_->dock)
			{
				return false;
			}

			checked_.insert(this);

			for(UInt32 i = static_cast<UInt32>(Rotation::Front); i <= static_cast<UInt32>(Rotation::Left); ++i)
			{
				auto &link = links[i];

				auto j = (i + static_cast<UInt32>(rotation_)) % 4;
				auto &linkedSlot = slot_->links[j];

				if(link && checked_.find(link.get()) == checked_.end()) // if link exist and not checked yet
				{
					if(!linkedSlot || !link->IsConnectPossible(linkedSlot, rotation_, checked_))
					{
						return false;
					}
				}
			}

			return true;
		}
	public:
		bool IsAttached() const
		{
			return slot != nullptr;
		}
		bool IsConnectPossible(const Reference<Slot>& slot_, const Rotation& rotation_ = Rotation::Front)
		{
			Set<const Dock*const> checked;

			return IsConnectPossible(slot_, rotation_, checked);
		}
		void Attach(const Reference<Slot>& slot_, const Rotation& rotation_ = Rotation::Front)
		{
			if(slot)
			{
				throw std::exception("");
			}

			slot = slot_;
			slot->dock = shared_from_this();

			for(UInt32 i = static_cast<UInt32>(Rotation::Front); i <= static_cast<UInt32>(Rotation::Left); ++i)
			{
				auto &link = links[i];

				auto j = (i + static_cast<UInt32>(rotation_)) % 4;
				auto &linkedSlot = slot_->links[j];

				if(link && !link->slot)
				{
					link->Attach(linkedSlot, rotation_);
				}
			}
		}
		void Detach()
		{
			auto shared = shared_from_this();
			
			if(!slot || slot->dock != shared)
			{
				throw std::exception("");
			}

			slot->dock = nullptr;
			slot = nullptr;

			for(UInt32 i = static_cast<UInt32>(Rotation::Front); i <= static_cast<UInt32>(Rotation::Left); ++i)
			{
				auto &link = links[i];

				if(link && link->slot)
				{
					link->Detach();
				}
			}
		}
	};
}


class Item;

class Slot:
	public Selectable::Selection,
	public Inventory::Slot
{
public:
	Reference<Graphics::OpenGL::Object> graphicsObject;
public:
	inline Slot(Reference<Graphics::OpenGL::Scene> scene_, Reference<Graphics::OpenGL::Material> material_, const Vec3& pos_)
	{
		graphicsObject = WrapReference(new Graphics::OpenGL::Object(scene_->GetEngine()));
		{
			graphicsObject->SetLocalPosition(pos_);

			auto geometry = Geometry::CreateBox(Vec3(9.0f, 1.0f, 9.0f), Vec3(1.0f), UVec3(1));
			auto shape = WrapReference(new Graphics::OpenGL::Shape(scene_->GetEngine(), geometry));
			auto model = WrapReference(new Graphics::OpenGL::Model(scene_->GetEngine(), shape, material_));
			graphicsObject->SetModel(model);
			scene_->Add(graphicsObject);
		}
	}
public:
	virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
	{
		if(Cast::Ray::BoxProcess({position_, direction_}, {graphicsObject->GetLocalPosition(), graphicsObject->GetLocalAngle(), Vec3(9.0f, 1.0f, 9.0f)}))
		{
			return Cast::Ray::GetLast().distance;
		}

		return Selection::GetDistance(position_, direction_);
	}
};
class Dock:
	public Selectable::Selection,
	public Inventory::Dock,
	public Helper::Transformation::Dimension3::HierarchyMatrix
{
public:
	Item* item;
public:
	inline Dock():
		HierarchyMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f), nullptr)
	{
	}
	virtual Float32 GetDistance(const Vec3& position_, const Vec3& direction_) const override
	{
		if(Cast::Ray::BoxProcess({position_, direction_}, {GetPosition(), GetAngle(), Vec3(8.0f)}))
		{
			return Cast::Ray::GetLast().distance;
		}

		return Selection::GetDistance(position_, direction_);
	}
	void Glue(const Reference<Slot>& slot_, const Inventory::Rotation& rotation_);
	void Glue(const Vec3& position_, const Inventory::Rotation& rotation_);
};
class Item:
	public Helper::Transformation::Dimension3::HierarchyMatrix
{
public:
	Inventory::Rotation rotation = Inventory::Rotation::Front;
	Vector<Reference<Dock>> docks;
	Reference<Graphics::OpenGL::Object> graphicsObject;
	inline Item():
		HierarchyMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f), nullptr)
	{
	}
};
class Item1:
	public Item
{
public:
	inline Item1(Reference<Graphics::OpenGL::Scene> scene_, Reference<Graphics::OpenGL::Material> material_)
	{
		graphicsObject = WrapReference(new Graphics::OpenGL::Object(scene_->GetEngine()));
		{
			auto geometry = Geometry::CreateBox(Vec3(18.0f, 8.0f, 8.0f), Vec3(1.0f), UVec3(1));
			auto shape = WrapReference(new Graphics::OpenGL::Shape(scene_->GetEngine(), geometry));
			auto model = WrapReference(new Graphics::OpenGL::Model(scene_->GetEngine(), shape, material_));
			graphicsObject->SetModel(model);
			scene_->Add(graphicsObject);

			graphicsObject->SetParent(this);
			graphicsObject->SetLocalPosition(Vec3(5.0f, 0.0f, 0.0f));
		}

		auto mainDock = WrapReference(new Dock());
		{
			mainDock->SetParent(this);
			mainDock->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
			mainDock->item = this;

			docks.push_back(mainDock);
		}

		auto dock1 = WrapReference(new Dock());
		{
			dock1->SetParent(this);
			dock1->SetLocalPosition(Vec3(10.0f, 0.0f, 0.0f));
			dock1->item = this;

			mainDock->links[static_cast<Size>(Inventory::Rotation::Right)] = dock1;
			dock1->links[static_cast<Size>(Inventory::Rotation::Left)] = mainDock;

			docks.push_back(dock1);
		}
	}
};


void Dock::Glue(const Reference<Slot>& slot_, const Inventory::Rotation& rotation_)
{
	if(item)
	{
		if(slot_->graphicsObject)
		{
			auto angle = static_cast<Size>(rotation_)* 90.0f;

			item->SetLocalPosition(slot_->graphicsObject->GetPosition() - RotateY3(angle) * GetLocalPosition());

			item->rotation = rotation_;
			item->SetLocalAngle(Vec3(0.0f, angle, 0.0f));
		}
		else
		{
			throw std::exception("");
		}
	}
	else
	{
		throw std::exception("");
	}
}
void Dock::Glue(const Vec3& position_, const Inventory::Rotation& rotation_)
{
	if(item)
	{
		auto angle = static_cast<Size>(rotation_)* 90.0f;

		item->SetLocalPosition(position_);

		item->rotation = rotation_;
		item->SetLocalAngle(Vec3(0.0f, angle, 0.0f));
	}
	else
	{
		throw std::exception("");
	}
}


void func()
{
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

	auto graphicsCamera = WrapReference(new Graphics::Camera());
	{
		graphicsCamera->SetPosition(Vec3(0.0f, 50.0f, -100.0f));
		graphicsCamera->SetAngle(Vec3(20.0f, 0.0f, 0.0f));
		graphicsCamera->SetProjection(Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, window->GetAspect(), 0.1f, 10000.0f));
	}

	auto graphicsEngine = WrapReference(new Graphics::OpenGL::Engine(deviceContext));

	auto graphicsScene = WrapReference(new Graphics::OpenGL::Scene(graphicsEngine, window->GetSize()));

	auto graphicsLightDirection1 = WrapReference(new Graphics::OpenGL::Lights::Direction());
	{
		graphicsLightDirection1->SetLocalAngle(Vec3(80.0f, 0.0f, 0.0f));
		graphicsLightDirection1->SetColor(Vec4(Vec3(1.0f), 0.8f));
		graphicsLightDirection1->SetAmbient(0.5f);
		graphicsScene->Add(graphicsLightDirection1);
	}

	auto graphicsMaterialFlat = WrapReference(new Graphics::OpenGL::Material(graphicsEngine));
	{
		graphicsMaterialFlat->Technique(Graphics::Material::TechniqueType::Basic) = Graphics::OpenGL::Technique::Load(graphicsEngine, Filepath("Media/Shaders/Materials/Blank/Basic/Common.glsl."), "vs", "", "", "", "fs");
		graphicsMaterialFlat->SetValue("materialColor", Vec3(1.0f));
		graphicsMaterialFlat->SetValue("materialSpecular", Vec3(1.0f)*1.0f);
		graphicsMaterialFlat->SetValue("materialGloss", 0.1f);
		graphicsMaterialFlat->SetValue("materialRoughness", 0.8f);
	}
	auto graphicsObject = WrapReference(new Graphics::OpenGL::Object(graphicsEngine));
	{
		auto geometry = Geometry::CreateBox(Vec3(400.0f, 1.0f, 400.0f), Vec3(1.0f), UVec3(1));
		auto shape = WrapReference(new Graphics::OpenGL::Shape(graphicsEngine, geometry));
		auto model = WrapReference(new Graphics::OpenGL::Model(graphicsEngine, shape, graphicsMaterialFlat));
		graphicsObject->SetModel(model);
		graphicsScene->Add(graphicsObject);
	}


	auto selector = WrapReference(new Selectable::Selector());

	auto slot1 = WrapReference(new Slot(graphicsScene, graphicsMaterialFlat, Vec3(0.0f, 10.0f, 0.0f)));
	{
		selector->Add(slot1);
	}
	auto slot2 = WrapReference(new Slot(graphicsScene, graphicsMaterialFlat, Vec3(10.0f, 10.0f, 0.0f)));
	{
		selector->Add(slot2);

		slot1->links[static_cast<Size>(Inventory::Rotation::Right)] = slot2;
		slot2->links[static_cast<Size>(Inventory::Rotation::Left)] = slot1;
	}
	auto slot3 = WrapReference(new Slot(graphicsScene, graphicsMaterialFlat, Vec3(0.0f, 10.0f, 10.0f)));
	{
		selector->Add(slot3);

		slot1->links[static_cast<Size>(Inventory::Rotation::Front)] = slot3;
		slot3->links[static_cast<Size>(Inventory::Rotation::Back)] = slot1;
	}
	auto slot4 = WrapReference(new Slot(graphicsScene, graphicsMaterialFlat, Vec3(0.0f, 10.0f, 20.0f)));
	{
		selector->Add(slot4);

		slot3->links[static_cast<Size>(Inventory::Rotation::Front)] = slot4;
		slot4->links[static_cast<Size>(Inventory::Rotation::Back)] = slot3;
	}
	auto slot5 = WrapReference(new Slot(graphicsScene, graphicsMaterialFlat, Vec3(0.0f, 10.0f, -10.0f)));
	{
		selector->Add(slot5);

		slot1->links[static_cast<Size>(Inventory::Rotation::Back)] = slot5;
		slot5->links[static_cast<Size>(Inventory::Rotation::Front)] = slot1;
	}
	auto slot6 = WrapReference(new Slot(graphicsScene, graphicsMaterialFlat, Vec3(-10.0f, 10.0f, 10.0f)));
	{
		selector->Add(slot6);

		slot3->links[static_cast<Size>(Inventory::Rotation::Left)] = slot6;
		slot6->links[static_cast<Size>(Inventory::Rotation::Right)] = slot3;
	}
	auto slot7 = WrapReference(new Slot(graphicsScene, graphicsMaterialFlat, Vec3(-10.0f, 10.0f, 20.0f)));
	{
		selector->Add(slot7);

		slot7->links[static_cast<Size>(Inventory::Rotation::Right)] = slot4;
		slot4->links[static_cast<Size>(Inventory::Rotation::Left)] = slot7;

		slot7->links[static_cast<Size>(Inventory::Rotation::Back)] = slot6;
		slot6->links[static_cast<Size>(Inventory::Rotation::Front)] = slot7;
	}

	auto dock1 = WrapReference(new Dock());
	auto dock2 = WrapReference(new Dock());
	{
		dock1->links[static_cast<Size>(Inventory::Rotation::Front)] = dock2;
		dock2->links[static_cast<Size>(Inventory::Rotation::Back)] = dock1;
	}
	auto dock3 = WrapReference(new Dock());
	{
		dock1->links[static_cast<Size>(Inventory::Rotation::Right)] = dock3;
		dock3->links[static_cast<Size>(Inventory::Rotation::Left)] = dock1;
	}

	auto item1 = WrapReference(new Item1(graphicsScene, graphicsMaterialFlat));
	{
		for(auto &dock : item1->docks)
		{
			selector->Add(dock);
		}
	}
	auto item2 = WrapReference(new Item1(graphicsScene, graphicsMaterialFlat));
	{
		item2->SetLocalPosition(Vec3(0.0f, 0.0f, 20.0f));

		for(auto &dock : item2->docks)
		{
			selector->Add(dock);
		}
	}

	bool grab = false;
	Reference<Dock> pickedDock = nullptr;
	Vec3 previousPosition;
	Inventory::Rotation previousRotation;


	Inventory::Rotation rotation = Inventory::Rotation::Front;

	Vec3 p1, p2;

	while(!KeyState(Keys::ESC))
	{
		Input::Loop();
		window->Loop();
		// Input::Mouse::SetPosition(Vec2(Input::Mouse::GetDesktopSize()) / 2.0f);

		if(ButtonState(Buttons::Right))
		{
			camAng.x -= Input::Mouse::GetPositionDelta().y * 0.1f;
			camAng.y += Input::Mouse::GetPositionDelta().x * 0.2f;
		}

		graphicsCamera->SetPosition(camPos + RotateZXY3(camAng) * Vec3(0.0f, 0.0f, -camDist));
		graphicsCamera->SetAngle(camAng);


		auto m = Vec2(window->ToLocalPosition(IVec2(Input::Mouse::GetPosition())));

		graphicsScene->DrawCircle(m, 5.0f, Vec4(0, 1, 0, 1));

		auto p = (m / Vec2(window->GetSize())) * 2.0f - 1.0f;

		if(KeyState(Keys::UP)) rotation = Inventory::Rotation::Front;
		if(KeyState(Keys::DOWN)) rotation = Inventory::Rotation::Back;
		if(KeyState(Keys::LEFT)) rotation = Inventory::Rotation::Left;
		if(KeyState(Keys::RIGHT)) rotation = Inventory::Rotation::Right;

		auto angle = static_cast<Size>(rotation)* 90.0f;

		for(auto &i : selector->selections)
		{
			auto slot = std::dynamic_pointer_cast<Slot>(i);
			if(slot && slot->graphicsObject)
			{
				slot->graphicsObject->SetColor(Vec4(1.0f));
			}

			auto dock = std::dynamic_pointer_cast<Dock>(i);
			if(dock && dock->item && dock->item->graphicsObject)
			{
				dock->item->graphicsObject->SetColor(Vec4(1.0f));
			}
		}

		if(Input::Mouse::GetButtonState(Input::Mouse::Button::Left))
		{
			if(!grab)
			{
				auto picked = selector->Pick(graphicsCamera, p);

				auto casted = std::dynamic_pointer_cast<Dock>(picked);
				if(casted && casted->item)
				{
					pickedDock = casted;
					if(pickedDock->IsAttached())
					{
						pickedDock->Detach();
					}

					if(pickedDock->item)
					{
						rotation = pickedDock->item->rotation;
					}

					previousPosition = casted->item->GetLocalPosition();
					previousRotation = rotation;
				}

				grab = true;
			}
			else if(pickedDock && pickedDock->item)
			{
				auto picked = selector->Pick(graphicsCamera, p, [](Reference<Selectable::Selection> selection){
					return std::dynamic_pointer_cast<Slot>(selection) != nullptr;
				});

				auto slot = std::dynamic_pointer_cast<Slot>(picked);
				if(slot && slot->graphicsObject)
				{
					pickedDock->Glue(slot, rotation);

					if(pickedDock->IsConnectPossible(slot, rotation))
					{
						slot->graphicsObject->SetColor(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
					}
					else
					{
						slot->graphicsObject->SetColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
					}
				}
			}
		}
		else
		{
			if(pickedDock && pickedDock->item)
			{
				auto picked = selector->Pick(graphicsCamera, p, [](Reference<Selectable::Selection> selection){
					return std::dynamic_pointer_cast<Slot>(selection) != nullptr;
				});

				auto slot = std::dynamic_pointer_cast<Slot>(picked);
				if(slot && slot->graphicsObject)
				{
					if(pickedDock->IsConnectPossible(slot, rotation))
					{
						pickedDock->Glue(slot, rotation);

						pickedDock->Attach(slot, rotation);
					}
					else
					{
						pickedDock->Glue(previousPosition, previousRotation);
					}
				}
			}

			pickedDock = nullptr;
			grab = false;
		}

		if(pickedDock)
		{
			if(pickedDock->item && pickedDock->item->graphicsObject)
			{
				pickedDock->item->graphicsObject->SetColor(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
		}

		/*slot1->Loop(picked.get());
		slot2->Loop(picked.get());
		slot3->Loop(picked.get());
		slot4->Loop(picked.get());

		if(picked)
		{
			auto casted = std::dynamic_pointer_cast<Slot>(picked);
			if(casted)
			{
				if(dock1->IsConnectPossible(casted, rotation))
				{
					graphicsScene->DrawCircle(Vec2(200.0f), 50.0f, Vec4(0, 1, 0, 1));
				}
				else
				{
					graphicsScene->DrawCircle(Vec2(200.0f), 50.0f, Vec4(1, 0, 0, 1));
				}
			}
		}*/

		// std::cout << window->GetPosition().x << ", " << window->GetPosition().y << std::endl;

		// auto t = VecXYZ(graphicsCamera->GetPIMat() * Vec4(p, 1.0f, 1.0f));
		// if(ButtonState(Buttons::Left))
		// {
		// 	p1 = VecXYZ(graphicsCamera->GetViewInverseMatrix() * Vec4(t * 10.0f, 1.0f));
		// 	p2 = VecXYZ(graphicsCamera->GetViewInverseMatrix() * Vec4(t * 1000.0f, 1.0f));
		// }
		// 
		// graphicsScene->DrawCircle(p1, 1.0f, Vec4(1, 1, 0, 1));
		// graphicsScene->DrawCircle(p2, 1.0f, Vec4(1, 1, 0, 1));
		// graphicsScene->DrawLine(p1, p2, Vec4(1, 1, 0, 1));

		graphicsScene->Render(graphicsCamera);

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
