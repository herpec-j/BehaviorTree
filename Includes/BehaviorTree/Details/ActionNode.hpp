#pragma once

#include "Details/LeafNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class ActionNode : public LeafNode < Entity, Args... >
			{
			public:
				virtual ~ActionNode(void) = default;

			protected:
				using EntityType = typename LeafNode<Entity, Args...>::EntityType;
				using EntityPtr = typename LeafNode<Entity, Args...>::EntityPtr;
				using Parent = typename LeafNode<Entity, Args...>::Parent;
				using ParentPtr = typename LeafNode<Entity, Args...>::ParentPtr;

				ActionNode(void) = default;
				ActionNode(const ActionNode &other) = default;
				ActionNode &operator=(const ActionNode &other) = default;

				virtual void initialize(EntityPtr entity) override = 0;
				virtual Status execute(EntityPtr entity, Args... args) override = 0;
			};
		}
	}
}