#pragma once

#include <functional>
#include <cassert>

#include "Details/ConditionNode.hpp"
#include "Details/ConditionTest.hpp"
#include "Details/Private/IntegerConditionEnabler.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < typename IntegerType, class Entity, typename... Args >
			class IntegerCondition final : public ConditionNode<Entity, Args...>, public Private::IntegerConditionEnabler<IntegerType>
			{
			private:
				using EntityType = typename ConditionNode<Entity, Args...>::EntityType;
				using EntityPtr = typename ConditionNode<Entity, Args...>::EntityPtr;
				using Parent = typename ConditionNode<Entity, Args...>::Parent;
				using ParentPtr = typename ConditionNode<Entity, Args...>::ParentPtr;
				using Function = std::function < IntegerType(EntityPtr, Args...) >;

			public:
				IntegerCondition(void) = delete;

				IntegerCondition(const Function &function, ConditionTest condition, IntegerType target)
					: ConditionNode<Entity, Args...>(), Private::IntegerConditionEnabler<IntegerType>(), function(function), condition(condition), target(target)
				{
					assert(function && "Invalid function");
				}

				IntegerCondition(const IntegerCondition &other)
					: ConditionNode<Entity, Args...>(other), Private::IntegerConditionEnabler<IntegerType>(other), function(other.function), condition(other.condition), target(other.target)
				{
					return;
				}

				IntegerCondition(IntegerCondition &&other)
					: ConditionNode<Entity, Args...>(std::move(other)), Private::IntegerConditionEnabler<IntegerType>(std::move(other)), function(std::move(other.function)), condition(other.condition), target(other.target)
				{
					other.target = 0;
				}

				IntegerCondition &operator=(const IntegerCondition &other)
				{
					if (this != &other)
					{
						ConditionNode<Entity, Args...>::operator=(other);
						Private::IntegerConditionEnabler<IntegerType>(other);
						function = other.function;
						condition = other.condition;
						target = other.target;
					}
					return *this;
				}

				IntegerCondition &operator=(IntegerCondition &&other)
				{
					if (this != &other)
					{
						ConditionNode<Entity, Args...>::operator=(std::move(other));
						Private::IntegerConditionEnabler<IntegerType>::operator=(std::move(other));
						function = std::move(other.function);
						condition = other.condition;
						target = other.target;
						other.target = 0;
					}
					return *this;
				}

				virtual ~IntegerCondition(void) = default;

			protected:
				virtual Status decide(EntityPtr entity, Args...args) override final
				{
					const IntegerType result = function(entity, args...);
					switch (condition)
					{
					case ConditionTest::LessThan:
						return result < target ? Status::Success : Status::Failure;
					case ConditionTest::GreaterThan:
						return result > target ? Status::Success : Status::Failure;
					case ConditionTest::LessOrEqualThan:
						return result <= target ? Status::Success : Status::Failure;
					case ConditionTest::GreaterOrEqualThan:
						return result >= target ? Status::Success : Status::Failure;
					case ConditionTest::Equal:
						return result == target ? Status::Success : Status::Failure;
					case ConditionTest::NotEqual:
						return result != target ? Status::Success : Status::Failure;
					default:
						return Status::Failure;
					}
				}

			private:
				Function function;
				ConditionTest condition;
				IntegerType target;
			};
		}
	}
}
