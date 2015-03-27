#pragma once

#include "Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class AlwaysSuccessDecorator final : public DecoratorNode < Entity, Args... >
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
				AlwaysSuccessDecorator(void) = default;
				AlwaysSuccessDecorator(const AlwaysSuccessDecorator &other) = default;
				AlwaysSuccessDecorator &operator=(const AlwaysSuccessDecorator &other) = default;
				virtual ~AlwaysSuccessDecorator(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					if (!children.empty())
					{
						children.front()->initialize(entity);
					}
				}

				virtual Status filter(EntityPtr entity, Args... args) override final
				{
					if (!children.empty())
					{
						const Status status = children.front()->execute(entity, args...);
						if (status == Status::Success || status == Status::Failure)
						{
							children.front()->initialize(entity);
						}
					}
					return Status::Success;
				}
			};
		}
	}
}