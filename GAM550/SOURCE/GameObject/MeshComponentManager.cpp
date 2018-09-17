/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void MeshComponentManager::Update(float dt)
{
	for each (MC mComp in m_Components) {
		mComp->m_UpdatedLastFrame = false;
		if (mComp->IsDirty()) {

			mComp->m_IsDirty = false;
			mComp->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void MeshComponentManager::HandleEvent(Event * pEvent)
{
	for each (MC mComp in m_Components) {
		mComp->HandleEvent(pEvent);
	}
}