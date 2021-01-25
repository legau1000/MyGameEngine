#include "Component.hpp"

Component::Component(MyComponentEnum myClass): _myClass(myClass)
{
}

Component::~Component()
{
}

const MyComponentEnum Component::getType()
{
	return (_myClass);
}
