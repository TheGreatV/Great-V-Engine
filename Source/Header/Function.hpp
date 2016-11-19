#pragma region Include
#pragma once


#include "Header.hpp"
#include "ErrorHandling.hpp"


#include <string>
#pragma endregion


namespace GreatVEngine
{
	template<class Result, class ... Arguments> class Function
	{
	protected:
		struct Holder;
		struct HolderFunction;
		struct HolderFunctor;
		struct HolderMethod;

		struct Holder
		{
			virtual bool Compare(Holder*) = 0;
			virtual bool Compare(HolderFunction*) = 0;
			virtual bool Compare(HolderFunctor*) = 0;
			virtual bool Compare(HolderMethod*) = 0;
			virtual Result Call(Arguments...) = 0;
		};
		struct HolderFunction: public Holder
		{
			using Pointer = Result(*)(Arguments...);
			Pointer function = nullptr;
			HolderFunction(Pointer function_): function(function_) {}
		public:
			virtual bool Compare(Holder* x) override
			{
				return x && x->Compare(this);
			}
			virtual bool Compare(HolderFunction* x) override
			{
				return x && (function == x->function);
			}
			virtual bool Compare(HolderFunctor* x) override
			{
				return false;
			}
			virtual bool Compare(HolderMethod* x) override
			{
				return false;
			}
			virtual Result Call(Arguments... arguments_) override
			{
				return function(arguments_...);
			}
		};
		struct HolderFunctor: public Holder
		{
		public:
			virtual bool Compare(Holder* x) override
			{
				return x && x->Compare(this);
			}
			virtual bool Compare(HolderFunction* x) override
			{
				return false;
			}
			virtual bool Compare(HolderMethod* x) override
			{
				return false;
			}
		};
		template<class Class> struct HolderFunctorSpecific: public HolderFunctor
		{
			using Pointer = Class*;
			Pointer functor = nullptr;
			HolderFunctorSpecific(Pointer functor_): functor(functor_) {}
		public:
			virtual bool Compare(HolderFunctor* x) override
			{
				if(x)
				{
					auto casted = dynamic_cast<HolderFunctorSpecific*>(x);

					return casted && casted->functor == functor;
				}

				return false;
			}
			virtual Result Call(Arguments... arguments_) override
			{
				return (*functor)(arguments_...);
			}
		};
		struct HolderMethod: public Holder
		{
		public:
			virtual bool Compare(Holder* x) override
			{
				return x && x->Compare(this);
			}
			virtual bool Compare(HolderFunction* x) override
			{
				return false;
			}
			virtual bool Compare(HolderFunctor* x) override
			{
				return false;
			}
		};
		template<class Class> struct HolderMethodSpecific: public HolderMethod
		{
			using Instance = Class*;
			using Pointer = Result(Class::*)(Arguments...);
			Instance instance = nullptr;
			Pointer method = nullptr;
			HolderMethodSpecific(Instance instance_, Pointer method_): instance(instance_), method(method_) {}
		public:
			virtual bool Compare(HolderMethod* x) override
			{
				if(x)
				{
					auto casted = dynamic_cast<HolderMethodSpecific*>(x);

					return casted && casted->instance == instance && casted->method == method;
				}

				return false;
			}
			virtual Result Call(Arguments... arguments_) override
			{
				return (*instance.*method)(arguments_...);
			}
		};
	protected:
		Holder* holder = nullptr;
	public:
		Function() = default;
		explicit Function(std::nullptr_t)
		{
		}
		explicit Function(typename HolderFunction::Pointer function_):
			holder(new HolderFunction(function_))
		{
		}
		template<class Class> explicit Function(typename Class& functor_):
			holder(new HolderFunctorSpecific<Class>(&functor_))
		{
		}
		template<class Class> explicit Function(typename Class& instance_, Result(Class::*method_)(Arguments...)) :
			holder(new HolderMethodSpecific<Class>(&instance_, method_))
		{
		}
		Function(const Function&) = default;
		Function(Function&& source_):
			holder(source_.holder)
		{
			source_.holder = nullptr;
		}
	public:
		Function& operator = (const Function&) = default;
		Function& operator = (Function&& source_)
		{
			holder = source_.holder;
			source_.holder = nullptr;

			return *this;
		}
	public:
		Result operator () (Arguments ... arguments_) const
		{
			return holder ?
				holder->Call(arguments_...) :
				throw Exception("Calling of null pointer");
		}
	public:
		bool operator == (const Function& source_) const
		{
			return (holder || source_.holder) ?
				(holder ? holder->Compare(source_.holder) : source_.holder->Compare(holder)) :
				false;
		}
	};
	template<class Result> class Function<Result, void>
	{
	protected:
		struct Holder;
		struct HolderFunction;
		struct HolderFunctor;
		struct HolderMethod;

		struct Holder
		{
			virtual bool Compare(Holder*) = 0;
			virtual bool Compare(HolderFunction*) = 0;
			virtual bool Compare(HolderFunctor*) = 0;
			virtual bool Compare(HolderMethod*) = 0;
			virtual Result Call() = 0;
		};
		struct HolderFunction: public Holder
		{
			using Pointer = Result(*)();
			Pointer function = nullptr;
			HolderFunction(Pointer function_): function(function_) {}
		public:
			virtual bool Compare(Holder* x) override
			{
				return x && x->Compare(this);
			}
			virtual bool Compare(HolderFunction* x) override
			{
				return x && (function == x->function);
			}
			virtual bool Compare(HolderFunctor* x) override
			{
				return false;
			}
			virtual bool Compare(HolderMethod* x) override
			{
				return false;
			}
			virtual Result Call() override
			{
				return function();
			}
		};
		struct HolderFunctor: public Holder
		{
		public:
			virtual bool Compare(Holder* x) override
			{
				return x && x->Compare(this);
			}
			virtual bool Compare(HolderFunction* x) override
			{
				return false;
			}
			virtual bool Compare(HolderMethod* x) override
			{
				return false;
			}
		};
		template<class Class> struct HolderFunctorSpecific: public HolderFunctor
		{
			using Pointer = Class*;
			Pointer functor = nullptr;
			HolderFunctorSpecific(Pointer functor_): functor(functor_) {}
		public:
			virtual bool Compare(HolderFunctor* x) override
			{
				if(x)
				{
					auto casted = dynamic_cast<HolderFunctorSpecific*>(x);

					return casted && casted->functor == functor;
				}

				return false;
			}
			virtual Result Call() override
			{
				return (*functor)();
			}
		};
		struct HolderMethod: public Holder
		{
		public:
			virtual bool Compare(Holder* x) override
			{
				return x && x->Compare(this);
			}
			virtual bool Compare(HolderFunction* x) override
			{
				return false;
			}
			virtual bool Compare(HolderFunctor* x) override
			{
				return false;
			}
		};
		template<class Class> struct HolderMethodSpecific: public HolderMethod
		{
			using Instance = Class*;
			using Pointer = Result(Class::*)();
			Instance instance = nullptr;
			Pointer method = nullptr;
			HolderMethodSpecific(Instance instance_, Pointer method_): instance(instance_), method(method_) {}
		public:
			virtual bool Compare(HolderMethod* x) override
			{
				if(x)
				{
					auto casted = dynamic_cast<HolderMethodSpecific*>(x);

					return casted && casted->instance == instance && casted->method == method;
				}

				return false;
			}
			virtual Result Call() override
			{
				return (*instance.*method)();
			}
		};
	protected:
		Holder* holder = nullptr;
	public:
		Function() = default;
		explicit Function(std::nullptr_t)
		{
		}
		explicit Function(typename HolderFunction::Pointer function_):
			holder(new HolderFunction(function_))
		{
		}
		template<class Class> explicit Function(typename Class& functor_):
			holder(new HolderFunctorSpecific<Class>(&functor_))
		{
		}
		template<class Class> Function(typename Class& instance_, Result(Class::*method_)()) :
			holder(new HolderMethodSpecific<Class>(&instance_, method_))
		{
		}
		Function(const Function&) = default;
		Function(Function&& source_):
			holder(source_.holder)
		{
			source_.holder = nullptr;
		}
	public:
		Function& operator = (const Function&) = default;
		Function& operator = (Function&& source_)
		{
			holder = source_.holder;
			source_.holder = nullptr;

			return *this;
		}
	public:
		Result operator () () const
		{
			return holder ?
				holder->Call() :
				throw Exception("Calling of null pointer");
		}
	public:
		bool operator == (const Function& source_) const
		{
			return (holder || source_.holder) ?
				(holder ? holder->Compare(source_.holder) : source_.holder->Compare(holder)) :
				true; // null == null
		}
	};
}


#pragma region
#pragma endregion
