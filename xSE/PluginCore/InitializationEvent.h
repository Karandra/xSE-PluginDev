#pragma once
#include "Framework.hpp"
#include "kxf/EventSystem/Event.h"

namespace xSE
{
	class InitializationEvent: public kxf::BasicEvent
	{
		public:
			KxEVENT_MEMBER(InitializationEvent, Query);
			KxEVENT_MEMBER(InitializationEvent, Load);

		public:
			InitializationEvent() = default;

		public:
			// IEvent
			std::unique_ptr<IEvent> Move() noexcept override
			{
				return std::make_unique<InitializationEvent>(std::move(*this));
			}
	};
}
