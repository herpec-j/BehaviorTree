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
					virtual ~ConditionNode(void) = default;

				protected:
					using EntityType = typename LeafNode<Entity, Args...>::EntityType;
					using EntityPtr = typename LeafNode<Entity, Args...>::EntityPtr;
					using Parent = typename LeafNode<Entity, Args...>::Parent;
					using ParentPtr = typename LeafNode<Entity, Args...>::ParentPtr;

					ConditionNode(void) = default;
					ConditionNode(const ConditionNode &other) = default;
					ConditionNode &operator=(const ConditionNode &other) = default;

					virtual void initialize(EntityPtr) override final
					{
						return;
					}

					virtual Status execute(EntityPtr entity, Args... args) override final
					{
						return decide(entity, args...);
					}

					virtual Status decide(EntityPtr entity, Args... args) = 0;
				};
			}
		}
	}
}