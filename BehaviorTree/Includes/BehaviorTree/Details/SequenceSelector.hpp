#pragma once

#include "BehaviorTree/Details/CompositeNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		inline namespace Version_1
		{
			namespace Details
			{
				template < class Entity, typename... Args >
				class SequenceSelector final : public CompositeNode < Entity, Args... >
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
					SequenceSelector(void) = default;

					SequenceSelector(const SequenceSelector &other)
						: CompositeNode<Entity, Args...>(other), currentPosition(other.currentPosition)
					{
						return;
					}

					SequenceSelector(SequenceSelector &&other)
						: CompositeNode<Entity, Args...>(std::move(other)), currentPosition(other.currentPosition)
					{
						other.currentPosition = InitialPosition;
					}

					SequenceSelector &operator=(const SequenceSelector &other)
					{
						if (this != &other)
						{
							CompositeNode<Entity, Args...>::operator=(other);
							currentPosition = other.currentPosition;
						}
						return *this;
					}

					SequenceSelector &operator=(SequenceSelector &&other)
					{
						if (this != &other)
						{
							CompositeNode<Entity, Args...>::operator=(std::move(other));
							currentPosition = other.currentPosition;
							other.currentPosition = 0;
						}
						return *this;
					}

					virtual ~SequenceSelector(void) = default;

				protected:
					virtual void initialize(EntityPtr entity) override final
					{
						currentPosition = InitialPosition;
						for (auto &child : this->children)
						{
							child->initialize(entity);
						}
					}

					virtual Status execute(EntityPtr entity, Args... args) override final
					{
						if (currentPosition == InitialPosition)
						{
							initialize(entity);
							currentPosition = 0;
						}
						if (this->children.empty())
						{
							return Status::Success;
						}
						ChildPtr currentTask = this->children[currentPosition];
						Status result = currentTask->execute(entity, args...);
						const std::size_t lastPosition = this->children.size() - 1;
						while (result == Status::Success)
						{
							if (currentPosition == lastPosition)
							{
								currentPosition = InitialPosition;
								return Status::Success;
							}
							else
							{
								++currentPosition;
								currentTask = this->children[currentPosition];
								result = currentTask->execute(entity, args...);
							}
						}
						if (result == Status::Failure)
						{
							currentPosition = InitialPosition;
						}
						return result;
					}
				};
			}
		}
	}
}
