#pragma once

#include "BehaviorTree/Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		inline namespace Version_1
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

					// Inherited Methods
					void initialize(EntityPtr entity) override final
					{
						if (!this->children.empty())
						{
							this->children.front()->initialize(entity);
						}
					}

					Status filter(EntityPtr entity, Args... args) override final
					{
						if (!this->children.empty())
						{
							Status const status = this->children.front()->execute(entity, args...);
							if (status == Status::Success || status == Status::Failure)
							{
								this->children.front()->initialize(entity);
							}
						}
						return Status::Running;
					}

				public:
					// Constructors
					AlwaysRunningDecorator(void) = default;

					AlwaysRunningDecorator(AlwaysRunningDecorator const &) = default;

					AlwaysRunningDecorator(AlwaysRunningDecorator &&) = default;

					// Assignment Operators
					AlwaysRunningDecorator &operator=(AlwaysRunningDecorator const &) = default;

					AlwaysRunningDecorator &operator=(AlwaysRunningDecorator &&) = default;

					// Destructor
					~AlwaysRunningDecorator(void) = default;
				};
			}
		}
	}
}
