/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void PointLightComponentManager::Update(float dt)
{
	for each (PLC plComp in m_Components) {
		plComp->m_UpdatedLastFrame = false;
		if (plComp->IsDirty()) {

			plComp->m_IsDirty = false;
			plComp->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void PointLightComponentManager::HandleEvent(Event * pEvent)
{
	for each (PLC plComp in m_Components) {
		plComp->HandleEvent(pEvent);
	}
}