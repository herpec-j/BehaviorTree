#pragma once

#include "Details/CompositeNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class PrioritySelector final : public CompositeNode<Entity, Args...>
			{
			private:
				using EntityType = typename CompositeNode<Entity, Args...>::EntityType;
				using EntityPtr = typename CompositeNode<Entity, Args...>::EntityPtr;
				using Parent = typename CompositeNode<Entity, Args...>::Parent;
				using ParentPtr = typename CompositeNode<Entity, Args...>::ParentPtr;
				using Child = typename CompositeNode<Entity, Args...>::Child;
				using ChildPtr = typename CompositeNode<Entity, Args...>::ChildPtr;
				using ChildrenList = typename CompositeNode<Entity, Args...>::ChildrenList;

				static const std::size_t InitialPosition = -1;

				std::size_t currentPosition = InitialPosition;

			public:
				PrioritySelector(void) = default;

				PrioritySelector(const PrioritySelector &other)
					: CompositeNode<Entity, Args...>(other), currentPosition(other.currentPosition)
				{
					return;
				}

				PrioritySelector(PrioritySelector &&other)
					: CompositeNode<Entity, Args...>(std::move(other)), currentPosition(other.currentPosition)
				{
					other.currentPosition = InitialPosition;
				}

				PrioritySelector &operator=(const PrioritySelector &other)
				{
					if (this != &other)
					{
						CompositeNode<Entity, Args...>::operator=(other);
						currentPosition = other.currentPosition;
					}
					return *this;
				}

				PrioritySelector &operator=(PrioritySelector &&other)
				{
					if (this != &other)
					{
						CompositeNode<Entity, Args...>::operator=(std::move(other));
						currentPosition = other.currentPosition;
						other.currentPosition = InitialPosition;
					}
					return *this;
				}

				virtual ~PrioritySelector(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					currentPosition = InitialPosition;
					for (auto &child : children)
					{
						child->initialize(entity);
					}
				}

				virtual Status execute(EntityPtr entity, Args... args) override final
				{
					if (currentPosition != InitialPosition)
					{
						const Status status = children[currentPosition]->execute(entity, args...);
						switch (status)
						{
						case Status::Running:
							return Status::Running;
						case Status::Success:
							currentPosition = InitialPosition;
							return Status::Success;
						case Status::Failure:
							++currentPosition;
							if (currentPosition == children.size())
							{
								currentPosition = InitialPosition;
								return Status::Failure;
							}
							else
							{
								break;
							}
						default:
							break;
						}
					}
					else
					{
						initialize(entity);
						currentPosition = 0;
					}
					if (children.empty())
					{
						return Status::Success;
					}
					ChildPtr currentlyRunningNode = children[currentPosition];
					Status status;
					while ((status = currentlyRunningNode->execute(entity, args...)) == Status::Failure)
					{
						++currentPosition;
						if (currentPosition == children.size())
						{
							currentPosition = InitialPosition;
							return Status::Failure;
						}
						currentlyRunningNode = children[currentPosition];
					}
					if (status == Status::Success)
					{
						currentPosition = InitialPosition;
					}
					return status;
				}
			};
		}
	}
}