#pragma once

// Status
#include "BehaviorTree/Details/Status.hpp"

// Policies
#include "BehaviorTree/Details/FailurePolicy.hpp"
#include "BehaviorTree/Details/SuccessPolicy.hpp"

// Conditions
#include "BehaviorTree/Details/ConditionTest.hpp"

// Root Nodes
#include "BehaviorTree/Details/RootNode.hpp"

// Base Nodes
#include "BehaviorTree/Details/ActionNode.hpp"
#include "BehaviorTree/Details/CompositeNode.hpp"
#include "BehaviorTree/Details/ConditionNode.hpp"
#include "BehaviorTree/Details/DecoratorNode.hpp"

// Condition Nodes
#include "BehaviorTree/Details/FunctionCondition.hpp"
#include "BehaviorTree/Details/BooleanCondition.hpp"
#include "BehaviorTree/Details/DecimalCondition.hpp"
#include "BehaviorTree/Details/IntegerCondition.hpp"

// Decorator Nodes
#include "BehaviorTree/Details/AlwaysFailureDecorator.hpp"
#include "BehaviorTree/Details/AlwaysRunningDecorator.hpp"
#include "BehaviorTree/Details/AlwaysSuccessDecorator.hpp"
#include "BehaviorTree/Details/FailureAfterDecorator.hpp"
#include "BehaviorTree/Details/RunningAfterDecorator.hpp"
#include "BehaviorTree/Details/SuccessAfterDecorator.hpp"
#include "BehaviorTree/Details/CountLimitDecorator.hpp"
#include "BehaviorTree/Details/RepeatDecorator.hpp"

// Selector Nodes
#include "BehaviorTree/Details/ConcurrentSelector.hpp"
#include "BehaviorTree/Details/PrioritySelector.hpp"
#include "BehaviorTree/Details/ProbabilitySelector.hpp"
#include "BehaviorTree/Details/RandomSelector.hpp"
#include "BehaviorTree/Details/SequenceSelector.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		inline namespace Version_1
		{
			// Status
			using Status = Details::Status;

			// Policies
			using FailurePolicy = Details::FailurePolicy;
			using SuccessPolicy = Details::SuccessPolicy;

			// Conditions
			using ConditionTest = Details::ConditionTest;

			template <class Entity, typename... Args>
			class BehaviorTree
			{
			private:
				// Root Nodes
				using Root = typename Details::RootNode < Entity, Args... >;
				using RootPtr = std::shared_ptr < Root >;

			public:
				// Entity Type
				using EntityType = Entity;
				using EntityPtr = EntityType *;

				// Status
				using Status = Details::Status;

				// Policies
				using FailurePolicy = Details::FailurePolicy;
				using SuccessPolicy = Details::SuccessPolicy;

				// Conditions
				using ConditionTest = Details::ConditionTest;

				// Base Nodes
				using Action = typename Details::ActionNode < Entity, Args... >;
				using Composite = typename Details::CompositeNode < Entity, Args... >;
				using Condition = typename Details::ConditionNode < Entity, Args... >;
				using Decorator = typename Details::DecoratorNode < Entity, Args... >;

				// Condition Nodes
				using FunctionCondition = typename Details::FunctionCondition < Entity, Args... >;
				using BooleanCondition = typename Details::BooleanCondition < Entity, Args... >;
				template <typename DecimalType>
				using DecimalCondition = typename Details::DecimalCondition < DecimalType, Entity, Args... >;
				template <typename IntegerType>
				using IntegerCondition = typename Details::IntegerCondition < IntegerType, Entity, Args... >;

				// Decorator Nodes
				using AlwaysFailureDecorator = typename Details::AlwaysFailureDecorator < Entity, Args... >;
				using AlwaysRunningDecorator = typename Details::AlwaysRunningDecorator < Entity, Args... >;
				using AlwaysSuccessDecorator = typename Details::AlwaysSuccessDecorator < Entity, Args... >;
				using FailureAfterDecorator = typename Details::FailureAfterDecorator < Entity, Args... >;
				using RunningAfterDecorator = typename Details::RunningAfterDecorator < Entity, Args... >;
				using SuccessAfterDecorator = typename Details::SuccessAfterDecorator < Entity, Args... >;
				using CountLimitDecorator = typename Details::CountLimitDecorator < Entity, Args... >;
				using RepeatDecorator = typename Details::RepeatDecorator < Entity, Args... >;

				// Selector Nodes
				using ConcurrentSelector = typename Details::ConcurrentSelector < Entity, Args... >;
				using PrioritySelector = typename Details::PrioritySelector < Entity, Args... >;
				using ProbabilitySelector = typename Details::ProbabilitySelector < Entity, Args... >;
				using RandomSelector = typename Details::RandomSelector < Entity, Args... >;
				using SequenceSelector = typename Details::SequenceSelector < Entity, Args... >;

				// Constructors
				BehaviorTree(void) = delete;

				BehaviorTree(EntityPtr owner)
					: owner(owner)
				{
					assert(owner && "Invalid entity");
					assignBehavior<AlwaysRunningDecorator>();
				}

				BehaviorTree(const BehaviorTree &other)
					: owner(other.owner), behavior(other.behavior)
				{
					return;
				}

				BehaviorTree(BehaviorTree &&other)
					: owner(other.owner), behavior(std::move(other.behavior))
				{
					other.owner = nullptr;
				}

				// Assignment Operators
				BehaviorTree &operator=(const BehaviorTree &other)
				{
					if (this != &other)
					{
						owner = other.owner;
						behavior = other.behavior;
					}
					return *this;
				}

				BehaviorTree &operator=(BehaviorTree &&other)
				{
					if (this != &other)
					{
						owner = other.owner;
						other.owner = nullptr;
						behavior = std::move(other.behavior);
					}
					return *this;
				}

				// Destructor
				virtual ~BehaviorTree(void) = default;

				// Methods
				Status update(Args... args)
				{
					return behavior->execute(owner, args...);
				}

				template <typename B, typename... BehaviorArgs>
				void assignBehavior(BehaviorArgs &&...args)
				{
					assignBehavior(std::make_shared<B>(std::forward<BehaviorArgs>(args)...));
				}

				void assignBehavior(RootPtr newBehavior)
				{
					assert(newBehavior && "Invalid behavior");
					behavior = newBehavior;
					reinitialize();
				}

				void reinitialize(void)
				{
					behavior->initialize(owner);
				}

			private:
				// Attributes
				EntityPtr owner;
				RootPtr behavior;
			};
		}
	}
}
