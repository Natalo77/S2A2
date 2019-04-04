//=============================================================
//				  Filename: CollisionClass.cpp
//=============================================================


//=============================================================
//					  User defined headers.
//=============================================================
#include "CollisionClass.h"
#include "ProjectileObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		CollisionClass::CollisionClass

Summary:	Default constructor for a Collision Class.

Modifies:	[m_D3D, m_Camera].

Returns:	CollisionClass
				A new CollisionClass Object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
CollisionClass::CollisionClass()
{
	//Set up the initial pointer state.
	m_D3D = 0;

	//Set up the initial camera pointer state.
	m_Camera = 0;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		CollisionClass

Summary:	A reference constructor for a CollisionClass Object.

Args:		const CollisionClass& other
				a reference to a CollisionClass object to create this
				object from.

Returns:	CollisionClass
				A CollisionClass object with the data stored at PARAM other.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
CollisionClass::CollisionClass(const CollisionClass & other)
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~CollisionClass

Summary:	A deconstructor for a CollisionClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
CollisionClass::~CollisionClass()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:     Initialize

Summary:	Initializes the data and member variables of a CollisionClass
			object.

Modifies:	[m_D3D, m_Camera].

Returns:	bool
				Represents whether or not the Initialization of this
				CollisionClass object and its member variables was
				successful or not.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool CollisionClass::Initialize(D3DClass* d3d, CameraClass* cam)
{
	//Store the reference to the existing D3DClass object.
	m_D3D = d3d;

	//Store the reference to the user camera.
	m_Camera = cam;

	//All initializations passed.
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Shutdown

Summary:	Cleans up all dynamic memory used and closes off all
				member variables of this CollisionClass Object.

Modifies:	[m_D3D, m_Camera].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void CollisionClass::Shutdown()
{
	//De-point the pointer.
	m_D3D = 0;

	//De point the camera class pointer.
	m_Camera = 0;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		TestRaySphereIntersect

Summary:	Begins the process of testing whether or not a ray at mousex
				and mouseY intersects with a sphere.

Args:		int mouseX
				the x co-ordinate on the screen that the ray is shot into.
			int mouseY
				the y co-ordinate on the screen that the ray is shot into.

Returns:	bool
				represents whether or not the ray collided with the specific 
				collision method used and the ray shot.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool CollisionClass::TestRaySphereIntersect(int mouseX, int mouseY)
{
	XMFLOAT3 rayOrigin, rayDirection;
	bool intersect;

	//Get the ray data with the mouse co-ordinates.
	GetRay(rayOrigin, rayDirection, mouseX, mouseY);

	// Now perform the ray-sphere intersection test.
	intersect = RaySphereIntersect(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), 1.0f);

	//return this result.
	return intersect;
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		TestRaySphereIntersect

Summary:	Begins the process of testing whether or not a ray at mousex
				and mouseY intersects with the specified BoundingBox

Args:		int mouseX
				the x co-ordinate on the screen that the ray is shot into.
			int mouseY
				the y co-ordinate on the screen that the ray is shot into.
			BoundingBox* AABB
				the Bounding box to check for collision against.

Returns:	bool
				represents whether or not the ray collided with the bounding box
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool CollisionClass::TestRayAABBIntersect(int mouseX, int mouseY, BoundingBox* AABB)
{
	XMFLOAT3 rayOrigin, rayDirection;
	bool intersect;

	//Get the ray data with the mouse co-ordinates.
	GetRay(rayOrigin, rayDirection, mouseX, mouseY);

	//Translate the AABB.
	//AABB->Center.x = -5.0f;
	//AABB->Center.y = 1.0f;
	//AABB->Center.z = 5.0f;

	//Perform the ray AABB intersection test.
	intersect = rayAABBIntersect(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), AABB);

	return intersect;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		CollisionTestLoop

Summary:	Tests for a ray collision intersection by a ray at mouseX
			and mouseY with all objects concerned by the GameObjectManager

Args:		int mouseX
				the x co-ordinate on the screen that the ray is shot into.
			int mouseY
				the y co-ordinate on the screen that the ray is shot into.
			GameObjectManager* objManager.
				a pointer to a gameObjectManager containing all the objects
				for testing ray collision against.
			FXMVECTOR FXMcamPosition
				an XMVECTOR storing the current world space position of
				the camera.

Returns:	GameObject*
				a pointer to the closest GameObject collided with.
				nullptr if nothing was collided with.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObject * CollisionClass::CollisionTestLoop(int mouseX, int mouseY, GameObjectManager * objManager, FXMVECTOR FXMcamPosition)
{
	//Calculate and store the ray information from the mouseX and mouseY
	XMFLOAT3 rayOrigin, rayDirection;
	GetRay(rayOrigin, rayDirection, mouseX, mouseY);

	//Create a temporary list of objects collided with.
	std::vector<GameObject*>* collidedList = new std::vector<GameObject*>();

	//For all lists in the GameObjectManager - Check for a collision with the AABB, if so - push it onto the collidedlist vector.
	for (std::vector<GameObject*>::iterator iter = objManager->GetList(GameObjectManager::OBJECTTYPE_DYNAMIC)->begin();
		iter != objManager->GetList(GameObjectManager::OBJECTTYPE_DYNAMIC)->end();
		iter++)
	{
		GameObject* obj = *iter;
		if (rayAABBIntersect(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), obj->GetAABB()))
			collidedList->push_back(obj);
	}
	for (std::vector<GameObject*>::iterator iter = objManager->GetList(GameObjectManager::OBJECTTYPE_STATIC)->begin();
		iter != objManager->GetList(GameObjectManager::OBJECTTYPE_STATIC)->end();
		iter++)
	{
		GameObject* obj = *iter;
		if (rayAABBIntersect(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), obj->GetAABB()))
			collidedList->push_back(obj);
	}
	for (std::vector<ProjectileObject*>::iterator iter = objManager->GetProjectileList()->begin();
		iter != objManager->GetProjectileList()->end();
		iter++)
	{
		GameObject* obj = *iter;
		if (rayAABBIntersect(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), obj->GetAABB()))
			collidedList->push_back(obj);
	}


	//If the list is not 0 length
	if (collidedList->size() > 0)
	{
		//Sort the collided list by distance from the camera.
		Sort(collidedList, FXMcamPosition);

		//Return the first 
		return collidedList->at(0);
	}

	//If this point is reached then nothing was collided with.
	return nullptr;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetRay

Summary:	A static function to get and return a vector for a ray
			from the position of cam in the direction of mousex,mouseY.

Args:		D3DClass* d3d
				a pointer to the D3DClass being used.
			CameraClass* cam
				a pointer to the Camera currently being used.
			XMFLOAT3 &directionOut
				a reference to an XMFLOAT3 to store the direction vector in.
			int mouseX, mouseY
				the position of the mouse on the screen.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void CollisionClass::GetRay(D3DClass* d3d, CameraClass* cam, XMFLOAT3 & directionOut, int mouseX, int mouseY)
{
	float pointX, pointY;
	XMMATRIX mprojectionMatrix, mviewMatrix, minverseViewMatrix, mworldMatrix, minverseWorldMatrix;
	XMFLOAT4X4 projectionMatrix, inverseViewMatrix;
	XMFLOAT3 direction;

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mouseX) / (float)d3d->m_screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)d3d->m_screenHeight) - 1.0f) * -1.0f;

	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	d3d->GetProjectionMatrix(mprojectionMatrix);
	XMStoreFloat4x4(&projectionMatrix, mprojectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// Get the inverse of the view matrix.
	cam->GetViewMatrix(mviewMatrix);
	XMVECTOR viewDeterminant = XMMatrixDeterminant(mviewMatrix);
	minverseViewMatrix = XMMatrixInverse(&viewDeterminant, mviewMatrix);

	XMStoreFloat4x4(&inverseViewMatrix, minverseViewMatrix);

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	// Get the world matrix
	d3d->GetWorldMatrix(mworldMatrix);

	// Now get the inverse of the translated world matrix.
	XMVECTOR worldDeterminant = XMMatrixDeterminant(mworldMatrix);
	minverseWorldMatrix = XMMatrixInverse(&worldDeterminant, mworldMatrix);
	XMStoreFloat3(&directionOut, XMVector3TransformNormal(XMLoadFloat3(&direction), minverseWorldMatrix));

	// Normalize the ray direction.
	XMStoreFloat3(&directionOut, XMVector3Normalize(XMLoadFloat3(&directionOut)));

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Intersects

Summary:	A static method to interface with DirectXCollision.h to test
			if a and b intersect.

Args:		BoundingBox* a, b
				pointers to the two bounding boxes to check intersection of.

Returns:	bool
				Whether or not the two bounding volumes intersect.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool CollisionClass::Intersects(BoundingBox * a, BoundingBox * b)
{
	return a->Intersects(*b);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		RaySphereIntersect

Summary:	Tests whether the ray defined by vrayOrigin and vrayDirection intersects
			a sphere described by its radius.

Args:		FXMVECTOR vrayOrigin.
				an XMVECTOR/XMFLOAT3 describing the origin point of the raycast.
			FXMVECTOR vrayDirection.
				an XMVECTOR/XMFLOAT3 describing the direction vector of the raycast.
			float radius.
				describing the radius of the sphere to test the intersection against.

Returns:	bool
				Whether or not the raycast intersected the sphere.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool CollisionClass::RaySphereIntersect(FXMVECTOR vrayOrigin, FXMVECTOR vrayDirection, float radius)
{
	float a, b, c, discriminant;

	XMFLOAT3 rayOrigin, rayDirection;
	XMStoreFloat3(&rayOrigin, vrayOrigin);
	XMStoreFloat3(&rayDirection, vrayDirection);

	// Calculate the a, b, and c coefficients.
	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		rayAABBIntersect

Summary:	Tests whether a ray described by rayOrigin and rayDirection intersects
			the boundingBox AABB

Args:		FXMVECTOR rayOrigin
				an XMVECTOR/XMFLOAT3 describing the origin point of the raycast.
			FXMVECTOR rayDirection
				an XMVECTOR/XMFLOAT3 describing the direction vector of the raycast.
			BoundingBox AABB
				a DirectXCollision::BoundingBox describing the bounding box to check
				intersection against.

Returns:	bool
				whether the raycast(rayOrigin, rayDirection) intersects AABB.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool CollisionClass::rayAABBIntersect(FXMVECTOR rayOrigin, FXMVECTOR rayDirection, BoundingBox* AABB)
{
	/*
	//The two intersection points on the AABB
	XMFLOAT3 intersections[2];

	//XMFLOAT 3 representations of the passed in XMVECTORS.
	XMFLOAT3 f_rayOrigin, f_rayDirection;
	{
		XMStoreFloat3(&f_rayOrigin, rayOrigin);
		XMStoreFloat3(&f_rayDirection, rayDirection);
	}

	//XMFLOAT3 representations constructed from the center and extents of the BoundingBox.
	XMFLOAT3 min, max;
	{
		min.x = AABB.Center.x - AABB.Extents.x;
		min.y = AABB.Center.y - AABB.Extents.y;
		min.z = AABB.Center.z - AABB.Extents.z;
		max.x = AABB.Center.x + AABB.Extents.x;
		max.y = AABB.Center.y + AABB.Extents.y;
		max.z = AABB.Center.z + AABB.Extents.z;
	}

	//Calculations of the intersection points.
	{
		intersections[0].x = (min.x - f_rayOrigin.x) / f_rayDirection.x;
		intersections[0].y = (min.y - f_rayOrigin.y) / f_rayDirection.y;
		intersections[0].z = (min.z - f_rayOrigin.z) / f_rayDirection.z;

		intersections[1].x = (max.x - f_rayOrigin.x) / f_rayDirection.x;
		intersections[1].y = (max.y - f_rayOrigin.y) / f_rayDirection.y;
		intersections[1].z = (max.z - f_rayOrigin.z) / f_rayDirection.z;
	}

	//logic depends on intersections[0] < intersections[1].
	if (intersections[0].x > intersections[1].x)
		Swap<float>(intersections[0].x, intersections[1].x);
	if (intersections[0].y > intersections[1].y)
		Swap<float>(intersections[0].y, intersections[1].y);
	if (intersections[0].z > intersections[1].z)
		Swap<float>(intersections[0].z, intersections[1].z);

	//Test for missing the 2d box
	if (intersections[0].x > intersections[1].y || intersections[0].y > intersections[1].x)
		return false;

	//Test for missing the cube.
	if (intersections[0].x > intersections[1].z || intersections[0].z > intersections[1].x)
		return false;*/

	float* infinity = new float;
	*infinity = (float)INFINITY;
	return AABB->Intersects(rayOrigin, rayDirection, *infinity);

	//If this point is reached then the intersection must be true.
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		getRay

Summary:	Calculates a 'into-screen' ray at mouseX,mouseY and stores
			the results in originOut and directionOut.

Args:		XMFLOAT3 &originOut
				a reference to an XMFLOAT3 object to store the rayOrigin data in
			XMFLOAT3 &directionOut
				a reference to an XMFLOAT3 object to store the rayDirection data in.
			int mouseX
				the x co-ordinate on the screen that the ray is shot into.
			int mouseY
				the y co-ordinate on the screen that the ray is shot into.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void CollisionClass::GetRay(XMFLOAT3 &originOut, XMFLOAT3 &directionOut, int mouseX, int mouseY)
{
	float pointX, pointY;
	XMMATRIX mprojectionMatrix, mviewMatrix, minverseViewMatrix, mworldMatrix, mtranslateMatrix, minverseWorldMatrix;
	XMFLOAT4X4 projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	XMFLOAT3 direction, origin;

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mouseX) / (float)m_D3D->m_screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)m_D3D->m_screenHeight) - 1.0f) * -1.0f;

	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	m_D3D->GetProjectionMatrix(mprojectionMatrix);
	XMStoreFloat4x4(&projectionMatrix, mprojectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// Get the inverse of the view matrix.
	m_Camera->GetViewMatrix(mviewMatrix);
	XMVECTOR viewDeterminant = XMMatrixDeterminant(mviewMatrix);
	minverseViewMatrix = XMMatrixInverse(&viewDeterminant, mviewMatrix);

	XMStoreFloat4x4(&inverseViewMatrix, minverseViewMatrix);

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	// Get the origin of the picking ray which is the position of the camera.
	origin = m_Camera->GetPosition();

	// Get the world matrix and translate to the location of the sphere.
	m_D3D->GetWorldMatrix(mworldMatrix);

	// Now get the inverse of the translated world matrix.
	XMVECTOR worldDeterminant = XMMatrixDeterminant(mworldMatrix);
	minverseWorldMatrix = XMMatrixInverse(&worldDeterminant, mworldMatrix);
	XMStoreFloat3(&originOut, XMVector3TransformCoord(XMLoadFloat3(&origin), minverseWorldMatrix));
	XMStoreFloat3(&directionOut, XMVector3TransformNormal(XMLoadFloat3(&direction), minverseWorldMatrix));

	// Normalize the ray direction.
	XMStoreFloat3(&directionOut, XMVector3Normalize(XMLoadFloat3(&directionOut)));

	return;
}



