#pragma once

#include "BehaviorTree/Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class AlwaysRunningDecorator final : public DecoratorNode < Entity, Args... >
			{
			private:
				using EntityType = typename DecoratorNode<Entity, Args...>::EntityType;
				using EntityPtr = typename DecoratorNode<Entity, Args...>::EntityPtr;
				using Parent = typename DecoratorNode<Entity, Args...>::Parent;
				using ParentPtr = typename DecoratorNode<Entity, Args...>::ParentPtr;
				using Child = typename DecoratorNode<Entity, Args...>::Child;
				using ChildPtr = typename DecoratorNode<Entity, Args...>::ChildPtr;
				using ChildrenList = typename DecoratorNode<Entity, Args...>::ChildrenList;

			public:
				AlwaysRunningDecorator(void) = default;
				AlwaysRunningDecorator(const AlwaysRunningDecorator &other) = default;
				AlwaysRunningDecorator &operator=(const AlwaysRunningDecorator &other) = default;
				virtual ~AlwaysRunningDecorator(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					if (!this->children.empty())
					{
						this->children.front()->initialize(entity);
					}
				}

				virtual Status filter(EntityPtr entity, Args... args) override final
				{
					if (!this->children.empty())
					{
						const Status status = this->children.front()->execute(entity, args...);
						if (status == Status::Success || status == Status::Failure)
						{
							this->children.front()->initialize(entity);
						}
					}
					return Status::Running;
				}
			};
		}
	}
}