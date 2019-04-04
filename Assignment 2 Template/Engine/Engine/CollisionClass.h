#pragma once

//====================================================
//					 DirectX Headers.
//====================================================
#include <DirectXMath.h>
#include <DirectXCollision.h>

//====================================================
//					 Defined Headers.
//====================================================
#include "cameraclass.h"
#include "d3dclass.h"
#include "GameObjectManager.h"
#include "GameObject.h"

//====================================================
//					   Namespaces.
//====================================================
using namespace DirectX;

/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		CollisionClass

Summary:	A Class designed to provide various method for testing the
				intersection between geometries.

Methods:	==================== PUBLIC ====================
			CollisionClass();
				Default Constructor.
			CollisionClass(const CollisionClass&);
				Reference Constructor.
			~CollisionClass();
				Deconstructor.

			Initialize(D3DClass*, CameraClass*);
				Call after creating to setup Collision Class for use.
			Shutdown();
				Call when finished using to tear down the object.

			TestRaySphereIntersect(int mouseX, int mouseY);
				returns whether or not an intersection occured with the 
				test sphere with a ray specified by mouseX and mouseY
			TestRayAABBIntersect(int mouseX, int mouseY, BoundingBox* AABB)
				Returns whether or not an intersection occured with the
				specified bounding box at the ray specified by mouseX
				and mouseY

			CollisionTestLoop(int mouseX, int mouseY, GameObjectManager* objManager, FXMVECTOR FXMcamPosition)
				Use to test collision for a ray at mouseX, mouseY with every object concerned
				by the GameObjectManager using the FXMcamPosition as the current position of the camera.

			static void GetRay(XMFLOAT3 &directionOut, int mouseX, int mouseY)
				Use to get the direction of a ray from the camera at mouseX and mouseY on the screen.

			static bool Intersects(BoundingBox* a, BoundingBox* b)
				Use to test if two bounding boxes intersect each other.

			==================== PRIVATE ====================
			getRay(XMFLOAT3 &originOut, XMFLOAT3 &directionOut, int mouseX, int mouseY)
				Called by both TestRaySphereIntersect functions
				Gets and stores information to specify a ray at mouseX and mouseY
				and stores then in the Out parameters.

			RaySphereIntersect(rayOrigin, rayDirection, radius);
				Checks if the ray specified by param 1 & 2 intersects with a
				sphere with given radius.
			RayAABBIntersect(rayOrigin, rayDirection, AABB)
				Checks if the specified ray intersects with AABB.

			==================== INLINE ====================
			Swap<T>(T&, T&)
				Swaps the two objects of type T around in memory.

			Sort(vector<GameObject*>*, FXMVECTOR* fxmCamPosition)
				Use to sort the specified list of GameObjects by distance from
				the specified camera position with the smallest at the start.

			VectorLengthToCamSq(FXMVECTOR* fxmCamPosition, GameObject* object)
				Use to return a float representing the squared length between the fxmCamPosition
				and the position of the object in worldspace.

Members:	==================== PRIVATE ====================
			D3DClass* m_D3D
				a pointer to the D3D class for this CollisionClass to interface with.
			CameraClass* m_Camera
				a pointer to the user's camera for this CollisionClass to interface with.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class CollisionClass
{
public:
	CollisionClass();
	CollisionClass(const CollisionClass&);
	~CollisionClass();

	bool Initialize(D3DClass*, CameraClass*);
	void Shutdown();

	bool TestRaySphereIntersect(int mouseX, int mouseY);
	bool TestRayAABBIntersect(int mouseX, int mouseY, BoundingBox* AABB);

	GameObject* CollisionTestLoop(int mouseX, int mouseY, GameObjectManager* objManager, FXMVECTOR FXMcamPosition);

	static void GetRay(D3DClass* d3d, CameraClass* cam, XMFLOAT3 &directionOut, int mouseX, int mouseY);

	static bool Intersects(BoundingBox* a, BoundingBox* b);

	

private:
	void GetRay(XMFLOAT3 &originOut, XMFLOAT3 &directionOut, int mouseX, int mouseY);

	bool RaySphereIntersect(FXMVECTOR vrayOrigin, FXMVECTOR vrayDirection, float radius);
	bool rayAABBIntersect(FXMVECTOR rayOrigin, FXMVECTOR rayDirection, BoundingBox* AABB);

	template<typename T> void Swap(T&, T&);
	void Sort(std::vector<GameObject*>*, FXMVECTOR fxmCamPosition);

	float VectorLengthToCamSq(FXMVECTOR fxmCamPosition, GameObject* object);

private:
	D3DClass * m_D3D;
	CameraClass* m_Camera;
};

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Swap

Summary:	Swaps the two objects of type T at the memory addresses specified.

Args:		T & a
				a reference to the first T object to swap.
			T & b
				a reference to the second T object to swap.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
template<typename T>
inline void CollisionClass::Swap(T & a, T & b)
{
	T temp(a);
	a = b;
	b = temp;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Sort

Summary:	Sorts the specified list by squared length from fxmCamPosition
			placing the lowest length at the start of the list.

Args:		vector<GameObject*>* list
				the vector list to sort.
			FXMVECTOR fxmCamPosition.
				

Returns:	bool
				Whether or not the raycast intersected the sphere.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
inline void CollisionClass::Sort(std::vector<GameObject*>* list, FXMVECTOR fxmCamPosition)
{
	GameObject* key;
	int i, j;
	for (i = 1; i < list->size(); i++) {
		key = list->at(i);
		j = i - 1;

		/* Move elements of arr[0..i-1], that are
		  greater than key, to one position ahead
		  of their current position */
		while (j >= 0 && VectorLengthToCamSq(fxmCamPosition, list->at(j)) > VectorLengthToCamSq(fxmCamPosition, key)) 
		{
			list->at(j+1) = list->at(j);
			j = j - 1;
		}
		list->at(j+1) = key;
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		VectorLengthToCamSq

Summary:	Returns the length squared between object and fxmCamPosition.

Args:		FXMVECTOR fxmCamPosition
				an XMVECTOR containing the world space position of the camera.
			GameObject* object
				a pointer to the GameObject to find the distance squared to.

Returns:	float
				The distance between fxmCamPosition and object.transform squared.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
inline float CollisionClass::VectorLengthToCamSq(FXMVECTOR fxmCamPosition, GameObject * object)
{
	//Get the position of the GameObject as an XMVECTOR
	XMVECTOR* objPosition = new XMVECTOR();
	*objPosition = XMLoadFloat3(object->GetPosition());

	//Return the distance squared between the two vectors.
	return XMVectorGetByIndex(XMVector3LengthSq(*objPosition - fxmCamPosition), 0);
	
}
