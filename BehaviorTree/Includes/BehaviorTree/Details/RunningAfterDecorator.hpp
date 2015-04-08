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
						: DecoratorNode<Entity, Args...>(), total(total), count(total)
					{
						return;
					}

					RunningAfterDecorator(const RunningAfterDecorator &other)
						: DecoratorNode<Entity, Args...>(other), total(other.total), count(other.count)
					{
						return;
					}

					RunningAfterDecorator(RunningAfterDecorator &&other)
						: DecoratorNode<Entity, Args...>(std::move(other)), total(other.total), count(other.count)
					{
						other.total = 0;
						other.count = 0;
					}

					RunningAfterDecorator &operator=(const RunningAfterDecorator &other)
					{
						if (this != &other)
						{
							DecoratorNode<Entity, Args...>::operator=(other);
							total = other.total;
							count = other.count;
						}
						return *this;
					}

					RunningAfterDecorator &operator=(RunningAfterDecorator &&other)
					{
						if (this != &other)
						{
							DecoratorNode<Entity, Args...>::operator=(std::move(other));
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
						if (!this->children.empty())
						{
							this->children.front()->initialize(entity);
						}
					}

					virtual Status filter(EntityPtr entity, Args... args) override final
					{
						if (count == 0)
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
						else
						{
							--count;
							if (!this->children.empty())
							{
								const Status status = this->children.front()->execute(entity, args...);
								if (status == Status::Success || status == Status::Failure)
								{
									this->children.front()->initialize(entity);
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
}
