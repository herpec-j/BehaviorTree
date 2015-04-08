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
				enum class Status : std::uint8_t
				{
					Success,
					Failure,
					Running
				};
			}
		}
	}
}
