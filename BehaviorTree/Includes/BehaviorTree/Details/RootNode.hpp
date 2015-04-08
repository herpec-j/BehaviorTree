#pragma once

#include "BehaviorTree/Details/Status.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		inline namespace Version_1
		{
			namespace Details
			{
				template <class Entity, typename... Args>
				class RootNode
				{
				protected:
					using EntityType = Entity;
					using EntityPtr = EntityType *;

					RootNode(void) = default;
					RootNode(const RootNode &other) = default;
					RootNode &operator=(const RootNode &other) = default;

				public:
					virtual ~RootNode(void) = default;

					virtual void initialize(EntityPtr entity) = 0;
					virtual Status execute(EntityPtr entity, Args... args) = 0;
				};
			}
		}
	}
}