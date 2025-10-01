/*
ABSTRACT BASE CLASS
Components define the functionality of a GameObject.
All components inherit from this class.

NOTE: May have components purely store data (So perhaps no update function), and instead have managers handle grabbing data to then define functionality.

Components should not be coupled, if a component needs/wants to communicate with another component, e.g.
transformComponent to a modelComponent, then that should be handled by managers. The component absolutely should
not depend on that other component existing on a GameObject.

Ideally component Derivations should not be inheriting other component derivations, as the components will just be storing the data.
The inheritance should only ever be ONE LEVEL deep. (Component Base > Derived Component)

All Components will inherit from the CloneableComponent class instead of Component directly. (This allows me to copy objects in the scene).
*/

#pragma once
#include <memory>

class Component
{
public:
	Component();
	~Component() = default;
	Component(const Component& component) = default; // Copy Constructor
	Component(Component&& component) = default; // Move Constructor

	virtual std::unique_ptr<Component> clone() const = 0; // Creates a copy // As this is abstract it CANNOT return

	virtual void Update() = 0;
};



/*
This handles creating a clone on each derivation of component.
Gets rid of the boilerplate for the most-part. However you still need to make each component inherit this.
*/
template <typename Derived>
class ClonableComponent : public Component {
public:
	virtual std::unique_ptr<Component> clone() const { return std::unique_ptr<Derived>(new Derived(*this)); }
};
