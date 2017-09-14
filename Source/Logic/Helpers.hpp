#pragma once


#include "Header.hpp"
#include "Mathematics.hpp"
#include <Header/ErrorHandling.hpp>
#include <Header/Function.hpp>


namespace GreatVEngine
{
	namespace Helper
	{
		namespace Subscrption
		{
			template<class ... Arguments> class OnEvent
			{
			public:
				// using Subscriber = void(*)(Arguments);
				using Subscriber = Function<void, Arguments...>;
			protected:
				Vector<Subscriber> subscribers;
			public:
				inline OnEvent() = default;
				inline OnEvent(const OnEvent&) = delete;
				inline ~OnEvent() = default;
			public:
				inline OnEvent& operator = (const OnEvent&) = delete;
				inline void operator += (Subscriber subscriber)
				{
					subscribers.push_back(subscriber);
				}
				inline void operator -= (Subscriber subscriber)
				{
					subscribers.erase(std::find(subscribers.begin(), subscribers.end(), subscriber));
				}
				inline void operator () (Arguments... arguments) const
				{
					for(auto &subscriber : subscribers)
					{
						subscriber(arguments...);
					}
				}
			};
			template<> class OnEvent<void>
			{
			public:
				// using Subscriber = void(*)();
				using Subscriber = Function<void, void>;
			protected:
				Vector<Subscriber> subscribers;
			public:
				inline OnEvent() = default;
				inline OnEvent(const OnEvent&) = delete;
				inline ~OnEvent() = default;
			public:
				inline OnEvent& operator = (const OnEvent&) = delete;
				inline void operator += (Subscriber subscriber)
				{
					subscribers.push_back(subscriber);
				}
				inline void operator -= (Subscriber subscriber)
				{
					subscribers.erase(std::find(subscribers.begin(), subscribers.end(), subscriber));
				}
				inline void operator () () const
				{
					for(auto &subscriber : subscribers)
					{
						subscriber();
					}
				}
			};
			
			template<class Arguments> class OnAction
			{
			public:
				using Data = void*;
				using Subscriber = bool(*)(Data, Arguments); // return true for remove from list
			public:
				struct Subscription
				{
					Data d;
					Subscriber s;
					inline bool operator == (const Subscription& x) { return d == x.d && s == x.s; }
				};
			protected:
				mutable Vector<Subscription> subscribers;
			public:
				inline OnAction() = default;
				inline OnAction(const OnAction&) = delete;
				inline ~OnAction() = default;
			public:
				inline OnAction& operator = (const OnAction&) = delete;
				inline void operator += (Subscription s)
				{
					subscribers.push_back(s);
				}
				inline void operator -= (Subscription s)
				{
					auto it = std::find(subscribers.begin(), subscribers.end(), s);
					if(it != subscribers.end())
					{
						subscribers.erase(it);
					}
					else
					{
						throw Exception("Unsibscribe of unexisting subscribe");
					}
				}
				inline void operator () (Arguments arguments) const
				{
					auto local = subscribers;

					for(auto it = local.begin(); it != local.end(); ++it)
					{
						auto &subscriber = *it;
						if(subscriber.s(subscriber.d, arguments))
						{
							subscribers.erase(std::find(subscribers.begin(), subscribers.end(), subscriber));
						}
					}
				}
			};
			template<> class OnAction<void>
			{
			public:
				using Data = void*;
				using Subscriber = bool(*)(Data);// return true for remove from list
			public:
				struct Subscription
				{
					Data d;
					Subscriber s;
					inline bool operator == (const Subscription& x) { return d == x.d && s == x.s; }
				};
			protected:
				mutable Vector<Subscription> subscribers;
			public:
				inline OnAction() = default;
				inline OnAction(const OnAction&) = delete;
				inline ~OnAction() = default;
			public:
				inline OnAction& operator = (const OnAction&) = delete;
				inline void operator += (Subscription s)
				{
					subscribers.push_back(s);
				}
				inline void operator -= (Subscription s)
				{
					subscribers.erase(std::find(subscribers.begin(), subscribers.end(), s));
				}
				inline void operator () () const
				{
					auto local = subscribers;

					for(auto it = local.begin(); it != local.end(); ++it)
					{
						auto &subscriber = *it;
						if(subscriber.s(subscriber.d))
						{
							subscribers.erase(std::find(subscribers.begin(), subscribers.end(), subscriber));
						}
					}
				}
			};

			class OnDestructionEvent
			{
			protected:
				OnEvent<void> onDestruction;
			public:
				inline OnDestructionEvent() = default;
				inline ~OnDestructionEvent()
				{
					onDestruction();
				}
			public:
				inline void Subscribe_OnDestruction(OnEvent<void>::Subscriber subscriber)
				{
					onDestruction += subscriber;
				}
				inline void Unsubscribe_OnDestruction(OnEvent<void>::Subscriber subscriber)
				{
					onDestruction -= subscriber;
				}
			};
			class OnDestructionAction
			{
			protected:
				OnAction<void> onDestruction;
			public:
				inline OnDestructionAction() = default;
				inline ~OnDestructionAction()
				{
					onDestruction();
				}
			public:
				inline void Subscribe_OnDestruction(void* data, OnAction<void>::Subscriber subscriber)
				{
					onDestruction += {data, subscriber};
				}
				inline void Unsubscribe_OnDestruction(void* data, OnAction<void>::Subscriber subscriber)
				{
					onDestruction -= {data, subscriber};
				}
			};
		}
		namespace Structure
		{
			template<typename T> class ClassCollection;
		}
		namespace Logic
		{
			class Named
			{
			public:
				using Name = String;
			protected:
				Name name;
			public:
				inline Name GetName() const
				{
					return name;
				}
				inline void SetName(const Name& name_)
				{
					name = name_;
				}
			};
			class Life
			{
			public:
				using Value = Float32;
			protected:
				Value life;
			public:
				inline Life() = default;
				inline Life(const Value& life_):
					life(life_)
				{
				}
				inline Life(const Life&) = default;
			public:
				inline Life& operator = (const Life& source)
				{
					life = source.life;

					return *this;
				}
			public:
				inline Value GetLife() const
				{
					return life;
				}
				inline void SetLife(const Value& life_)
				{
					life = life_;
				}
			};
			class Visible
			{
			protected:
				bool isVisible = false;
			public:
				inline Visible() = default;
				inline Visible(const bool& isVisible_):
					isVisible(isVisible_)
				{
				}
				inline Visible(const Visible&) = default;
			public:
				inline Visible& operator = (const Visible& source)
				{
					isVisible = source.isVisible;

					return *this;
				}
			public:
				inline bool IsVisible() const
				{
					return isVisible;
				}
				inline void SetVisible(const bool& isVisible_)
				{
					isVisible = isVisible_;
				}
				inline void Hide()
				{
					isVisible = false;
				}
				inline void Show()
				{
					isVisible = true;
				}
			};
			class Priority
			{
			public:
				using Value = Float32;
			protected:
				Value priority;
			public:
				inline Priority() = default;
				inline Priority(const Value& priority_):
					priority(priority_)
				{
				}
				inline Priority(const Priority&) = default;
			public:
				inline Priority& operator = (const Priority& source)
				{
					priority = source.priority;

					return *this;
				}
				inline bool operator < (const Priority& source) const
				{
					return priority < source.priority;
				}
				inline bool operator <= (const Priority& source) const
				{
					return priority <= source.priority;
				}
				inline bool operator > (const Priority& source) const
				{
					return priority > source.priority;
				}
				inline bool operator >= (const Priority& source) const
				{
					return priority >= source.priority;
				}
				inline bool operator == (const Priority& source) const
				{
					return priority == source.priority;
				}
				inline bool operator != (const Priority& source) const
				{
					return priority != source.priority;
				}
			public:
				inline Value GetPriority() const
				{
					return priority;
				}
				inline void SetPriority(const Value& priority_)
				{
					priority = priority_;
				}
			};
			class Color
			{
			public:
				using Value = Vec4;
			protected:
				Value color = Value(1.0f);
			public:
				inline Color() = default;
				inline Color(const Value& color_):
					color(color_)
				{
				}
				inline Color(const Color&) = default;
			public:
				inline Color& operator = (const Color& source)
				{
					color = source.color;

					return *this;
				}
			public:
				inline Value GetColor() const
				{
					return color;
				}
				inline void SetColor(const Value& color_)
				{
					color = color_;
				}
			};
			template<class T> class BitGroup
			{
			public:
				using Value = T;
				using OnGroupsChange = Subscrption::OnAction<Value>;
			public:
				static const Size GROUPS_COUNT = sizeof(Value) * BITS_IN_BYTE;
			protected:
				Value groups;
				OnGroupsChange onGroupsChange;
			protected:
				static inline Value Bit(const Size& bit_)
				{
#if GVE_DEBUG
					if(bit_ >= GROUPS_COUNT)
					{
						throw Exception("Bin index '" + std::to_string(bit_) + "' is out of range '" + std::to_string(GROUPS_COUNT) + "'");
					}
#endif
					return (Value)1 << bit_;
				}
				static inline Value Bit(const Initializer<Size>& bits_)
				{
					Value value;

					for(auto &bit : bits_)
					{
#if GVE_DEBUG
						if(bit >= GROUPS_COUNT)
						{
							throw Exception("Bin index '" + std::to_string(bit) + "' is out of range '" + std::to_string(GROUPS_COUNT) + "'");
						}
#endif
						value |= (Value)1 << bit;
					}

					return value;
				}
			public:
				inline BitGroup() = default;
				inline BitGroup(const Size& group_):
					groups(Bit(group_))
				{
				}
				inline BitGroup(const Initializer<Size>& groups_):
					groups(Bit(groups_))
				{

				}
			public:
				inline Value GetGroups() const
				{
					return groups;
				}
				inline void SetGroup(const Size& group_)
				{
					auto t = groups | Bit(group_);

					onGroupsChange(t);

					groups = t;
				}
				inline void SetGroup(const Initializer<Size>& groups_)
				{
					auto t = groups | Bit(groups_);

					onGroupsChange(t);

					groups = t;
				}
				inline void ResetGroup(const Size& group_)
				{
					auto t = groups & ~Bit(group_);

					onGroupsChange(t);

					groups = t;
				}
				inline void ResetGroup(const Initializer<Size>& groups_)
				{
					auto t = groups & ~Bit(groups_);

					onGroupsChange(t);

					groups = t;
				}
				inline bool IsGroup(const Size& group_) const
				{
					return (groups & Bit(group_)) != 0;
				}
				inline bool IsGroup(const Initializer<Size>& groups_) const
				{
					for(auto &group : groups_)
					{
						if((groups & Bit(group_)) != 0)
						{
							return true;
						}
					}

					return false;
				}
			public:
				template <class Pr_>
				inline void Subscribe_OnGroupsChange(void* data, Pr_ subscriber)
				{
					onGroupsChange += {data, subscriber};
				}
				template <class Pr_>
				inline void Unsubscribe_OnGroupsChange(void* data, Pr_ subscriber)
				{
					onGroupsChange -= {data, subscriber};
				}
				// inline void Subscribe_OnGroupsChange(void* data, OnGroupsChange::Subscriber subscriber)
				// {
				// 	onGroupsChange += {data, subscriber};
				// }
				// inline void Unsubscribe_OnGroupsChange(void* data, OnGroupsChange::Subscriber subscriber)
				// {
				// 	onGroupsChange -= {data, subscriber};
				// }
			};
		}
		namespace Transformation
		{
			namespace Dimension1
			{
				class Size;
				class Position;
				class Angle;
				class Scale;
				class Velocity;
				class Torque;

				class Position
				{
				public:
					using Value = Float32;
				protected:
					Value position;
				public:
					inline Position() = default;
					inline Position(const Value& position_):
						position(position_)
					{
					}
					inline Position(const Position&) = default;
				public:
					inline Position& operator = (const Position& source) = default;
				public:
					inline Value GetPosition() const
					{
						return position;
					}
					inline void SetPosition(const Value& position_)
					{
						position = position_;
					}
				};
				class Angle
				{
				public:
					using Value = Float32;
				protected:
					Value angle;
				public:
					inline Angle() = default;
					inline Angle(const Value& angle_):
						angle(angle_)
					{
					}
					inline Angle(const Angle&) = default;
				public:
					inline Angle& operator = (const Angle& source) = default;
				public:
					inline Value GetAngle() const
					{
						return angle;
					}
					inline void SetAngle(const Value& angle_)
					{
						angle = angle_;
					}
				};
				class Size
				{
				public:
					using Value = Float32;
				protected:
					Value size;
				public:
					inline Size() = default;
					inline Size(const Value& size_):
						size(size_)
					{
					}
					inline Size(const Size&) = default;
				public:
					inline Size& operator = (const Size& source) = default;
				public:
					inline Value GetSize() const
					{
						return size;
					}
					inline void SetSize(const Value& size_)
					{
						size = size_;
					}
				};
				class Scale
				{
				public:
					using Value = Float32;
				protected:
					Value scale;
				public:
					inline Scale() = default;
					inline Scale(const Value& scale_):
						scale(scale_)
					{
					}
					inline Scale(const Scale&) = default;
				public:
					inline Scale& operator = (const Scale& source) = default;
				public:
					inline Value GetScale() const
					{
						return scale;
					}
					inline void SetScale(const Value& scale_)
					{
						scale = scale_;
					}
				};
				class Velocity
				{
				public:
					using Value = Float32;
				protected:
					Value velocity;
				public:
					inline Velocity() = default;
					inline Velocity(const Value& velocity_):
						velocity(velocity_)
					{
					}
					inline Velocity(const Velocity&) = default;
				public:
					inline Velocity& operator = (const Velocity& source) = default;
				public:
					inline Value GetVelocity() const
					{
						return velocity;
					}
					inline void SetVelocity(const Value& velocity_)
					{
						velocity = velocity_;
					}
				};
				class Torque
				{
				public:
					using Value = Float32;
				protected:
					Value omega;
				public:
					inline Torque() = default;
					inline Torque(const Value& omega_):
						omega(omega_)
					{
					}
					inline Torque(const Torque&) = default;
				public:
					inline Torque& operator = (const Torque& source) = default;
				public:
					inline Value GetTorque() const
					{
						return omega;
					}
					inline void SetTorque(const Value& omega_)
					{
						omega = omega_;
					}
				};
			}
			namespace Dimension2
			{
				class Homogeneous;
				class Size;
				class Position;
				class Angle;
				class Scale;
				class Velocity;
				class Torque;

				class Homogeneous
				{
				public:
					using Value = Vec2;
				public:
					enum class Side: UInt32
					{
						None,
						Center,
						Left,
						Right,
						Bottom,
						Top,
						LeftBottom,
						LeftTop,
						RightBottom,
						RightTop,
					};
				public:
					static const Value DEFAULT_VALUE;
				public:
					inline static Value GetValue(const Side& side_)
					{
						switch(side_)
						{
							case Side::Center:		return Value(+0.0f, +0.0f);
							case Side::Left:		return Value(-1.0f, +0.0f);
							case Side::Right:		return Value(+1.0f, +0.0f);
							case Side::Bottom:		return Value(+0.0f, -1.0f);
							case Side::Top:			return Value(+0.0f, +1.0f);
							case Side::LeftBottom:	return Value(-1.0f, -1.0f);
							case Side::LeftTop:		return Value(-1.0f, +1.0f);
							case Side::RightBottom:	return Value(+1.0f, -1.0f);
							case Side::RightTop:	return Value(+1.0f, +1.0f);
							default: throw Exception("Invalid side: " + std::to_string((UInt32)side_));
						}
					}
				protected:
					Value value = DEFAULT_VALUE; // [-1.0f, +1.0f] left->right bottom->top
				public:
					inline Homogeneous() = default;
					inline Homogeneous(const Value& value_):
						value(value_)
					{
					}
					inline Homogeneous(const Side& side_):
						value(GetValue(side_))
					{
					}
					inline Homogeneous(const Homogeneous&) = default;
					inline ~Homogeneous() = default;
				public:
					inline Homogeneous& operator = (const Homogeneous&) = default;
				public:
					inline void SetValue(const Value& value_)
					{
						value = value_;
					}
					inline void SetValue(const Side& side_)
					{
						value = GetValue(side_);
					}
					inline Value GetValue() const
					{
						return value;
					}
				};
				class Position
				{
				public:
					using Value = Vec2;
				protected:
					Value position;
				public:
					inline Position() = default;
					inline Position(const Value& position_):
						position(position_)
					{
					}
					inline Position(const Position&) = default;
				public:
					inline Position& operator = (const Position& source) = default;
				public:
					inline Value GetPosition() const
					{
						return position;
					}
					inline void SetPosition(const Value& position_)
					{
						position = position_;
					}
				};
				class Angle
				{
				public:
					using Value = Vec2;
				protected:
					Value angle;
				public:
					inline Angle() = default;
					inline Angle(const Value& angle_):
						angle(angle_)
					{
					}
					inline Angle(const Angle&) = default;
				public:
					inline Angle& operator = (const Angle& source) = default;
				public:
					inline Value GetAngle() const
					{
						return angle;
					}
					inline void SetAngle(const Value& angle_)
					{
						angle = angle_;
					}
				};
				class Size
				{
				public:
					using Value = Vec2;
				protected:
					Value size;
				public:
					inline Size() = default;
					inline Size(const Value& size_):
						size(size_)
					{
					}
					inline Size(const Size&) = default;
				public:
					inline Size& operator = (const Size& source) = default;
				public:
					inline Value GetSize() const
					{
						return size;
					}
					inline void SetSize(const Value& size_)
					{
						size = size_;
					}
				};
				class Scale
				{
				public:
					using Value = Vec2;
				protected:
					Value scale;
				public:
					inline Scale() = default;
					inline Scale(const Value& scale_):
						scale(scale_)
					{
					}
					inline Scale(const Scale&) = default;
				public:
					inline Scale& operator = (const Scale& source) = default;
				public:
					inline Value GetScale() const
					{
						return scale;
					}
					inline void SetScale(const Value& scale_)
					{
						scale = scale_;
					}
				};
				class Velocity
				{
				public:
					using Value = Vec2;
				protected:
					Value velocity;
				public:
					inline Velocity() = default;
					inline Velocity(const Value& velocity_):
						velocity(velocity_)
					{
					}
					inline Velocity(const Velocity&) = default;
				public:
					inline Velocity& operator = (const Velocity& source) = default;
				public:
					inline Value GetVelocity() const
					{
						return velocity;
					}
					inline void SetVelocity(const Value& velocity_)
					{
						velocity = velocity_;
					}
				};
				class Torque
				{
				public:
					using Value = Vec2;
				protected:
					Value omega;
				public:
					inline Torque() = default;
					inline Torque(const Value& omega_):
						omega(omega_)
					{
					}
					inline Torque(const Torque&) = default;
				public:
					inline Torque& operator = (const Torque& source) = default;
				public:
					inline Value GetTorque() const
					{
						return omega;
					}
					inline void SetTorque(const Value& omega_)
					{
						omega = omega_;
					}
				};
			}
			namespace Dimension3
			{
				namespace Projection
				{
					enum struct Type
					{
						Orthogonal,
						Perspective
					};
					struct Params
					{
						struct Orthogonal
						{
						public:
							Float32
								left,
								right,
								bottom,
								top,
								back,
								front;
						public:
							inline Orthogonal() = default;
							inline Orthogonal(float32 left_, float32 right_, float32 bottom_, float32 top_, float32 back_, float32 front_):
								left(left_),
								right(right_),
								bottom(bottom_),
								top(top_),
								back(back_),
								front(front_)
							{
							}
						public:
							inline bool operator != (const Orthogonal& source) const
							{
								return(
									left != source.left ||
									right != source.right ||
									bottom != source.bottom ||
									top != source.top ||
									back != source.back ||
									front != source.front);
							}
						};
						struct Perspective
						{
						public:
							float32
								fov,
								aspect,
								zNear,
								zFar;
						public:
							inline Perspective() = default;
							inline Perspective(float32 fov_, float32 aspect_, float32 zNear_, float32 zFar_):
								fov(fov_),
								aspect(aspect_),
								zNear(zNear_),
								zFar(zFar_)
							{
							}
						public:
							inline bool operator != (const Perspective& source) const
							{
								return(
									fov != source.fov ||
									aspect != source.aspect ||
									zNear != source.zNear ||
									zFar != source.zFar);
							}
						};
					};
				}

				class BoundBox
				{
				public:
					using Value = Vec3;
				protected:
					Value minimal;
					Value maximal;
				public:
					inline BoundBox() = default;
					inline BoundBox(const Value& minimal_, const Value& maximal_):
						minimal(minimal_),
						maximal(maximal_)
					{
					}
					inline BoundBox(const BoundBox& source_) = default;
					inline BoundBox(BoundBox&& source_):
						minimal(source_.minimal),
						maximal(source_.maximal)
					{
					}
				public:
					inline BoundBox& operator = (const BoundBox&) = default;
					inline BoundBox& operator = (BoundBox&& source_)
					{
						minimal = source_.minimal;
						maximal = source_.maximal;
					}
				public:
					inline Value GetMinimal() const
					{
						return minimal;
					}
					inline void SetMinimal(const Value& value_)
					{
						minimal = value_;
					}
					inline Value GetMaximal() const
					{
						return maximal;
					}
					inline void SetMaximal(const Value& value_)
					{
						maximal = value_;
					}
					inline Value GetMin() const
					{
						return GetMinimal();
					}
					inline void SetMin(const Value& value_)
					{
						SetMinimal(value_);
					}
					inline Value GetMax() const
					{
						return GetMaximal();
					}
					inline void SetMax(const Value& value_)
					{
						SetMaximal(value_);
					}
				};
				class ConstantBoundBox
				{
				public:
					using Value = Vec3;
				protected:
					const Value minimal;
					const Value maximal;
				public:
					inline ConstantBoundBox(const Value& minimal_, const Value& maximal_):
						minimal(minimal_),
						maximal(maximal_)
					{
					}
					inline ConstantBoundBox(const ConstantBoundBox& source_) = default;
					inline ConstantBoundBox(ConstantBoundBox&& source_) :
						minimal(source_.minimal),
						maximal(source_.maximal)
					{
					}
				public:
					inline Value GetMinimal() const
					{
						return minimal;
					}
					inline Value GetMaximal() const
					{
						return maximal;
					}
					inline Value GetMin() const
					{
						return GetMinimal();
					}
					inline Value GetMax() const
					{
						return GetMaximal();
					}
				};
				class Position
				{
				public:
					using Value = Vec3;
				protected:
					Value position;
				public:
					inline Position() = default;
					inline Position(const Value& position_):
						position(position_)
					{
					}
					inline Position(const Position&) = default;
				public:
					inline Position& operator = (const Position& source) = default;
				public:
					inline Value GetPosition() const
					{
						return position;
					}
					inline void SetPosition(const Value& position_)
					{
						position = position_;
					}
				};
				class Angle
				{
				public:
					using Value = Vec3;
				protected:
					Value angle;
				public:
					inline Angle() = default;
					inline Angle(const Value& angle_):
						angle(angle_)
					{
					}
					inline Angle(const Angle&) = default;
				public:
					inline Angle& operator = (const Angle& source) = default;
				public:
					inline Value GetAngle() const
					{
						return angle;
					}
					inline void SetAngle(const Value& angle_)
					{
						angle = angle_;
					}
				};
				class Size
				{
				public:
					using Value = Vec3;
				protected:
					Value size;
				public:
					inline Size() = default;
					inline Size(const Value& size_):
						size(size_)
					{
					}
					inline Size(const Size&) = default;
				public:
					inline Size& operator = (const Size& source) = default;
				public:
					inline Value GetSize() const
					{
						return size;
					}
					inline void SetSize(const Value& size_)
					{
						size = size_;
					}
				};
				class Scale
				{
				public:
					using Value = Vec3;
				protected:
					Value scale;
				public:
					inline Scale() = default;
					inline Scale(const Value& scale_):
						scale(scale_)
					{
					}
					inline Scale(const Scale&) = default;
				public:
					inline Scale& operator = (const Scale& source) = default;
				public:
					inline Value GetScale() const
					{
						return scale;
					}
					inline void SetScale(const Value& scale_)
					{
						scale = scale_;
					}
				};
				class Velocity
				{
				public:
					using Value = Vec3;
				protected:
					Value velocity;
				public:
					inline Velocity() = default;
					inline Velocity(const Value& velocity_):
						velocity(velocity_)
					{
					}
					inline Velocity(const Velocity&) = default;
				public:
					inline Velocity& operator = (const Velocity& source) = default;
				public:
					inline Value GetVelocity() const
					{
						return velocity;
					}
					inline void SetVelocity(const Value& velocity_)
					{
						velocity = velocity_;
					}
				};
				class Torque
				{
				public:
					using Value = Vec3;
				protected:
					Value omega;
				public:
					inline Torque() = default;
					inline Torque(const Value& omega_):
						omega(omega_)
					{
					}
					inline Torque(const Torque&) = default;
				public:
					inline Torque& operator = (const Torque& source) = default;
				public:
					inline Value GetTorque() const
					{
						return omega;
					}
					inline void SetTorque(const Value& omega_)
					{
						omega = omega_;
					}
				};
				class ProjectionMatrix;
				class RotateMatrix:
					protected Angle
				{
				public:
					using Value = Mat3;
				protected:
					using UpdateFlags = UInt32;
					enum class UpdateFlagBits: UpdateFlags
					{
						None = 0x00,
						DirectRotateMatrixUpdated = 0x01,
						InverseRotateMatrixUpdated = 0x02,
					};
				protected:
					mutable UpdateFlags updateFlags = (UpdateFlags)UpdateFlagBits::None;
					mutable Value directRotateMatrix;
					mutable Value inverseRotateMatrix;
				public:
					inline RotateMatrix() = default;
					inline explicit RotateMatrix(const Angle& angle_):
						Angle(angle_)
					{
					}
					inline explicit RotateMatrix(const Angle::Value& angle_):
						Angle(angle_)
					{
					}
				public:
					inline void SetAngle(const Angle::Value& angle_)
					{
						if(angle_ != angle)
						{
							angle = angle_;
							updateFlags &= ~((UpdateFlags)UpdateFlagBits::DirectRotateMatrixUpdated | (UpdateFlags)UpdateFlagBits::InverseRotateMatrixUpdated);
						}
					}
					inline Angle::Value GetAngle() const
					{
						return Angle::GetAngle();
					}
					inline Value GetRotateMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::DirectRotateMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::DirectRotateMatrixUpdated;
							directRotateMatrix = RotateZXY3(angle);
						}

						return directRotateMatrix;
					}
					inline Value GetRotateInverseMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::InverseRotateMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::InverseRotateMatrixUpdated;
							inverseRotateMatrix = RotateYXZ3(-angle);
						}

						return inverseRotateMatrix;
					}
				public:
					inline Value GetRMat() const
					{
						return GetRotateMatrix();
					}
					inline Value GetRIMat() const
					{
						return GetRotateInverseMatrix();
					}
				};
				class ViewMatrix:
					protected Position,
					protected RotateMatrix
				{
				public:
					using Value = Mat4;
				protected:
					enum class UpdateFlagBits: RotateMatrix::UpdateFlags
					{
						None = 0x00,
						DirectRotateMatrixUpdated = 0x01,
						InverseRotateMatrixUpdated = 0x02,
						DirectViewMatrixUpdated = 0x04,
						InverseViewMatrixUpdated = 0x08,
					};
					mutable Value directViewMatrix;
					mutable Value inverseViewMatrix;
				public:
					inline ViewMatrix() = default;
					inline ViewMatrix(const Position& position_, const Angle& angle_):
						Position(position_),
						RotateMatrix(angle_)
					{
					}
					inline ViewMatrix(const Position::Value& position_, const Angle::Value& angle_):
						Position(position_),
						RotateMatrix(angle_)
					{
					}
				public:
					inline void SetAngle(const Angle::Value& angle_)
					{
						if(angle_ != angle)
						{
							angle = angle_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectRotateMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseRotateMatrixUpdated);
						}
					}
					inline Angle::Value GetAngle() const
					{
						return RotateMatrix::GetAngle();
					}
					inline RotateMatrix::Value GetRotateMatrix() const
					{
						return RotateMatrix::GetRotateMatrix();
					}
					inline RotateMatrix::Value GetRotateInverseMatrix() const
					{
						return RotateMatrix::GetRotateInverseMatrix();
					}
					inline void SetPosition(const Position::Value& position_)
					{
						if(position_ != position)
						{
							position = position_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewMatrixUpdated);
						}
					}
					inline Position::Value GetPosition() const
					{
						return Position::GetPosition();
					}
					inline Value GetViewMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::DirectViewMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::DirectViewMatrixUpdated;
							directViewMatrix = To4(GetRotateInverseMatrix()) * Move4(-position);
						}

						return directViewMatrix;
					}
					inline Value GetViewInverseMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::InverseViewMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::InverseViewMatrixUpdated;
							inverseViewMatrix = Move4(position) * To4(GetRotateMatrix());
						}

						return inverseViewMatrix;
					}
					inline void Rotate(const Angle::Value& angle_)
					{
						SetAngle(GreatVEngine::GetAngle(GetRotateMatrix() * RotateZXY3(angle_)));
					}
					inline void Move(const Position::Value& position_)
					{
						SetPosition(VecXYZ(GetViewInverseMatrix() * Vec4(position_, 1.0f)));
					}
				public:
					inline RotateMatrix::Value GetRMat() const
					{
						return RotateMatrix::GetRMat();
					}
					inline RotateMatrix::Value GetRIMat() const
					{
						return RotateMatrix::GetRIMat();
					}
					inline Value GetVMat() const
					{
						return GetViewMatrix();
					}
					inline Value GetVIMat() const
					{
						return GetViewInverseMatrix();
					}
				};
				class ViewProjectionMatrix:
					protected ViewMatrix
				{
				public:
					using Value = Mat4;
				public:
					enum class Type
					{
						Perspective,
						Orthogonal
					};
					enum class UpdateFlagBits: ViewMatrix::UpdateFlags
					{
						None = 0x00,
						DirectRotateMatrixUpdated = 0x01,
						InverseRotateMatrixUpdated = 0x02,
						DirectViewMatrixUpdated = 0x04,
						InverseViewMatrixUpdated = 0x08,
						DirectProjectionMatrixUpdated = 0x10,
						InverseProjectionMatrixUpdated = 0x20,
						DirectViewProjectionMatrixUpdated = 0x40,
						InverseViewProjectionMatrixUpdated = 0x80,
					};
				protected:
					Type type;
					Projection::Params::Orthogonal orthogonal;
					Projection::Params::Perspective perspective;
					mutable Value directProjectionMatrix;
					mutable Value inverseProjectionMatrix;
					mutable Value directViewProjectionMatrix;
					mutable Value inverseViewProjectionMatrix;
				public:
					inline ViewProjectionMatrix() = default;
					inline ViewProjectionMatrix(const Position& position_, const Angle& angle_, const Projection::Params::Orthogonal& orthogonal_):
						ViewMatrix(position_, angle_),
						type(Type::Orthogonal),
						orthogonal(orthogonal_),
						perspective()
					{
					}
					inline ViewProjectionMatrix(const Position& position_, const Angle& angle_, const Projection::Params::Perspective& perspective_):
						ViewMatrix(position_, angle_),
						type(Type::Perspective),
						orthogonal(),
						perspective(perspective_)
					{
					}
					inline ViewProjectionMatrix(const Position::Value& position_, const Angle::Value& angle_, const Projection::Params::Orthogonal& orthogonal_):
						ViewMatrix(position_, angle_),
						type(Type::Orthogonal),
						orthogonal(orthogonal_),
						perspective()
					{
					}
					inline ViewProjectionMatrix(const Position::Value& position_, const Angle::Value& angle_, const Projection::Params::Perspective& perspective_):
						ViewMatrix(position_, angle_),
						type(Type::Perspective),
						orthogonal(),
						perspective(perspective_)
					{
					}
				public:
					inline void SetAngle(const Angle::Value& angle_)
					{
						if(angle_ != angle)
						{
							angle = angle_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectRotateMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseRotateMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectViewProjectionMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewProjectionMatrixUpdated);
						}
					}
					inline Angle::Value GetAngle() const
					{
						return RotateMatrix::GetAngle();
					}
					inline RotateMatrix::Value GetRotateMatrix() const
					{
						return ViewMatrix::GetRotateMatrix();
					}
					inline RotateMatrix::Value GetRotateInverseMatrix() const
					{
						return ViewMatrix::GetRotateInverseMatrix();
					}
					inline void SetPosition(const Position::Value& position_)
					{
						if(position_ != position)
						{
							position = position_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectViewProjectionMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewProjectionMatrixUpdated);
						}
					}
					inline Position::Value GetPosition() const
					{
						return Position::GetPosition();
					}
					inline Value GetViewMatrix() const
					{
						return ViewMatrix::GetViewMatrix();
					}
					inline Value GetViewInverseMatrix() const
					{
						return ViewMatrix::GetViewInverseMatrix();
					}
					inline void SetProjection(const Projection::Params::Perspective& perspective_)
					{
						if(perspective != perspective_)
						{
							type = Type::Perspective;
							perspective = perspective_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectProjectionMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseProjectionMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectViewProjectionMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewProjectionMatrixUpdated);
						}
					}
					inline void SetProjection(const Projection::Params::Orthogonal& orthogonal_)
					{
						if(orthogonal != orthogonal_)
						{
							type = Type::Orthogonal;
							orthogonal = orthogonal_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectProjectionMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseProjectionMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectViewProjectionMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewProjectionMatrixUpdated);
						}
					}
					inline Type GetProjectionType() const
					{
						return type;
					}
					inline Projection::Params::Orthogonal GetOrthogonal() const
					{
						return orthogonal;
					}
					inline Projection::Params::Perspective GetPerspective() const
					{
						return perspective;
					}
					inline Value GetProjectionMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::DirectProjectionMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::DirectProjectionMatrixUpdated;
							directProjectionMatrix = (type == Type::Perspective) ?
								Perspective(perspective.fov, perspective.aspect, perspective.zNear, perspective.zFar) :
								Orthogonal(orthogonal.left, orthogonal.right, orthogonal.bottom, orthogonal.top, orthogonal.back, orthogonal.front);
						}

						return directProjectionMatrix;
					}
					inline Value GetProjectionInverseMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::InverseProjectionMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::InverseProjectionMatrixUpdated;
							inverseProjectionMatrix = (type == Type::Perspective) ?
								PerspectiveInverse(perspective.fov, perspective.aspect, perspective.zNear, perspective.zFar) :
								OrthogonalInverse(orthogonal.left, orthogonal.right, orthogonal.bottom, orthogonal.top, orthogonal.back, orthogonal.front);
						}

						return inverseProjectionMatrix;
					}
					inline Value GetViewProjectionMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::DirectViewProjectionMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::DirectViewProjectionMatrixUpdated;
							directViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
						}

						return directViewProjectionMatrix;
					}
					inline Value GetViewProjectionInverseMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::InverseViewProjectionMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::InverseViewProjectionMatrixUpdated;
							inverseViewProjectionMatrix = GetViewInverseMatrix() * GetProjectionInverseMatrix();
						}

						return inverseViewProjectionMatrix;
					}
					inline void Rotate(const Angle::Value& angle_)
					{
						SetAngle(GreatVEngine::GetAngle(GetRotateMatrix() * RotateZXY3(angle_)));
					}
					inline void Move(const Position::Value& position_)
					{
						SetPosition(VecXYZ(GetViewInverseMatrix() * Vec4(position_, 1.0f)));
					}
				public:
					inline RotateMatrix::Value GetRMat() const
					{
						return ViewMatrix::GetRMat();
					}
					inline RotateMatrix::Value GetRIMat() const
					{
						return ViewMatrix::GetRIMat();
					}
					inline Value GetVMat() const
					{
						return ViewMatrix::GetViewMatrix();
					}
					inline Value GetVIMat() const
					{
						return ViewMatrix::GetViewInverseMatrix();
					}
					inline Value GetPMat() const
					{
						return GetProjectionMatrix();
					}
					inline Value GetPIMat() const
					{
						return GetProjectionInverseMatrix();
					}
					inline Value GetVPMat() const
					{
						return GetViewProjectionMatrix();
					}
					inline Value GetVPIMat() const
					{
						return GetViewProjectionInverseMatrix();
					}
				};
				class ModelMatrix:
					protected Scale,
					protected ViewMatrix
				{
				public:
					using Value = Mat4;
				protected:
					enum class UpdateFlagBits: RotateMatrix::UpdateFlags
					{
						None = 0x00,
						DirectRotateMatrixUpdated = 0x01,
						InverseRotateMatrixUpdated = 0x02,
						DirectViewMatrixUpdated = 0x04,
						InverseViewMatrixUpdated = 0x08,
						DirectModelMatrixUpdated = 0x10,
						InverseModelMatrixUpdated = 0x20,
					};
					mutable Value directModelMatrix;
					mutable Value inverseModelMatrix;
				public:
					inline ModelMatrix() = default;
					inline ModelMatrix(const Position& position_, const Angle& angle_, const Scale& scale_):
						Scale(scale_),
						ViewMatrix(position_, angle_)
					{
					}
					inline ModelMatrix(const Position::Value& position_, const Angle::Value& angle_, const Scale::Value& scale_):
						Scale(scale_),
						ViewMatrix(position_, angle_)
					{
					}
				public:
					inline void SetAngle(const Angle::Value& angle_)
					{
						if(angle_ != angle)
						{
							angle = angle_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectRotateMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseRotateMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectModelMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseModelMatrixUpdated);
						}
					}
					inline Angle::Value GetAngle() const
					{
						return ViewMatrix::GetAngle();
					}
					inline RotateMatrix::Value GetRotateMatrix() const
					{
						return ViewMatrix::GetRotateMatrix();
					}
					inline RotateMatrix::Value GetRotateInverseMatrix() const
					{
						return ViewMatrix::GetRotateInverseMatrix();
					}
					inline void SetPosition(const Position::Value& position_)
					{
						if(position_ != position)
						{
							position = position_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseViewMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::DirectModelMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseModelMatrixUpdated);
						}
					}
					inline Position::Value GetPosition() const
					{
						return Position::GetPosition();
					}
					inline ViewMatrix::Value GetViewMatrix() const
					{
						return ViewMatrix::GetViewMatrix();
					}
					inline ViewMatrix::Value GetViewInverseMatrix() const
					{
						return ViewMatrix::GetViewInverseMatrix();
					}
					inline Scale::Value GetScale() const
					{
						return Scale::GetScale();
					}
					inline void SetScale(const Scale::Value& scale_)
					{
						if(scale_ != scale)
						{
							scale = scale_;
							updateFlags &= ~(
								(UpdateFlags)UpdateFlagBits::DirectModelMatrixUpdated |
								(UpdateFlags)UpdateFlagBits::InverseModelMatrixUpdated);
						}
					}
					inline Value GetModelMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::DirectModelMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::DirectModelMatrixUpdated;
							directModelMatrix = GetViewInverseMatrix() * Scale4(scale);
						}

						return directModelMatrix;
					}
					inline Value GetModelInverseMatrix() const
					{
						if((updateFlags & (UpdateFlags)UpdateFlagBits::InverseModelMatrixUpdated) == 0)
						{
							updateFlags |= (UpdateFlags)UpdateFlagBits::InverseModelMatrixUpdated;
							inverseModelMatrix = Scale4(1.0f / scale) * GetViewMatrix();
						}

						return inverseModelMatrix;
					}
					inline void Rotate(const Angle::Value& angle_)
					{
						SetAngle(GreatVEngine::GetAngle(GetRotateMatrix() * RotateZXY3(angle_)));
					}
					inline void Move(const Position::Value& position_)
					{
						SetPosition(VecXYZ(GetModelMatrix() * Vec4(position_, 1.0f)));
					}
				public:
					inline RotateMatrix::Value GetRMat() const
					{
						return RotateMatrix::GetRMat();
					}
					inline RotateMatrix::Value GetRIMat() const
					{
						return RotateMatrix::GetRIMat();
					}
					inline ViewMatrix::Value GetVMat() const
					{
						return ViewMatrix::GetVMat();
					}
					inline ViewMatrix::Value GetVIMat() const
					{
						return ViewMatrix::GetVIMat();
					}
					inline Value GetMMat() const
					{
						return GetModelMatrix();
					}
					inline Value GetMIMat() const
					{
						return GetModelInverseMatrix();
					}
				};
				class HierarchyMatrix:
					protected ModelMatrix
				{
				public:
					using Parent = HierarchyMatrix*;
				protected:
					Parent parent;
				public:
					inline HierarchyMatrix() = default;
					inline HierarchyMatrix(const Position& position_, const Angle& angle_, const Scale& scale_, const Parent& parent_ = nullptr):
						ModelMatrix(position_, angle_, scale_),
						parent(parent_)
					{
					}
					inline HierarchyMatrix(const Position::Value& position_, const Angle::Value& angle_, const Scale::Value& scale_, const Parent& parent_ = nullptr) :
						ModelMatrix(position_, angle_, scale_),
						parent(parent_)
					{
					}
				public:
					inline void SetParent(Parent parent_)
					{
						parent = parent_;
					}
					inline Parent GetParent() const
					{
						return parent;
					}
				public:
					inline void SetLocalAngle(const Angle::Value& angle_)
					{
						ModelMatrix::SetAngle(angle_);
					}
					inline void SetLocalPosition(const Position::Value& position_)
					{
						ModelMatrix::SetPosition(position_);
					}
					inline void SetLocalScale(const Scale::Value& scale_)
					{
						ModelMatrix::SetScale(scale_);
					}
					inline Angle::Value GetLocalAngle() const
					{
						return ModelMatrix::GetAngle();
					}
					inline Position::Value GetLocalPosition() const
					{
						return ModelMatrix::GetPosition();
					}
					inline Scale::Value GetLocalScale() const
					{
						return ModelMatrix::GetScale();
					}
					inline RotateMatrix::Value GetLocalRotateMatrix() const
					{
						return ModelMatrix::GetRotateMatrix();
					}
					inline RotateMatrix::Value GetLocalRotateInverseMatrix() const
					{
						return ModelMatrix::GetRotateInverseMatrix();
					}
					inline ViewMatrix::Value GetLocalViewMatrix() const
					{
						return ModelMatrix::GetViewMatrix();
					}
					inline ViewMatrix::Value GetLocalViewInverseMatrix() const
					{
						return ModelMatrix::GetViewInverseMatrix();
					}
					inline ModelMatrix::Value GetLocalModelMatrix() const
					{
						return ModelMatrix::GetModelMatrix();
					}
					inline ModelMatrix::Value GetLocalModelInverseMatrix() const
					{
						return ModelMatrix::GetModelInverseMatrix();
					}
				public:
					inline RotateMatrix::Value GetLRMat() const
					{
						return ModelMatrix::GetRMat();
					}
					inline RotateMatrix::Value GetLRIMat() const
					{
						return ModelMatrix::GetRIMat();
					}
					inline ViewMatrix::Value GetLVMat() const
					{
						return ModelMatrix::GetVMat();
					}
					inline ViewMatrix::Value GetLVIMat() const
					{
						return ModelMatrix::GetVIMat();
					}
					inline Value GetLMMat() const
					{
						return ModelMatrix::GetMMat();
					}
					inline Value GetLMIMat() const
					{
						return ModelMatrix::GetMIMat();
					}
					inline void LocalRotate(const Angle::Value& angle_)
					{
						SetLocalAngle(GreatVEngine::GetAngle(GetLocalRotateMatrix() * RotateZXY3(angle_)));
					}
					inline void LocalMove(const Position::Value& position_)
					{
						SetLocalPosition(VecXYZ(GetLocalModelMatrix() * Vec4(position_, 1.0f)));
					}
				public:
					inline void SetAngle(const Angle::Value& angle_);
					inline void SetPosition(const Position::Value& position_);
					inline void SetScale(const Scale::Value& scale_);
					inline Angle::Value GetAngle() const
					{
						// TODO: ensure that this is OK
						return GreatVEngine::GetAngle(parent ?
							parent->GetRotateMatrix() * GetLocalRotateMatrix() :
							GetLocalRotateMatrix());
					}
					inline Position::Value GetPosition() const
					{
						if(parent)
						{
							auto v = parent->GetMMat() * Vec4(position, 1.0f);

							return Vec3(v.x, v.y, v.z);
						}

						return position;
					}
					inline Scale::Value GetScale() const;
					inline RotateMatrix::Value GetRotateMatrix() const
					{
						return parent ?
							parent->GetRotateMatrix() * GetLocalRotateMatrix() :
							GetLocalRotateMatrix();
					}
					inline RotateMatrix::Value GetRotateInverseMatrix() const
					{
						return parent ?
							GetLocalRotateInverseMatrix() * parent->GetRotateInverseMatrix() :
							GetLocalRotateInverseMatrix();
					}
					inline ViewMatrix::Value GetViewMatrix() const
					{
						return parent ?
							parent->GetViewMatrix() * GetLocalViewMatrix() :
							GetLocalViewMatrix();
					}
					inline ViewMatrix::Value GetViewInverseMatrix() const
					{
						return parent ?
							GetLocalViewInverseMatrix() * parent->GetViewInverseMatrix() :
							GetLocalViewInverseMatrix();
					}
					inline Value GetModelMatrix() const
					{
						return parent ?
							parent->GetModelMatrix() * GetLocalModelMatrix() :
							GetLocalModelMatrix();
					}
					inline Value GetModelInverseMatrix() const
					{
						return parent ?
							GetLocalModelInverseMatrix() * parent->GetModelInverseMatrix() :
							GetLocalModelInverseMatrix();
					}
				public:
					inline RotateMatrix::Value GetRMat() const
					{
						return GetRotateMatrix();
					}
					inline RotateMatrix::Value GetRIMat() const
					{
						return GetRotateInverseMatrix();
					}
					inline ViewMatrix::Value GetVMat() const
					{
						return GetViewMatrix();
					}
					inline ViewMatrix::Value GetVIMat() const
					{
						return GetViewInverseMatrix();
					}
					inline Value GetMMat() const
					{
						return GetModelMatrix();
					}
					inline Value GetMIMat() const
					{
						return GetModelInverseMatrix();
					}
				};
			}

			using Pos3	= Dimension3::Position;
			using HMat3	= Dimension3::HierarchyMatrix;
		}
	}
}


#pragma region
#pragma endregion


