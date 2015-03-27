#pragma once

#include <chrono>
#include <random>

#include "Details/CompositeNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class RandomSelector final : public CompositeNode<Entity, Args...>
			{
			private:
				using EntityType = typename CompositeNode<Entity, Args...>::EntityType;
				using EntityPtr = typename CompositeNode<Entity, Args...>::EntityPtr;
				using Parent = typename CompositeNode<Entity, Args...>::Parent;
				using ParentPtr = typename CompositeNode<Entity, Args...>::ParentPtr;
				using Child = typename CompositeNode<Entity, Args...>::Child;
				using ChildPtr = typename CompositeNode<Entity, Args...>::ChildPtr;
				using ChildrenList = typename CompositeNode<Entity, Args...>::ChildrenList;

				ChildPtr currentNode = nullptr;

			public:
				RandomSelector(void) = default;

				RandomSelector(const RandomSelector &other)
					: CompositeNode(other), currentNode(other.currentNode)
				{
					return;
				}

				RandomSelector(RandomSelector &&other)
					: CompositeNode(std::move(other)), currentNode(std::move(other.currentNode))
				{
					return;
				}

				RandomSelector &operator=(const RandomSelector &other)
				{
					if (this != &other)
					{
						CompositeNode::operator=(other);
						currentNode = other.currentNode;
					}
					return *this;
				}

				RandomSelector &operator=(RandomSelector &&other)
				{
					if (this != &other)
					{
						CompositeNode::operator=(std::move(other));
						currentNode = std::move(other.currentNode);
					}
					return *this;
				}

				virtual ~RandomSelector(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					currentNode.reset();
					for (auto &child : children)
					{
						child->initialize(entity);
					}
				}

				virtual Status execute(EntityPtr entity, Args... args) override final
				{
					if (currentNode)
					{
						const Status status = currentNode->execute(entity, args...);
						if (status != Status::Running)
						{
							currentNode.reset();
						}
						return status;
					}
					else
					{
						const std::size_t childrenNumber = children.size();
						if (childrenNumber != 0)
						{
							static std::default_random_engine generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
							std::uniform_int_distribution<std::size_t> distribution(0, childrenNumber - 1);
							const std::size_t chosen = distribution(generator);
							currentNode = children[chosen];
							const Status status = currentNode->execute(entity, args...);
							if (status != Status::Running)
							{
								currentNode.reset();
							}
							return status;
						}
						else
						{
							assert(false && "No children assigned");
							return Status::Failure;
						}
					}
				}
			};
		}
	}
}