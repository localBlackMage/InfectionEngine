#include "Stdafx.h"

GameObjectManager::GameObjectManager() : m_GUID(0)
{
}

GameObjectManager::~GameObjectManager()
{
	for (int i = 0; i < mp_GameObjects.size(); ++i) {
		if (mp_GameObjects[i])
			delete mp_GameObjects[i];
	}
}

GameObject * GameObjectManager::SpawnGameObject()
{
	GameObject* newGO = new GameObject(m_GUID++);
	mp_GameObjects.push_back(newGO);
	return newGO;
}

void GameObjectManager::RegisterCamera(GameObject * cameraGO)
{
	mp_Cameras.push_back(cameraGO);
	Sorting::InsertionSort(mp_Cameras, &CameraComponent::LeftDepthGreaterThanRight);
}

void GameObjectManager::RegisterLight(GameObject * lightGO)
{
	mp_Lights.push_back(lightGO);
}

void GameObjectManager::UnregisterLight(GameObject * lightGO)
{
	mp_Lights.erase(
		std::remove(mp_Lights.begin(), mp_Lights.end(), lightGO),
		mp_Lights.end()
	);
}

void GameObjectManager::RenderCameras()
{
	for (int camIdx = 0; camIdx < mp_Cameras.size(); ++camIdx) {
		for (int objIdx = 0; objIdx < mp_GameObjects.size(); ++objIdx) {
			if (mp_GameObjects[objIdx]->IsActive() && mp_Cameras[camIdx] != mp_GameObjects[objIdx])
				INFECT_RENDERER.RenderObject((*mp_Cameras[camIdx]), (*mp_GameObjects[objIdx]));
		}
	}
}

void GameObjectManager::Update(float deltaTime)
{
	for (int objIdx = 0; objIdx < mp_GameObjects.size(); ++objIdx) {
		if (mp_GameObjects[objIdx]->IsActive())
			mp_GameObjects[objIdx]->Update(deltaTime);
	}
}

void GameObjectManager::LateUpdate(float deltaTime)
{
	for (int objIdx = 0; objIdx < mp_GameObjects.size(); ++objIdx) {
		if (mp_GameObjects[objIdx]->IsActive())
			mp_GameObjects[objIdx]->LateUpdate(deltaTime);
	}
}

