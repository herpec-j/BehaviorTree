#pragma once

#include <functional>
#include <cassert>

#include "BehaviorTree/Details/ConditionNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class FunctionCondition final : public ConditionNode<Entity, Args...>
			{
			private:
				using EntityType = typename ConditionNode<Entity, Args...>::EntityType;
				using EntityPtr = typename ConditionNode<Entity, Args...>::EntityPtr;
				using Parent = typename ConditionNode<Entity, Args...>::Parent;
				using ParentPtr = typename ConditionNode<Entity, Args...>::ParentPtr;
				using Function = std::function < Status(EntityPtr, Args...) >;

			public:
				FunctionCondition(void) = delete;

				FunctionCondition(const Function &function)
					: ConditionNode<Entity, Args...>(), function(function)
				{
					assert(function && "Invalid function");
				}

				FunctionCondition(const FunctionCondition &other)
					: ConditionNode<Entity, Args...>(other), function(other.function)
				{
					return;
				}

				FunctionCondition(FunctionCondition &&other)
					: ConditionNode<Entity, Args...>(std::move(other)), function(std::move(other.function))
				{
					return;
				}

				FunctionCondition &operator=(const FunctionCondition &other)
				{
					if (this != &other)
					{
						ConditionNode<Entity, Args...>::operator=(other);
						function = other.function;
					}
					return *this;
				}

				FunctionCondition &operator=(FunctionCondition &&other)
				{
					if (this != &other)
					{
						ConditionNode<Entity, Args...>::operator=(std::move(other));
						function = std::move(other.function);
					}
					return *this;
				}

				virtual ~FunctionCondition(void) = default;

			protected:
				virtual Status decide(EntityPtr entity, Args... args) override final
				{
					return function(entity, args...);
				}

			private:
				Function function;
			};
		}
	}
}
