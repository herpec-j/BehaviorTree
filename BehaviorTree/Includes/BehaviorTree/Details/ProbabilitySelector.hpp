#pragma once

#include <unordered_map>
#include <chrono>
#include <random>

#include "BehaviorTree/Details/CompositeNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class ProbabilitySelector final : public CompositeNode<Entity, Args...>
			{
			private:
				using EntityType = typename CompositeNode<Entity, Args...>::EntityType;
				using EntityPtr = typename CompositeNode<Entity, Args...>::EntityPtr;
				using Parent = typename CompositeNode<Entity, Args...>::Parent;
				using ParentPtr = typename CompositeNode<Entity, Args...>::ParentPtr;
				using Child = typename CompositeNode<Entity, Args...>::Child;
				using ChildPtr = typename CompositeNode<Entity, Args...>::ChildPtr;
				using ChildrenList = typename CompositeNode<Entity, Args...>::ChildrenList;
				using WeightingMap = std::unordered_map < ChildPtr, float > ;

				float totalSum = 0.0f;
				ChildPtr currentNode;
				WeightingMap weightingMap;

			public:
				ProbabilitySelector(void) = default;

				ProbabilitySelector(const ProbabilitySelector &other)
					: CompositeNode<Entity, Args...>(other), totalSum(other.totalSum), currentNode(other.currentNode), weightingMap(other.weightingMap)
				{
					return;
				}

				ProbabilitySelector(ProbabilitySelector &&other)
					: CompositeNode<Entity, Args...>(std::move(other)), totalSum(other.totalSum), currentNode(std::move(other.currentNode)), weightingMap(std::move(other.weightingMap))
				{
					other.totalSum = 0.0f;
				}

				ProbabilitySelector &operator=(const ProbabilitySelector &other)
				{
					if (this != &other)
					{
						CompositeNode<Entity, Args...>::operator=(other);
						totalSum = other.totalSum;
						currentNode = other.currentNode;
						weightingMap = other.weightingMap;
					}
					return *this;
				}

				ProbabilitySelector &operator=(ProbabilitySelector &&other)
				{
					if (this != &other)
					{
						CompositeNode<Entity, Args...>::operator=(std::move(other));
						totalSum = other.totalSum;
						other.totalSum = 0.0f;
						currentNode = std::move(other.currentNode);
						weightingMap = std::move(other.weightingMap);
					}
					return *this;
				}

				virtual ~ProbabilitySelector(void) = default;

				virtual ParentPtr addChild(ChildPtr child) override final
				{
					return addChild(child, 1.0f);
				}

				ParentPtr addChild(ChildPtr child, float weighting)
				{
					assert(child && "Invalid child");
					assert(weighting >= 0.0f && "Invalid weighting");
					weightingMap[child] = weighting;
					totalSum += weighting;
					return CompositeNode<Entity, Args...>::addChild(child);
				}

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					currentNode.reset();
					for (auto &child : this->children)
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
						static std::default_random_engine generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
						std::uniform_real_distribution<float> distribution(0.0f, totalSum);
						const float chosen = distribution(generator);
						float sum = 0.0f;
						for (auto &weighting : weightingMap)
						{
							sum += weighting.second;
							if (sum >= chosen)
							{
								const Status status = weighting.first->execute(entity, args...);
								if (status == Status::Running)
								{
									currentNode = weighting.first;
								}
								return status;
							}
						}
						assert(false && "Weighting is too high");
						return Status::Failure;
					}
				}
			};
		}
	}
}
