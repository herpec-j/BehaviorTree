#pragma once

#include <functional>
#include <limits>
#include <cmath>
#include <cassert>

#include "BehaviorTree/Details/ConditionNode.hpp"
#include "BehaviorTree/Details/ConditionTest.hpp"
#include "BehaviorTree/Details/Private/DecimalConditionEnabler.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < typename DecimalType, class Entity, typename... Args >
			class DecimalCondition final : public ConditionNode<Entity, Args...>, public Private::DecimalConditionEnabler<DecimalType>
			{
			private:
				using EntityType = typename ConditionNode<Entity, Args...>::EntityType;
				using EntityPtr = typename ConditionNode<Entity, Args...>::EntityPtr;
				using Parent = typename ConditionNode<Entity, Args...>::Parent;
				using ParentPtr = typename ConditionNode<Entity, Args...>::ParentPtr;
				using Function = std::function < DecimalType(EntityPtr, Args...) >;

			public:
				DecimalCondition(void) = delete;

				DecimalCondition(const Function &function, ConditionTest condition, DecimalType target, DecimalType epsilon = std::numeric_limits<DecimalType>::epsilon())
					: ConditionNode<Entity, Args...>(), Private::DecimalConditionEnabler<DecimalType>(), function(function), condition(condition), target(target), epsilon(epsilon)
				{
					assert(function && "Invalid function");
				}

				DecimalCondition(const DecimalCondition &other)
					: ConditionNode<Entity, Args...>(other), Private::DecimalConditionEnabler<DecimalType>(other), function(other.function), condition(other.condition), target(other.target), epsilon(other.epsilon)
				{
					return;
				}

				DecimalCondition(DecimalCondition &&other)
					: ConditionNode<Entity, Args...>(std::move(other)), Private::DecimalConditionEnabler<DecimalType>(std::move(other)), function(std::move(other.function)), condition(other.condition), target(other.target), epsilon(other.epsilon)
				{
					other.target = 0;
					other.epsilon = 0.0f;
				}

				DecimalCondition &operator=(const DecimalCondition &other)
				{
					if (this != &other)
					{
						ConditionNode<Entity, Args...>::operator=(other);
						Private::DecimalConditionEnabler<DecimalType>::operator=(other);
						function = other.function;
						condition = other.condition;
						target = other.target;
						epsilon = other.epsilon;
					}
					return *this;
				}

				DecimalCondition &operator=(DecimalCondition &&other)
				{
					if (this != &other)
					{
						ConditionNode<Entity, Args...>::operator=(std::move(other));
						Private::DecimalConditionEnabler<DecimalType>::operator=(std::move(other));
						function = std::move(other.function);
						condition = other.condition;
						target = other.target;
						other.target = 0;
						epsilon = other.epsilon;
						other.epsilon = 0.0f;
					}
					return *this;
				}

				virtual ~DecimalCondition(void) = default;

			protected:
				virtual Status decide(EntityPtr entity, Args... args) override final
				{
					const DecimalType result = function(entity, args...);
					switch (condition)
					{
					case ConditionTest::LessThan:
						return result < target && !DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
					case ConditionTest::GreaterThan:
						return result > target && !DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
					case ConditionTest::LessOrEqualThan:
						return result < target || DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
					case ConditionTest::GreaterOrEqualThan:
						return result > target || DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
					case ConditionTest::Equal:
						return DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
					case ConditionTest::NotEqual:
						return !DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
					default:
						return Status::Failure;
					}
				}

			private:
				Function function;
				ConditionTest condition;
				DecimalType target;
				DecimalType epsilon;

				static bool Equals(DecimalType lhs, DecimalType rhs, DecimalType epsilon)
				{
					return std::abs(lhs - rhs) <= epsilon;
				}
			};
		}
	}
}
