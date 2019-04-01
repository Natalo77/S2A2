#pragma once
//===============================================
//		  Filename: GameObjectManager.h
//===============================================


//===============================================
//			 User Defined Headers.
//===============================================
#include "GameObject.h"
#include "shadermanagerclass.h"
#include "d3dclass.h"


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		GameObjectManager

Summary:	A class to provide various functions for storing, managing
			and accessing both static and dynamic gameobjects at run time.

Enums:		==================== PUBLIC ====================
			ObjectType {OBJECTTYPE_STATIC, ..._DYNAMIC}
				an enum to clarify the type of object being dealt with.

Methods:	==================== PUBLIC ====================
			GameObjectManager
				Default constructor
			~GameObjectManager
				Default deconstructor

			void AddItem
				Use to add an item of the specified type to the GameObjectManager.
			void AddItem(+...)
				An overload of AddItem to allow GameObjects to be added to the GameObjectManager
				with an initial transform, rotation and scaling.

			GameObject* SearchFor
				Use to check a specified gameObject exists within the GameObjectManager.

			void Render()
				Use to render all the objects within the scope of the GameObjectManager.

			==================== PRIVATE ====================
			std::vector<GameObject*>* GetList
				Use to return the appropriate list according to the object type passed in.

			GameObject* Search
				Use to search for the specified gameObject in the specified List.

Members:	==================== PRIVATE ====================
			vector<GameObject*>* staticList
				A list of all the static gameObjects being handled by the GameObjectManager.
			vector<GameObject*>* dynamicList
				A list of all the dynamic GameObjects being handled by the GameObjectManager.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class GameObjectManager
{
public:
	enum ObjectType{OBJECTTYPE_STATIC, OBJECTTYPE_DYNAMIC};

public:
	GameObjectManager();
	~GameObjectManager();

	void Shutdown();

	void AddItem(ObjectType objectType, GameObject* object);
	void AddItem(ObjectType objectType, GameObject* object, XMFLOAT3* transform, XMFLOAT3* rotation, XMFLOAT3* scaling);

	GameObject* SearchFor(ObjectType objectType, GameObject* object);

	bool RenderAll(ShaderManagerClass* shaderManager, D3DClass* d3d, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix);

private:
	std::vector<GameObject*>* GetList(ObjectType listType);

	GameObject* Search(std::vector<GameObject*>* list, GameObject* object);

private:
	std::vector<GameObject*>* staticList;
	std::vector<GameObject*>* dynamicList;
};

