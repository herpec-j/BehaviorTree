#pragma once

#include <cstddef>

#include "Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class RepeatDecorator final : public DecoratorNode<Entity, Args...>
			{
			private:
				using EntityType = typename DecoratorNode<Entity, Args...>::EntityType;
				using EntityPtr = typename DecoratorNode<Entity, Args...>::EntityPtr;
				using Parent = typename DecoratorNode<Entity, Args...>::Parent;
				using ParentPtr = typename DecoratorNode<Entity, Args...>::ParentPtr;
				using Child = typename DecoratorNode<Entity, Args...>::Child;
				using ChildPtr = typename DecoratorNode<Entity, Args...>::ChildPtr;
				using ChildrenList = typename DecoratorNode<Entity, Args...>::ChildrenList;

				std::size_t target;
				std::size_t count = 0;

			public:
				RepeatDecorator(std::size_t repeats)
					: DecoratorNode<Entity, Args...>(), target(repeats)
				{
					return;
				}

				RepeatDecorator(const RepeatDecorator &other)
					: DecoratorNode<Entity, Args...>(other), target(other.target), count(other.count)
				{
					return;
				}

				RepeatDecorator(RepeatDecorator &&other)
					: DecoratorNode<Entity, Args...>(std::move(other)), target(other.target), count(other.count)
				{
					other.target = 0;
					other.count = 0;
				}

				RepeatDecorator &operator=(const RepeatDecorator &other)
				{
					if (this != &other)
					{
						DecoratorNode<Entity, Args...>::operator=(other);
						target = other.target;
						count = other.count;
					}
					return *this;
				}

				RepeatDecorator &operator=(RepeatDecorator &&other)
				{
					if (this != &other)
					{
						DecoratorNode<Entity, Args...>::operator=(std::move(other));
						target = other.target;
						other.target = 0;
						count = other.count;
						other.count = 0;
					}
					return *this;
				}

				virtual ~RepeatDecorator(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					count = 0;
					if (!this->children.empty())
					{
						this->children.front()->initialize(entity);
					}
				}

				virtual Status filter(EntityPtr entity, Args... args) override final
				{
					if (this->children.empty())
					{
						return Status::Success;
					}
					else
					{
						const Status status = this->children.front()->execute(entity, args...);
						if (status == Status::Success)
						{
							++count;
							if (count == target)
							{
								initialize(entity);
								return Status::Success;
							}
							else
							{
								return Status::Running;
							}
						}
						else
						{
							return status;
						}
					}
				}
			};
		}
	}
}
