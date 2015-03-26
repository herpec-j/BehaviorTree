#pragma once

#include <functional>
#include <cassert>

#include "Details/ConditionNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class FunctionCondition final : public ConditionNode<Entity, Args...>
			{
			public:
				using Function = std::function < Status(EntityPtr, Args...) > ;

				FunctionCondition(void) = delete;

				FunctionCondition(const Function &function)
					: ConditionNode(), function(function)
				{
					assert(function && "Invalid function");
				}

				FunctionCondition(const FunctionCondition &other)
					: ConditionNode(other), function(other.function)
				{
					return;
				}

				FunctionCondition(FunctionCondition &&other)
					: ConditionNode(std::move(other)), function(std::move(other.function))
				{
					return;
				}

				FunctionCondition &operator=(const FunctionCondition &other)
				{
					if (this != &other)
					{
						ConditionNode::operator=(other);
						function = other.function;
					}
					return *this;
				}

				FunctionCondition &operator=(FunctionCondition &&other)
				{
					if (this != &other)
					{
						ConditionNode::operator=(std::move(other));
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