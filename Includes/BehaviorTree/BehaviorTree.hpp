#pragma once

// Status
#include "Details/Status.hpp"

// Policies
#include "Details/FailurePolicy.hpp"
#include "Details/SuccessPolicy.hpp"

// Conditions
#include "Details/ConditionTest.hpp"

// Root Nodes
#include "Details/RootNode.hpp"

// Base Nodes
#include "Details/ActionNode.hpp"
#include "Details/CompositeNode.hpp"
#include "Details/ConditionNode.hpp"
#include "Details/DecoratorNode.hpp"

// Condition Nodes
#include "Details/FunctionCondition.hpp"
#include "Details/BooleanCondition.hpp"
#include "Details/DecimalCondition.hpp"
#include "Details/IntegerCondition.hpp"

// Decorator Nodes
#include "Details/AlwaysFailureDecorator.hpp"
#include "Details/AlwaysRunningDecorator.hpp"
#include "Details/AlwaysSuccessDecorator.hpp"
#include "Details/FailureAfterDecorator.hpp"
#include "Details/RunningAfterDecorator.hpp"
#include "Details/SuccessAfterDecorator.hpp"
#include "Details/CountLimitDecorator.hpp"
#include "Details/RepeatDecorator.hpp"

// Selector Nodes
#include "Details/ConcurrentSelector.hpp"
#include "Details/PrioritySelector.hpp"
#include "Details/ProbabilitySelector.hpp"
#include "Details/RandomSelector.hpp"
#include "Details/SequenceSelector.hpp"

namespace AO
{
	namespace BehaviorTree
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
			using Action = typename Details::ActionNode < Entity, Args... > ;
			using Composite = typename Details::CompositeNode < Entity, Args... >;
			using Condition = typename Details::ConditionNode < Entity, Args... >;
			using Decorator = typename Details::DecoratorNode < Entity, Args... > ;

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

			virtual ~BehaviorTree(void) = default;

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
			EntityPtr owner;
			RootPtr behavior;
		};
	}
}
