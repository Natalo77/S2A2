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


//===============================================
//				Forward declarations.
//===============================================
class ProjectileObject;


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

			void AddProjectile(GameObject* projectile, XMFLOAT3* position, XMFLOAT3* rotation)
				Use to add a projectile into consideration by the GameObjectManager
				at the specified position and rotation(radians).

			GameObject* SearchFor
				Use to check a specified gameObject exists within the GameObjectManager.

			void RenderAll(...)
				Use to render all the objects within the scope of the GameObjectManager.
			
			std::vector<GameObject*>* GetList
				Use to return the appropriate list according to the object type passed in.

			==================== PRIVATE ====================
			GameObject* Search
				Use to search for the specified gameObject in the specified List.

Members:	==================== PRIVATE ====================
			vector<GameObject*>* m_StaticList
				A list of all the static gameObjects being handled by the GameObjectManager.
			vector<GameObject*>* m_DynamicList
				A list of all the dynamic GameObjects being handled by the GameObjectManager.

			vector<GameObject*>* m_BulletList
				A list of all the projectiles currently in the scene.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class GameObjectManager
{
public:
	enum ObjectType{OBJECTTYPE_STATIC, OBJECTTYPE_DYNAMIC, OBJECTTYPE_PROJECTILE};

public:
	GameObjectManager();
	~GameObjectManager();

	void Shutdown();

	void AddItem(ObjectType objectType, GameObject* object);
	void AddItem(ObjectType objectType, GameObject* object, XMFLOAT3* transform, XMFLOAT3* rotation, XMFLOAT3* scaling);

	void AddProjectile(ProjectileObject* projectile, XMFLOAT3* position, XMFLOAT3* rotation);

	GameObject* SearchFor(ObjectType objectType, GameObject* object);

	bool RenderAll(ShaderManagerClass* shaderManager, D3DClass* d3d, CameraClass* cam, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, float deltaTime);

	std::vector<GameObject*>* GetList(ObjectType listType);
	vector<ProjectileObject*>* GetProjectileList();

private:
	GameObject* Search(std::vector<GameObject*>* list, GameObject* object);

private:
	std::vector<GameObject*>* m_StaticList;
	std::vector<GameObject*>* m_DynamicList;

	vector<ProjectileObject*>* m_BulletList;
};

