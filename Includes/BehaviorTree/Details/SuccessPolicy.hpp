#pragma once

#include <cstdint>

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			enum class SuccessPolicy : std::uint8_t
			{
				SuccessOnOne,
				SuccessOnAll
			};
		}
	}
}