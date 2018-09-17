/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Hyoyup Chung
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

static void PrintLinkedList(MemoryBlock* node) {
	if (!node) {
		printf("\n\nFINISHED\n\n");
		return;
	}

	printf("MEMORY BLOCK :: node: %p :: pData: %p :: Size: %d :: FreeSize: %d :: Next: %p :: Prev: %p\n", node, node->pData, node->size, node->freesize, node->next, node->prev);
	PrintLinkedList(node->next);
}

static void PrintPointer(const char* msg, void* ptr) {
	printf("%s: %p\n", msg, ptr);
}

static void PrintMemoryBlock(MemoryBlock* block) {
	PrintPointer("Next MB", block->next);
	if (block->next)
		PrintPointer("Next->Prev MB", block->next->prev);
	PrintPointer("Prev MB", block->prev);
	printf("Block Size: %d : Block Freesize: %d\n", block->size, block->freesize);
}

MemoryManager::MemoryManager():
	m_TotalBufferSize(DEFAULT_BUFFER_SIZE_BYTE), m_pHead(nullptr) {

	m_Buffer = malloc(m_TotalBufferSize);
	m_pHead = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	m_pHead->Initialize(m_Buffer, 0, m_TotalBufferSize);
	m_NumCachedBlock = 0;
	for (unsigned i = 0; i < MAX_CACHE_SIZE_NUM; i++) {
		//TODO: Add GameObject Ctr w.o. ID
		m_GameObjectCache[i] = new GameObject();
	}
}

MemoryManager::~MemoryManager() {
	if (m_pHead){
		m_pHead->CleanUp();
		free(m_pHead);
		m_pHead = nullptr;
	}
	free(m_Buffer);
}

void* MemoryManager::Alloc(std::size_t size) {
	MemoryBlock *current = m_pHead;
	// find next available slot 
	while (current && (current->freesize < size)) {
		current = current->next;
	}

	// request failed
	if (!current) {
		printf("MemoryManager::Alloc() FAILED - No Available Slot\n");
		return nullptr;
	}
	// create a new memoryblock and insert into the list
	MemoryBlock* temp = current->next;
	current->next = NewMemoryBlock();
	current->next->Initialize(	static_cast<unsigned char*>(current->pData) + current->size, 
								size, 
								current->freesize - size );

	if (temp) 
		temp->prev = current->next;
	current->next->next = temp;
	current->freesize = 0;
	current->next->prev = current;

	return current->next->pData;
}

void MemoryManager::Free(void* ptr){
	MemoryBlock *current = m_pHead->next;
	if (!current || (current->pData == nullptr)) {
		printf("MEMMNGR::FREE() NEVER REACH HERE!\n");
		return;
	}

	// TODO: remove while-loop to O(1) of block node find
	//		 use unordered_map<?,MemoryBlock*>
	//						  <?,std::shared_ptr<MemoryBlock> >
	// ? = reinterpret_case<unsigned long>
	while(current && (current->pData != ptr)){
		current = current->next;
	}

	if (!current) {
		//printf("MEMMNGR::FREE() NEVER REACH HERE --2 !!!\n");
		return;
	}
	
	// remove links
	if (current->next)
		current->next->prev = current->prev;
	if (current->prev) {
		current->prev->next = current->next;
		current->prev->freesize += current->size + current->freesize;
	}
	current->freesize = 0;

	current->next = nullptr;
	current->prev = nullptr;

	// try caching current block. If not, free.
	Recycle(current);
}

 MemoryBlock* MemoryManager::NewMemoryBlock(){
	if (m_NumCachedBlock == 0) { // no cached memoryblock to be reused
		return (MemoryBlock*)malloc(sizeof(MemoryBlock));
	}
	else {									// use previously recycled memory block
		return m_Cache[--m_NumCachedBlock]; // NOTE: cacheIndex = m_NumCachedBlock-1
	}
}

 void MemoryManager::Recycle(MemoryBlock* pNode){
	if (m_NumCachedBlock == MAX_CACHE_SIZE_NUM-1){
		//printf("CACHE IS FULL -- MAX_CACHE_SIZE_NUM: %d\n", MAX_CACHE_SIZE_NUM);
		free(pNode);
		return;
	}
	m_Cache[m_NumCachedBlock++] = pNode;
}

void* MemoryManager::m_Buffer = nullptr;


// GAMEOBJECT RECYCLE 
GameObject* MemoryManager::GetNewGameObject() {
	//return new GameObject(id);
	for (unsigned i = 0; i<MAX_CACHE_SIZE_NUM; i++) {
		if (!m_GameObjectCache[i]->IsActive()) {
			GameObject* pGO = m_GameObjectCache[i];
			pGO->SetID(INFECT_GUID.GetGUID());
			pGO->SetActive(true);
			return pGO; // this pGO needs to be reset with appropriate data
						// only GUID is valid
		}
	}
	
	printf("NOT ENOUGH CACHE SIZE - Current cache size: %d\n", MAX_CACHE_SIZE_NUM);
}

// GAMEOBJECT CACHING 
void MemoryManager::DeleteGameObject(GameObject* ptr) {
	//delete ptr; 
	ptr->Deactivate();
	ptr->SetActive(false);
}

Component* MemoryManager::GetNewComponent(std::string type) {
	return nullptr;

	//std::string typeName = "C_" + type;
	//if (m_ComponentCache[typeName].empty()) {
	//	return nullptr;
	//}
	//else { // get empty component from cache
	//	Component* emptyComp = m_ComponentCache[typeName].back();
	//	m_ComponentCache[typeName].pop_back();
	//	return emptyComp;
	//}
}
void MemoryManager::DeleteComponent(Component* ptr) {
	delete ptr;

	//std::string typeString = ComponentTypeText[ptr->Type()];
	//if (m_ComponentCache[typeString].size() < MAX_GAMEOBJECT_CACHE) {
	//	ptr->Deactivate();
	//	m_ComponentCache[typeString].push_back(ptr);
	//}
	//else {
	//	delete ptr;
	//}
}