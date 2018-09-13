/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component : 
	public Subscriber
{
protected:
	InfectGUID m_GUID;
	GameObject *mp_Parent;
	bool m_IsDirty;
public:
	static const ComponentType Type = ComponentType::NUM_COMPONENTS;

	Component(InfectGUID guid) : m_GUID(guid) {};
	virtual ~Component() {};
	virtual void LateInitialize() {};
	virtual void Update(float dt) = 0;
	virtual void LateUpdate(float dt) {};
	virtual void Serialize(const json& j) = 0;
	virtual void Deactivate() {};
	//virtual void Override(const json& j) {};
	virtual void Override() {};
	virtual void HandleEvent(Event* pEvent) {}

	virtual ComponentType GetType() const { return Type; }
	inline GameObject* Parent() { return mp_Parent; }
	inline void SetParent(GameObject* parent) { mp_Parent = parent; }
	inline InfectGUID ID() const { return m_GUID; }
	inline void SetID(InfectGUID guid) { m_GUID = guid; }
	inline bool IsDirty() const { return m_IsDirty; }
};

#endif
