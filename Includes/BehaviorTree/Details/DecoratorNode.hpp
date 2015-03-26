#pragma once

#include "Details/CompositeNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class DecoratorNode : public CompositeNode < Entity, Args... >
			{
			public:
				virtual ~DecoratorNode(void) = default;

				virtual ParentPtr addChild(ChildPtr child) override final
				{
					assert(children.empty() && "Tree should be empty");
					return CompositeNode::addChild(child);
				}

			protected:
				DecoratorNode(void) = default;
				DecoratorNode(const DecoratorNode &other) = default;
				DecoratorNode &operator=(const DecoratorNode &other) = default;

				virtual void initialize(EntityPtr) override = 0;

				virtual Status execute(EntityPtr entity, Args... args) override final
				{
					return filter(entity, args...);
				}

				virtual Status filter(EntityPtr entity, Args... args) = 0;
			};
		}
	}
}
