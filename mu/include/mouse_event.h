#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "event.h"

namespace Mu {
	struct MouseButtonEvents : public Event {
		MouseButtonEvents(bool action, int button_id)
			: Event("Mouse Button Event"), m_action(action), m_button_id(button_id) { }
		virtual ~MouseButtonEvents() = default;

		const char* Name() const { return m_name; }

		int m_action;
		int m_button_id;
	};

	struct MousePositionEvent : public Event {
		MousePositionEvent(double x, double y)
			: Event("Mouse Position Event"), m_x(x), m_y(y) { }
		virtual ~MousePositionEvent() = default;

		const char* Name() const { return m_name; }

		double m_x, m_y;
	};

	struct MouseWheelEvent : public Event {
		MouseWheelEvent(double xoffset, double yoffset) : Event("Mouse Wheel"), m_xoffset(xoffset), m_yoffset(yoffset) { }
		virtual ~MouseWheelEvent() = default;

		const char* Name() const { return m_name; }

		double m_xoffset, m_yoffset;
	};
}

#endif // !MOUSE_EVENT_H