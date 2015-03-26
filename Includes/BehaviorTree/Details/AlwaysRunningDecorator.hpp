#pragma once

#include "Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class AlwaysRunningDecorator final : public DecoratorNode < Entity, Args... >
			{
			public:
				AlwaysRunningDecorator(void) = default;
				AlwaysRunningDecorator(const AlwaysRunningDecorator &other) = default;
				AlwaysRunningDecorator &operator=(const AlwaysRunningDecorator &other) = default;
				virtual ~AlwaysRunningDecorator(void) = default;

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
					return Status::Running;
				}
			};
		}
	}
}