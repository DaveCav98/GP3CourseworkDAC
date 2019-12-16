#pragma once

//forward delcaring to avoid circular includes
class Entity;

class Component
{
protected:
	Entity* m_entity = nullptr;
	friend class Entity;

public:
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
};