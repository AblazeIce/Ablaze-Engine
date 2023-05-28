#pragma once
#include "AblazePch.h"
#include "Ablaze/Core/Core.h"

namespace Ablaze {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};
	//##：参数连接操作
	//#:参数字符串化
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
					virtual EventType GetEventType() const override{return GetStaticType();}\
					virtual const char* GetName() const override {return #type;}
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class ABLAZE_API Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;//作用一：获得事件类型
		virtual	 const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const { return GetName(); }//作用二：获得事件名称
		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }//作用三：检测事件是否存在于目录中
		inline bool GetEventIsHandled() { return m_Handled; }
	private:
		//是否被处理
		bool m_Handled = false;
	};
	//事件调度器
	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			:m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	//重写日志系统里的<<
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}
