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
					template <typename IntegerType, class = typename std::enable_if<std::is_integral<IntegerType>::value && !std::is_same<IntegerType, bool>::value>::type>
					class IntegerConditionEnabler
					{
					public:
						// Constructors
						IntegerConditionEnabler(void) = default;

						IntegerConditionEnabler(IntegerConditionEnabler const &) = default;

						IntegerConditionEnabler(IntegerConditionEnabler &&) = default;
						
						// Assignment Operators
						IntegerConditionEnabler &operator=(IntegerConditionEnabler const &) = default;

						IntegerConditionEnabler &operator=(IntegerConditionEnabler &&) = default;

						// Destructor
						virtual ~IntegerConditionEnabler(void) = default;
					};
				}
			}
		}
	}
}