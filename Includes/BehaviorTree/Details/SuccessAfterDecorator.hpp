#pragma once

#include "Details/DecoratorNode.hpp"

namespace AO
{
	namespace BehaviorTree
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class SuccessAfterDecorator final : public DecoratorNode < Entity, Args... >
			{
			private:
				std::size_t total;
				std::size_t count;

			public:
				SuccessAfterDecorator(void) = delete;

				SuccessAfterDecorator(std::size_t total)
					: DecoratorNode(), total(total), count(total)
				{
					return;
				}

				SuccessAfterDecorator(const SuccessAfterDecorator &other)
					: DecoratorNode(other), total(other.total), count(other.count)
				{
					return;
				}

				SuccessAfterDecorator(SuccessAfterDecorator &&other)
					: DecoratorNode(std::move(other)), total(other.total), count(other.count)
				{
					other.total = 0;
					other.count = 0;
				}

				SuccessAfterDecorator &operator=(const SuccessAfterDecorator &other)
				{
					if (this != &other)
					{
						DecoratorNode::operator=(other);
						total = other.total;
						count = other.count;
					}
					return *this;
				}

				SuccessAfterDecorator &operator=(SuccessAfterDecorator &&other)
				{
					if (this != &other)
					{
						DecoratorNode::operator=(std::move(other));
						total = other.total;
						other.total = 0;
						count = other.count;
						other.count = 0;
					}
					return *this;
				}

				virtual ~SuccessAfterDecorator(void) = default;

			protected:
				virtual void initialize(EntityPtr entity) override final
				{
					count = total;
					if (!children.empty())
					{
						children.front()->initialize(entity);
					}
				}

				virtual Status filter(EntityPtr entity, Args... args) override final
				{
					if (count == 0)
					{
						if (!children.empty())
						{
							const Status status = children.front()->execute(entity, args...);
							if (status == Status::Success || status == Status::Failure)
							{
								children.front()->initialize(entity);
							}
						}
						return Status::Success;
					}
					else
					{
						--count;
						if (!children.empty())
						{
							const Status status = children.front()->execute(entity, args...);
							if (status == Status::Success || status == Status::Failure)
							{
								children.front()->initialize(entity);
							}
							return status;
						}
						return Status::Running;
					}
				}
			};
		}
	}
}