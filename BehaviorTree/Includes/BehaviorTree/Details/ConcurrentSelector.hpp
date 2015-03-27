#pragma once

#include <unordered_map>

#include "BehaviorTree/Details/CompositeNode.hpp"
#include "BehaviorTree/Details/FailurePolicy.hpp"
#include "BehaviorTree/Details/SuccessPolicy.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class ConcurrentSelector final : public CompositeNode<Entity, Args...>
			{
			private:
				using EntityType = typename CompositeNode<Entity, Args...>::EntityType;
				using EntityPtr = typename CompositeNode<Entity, Args...>::EntityPtr;
				using Parent = typename CompositeNode<Entity, Args...>::Parent;
				using ParentPtr = typename CompositeNode<Entity, Args...>::ParentPtr;
				using Child = typename CompositeNode<Entity, Args...>::Child;
				using ChildPtr = typename CompositeNode<Entity, Args...>::ChildPtr;
				using ChildrenList = typename CompositeNode<Entity, Args...>::ChildrenList;
				using ChildrenStatusMap = std::unordered_map < ChildPtr, Status > ;

				FailurePolicy failPolicy;
				SuccessPolicy succeedPolicy;
				ChildrenStatusMap childrenStatus;

			public:
				ConcurrentSelector(FailurePolicy failPolicy = FailurePolicy::FailOnAll, SuccessPolicy succeedPolicy = SuccessPolicy::SuccessOnAll)
					: CompositeNode<Entity, Args...>(), failPolicy(failPolicy), succeedPolicy(succeedPolicy)
				{
					return;
				}

				ConcurrentSelector(const ConcurrentSelector &other)
					: CompositeNode<Entity, Args...>(other), failPolicy(other.failPolicy), succeedPolicy(other.succeedPolicy), childrenStatus(other.childrenStatus)
				{
					return;
				}

				ConcurrentSelector(ConcurrentSelector &&other)
					: CompositeNode<Entity, Args...>(std::move(other)), failPolicy(other.failPolicy), succeedPolicy(other.succeedPolicy), childrenStatus(std::move(other.childrenStatus))
				{
					return;
				}

				ConcurrentSelector &operator=(const ConcurrentSelector &other)
				{
					if (this != &other)
					{
						CompositeNode<Entity, Args...>::operator=(other);
						failPolicy = other.failPolicy;
						succeedPolicy = other.succeedPolicy;
						childrenStatus = other.childrenStatus;
					}
					return *this;
				}

				ConcurrentSelector &operator=(ConcurrentSelector &&other)
				{
					if (this != &other)
					{
						CompositeNode<Entity, Args...>::operator=(std::move(other));
						failPolicy = other.failPolicy;
						succeedPolicy = other.succeedPolicy;
						childrenStatus = std::move(other.childrenStatus);
					}
					return *this;
				}

				virtual ~ConcurrentSelector(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					childrenStatus.clear();
					for (auto &child : this->children)
					{
						child->initialize(entity);
						childrenStatus.insert(std::make_pair(child, Status::Running));
					}
				}

				virtual Status execute(EntityPtr entity, Args... args) override final
				{
					if (childrenStatus.empty())
					{
						initialize(entity);
					}
					for (auto &child : this->children)
					{
						if (childrenStatus[child] == Status::Running)
						{
							const Status status = child->execute(entity, args...);
							if (status == Status::Failure)
							{
								if (failPolicy == FailurePolicy::FailOnOne)
								{
									initialize(entity);
									return Status::Failure;
								}
								else
								{
									childrenStatus[child] = Status::Failure;
								}
							}
							else if (status == Status::Success)
							{
								childrenStatus[child] = Status::Success;
							}
						}
						if (childrenStatus[child] == Status::Failure && failPolicy == FailurePolicy::FailOnAll)
						{
							childrenStatus[child] = child->execute(entity, args...);
						}
					}
					bool sawSuccess = false;
					bool sawAllFails = true;
					bool sawAllSuccess = true;
					for (auto &pair : childrenStatus)
					{
						switch (pair.second)
						{
						case Status::Success:
							if (succeedPolicy == SuccessPolicy::SuccessOnOne && failPolicy != FailurePolicy::FailOnOne)
							{
								initialize(entity);
								return Status::Success;
							}
							else
							{
								sawSuccess = true;
								sawAllFails = false;
							}
							break;
						case Status::Failure:
							if (failPolicy == FailurePolicy::FailOnOne)
							{
								initialize(entity);
								return Status::Failure;
							}
							else
							{
								sawAllSuccess = false;
							}
							break;
						case Status::Running:
							if (failPolicy == FailurePolicy::FailOnAll && succeedPolicy == SuccessPolicy::SuccessOnAll)
							{
								return Status::Running;
							}
							sawAllFails = false;
							sawAllSuccess = false;
							break;
						default:
							break;
						}
					}
					if (failPolicy == FailurePolicy::FailOnAll && sawAllFails)
					{
						initialize(entity);
						return Status::Failure;
					}
					else if ((succeedPolicy == SuccessPolicy::SuccessOnAll && sawAllSuccess) || (succeedPolicy == SuccessPolicy::SuccessOnOne && sawSuccess))
					{
						initialize(entity);
						return Status::Success;
					}
					else
					{
						return Status::Running;
					}
				}
			};
		}
	}
}
