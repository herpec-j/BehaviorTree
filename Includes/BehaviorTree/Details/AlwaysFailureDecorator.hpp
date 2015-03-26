#pragma once

#include "Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class AlwaysFailureDecorator final : public DecoratorNode < Entity, Args... >
			{
			public:
				AlwaysFailureDecorator(void) = default;
				AlwaysFailureDecorator(const AlwaysFailureDecorator &other) = default;
				AlwaysFailureDecorator &operator=(const AlwaysFailureDecorator &other) = default;
				virtual ~AlwaysFailureDecorator(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					if (!children.empty())
					{
						children.front()->initialize(entity);
					}
				}

				virtual Status filter(EntityPtr entity, Args... args) override final
				{
					if (!children.empty())
					{
						const Status status = children.front()->execute(entity, args...);
						if (status == Status::Success || status == Status::Failure)
						{
							children.front()->initialize(entity);
						}
					}
					return Status::Failure;
				}
			};
		}
	}
}