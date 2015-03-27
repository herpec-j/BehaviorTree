#pragma once

#include <cstdint>

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			enum class Status : std::uint8_t
			{
				Success,
				Failure,
				Running
			};
		}
	}
}
