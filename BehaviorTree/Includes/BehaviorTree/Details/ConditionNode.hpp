#pragma once

#include "BehaviorTree/Details/LeafNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		inline namespace Version_1
		{
			namespace Details
			{
				template <class Entity, typename... Args>
				class ConditionNode : public LeafNode < Entity, Args... >
				{
				public:
					// Destructor
					virtual ~ConditionNode(void) = default;

				protected:
					using EntityType = typename LeafNode<Entity, Args...>::EntityType;

					using EntityPtr = typename LeafNode<Entity, Args...>::EntityPtr;

					using Parent = typename LeafNode<Entity, Args...>::Parent;

					using ParentPtr = typename LeafNode<Entity, Args...>::ParentPtr;

					// Constructors
					ConditionNode(void) = default;

					ConditionNode(ConditionNode const &) = default;

					ConditionNode(ConditionNode &&) = default;

					// Assignment Operators
					ConditionNode &operator=(ConditionNode const &) = default;

					ConditionNode &operator=(ConditionNode &&) = default;

					// Virtual Methods
					virtual void initialize(EntityPtr) override = 0;

					virtual Status decide(EntityPtr entity, Args... args) = 0;

					// Inherited Methods
					Status execute(EntityPtr entity, Args... args) override final
					{
						return decide(entity, args...);
					}
				};
			}
		}
	}
}