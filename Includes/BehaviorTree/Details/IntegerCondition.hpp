#pragma once

#include <functional>
#include <type_traits>
#include <cassert>

#include "Details/ConditionNode.hpp"
#include "Details/ConditionTest.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < typename IntegerType, class Entity, typename... Args >
			class IntegerCondition final : public ConditionNode<Entity, Args...>
			{
				static_assert(std::is_integral<IntegerType>::value && !std::is_same<IntegerType, bool>::value, "DecimalType should be a integer type (std::int8_t, std::int16_t, std::int32_t, std::int64_t, std::uint8_t, std::uint16_t, std::uint32_t, or std::uint64_t)");

			public:
				using Function = std::function < IntegerType(EntityPtr, Args...) > ;

				IntegerCondition(void) = delete;

				IntegerCondition(const Function &function, ConditionTest condition, IntegerType target)
					: ConditionNode(), function(function), condition(condition), target(target)
				{
					assert(function && "Invalid function");

				}

				IntegerCondition(const IntegerCondition &other)
					: ConditionNode(other), function(other.function), condition(other.condition), target(other.target)
				{
					return;
				}

				IntegerCondition(IntegerCondition &&other)
					: ConditionNode(std::move(other)), function(std::move(other.function)), condition(other.condition), target(other.target)
				{
					other.target = 0;
				}

				IntegerCondition &operator=(const IntegerCondition &other)
				{
					if (this != &other)
					{
						ConditionNode::operator=(other);
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
						ConditionNode::operator=(std::move(other));
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