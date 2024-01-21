#pragma once
#include "SimpleDelegates.h"
#include "imgui-SFML.h"
#include "imgui.h"

class Inspectable
{
public:
	virtual ~Inspectable() = default;
	static inline fluczak::sdel::Delegate<void(std::reference_wrapper<Inspectable>)> OnInspectableCreated;
	virtual void Inspect() = 0;
	Inspectable();
};

