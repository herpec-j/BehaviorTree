#pragma once

#include "Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class CountLimitDecorator final : public DecoratorNode<Entity, Args...>
			{
			private:
				using EntityType = typename DecoratorNode<Entity, Args...>::EntityType;
				using EntityPtr = typename DecoratorNode<Entity, Args...>::EntityPtr;
				using Parent = typename DecoratorNode<Entity, Args...>::Parent;
				using ParentPtr = typename DecoratorNode<Entity, Args...>::ParentPtr;
				using Child = typename DecoratorNode<Entity, Args...>::Child;
				using ChildPtr = typename DecoratorNode<Entity, Args...>::ChildPtr;
				using ChildrenList = typename DecoratorNode<Entity, Args...>::ChildrenList;

				std::size_t limit;
				std::size_t currentRepetition = 0;
				bool allowReinitialize;

			public:
				CountLimitDecorator(void) = delete;

				CountLimitDecorator(std::size_t limit, bool allowReinitialize = true)
					: DecoratorNode<Entity, Args...>(), limit(limit), allowReinitialize(allowReinitialize)
				{
					return;
				}

				CountLimitDecorator(const CountLimitDecorator &other)
					: DecoratorNode<Entity, Args...>(other), limit(other.limit), currentRepetition(other.currentRepetition), allowReinitialize(other.allowReinitialize)
				{
					return;
				}

				CountLimitDecorator(CountLimitDecorator &&other)
					: DecoratorNode<Entity, Args...>(std::move(other)), limit(other.limit), currentRepetition(other.currentRepetition), allowReinitialize(other.allowReinitialize)
				{
					other.limit = 0;
					other.currentRepetition = 0;
				}

				CountLimitDecorator &operator=(const CountLimitDecorator &other)
				{
					if (this != &other)
					{
						DecoratorNode<Entity, Args...>::operator=(other);
						limit = other.limit;
						currentRepetition = other.currentRepetition;
						allowReinitialize = other.allowReinitialize;
					}
					return *this;
				}

				CountLimitDecorator &operator=(CountLimitDecorator &&other)
				{
					if (this != &other)
					{
						DecoratorNode<Entity, Args...>::operator=(std::move(other));
						limit = other.limit;
						other.limit = 0;
						currentRepetition = other.currentRepetition;
						other.currentRepetition = 0;
						allowReinitialize = other.allowReinitialize;
					}
					return *this;
				}

				virtual ~CountLimitDecorator(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					if (allowReinitialize)
					{
						currentRepetition = 0;
					}
					if (!this->children.empty())
					{
						this->children.front()->initialize(entity);
					}
				}

				virtual Status filter(EntityPtr entity, Args... args) override final
				{
					if (currentRepetition == limit)
					{
						return Status::Failure;
					}
					else if (this->children.empty())
					{
						++currentRepetition;
						return Status::Success;
					}
					else
					{
						const Status status = this->children.front()->execute(entity, args...);
						if (status == Status::Success || status == Status::Failure)
						{
							++currentRepetition;
							if (!this->children.empty())
							{
								this->children.front()->initialize(entity);
							}
						}
						return status;
					}
				}
			};
		}
	}
}
