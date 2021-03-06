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
				class FailureAfterDecorator final : public DecoratorNode < Entity, Args... >
				{
				private:
					using EntityType = typename DecoratorNode<Entity, Args...>::EntityType;

					using EntityPtr = typename DecoratorNode<Entity, Args...>::EntityPtr;

					using Parent = typename DecoratorNode<Entity, Args...>::Parent;

					using ParentPtr = typename DecoratorNode<Entity, Args...>::ParentPtr;

					using Child = typename DecoratorNode<Entity, Args...>::Child;

					using ChildPtr = typename DecoratorNode<Entity, Args...>::ChildPtr;

					using ChildrenList = typename DecoratorNode<Entity, Args...>::ChildrenList;

					// Attributes
					std::size_t total;

					std::size_t count;

					// Inherited Methods
					void initialize(EntityPtr entity) override final
					{
						count = total;
						if (!this->children.empty())
						{
							this->children.front()->initialize(entity);
						}
					}

					Status filter(EntityPtr entity, Args... args) override final
					{
						if (count == 0)
						{
							if (!this->children.empty())
							{
								Status const status = this->children.front()->execute(entity, args...);
								if (status == Status::Success || status == Status::Failure)
								{
									this->children.front()->initialize(entity);
								}
							}
							return Status::Failure;
						}
						else
						{
							--count;
							if (!this->children.empty())
							{
								Status const status = this->children.front()->execute(entity, args...);
								if (status == Status::Success || status == Status::Failure)
								{
									this->children.front()->initialize(entity);
								}
								return status;
							}
							return Status::Running;
						}
					}

				public:
					// Constructors
					FailureAfterDecorator(void) = delete;

					FailureAfterDecorator(std::size_t total)
						: DecoratorNode<Entity, Args...>(), total(total), count(total)
					{
						return;
					}

					FailureAfterDecorator(FailureAfterDecorator const &) = default;

					FailureAfterDecorator(FailureAfterDecorator &&) = default;

					// Assignment Operators
					FailureAfterDecorator &operator=(FailureAfterDecorator const &) = default;

					FailureAfterDecorator &operator=(FailureAfterDecorator &&) = default;

					// Destructor
					~FailureAfterDecorator(void) = default;
				};
			}
		}
	}
}
