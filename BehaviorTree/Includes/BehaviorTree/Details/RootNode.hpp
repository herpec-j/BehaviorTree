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

					// Constructors
					RootNode(void) = default;

					RootNode(RootNode const &) = default;

					RootNode(RootNode &&) = default;

					// Assignment Operators
					RootNode &operator=(RootNode const &) = default;

					RootNode &operator=(RootNode &&) = default;

				public:
					// Destructor
					virtual ~RootNode(void) = default;

					// Virtual Methods
					virtual void initialize(EntityPtr entity) = 0;

					virtual Status execute(EntityPtr entity, Args... args) = 0;
				};
			}
		}
	}
}