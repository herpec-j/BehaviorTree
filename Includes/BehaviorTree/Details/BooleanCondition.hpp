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
			class BooleanCondition final : public ConditionNode<Entity, Args...>
			{
			public:
				using Function = std::function < bool(EntityPtr, Args...) > ;

				BooleanCondition(void) = delete;

				BooleanCondition(const Function &function, bool testValue = true)
					: ConditionNode(), function(function), testValue(testValue)
				{
					assert(function && "Invalid function");
				}

				BooleanCondition(const BooleanCondition &other)
					: ConditionNode(other), function(other.function), testValue(other.testValue)
				{
					return;
				}

				BooleanCondition(BooleanCondition &&other)
					: ConditionNode(std::move(other)), function(std::move(other.function)), testValue(other.testValue)
				{
					return;
				}

				BooleanCondition &operator=(const BooleanCondition &other)
				{
					if (this != &other)
					{
						ConditionNode::operator=(other);
						function = other.function;
						testValue = other.testValue;
					}
					return *this;
				}

				BooleanCondition &operator=(BooleanCondition &&other)
				{
					if (this != &other)
					{
						ConditionNode::operator=(std::move(other));
						function = std::move(other.function);
						testValue = other.testValue;
					}
					return *this;
				}

				virtual ~BooleanCondition(void) = default;

			protected:
				virtual Status decide(EntityPtr entity, Args... args) override final
				{
					return function(entity, args...) == testValue ? Status::Success : Status::Failure;
				}

			private:
				Function function;
				bool testValue;
			};
		}
	}
}