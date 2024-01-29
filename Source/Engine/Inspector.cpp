#include "Engine/Inspector.hpp"

Inspector::Inspector()
{
	Inspectable::OnInspectableCreated.bind(this, &Inspector::RegisterInspectable);
}

void Inspector::InspectAll() const
{
	for (auto& element : inspectables)
	{
		element.get().Inspect();
	}
}

void Inspector::RegisterInspectable(Inspectable& inspectable)
{
	inspectables.emplace_back(inspectable);
}

void Inspector::RegisterInspectable(std::reference_wrapper<Inspectable> inspectable)
{
	inspectables.push_back(inspectable);
}
