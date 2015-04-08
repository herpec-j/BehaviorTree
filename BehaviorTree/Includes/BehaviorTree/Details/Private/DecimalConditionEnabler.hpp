#pragma once

#include <type_traits>

namespace AO
{
	namespace BehaviorTree
	{
		inline namespace Version_1
		{
			namespace Details
			{
				namespace Private
				{
					template < typename DecimalType, class = typename std::enable_if<std::is_floating_point<DecimalType>::value>::type>
					class DecimalConditionEnabler
					{
					public:
						// Constructors
						DecimalConditionEnabler(void) = default;

						DecimalConditionEnabler(DecimalConditionEnabler const &) = default;

						DecimalConditionEnabler(DecimalConditionEnabler &&) = default;

						// Assignment Operators
						DecimalConditionEnabler &operator=(DecimalConditionEnabler const &) = default;

						DecimalConditionEnabler &operator=(DecimalConditionEnabler &&) = default;

						// Destructor
						virtual ~DecimalConditionEnabler(void) = default;
					};
				}
			}
		}
	}
}