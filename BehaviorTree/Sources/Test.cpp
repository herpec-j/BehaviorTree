#include <iostream>

#include "BehaviorTree/BehaviorTree.hpp"

struct Agent : public AO::BehaviorTree::BehaviorTree < Agent, short >
{
	Agent(void)
		: BehaviorTree(this)
	{
		return;
	}

	AO::BehaviorTree::Status returnSuccess(Agent *, short) const
	{
		return AO::BehaviorTree::Status::Success;
	}

	AO::BehaviorTree::Status returnFailure(Agent *, short) const
	{
		return AO::BehaviorTree::Status::Failure;
	}

	AO::BehaviorTree::Status returnRunning(Agent *, short) const
	{
		return AO::BehaviorTree::Status::Running;
	}

	int returnInt(void)
	{
		return 5;
	}

	int returnIntConst(void) const
	{
		return 5;
	}

	float returnFloat(void) const
	{
		return 5.0f;
	}

	bool returnTrue(void) const
	{
		return true;
	}

	bool returnFalse(void) const
	{
		return false;
	}
};

static void testBehaviorTree(void)
{
	Agent agent;
	{
		agent.assignBehavior<Agent::BooleanCondition>(std::bind(&Agent::returnTrue, &agent), true);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		agent.assignBehavior<Agent::BooleanCondition>(std::bind(&Agent::returnTrue, &agent), false);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		agent.assignBehavior<Agent::BooleanCondition>(std::bind(&Agent::returnFalse, &agent), true);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		agent.assignBehavior<Agent::BooleanCondition>(std::bind(&Agent::returnFalse, &agent), false);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
	{
		auto node = std::make_shared<Agent::CountLimitDecorator>(0);
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::CountLimitDecorator>(3);
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		}
	}
	{
		auto node = std::make_shared<Agent::CountLimitDecorator>(3);
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		}
	}
	{
		auto node = std::make_shared<Agent::CountLimitDecorator>(3);
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		}
	}
	{
		auto node = std::make_shared<Agent::CountLimitDecorator>(3, false);
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		for (int i = 0; i < 2; ++i)
		{
			agent.reinitialize();
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		}
	}
	{
		auto node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::Equal, 5.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::Equal, 10.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::NotEqual, 5.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::NotEqual, 10.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
	{
		auto node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::GreaterThan, 2.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::GreaterThan, 10.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::LessThan, 10.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::LessThan, -1.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::GreaterOrEqualThan, 5.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::GreaterOrEqualThan, 10.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::GreaterOrEqualThan, 4.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::GreaterOrEqualThan, 5.000000001f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
	{
		auto node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::LessOrEqualThan, 5.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::LessOrEqualThan, 10.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::DecimalCondition<float>>(std::bind(&Agent::returnFloat, &agent), AO::BehaviorTree::ConditionTest::LessOrEqualThan, 4.0f);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		agent.assignBehavior<Agent::FunctionCondition>(std::bind(&Agent::returnSuccess, &agent, std::placeholders::_1, std::placeholders::_2));
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		assert(agent.update(5) == AO::BehaviorTree::Status::Success);
		agent.assignBehavior<Agent::FunctionCondition>(std::bind(&Agent::returnFailure, &agent, std::placeholders::_1, std::placeholders::_2));
		assert(agent.update(10) == AO::BehaviorTree::Status::Failure);
		agent.assignBehavior<Agent::FunctionCondition>(std::bind(&Agent::returnRunning, &agent, std::placeholders::_1, std::placeholders::_2));
		assert(agent.update(35) == AO::BehaviorTree::Status::Running);
	}
	{
		auto node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnInt, &agent), AO::BehaviorTree::ConditionTest::Equal, 5);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnInt, &agent), AO::BehaviorTree::ConditionTest::Equal, 10);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::Equal, 5);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::Equal, 10);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::NotEqual, 5);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::NotEqual, 10);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
	{
		auto node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::GreaterThan, 2);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::GreaterThan, 10);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::LessThan, 10);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::LessThan, -1);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
{
	auto node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::GreaterOrEqualThan, 5);
	agent.assignBehavior(node);
	assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::GreaterOrEqualThan, 10);
	agent.assignBehavior(node);
	assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::GreaterOrEqualThan, 4);
	agent.assignBehavior(node);
	assert(agent.update(42) == AO::BehaviorTree::Status::Success);
}
	{
		auto node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::LessOrEqualThan, 5);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::LessOrEqualThan, 10);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		node = std::make_shared<Agent::IntegerCondition<int>>(std::bind(&Agent::returnIntConst, &agent), AO::BehaviorTree::ConditionTest::LessOrEqualThan, 4);
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::ConcurrentSelector>();
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(3));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		}
	}
	{
		auto node = std::make_shared<Agent::ConcurrentSelector>();
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(0));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::ConcurrentSelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
	}
	{
		auto node = std::make_shared<Agent::ConcurrentSelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(2));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		}
	}
{
	auto node = std::make_shared<Agent::ConcurrentSelector>(AO::BehaviorTree::FailurePolicy::FailOnOne);
	node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
	node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(2));
	node->addChild(std::make_shared<Agent::FailureAfterDecorator>(3));
	node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(4));
	for (int i = 0; i < 2; ++i)
	{
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
}
	{
		auto node = std::make_shared<Agent::ConcurrentSelector>(AO::BehaviorTree::FailurePolicy::FailOnAll, AO::BehaviorTree::SuccessPolicy::SuccessOnOne);
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(2));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(0));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(4));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		}
	}
	{
		auto node = std::make_shared<Agent::ConcurrentSelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(2));
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(4));
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(7));
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(6));
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(2));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		}
	}
	{
		auto node = std::make_shared<Agent::PrioritySelector>();
		node->addChild(std::make_shared<Agent::AlwaysRunningDecorator>());
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
	}
	{
		auto node = std::make_shared<Agent::PrioritySelector>();
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		}
	}
	{
		auto node = std::make_shared<Agent::PrioritySelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		}
	}
	{
		auto node = std::make_shared<Agent::PrioritySelector>();
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(1));
		for (int i = 0; i < 2; ++i)
		{
			agent.assignBehavior(node);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		}
	}
	{
		auto node = std::make_shared<Agent::ProbabilitySelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
	{
		auto node = std::make_shared<Agent::ProbabilitySelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(0));
		agent.assignBehavior(node);
		std::size_t successes = 0;
		std::size_t failures = 0;
		for (std::size_t i = 0; i < 10000; ++i)
		{
			switch (agent.update(42))
			{
				case AO::BehaviorTree::Status::Success:
					++successes;
					break;
				case AO::BehaviorTree::Status::Failure:
					++failures;
					break;
				default:
					break;
			}
		}
		assert(std::max(successes, failures) <= 5200);
	}
	{
		auto node = std::make_shared<Agent::ProbabilitySelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0), 9);
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(0), 1);
		agent.assignBehavior(node);
		std::size_t successes = 0;
		std::size_t failures = 0;
		for (std::size_t i = 0; i < 10000; ++i)
		{
			switch (agent.update(42))
			{
				case AO::BehaviorTree::Status::Success:
					++successes;
					break;
				case AO::BehaviorTree::Status::Failure:
					++failures;
					break;
				default:
					break;
			}
		}
		assert(successes >= 8950 && successes <= 9050 && failures >= 950 && failures <= 1050);
	}
	{
		auto node = std::make_shared<Agent::RandomSelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(0));
		agent.assignBehavior(node);
		std::size_t successes = 0;
		std::size_t failures = 0;
		for (std::size_t i = 0; i < 10000; ++i)
		{
			switch (agent.update(42))
			{
				case AO::BehaviorTree::Status::Success:
					++successes;
					break;
				case AO::BehaviorTree::Status::Failure:
					++failures;
					break;
				default:
					break;
			}
		}
		assert(std::max(successes, failures) <= 5200);
	}
{
	auto node = std::make_shared<Agent::RepeatDecorator>(5);
	node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
	agent.assignBehavior(node);
	for (int i = 0; i < 2; ++i)
	{
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
}
	{
		auto node = std::make_shared<Agent::RepeatDecorator>(5);
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(0));
		agent.assignBehavior(node);
		for (int i = 0; i < 2; ++i)
		{
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		}
	}
	{
		auto node = std::make_shared<Agent::SequenceSelector>();
		node->addChild(std::make_shared<Agent::AlwaysRunningDecorator>());
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
	}
	{
		auto node = std::make_shared<Agent::SequenceSelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(0));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
	{
		auto node = std::make_shared<Agent::SequenceSelector>();
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(0));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
	}
	{
		auto node = std::make_shared<Agent::SequenceSelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
	{
		auto node = std::make_shared<Agent::SequenceSelector>();
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		agent.assignBehavior(node);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		agent.reinitialize();
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Running);
		assert(agent.update(42) == AO::BehaviorTree::Status::Success);
	}
	{
		auto node = std::make_shared<Agent::SequenceSelector>();
		auto childNode = std::make_shared<Agent::SequenceSelector>();
		childNode->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		node->addChild(childNode);
		node->addChild(std::make_shared<Agent::SuccessAfterDecorator>(1));
		node->addChild(std::make_shared<Agent::FailureAfterDecorator>(0));
		agent.assignBehavior(node);
		for (int i = 0; i < 2; ++i)
		{
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Running);
			assert(agent.update(42) == AO::BehaviorTree::Status::Failure);
		}
	}
}

int main(int, char *[])
{
	testBehaviorTree();
	std::cout << "Success!" << std::endl;
	return EXIT_SUCCESS;
}
