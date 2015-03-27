#pragma once

#include <cstdint>

namespace AO
{
	namespace BehaviorTree
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
