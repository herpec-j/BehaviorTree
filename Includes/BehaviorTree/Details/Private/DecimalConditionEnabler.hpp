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
				template < typename DecimalType, class = typename std::enable_if<std::is_floating_point<DecimalType>::value>::type>
				class DecimalConditionEnabler
				{
				public:
					DecimalConditionEnabler(void) = default;
					DecimalConditionEnabler(const DecimalConditionEnabler &other) = default;
					DecimalConditionEnabler &operator=(const DecimalConditionEnabler &other) = default;
					virtual ~DecimalConditionEnabler(void) = default;
				};
			}
		}
	}
}