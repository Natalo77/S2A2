//===============================================
// Filename: GameObjectManager.cpp
//===============================================


//===============================================
//			   User Defined Headers.
//===============================================
#include "GameObjectManager.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GameObjectManager()

Summary:	The default constructor for a gameObjectManager object.

Modifies:	[staticList, dynamicList].

Returns:	GameObjectManager
				the newly created GameObjectManager object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObjectManager::GameObjectManager()
{
	staticList = new std::vector<GameObject*>();
	dynamicList = new std::vector<GameObject*>();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~GameObjectManager

Summary:	The default deconstructor for a GameObjectManager object.

Modifies:	[staticList, dynamicList].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObjectManager::~GameObjectManager()
{
	delete staticList;
	delete dynamicList;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		AddItem

Summary:	Use to add the specified object as the specifiedType of object
			into consideration by the GameObjectManager.

Args:		ObjectType objectType
				the type of object this is.
				see GameObjectManager.h (ObjectType)
			GameObject* object
				a pointer to the GameObject object to be added.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObjectManager::AddItem(ObjectType objectType, GameObject* object)
{
	(GetList(objectType))->push_back(object);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		AddItem

Summary:	Use to add the specified object as the specifiedType of object
			into consideration by the GameObjectManager.
			Initialize the gameObject to have the specified transform,
			rotation and scaling data.

Args:		ObjectType objectType
				the type of object this is.
				see GameObjectManager.h (ObjectType)
			GameObject* object
				a pointer to the GameObject object to be added.
			XMFLOAT3* transform, rotation, scaling
				XMFLOAT3s representing the transformation, rotation
				and scaling of the GameObject, respectively.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObjectManager::AddItem(ObjectType objectType, GameObject* object, XMFLOAT3* transform, XMFLOAT3* rotation, XMFLOAT3* scaling)
{
	object->setTransform(transform->x, transform->y, transform->z);
	object->setRotation(rotation->x, rotation->y, rotation->z);
	object->setScale(scaling->x, scaling->y, scaling->z);
	AddItem(objectType, object);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SearchFor

Summary:	Use to check that the specified gameObject of specified type
			exists within the scope of the GameObjectManager

Args:		ObjectType objectType
				the type of object this is.
				see GameObjectManager.h (ObjectType)
			GameObject* object
				A pointer to the GameObject object to be added.

Modifies:	[none].

Returns:	GameObject*
				a pointer to the GameObject.
				if it does not exist within the system, nullptr will be returned.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObject* GameObjectManager::SearchFor(ObjectType objectType, GameObject* object)
{
	GameObject* a = Search(GetList(objectType), object);
	return a;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Render

Summary:	Use to render all objects within the scope of the GameObjectManager.

Args:		ShaderManagerClass* shaderManager
				a pointer to the ShaderManagerClass object that is being
				used to render the models.
			D3DClass* d3d
				a pointer to the d3d class containing the device context.
			XMMATRIX &viewMatrix
				a reference to an XMMATRIX representing the view of the
				current user.
			XMMATRIX &projectionMatrix
				a reference to an XMMATRIX representing the projection
				of the current camera.

Modifies:	[none].

Returns:	bool	
				was the rendering of every object successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GameObjectManager::Render(ShaderManagerClass* shaderManager, D3DClass* d3d, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix)
{
	XMMATRIX worldMatrix;
	bool result = true;

	if (staticList->size() != 0)
	{
		for (std::vector<GameObject*>::iterator iter = staticList->begin();
			iter != staticList->end();
			iter++)
		{
			GameObject* a = *iter;
			d3d->GetWorldMatrix(worldMatrix);
			result = a->Render(shaderManager, d3d->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
			if (!result)
				return false;
		}
	}

	if (dynamicList->size() != 0)
	{
		for (std::vector<GameObject*>::iterator iter = dynamicList->begin();
			iter != dynamicList->end();
			iter++)
		{
			GameObject* a = *iter;
			d3d->GetWorldMatrix(worldMatrix);
			result = a->Render(shaderManager, d3d->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
			if (!result)
				return false;
		}
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetList

Summary:	Use to return a pointer to the appropriate list according
			to the objectType passed in.

Args:		ObjectType objectType
				the type of object this is.
				see GameObjectManager.h (ObjectType)

Modifies:	[none].

Returns:	vector<GameObject*>*
				a Pointer to the list managing all objects of the
				ObjectType specified.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
std::vector<GameObject*>* GameObjectManager::GetList(ObjectType listType)
{
	if (listType == ObjectType::OBJECTTYPE_DYNAMIC)
	{
		return dynamicList;
	}
	else if (listType == ObjectType::OBJECTTYPE_STATIC)
	{
		return staticList;
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Search

Summary:	Use to search a specified List for the specified GameObject.

Args:		vector<GameObject*>* list
				a pointer to the GameObject* vector to be searched
			GameObject* object
				a pointer to the GameObject to be searched for.

Modifies:	[none].

Returns:	GameObject*
				a pointer to the gameObject when found.
				nullptr will be returned if the gameObject was not found.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObject* GameObjectManager::Search(std::vector<GameObject*>* list, GameObject* object)
{
	for (std::vector<GameObject*>::iterator iter = list->begin();
		iter != list->end();
		iter++)
	{
		GameObject* comp = *iter;
		if (comp == object)
			return *iter;
	}

	return nullptr;
}
