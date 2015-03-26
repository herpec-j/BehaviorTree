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
				ActionNode(void) = default;
				ActionNode(const ActionNode &other) = default;
				ActionNode &operator=(const ActionNode &other) = default;

				virtual void initialize(EntityPtr entity) override = 0;
				virtual Status execute(EntityPtr entity, Args... args) override = 0;
			};
		}
	}
}