#pragma once

#include <type_traits>

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			namespace Private
			{
				template <typename IntegerType, class = typename std::enable_if<std::is_integral<IntegerType>::value && !std::is_same<IntegerType, bool>::value>::type>
				class IntegerConditionEnabler
				{
				public:
					IntegerConditionEnabler(void) = default;
					IntegerConditionEnabler(const IntegerConditionEnabler &other) = default;
					IntegerConditionEnabler &operator=(const IntegerConditionEnabler &other) = default;
					virtual ~IntegerConditionEnabler(void) = default;
				};
			}
		}
	}
}