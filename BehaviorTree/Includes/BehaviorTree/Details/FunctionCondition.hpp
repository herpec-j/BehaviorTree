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
				class FunctionCondition final : public ConditionNode < Entity, Args... >
				{
				private:
					using EntityType = typename ConditionNode<Entity, Args...>::EntityType;

					using EntityPtr = typename ConditionNode<Entity, Args...>::EntityPtr;

					using Parent = typename ConditionNode<Entity, Args...>::Parent;

					using ParentPtr = typename ConditionNode<Entity, Args...>::ParentPtr;

					using Function = std::function < Status(EntityPtr, Args...) > ;

					// Attributes
					Function function;

					// Inherited Methods
					void initialize(EntityPtr) override final
					{
						return;
					}

					Status decide(EntityPtr entity, Args... args) override final
					{
						return function(entity, args...);
					}

				public:
					// Constructors
					FunctionCondition(void) = delete;

					FunctionCondition(Function const &function)
						: ConditionNode<Entity, Args...>(), function(function)
					{
						assert(function && "Invalid function");
					}

					FunctionCondition(FunctionCondition const &) = default;

					FunctionCondition(FunctionCondition &&) = default;

					// Assignment Operators
					FunctionCondition &operator=(FunctionCondition const &) = default;

					FunctionCondition &operator=(FunctionCondition &&) = default;

					// Destructor
					~FunctionCondition(void) = default;
				};
			}
		}
	}
}
