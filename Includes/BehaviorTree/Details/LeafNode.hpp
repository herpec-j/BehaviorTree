#pragma once

#include <memory>

#include "Details/RootNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class CompositeNode;

			template <class Entity, typename... Args>
			class LeafNode : public RootNode<Entity, Args...>
			{
				friend class CompositeNode < Entity, Args... > ;

			public:
				virtual ~LeafNode(void) = default;

				virtual void initialize(EntityPtr entity) override = 0;
				virtual Status execute(EntityPtr entity, Args... args) override = 0;

			protected:
				using Parent = CompositeNode<Entity, Args...>;
				using ParentPtr = std::shared_ptr<Parent>;

				LeafNode(void) = default;
				LeafNode(const LeafNode &other) = default;
				LeafNode &operator=(const LeafNode &other) = default;

				virtual void onParentAdded(ParentPtr)
				{
					return;
				}

				virtual void onParentRemoved(ParentPtr)
				{
					return;
				}
			};
		}
	}
}