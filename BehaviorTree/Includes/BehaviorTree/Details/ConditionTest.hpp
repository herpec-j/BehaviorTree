#pragma once

#include <cstdint>

namespace AO
{
	namespace BehaviorTree
	{
		inline namespace Version_1
		{
			namespace Details
			{
				enum class ConditionTest : std::uint8_t
				{
					LessThan,
					GreaterThan,
					LessOrEqualThan,
					GreaterOrEqualThan,
					Equal,
					NotEqual
				};
			}
		}
	}
}
