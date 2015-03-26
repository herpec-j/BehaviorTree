#pragma once

#include "Details/LeafNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class ConditionNode : public LeafNode < Entity, Args... >
			{
			public:
				virtual ~ConditionNode(void) = default;

			protected:
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