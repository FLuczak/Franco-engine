#pragma once
#include <vector>

#include "Inspectable.hpp"

class Inspector
{
public:
	Inspector();
	void InspectAll() const;
	void RegisterInspectable(Inspectable& inspectable);
private:
	void RegisterInspectable(std::reference_wrapper<Inspectable> inspectable);
	std::vector<std::reference_wrapper<Inspectable>> inspectables{};
};

