#pragma once

#include <functional>
#include <cassert>

#include "BehaviorTree/Details/ConditionNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		inline namespace Version_1
		{
			namespace Details
			{
				template < class Entity, typename... Args >
				class BooleanCondition final : public ConditionNode < Entity, Args... >
				{
				private:
					using EntityType = typename ConditionNode<Entity, Args...>::EntityType;
					using EntityPtr = typename ConditionNode<Entity, Args...>::EntityPtr;
					using Parent = typename ConditionNode<Entity, Args...>::Parent;
					using ParentPtr = typename ConditionNode<Entity, Args...>::ParentPtr;
					using Function = std::function < bool(EntityPtr, Args...) > ;

				public:
					BooleanCondition(void) = delete;

					BooleanCondition(const Function &function, bool testValue = true)
						: ConditionNode<Entity, Args...>(), function(function), testValue(testValue)
					{
						assert(function && "Invalid function");
					}

					BooleanCondition(const BooleanCondition &other)
						: ConditionNode<Entity, Args...>(other), function(other.function), testValue(other.testValue)
					{
						return;
					}

					BooleanCondition(BooleanCondition &&other)
						: ConditionNode<Entity, Args...>(std::move(other)), function(std::move(other.function)), testValue(other.testValue)
					{
						return;
					}

					BooleanCondition &operator=(const BooleanCondition &other)
					{
						if (this != &other)
						{
							ConditionNode<Entity, Args...>::operator=(other);
							function = other.function;
							testValue = other.testValue;
						}
						return *this;
					}

					BooleanCondition &operator=(BooleanCondition &&other)
					{
						if (this != &other)
						{
							ConditionNode<Entity, Args...>::operator=(std::move(other));
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
}
