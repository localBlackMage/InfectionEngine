/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

class Behavior;

class BT_Scheduler {
public:
	BT_Scheduler() {}
	~BT_Scheduler() {}

	void ScheduleBehavior(Behavior* b) {
		m_Behaviors.push_back(b);
	}

	Behavior* NextBehavior() {
		Behavior* next = m_Behaviors.front();
		m_Behaviors.pop_front();
		return next;
	}

	bool Empty() {
		return m_Behaviors.empty();
	}

	void Clear() {
		m_Behaviors.clear();
	}
private:
	std::deque<Behavior*> m_Behaviors;
};

class BehaviorTree: public Subscriber{
public:
	BehaviorTree();
	~BehaviorTree();

	// push behavior to scheduler for the next tick()
	void Init(Behavior& b, BH_Observer* observer = NULL );
	// sets the status of behavior and fires the observer
	void StopBehavior(Behavior& b, BH_Status res);

	void Tick(); // main entry point for this BT
	bool Step(); // single stepping of behaviors

	void SetParent(AI_State* pState) { pAI_State = pState; }

	BT_Scheduler m_Scheduler; // MOVE THESE TO PROTECTED LATER
	//std::deque<Behavior*> m_behaviors;
	Behavior* m_pRootNode;
private:
	AI_State* pAI_State;
};

#endif