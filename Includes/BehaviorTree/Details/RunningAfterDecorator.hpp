#pragma once

#include "Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class RunningAfterDecorator final : public DecoratorNode < Entity, Args... >
			{
			private:
				using EntityType = typename DecoratorNode<Entity, Args...>::EntityType;
				using EntityPtr = typename DecoratorNode<Entity, Args...>::EntityPtr;
				using Parent = typename DecoratorNode<Entity, Args...>::Parent;
				using ParentPtr = typename DecoratorNode<Entity, Args...>::ParentPtr;
				using Child = typename DecoratorNode<Entity, Args...>::Child;
				using ChildPtr = typename DecoratorNode<Entity, Args...>::ChildPtr;
				using ChildrenList = typename DecoratorNode<Entity, Args...>::ChildrenList;

				std::size_t total;
				std::size_t count;

			public:
				RunningAfterDecorator(void) = delete;

				RunningAfterDecorator(std::size_t total)
					: DecoratorNode(), total(total), count(total)
				{
					return;
				}

				RunningAfterDecorator(const RunningAfterDecorator &other)
					: DecoratorNode(other), total(other.total), count(other.count)
				{
					return;
				}

				RunningAfterDecorator(RunningAfterDecorator &&other)
					: DecoratorNode(std::move(other)), total(other.total), count(other.count)
				{
					other.total = 0;
					other.count = 0;
				}

				RunningAfterDecorator &operator=(const RunningAfterDecorator &other)
				{
					if (this != &other)
					{
						DecoratorNode::operator=(other);
						total = other.total;
						count = other.count;
					}
					return *this;
				}

				RunningAfterDecorator &operator=(RunningAfterDecorator &&other)
				{
					if (this != &other)
					{
						DecoratorNode::operator=(std::move(other));
						total = other.total;
						other.total = 0;
						count = other.count;
						other.count = 0;
					}
					return *this;
				}

				virtual ~RunningAfterDecorator(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					count = total;
					if (!children.empty())
					{
						children.front()->initialize(entity);
					}
				}

				virtual Status filter(EntityPtr entity, Args... args) override final
				{
					if (count == 0)
					{
						if (!children.empty())
						{
							const Status status = children.front()->execute(entity, args...);
							if (status == Status::Success || status == Status::Failure)
							{
								children.front()->initialize(entity);
							}
						}
						return Status::Running;
					}
					else
					{
						--count;
						if (!children.empty())
						{
							const Status status = children.front()->execute(entity, args...);
							if (status == Status::Success || status == Status::Failure)
							{
								children.front()->initialize(entity);
							}
							return status;
						}
						return Status::Running;
					}
				}
			};
		}
	}
}