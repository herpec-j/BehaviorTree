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
				class ActionNode : public LeafNode < Entity, Args... >
				{
				public:
					// Destructor
					virtual ~ActionNode(void) = default;

				protected:
					using EntityType = typename LeafNode<Entity, Args...>::EntityType;

					using EntityPtr = typename LeafNode<Entity, Args...>::EntityPtr;

					using Parent = typename LeafNode<Entity, Args...>::Parent;

					using ParentPtr = typename LeafNode<Entity, Args...>::ParentPtr;

					// Constructors
					ActionNode(void) = default;

					ActionNode(ActionNode const &) = default;

					ActionNode(ActionNode &&) = default;

					// Assignment Operators
					ActionNode &operator=(ActionNode const &) = default;

					ActionNode &operator=(ActionNode &&) = default;

					// Virtual Methods
					virtual void initialize(EntityPtr entity) override = 0;

					virtual Status execute(EntityPtr entity, Args... args) override = 0;
				};
			}
		}
	}
}