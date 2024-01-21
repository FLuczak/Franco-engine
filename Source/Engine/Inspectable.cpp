#include "Engine/Inspectable.hpp"



Inspectable::Inspectable()
{
	OnInspectableCreated(std::reference_wrapper(*this));
}
