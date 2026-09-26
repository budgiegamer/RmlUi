#pragma once

#include "Dictionary.h"
#include "Header.h"
#include "ID.h"
#include "ScriptInterface.h"

namespace Rml {

class Factory;
class Element;
class EventInstancer;
struct EventSpecification;

enum class EventPhase { None, Capture = 1, Target = 2, Bubble = 4 };
enum class DefaultActionPhase { None, Target = (int)EventPhase::Target, TargetAndBubble = ((int)Target | (int)EventPhase::Bubble) };

struct IEvent
{
	virtual EventPhase GetPhase() const = 0;
	virtual void SetPhase(EventPhase phase) = 0;
	virtual void SetCurrentElement(Element* element) = 0;
	virtual Element* GetCurrentElement() const = 0;
	virtual Element* GetTargetElement() const = 0;
	virtual const String& GetType() const = 0;
	virtual EventId GetId() const = 0;
	virtual void StopPropagation() = 0;
	virtual void StopImmediatePropagation() = 0;
	virtual bool IsInterruptible() const = 0;
	virtual bool IsPropagating() const = 0;
	virtual bool IsImmediatePropagating() const = 0;
	virtual bool GetBoolParamater(const char* key, bool def = false) = 0;
	virtual int GetIntParamater(const char* key, int def = 0) = 0;
	virtual float GetFloatParamater(const char* key, float def = 0.0) = 0;
	virtual bool GetStringParamater(const char* key, char* str) = 0;
	//virtual const char* GetStringParamaterUnsafe(const char* key, const char* def = "") = 0;
	virtual size_t GetStringParamLen(const char* key, const char* def = "") = 0;
	virtual const char* GetKey(size_t i) = 0;
	virtual size_t GetParamCount() = 0;
};

/*
    An event that propagates through the element hierarchy. Events follow the DOM3 event specification. See
    http://www.w3.org/TR/DOM-Level-3-Events/events.html.
*/

class RMLUICORE_API Event : public IEvent, public ScriptInterface {
public:
	/// Constructor
	Event();
	/// Constructor
	/// @param[in] target The target element of this event
	/// @param[in] id The event id
	/// @param[in] type The event type
	/// @param[in] parameters The event parameters
	/// @param[in] interruptible Can this event have is propagation stopped?
	Event(Element* target, EventId id, const String& type, const Dictionary& parameters, bool interruptible);
	/// Destructor
	virtual ~Event();

	/// Get the current propagation phase.
	EventPhase GetPhase() const override;
	/// Set the current propagation phase
	void SetPhase(EventPhase phase) override;

	/// Set the current element in the propagation.
	void SetCurrentElement(Element* element) override;
	/// Get the current element in the propagation.
	Element* GetCurrentElement() const override;
	/// Get the target element of this event.
	Element* GetTargetElement() const override;

	/// Get the event type.
	const String& GetType() const override;
	/// Get the event id.
	EventId GetId() const override;

	/// Stops propagation of the event if it is interruptible, but finish all listeners on the current element.
	void StopPropagation() override;
	/// Stops propagation of the event if it is interruptible, including to any other listeners on the current element.
	void StopImmediatePropagation() override;

	/// Returns true if the event can be interrupted, that is, stopped from propagating.
	bool IsInterruptible() const override;
	/// Returns true if the event is still propagating.
	bool IsPropagating() const override;
	/// Returns true if the event is still immediate propagating.
	bool IsImmediatePropagating() const override;

	/// Checks if the event is of a certain type.
	/// @param type The name of the type to check for.
	/// @return True if the event is of the requested type, false otherwise.
	bool operator==(const String& type) const;
	/// Checks if the event is of a certain id.
	bool operator==(EventId id) const;

	/// Returns the value of one of the event's parameters.
	/// @param key[in] The name of the desired parameter.
	/// @param default_value[in] The default value.
	/// @return The value of the requested parameter, or the default value if the key does not exist.
	template <typename T>
	T GetParameter(const String& key, const T& default_value = T()) const
	{
		return Get(parameters, key, default_value);
	}
	bool GetBoolParamater(const char* key, bool def = false) override
	{
		return Get(parameters, key, def);
	};
	int GetIntParamater(const char* key, int def = 0) override
	{
		return Get(parameters, key, def);
	};
	float GetFloatParamater(const char* key, float def = 0.0) override
	{
		return Get(parameters, key, def);
	};
	bool GetStringParamater(const char* key, char* str) override
	{
		auto sstr = Get(parameters, key, String(str));

		std::strcpy(str, sstr.c_str());

		return true;
	};
	/*const char* GetStringParamaterUnsafe(const char* key, const char* def = "") override
	{
		auto sstr = Get(parameters, key, String(def));
		//auto cstr = sstr.c_str();
		return sstr.c_str();
	};*/
	size_t GetStringParamLen(const char* key, const char* def = "") override
	{
		auto str = Get(parameters, key, String(def));
		if (str == def)
			return 0;
		return str.size() + 1;
	};

	/// Access the dictionary of parameters
	/// @return The dictionary of parameters
	const Dictionary& GetParameters() const;

	const char* GetKey(size_t i) override
	{
		size_t cur_index = 0;
		for(auto& s : parameters)
		{
			if (cur_index == i)
				if (!s.first.empty())
					return s.first.c_str();
			cur_index++;
		}
		return nullptr;
	};

	size_t GetParamCount() override
	{
		return parameters.size();
	}

	/// Return the unprojected mouse screen position.
	/// Note: Only specified for events with 'mouse_x' and 'mouse_y' parameters.
	Vector2f GetUnprojectedMouseScreenPos() const;

protected:
	Dictionary parameters;

	Element* target_element = nullptr;
	Element* current_element = nullptr;

private:
	/// Project the mouse coordinates to the current element to enable
	/// interacting with transformed elements.
	void ProjectMouse(Element* element);

	/// Release this event through its instancer.
	void Release() override;

	String type;
	EventId id = EventId::Invalid;
	bool interruptible = false;

	bool interrupted = false;
	bool interrupted_immediate = false;

	bool has_mouse_position = false;
	Vector2f mouse_screen_position = Vector2f(0, 0);

	EventPhase phase = EventPhase::None;

	EventInstancer* instancer = nullptr;

	friend class Rml::Factory;
};

} // namespace Rml
