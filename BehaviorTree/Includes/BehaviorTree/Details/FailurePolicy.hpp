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
				enum class FailurePolicy : std::uint8_t
				{
					FailOnOne,
					FailOnAll
				};
			}
		}
	}
}
