#pragma once
#include <functional>

namespace gateserver {

class Defer {
public:
	Defer(std::function<void()> func) : func_(func) {}
	~Defer() {
		func_();
	}
private:
	std::function<void()> func_;
};

}
