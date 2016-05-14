#pragma once

#include <vector>
#include <functional>
#include "base.h"
#include "control.h"
using namespace std;

namespace HUI {

typedef tr1::function<void(const UIControl *sender)> Proc;

template<typename T>
class EventHandler
{
public:
	EventHandler(void(T::*theProc)(const UIControl *sender), T *theObj) : proc(theProc), obj(theObj) {}
public:
	void(T::*proc)(const UIControl *sender);
	T *obj;
}; // class handler

class MouseEvent
{
public:
	template<typename T>
	void AddHandler(EventHandler<T> handler) { 
		mv_procs.push_back(bind(handler.proc, handler.obj, tr1::placeholders::_1));
	}

	template<typename T>
	inline const MouseEvent& operator+=(EventHandler<T> handler) {
		mv_procs.push_back(bind(handler.proc, handler.obj, tr1::placeholders::_1));
		return *this;
	}

	void RaiseEvent(const UIControl *sender) {
	for (auto var : mv_procs) { 
		var(sender);
	}
}
private:
	vector<Proc> mv_procs;
}; // class MouseEvent

} // namespace HUI 