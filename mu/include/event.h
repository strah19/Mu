#ifndef EVENT_H
#define EVENT_H

#include <functional>

#define BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Mu {
    class Event {
	public:
		Event(const char* name)
			: m_name(name) { }
		virtual ~Event() = default;

		virtual const char* Name() const = 0;
	protected:
		const char* m_name;
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event* event)
			: m_event(event) {
		}

		virtual ~EventDispatcher() {
			m_event = nullptr;
		}

		template<typename T>
		bool Dispatch(const std::function<void(T&)> func) {
			if (dynamic_cast<T*>(m_event)) {
				func(static_cast<T&>(*m_event));
				return true;
			}
			return false;
		}

		inline Event* GetEvent() { return m_event; }
	private:
		Event* m_event;
	};

}

#endif // !EVENT_H