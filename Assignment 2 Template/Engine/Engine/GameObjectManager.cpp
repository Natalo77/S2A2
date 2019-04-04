//===============================================
// Filename: GameObjectManager.cpp
//===============================================


//===============================================
//					Constants
//===============================================
const int MAX_PROJECTILE_DISTANCE_FROM_00 = 1000;

//===============================================
//			   User Defined Headers.
//===============================================
#include "GameObjectManager.h"
#include "cameraclass.h"
#include "ProjectileObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GameObjectManager()

Summary:	The default constructor for a gameObjectManager object.

Modifies:	[m_StaticList, m_DynamicList, m_BulletList].

Returns:	GameObjectManager
				the newly created GameObjectManager object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObjectManager::GameObjectManager()
{
	m_StaticList = new std::vector<GameObject*>();
	m_DynamicList = new std::vector<GameObject*>();
	m_BulletList = new vector<ProjectileObject*>();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~GameObjectManager

Summary:	The default deconstructor for a GameObjectManager object.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObjectManager::~GameObjectManager()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Shutdown()

Summary:	Call before deletion to ensure memory is freed.

Modifies:	[m_StaticList, m_DynamicList, m_BulletList].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObjectManager::Shutdown()
{
	delete m_StaticList;
	delete m_DynamicList;
	delete m_BulletList;
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

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObjectManager::AddItem(ObjectType objectType, GameObject* object, XMFLOAT3* transform, XMFLOAT3* rotation, XMFLOAT3* scaling)
{
	object->setTransform(transform->x, transform->y, transform->z);
	object->setRotation(rotation->x, rotation->y, rotation->z);
	object->setScale(scaling->x, scaling->y, scaling->z);
	AddItem(objectType, object);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		AddProjectile

Summary:	Use to add a projectile into consideration by the GameObjectManager.
			Gives the projectile basic sizing and location data.

Args:		GameObject* projectile
				a pointer to the gameObject to add.
			XMFLOAT3* position
				the x y z in world space to spawn the gameObject at.
			XMFLOAT3* rotation
				the x y z rotation in local space in radians to spawn
				the gameObject at.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObjectManager::AddProjectile(ProjectileObject * projectile, XMFLOAT3* position, XMFLOAT3* rotation)
{
	//Set initial properties of the projectile.
	projectile->setTransform(position->x, position->y, position->z);
	projectile->setScale(0.5f, 0.5f, 0.5f);
	projectile->setRotation(rotation->x / 0.0174532925f, rotation->y / 0.0174532925f, rotation->z / 0.0174532925f);

	//Push the reference to the projectile back onto the projectile list.
	m_BulletList->push_back(projectile);
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
			Renders them with their AABBs right now as well.

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
			float deltaTime
				the time that has passed since the last frame.

Modifies:	[none].

Returns:	bool	
				was the rendering of every object successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GameObjectManager::RenderAll(ShaderManagerClass* shaderManager, D3DClass* d3d, CameraClass* cam, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, float deltaTime)
{
	//Temporary storage for the worldMatrix.
	XMMATRIX worldMatrix;
	bool result = true;

	//If there is at least one element in the m_StaticList.
	if (m_StaticList->size() != 0)
	{
		//Iterate through the list.
		for (std::vector<GameObject*>::iterator iter = m_StaticList->begin();
			iter != m_StaticList->end();
			iter++)
		{
			//Dereference the iterator.
			GameObject* a = *iter;

			//Obtain the worldMatrix from the D3Dclass.
			d3d->GetWorldMatrix(worldMatrix);

			//Render the model.
			result = a->Render(shaderManager, d3d->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
			if (!result)
				return false;

			//Render the model's AABB.
			a->RenderAABB(shaderManager, d3d, cam);
		}
	}

	//If the m_DynamicList has at least one item,
	if (m_DynamicList->size() != 0)
	{
		//Iterate through the list,
		for (std::vector<GameObject*>::iterator iter = m_DynamicList->begin();
			iter != m_DynamicList->end();
			iter++)
		{
			//Dereference the iterator.
			GameObject* a = *iter;

			//Get the worldMatrix using the D3D class.
			d3d->GetWorldMatrix(worldMatrix);

			//Render the model
			result = a->Render(shaderManager, d3d->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
			if (!result)
				return false;

			//Render the model's AABB.
			a->RenderAABB(shaderManager, d3d, cam);
		}
	}

	//If the bullet list has at least one item.
	if (m_BulletList->size() != 0)
	{
		//Cull the projectileList for any projectiles too far away.
		CullProjectiles();

		//Iterate through the list.
		for (vector<ProjectileObject*>::iterator iter = m_BulletList->begin();
			iter != m_BulletList->end();
			iter++)
		{
			//Dereference the iterator.
			ProjectileObject* a = *iter;

			//Advance the projectileObjects.
			a->Frame();

			//Get the world matrix using the d3d class.
			d3d->GetWorldMatrix(worldMatrix);

			//render the model.
			result = a->Render(shaderManager, d3d->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
			if (!result)
				return false;

			//render the model's aabb
			a->RenderAABB(shaderManager, d3d, cam);
		}
	}

	return true;
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
		return m_DynamicList;
	else if (listType == ObjectType::OBJECTTYPE_STATIC)
		return m_StaticList;
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetProjectileList

Summary:	Use to return a pointer to the List of projectiles currently
			being considered by the gameObjectManager.

Modifies:	[none].

Returns:	vector<ProjectileObject*>*
				a Pointer to the list managing all Projectile objects
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
vector<ProjectileObject*>* GameObjectManager::GetProjectileList()
{
	return m_BulletList;
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
	//iterate through the given list.
	for (std::vector<GameObject*>::iterator iter = list->begin();
		iter != list->end();
		iter++)
	{
		//Dereference the iterator.
		GameObject* comp = *iter;

		//If the search and current are equal, return the iterator(dereferenced).
		if (comp == object)
			return *iter;
	}
	
	//If this point is reached then the search-for element was not found.
	return nullptr;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		CullProjectiles.

Summary:	Uses the MAX_PROJECTILE_DISTANCE_FROM_00 to look through the
			bulletList and destroy any particles deemed to be over the
			max distance away.

Modifies:	[m_BulletList].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObjectManager::CullProjectiles()
{
	//Storage for the indices that need to be removed from the list.
	vector<int> indices;

	//i = the current index.
	int i = 0;

	//Iterate over the projectile list.
	for (vector<ProjectileObject*>::iterator iter = m_BulletList->begin();
		iter != m_BulletList->end();
		iter++)
	{
		//Dereference the iterator.
		ProjectileObject* a = *iter;

		//Create an XMVECTOR to store the position of the object.
		XMVECTOR position;
		position = XMVectorSet(a->GetPosition()->x, a->GetPosition()->y, a->GetPosition()->z, 1.0f);

		//Use the position vector to store the length of the vector est.
		position = XMVector3LengthEst(position);

		//If the length by estimate is greater than the maximum length from the origin.
		if (XMVectorGetByIndex(position, 0) > MAX_PROJECTILE_DISTANCE_FROM_00)
		{
			//Push the index number onto the indices vector,
			indices.push_back(i);
		}

		//Increment the index being looked at.
		i++;
	}

	//For each element in the indexes to remove.
	for (int x = 0; x < indices.size(); x++)
	{
		//Erase the entry at the specified index, adjusted for the number of entries removed already.
		//. start of list = begin()
		//. increment by the index number.
		//. x naturally equals no. entries removed already.
		m_BulletList->erase(m_BulletList->begin() + indices.at(x) - x);
	}
}
