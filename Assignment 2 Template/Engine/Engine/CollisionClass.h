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

			TestIntersection(int mouseX, int mouseY);
				returns whether or not an intersection occured with a ray specified
				by mouseX and mouseY

			RaySphereIntersect(rayOrigin, rayDirection, radius);
				Checks if the ray specified by param 1 & 2 intersects with a
				sphere with given radius.
			RayAABBIntersect(rayOrigin, rayDirection, AABB)
				Checks if the specified ray intersects with AABB.

			==================== INLINE ====================
			Swap<T>(T&, T&)
				Swaps the two objects of type T around in memory.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class CollisionClass
{
public:
	CollisionClass();
	CollisionClass(const CollisionClass&);
	~CollisionClass();

	bool Initialize(D3DClass*, CameraClass*);
	void Shutdown();

	bool TestIntersection(int mouseX, int mouseY);

	bool RaySphereIntersect(FXMVECTOR vrayOrigin, FXMVECTOR vrayDirection, float radius);
	bool rayAABBIntersect(FXMVECTOR rayOrigin, FXMVECTOR rayDirection, BoundingBox AABB);

private:
	template<typename T> void Swap(T&, T&);

private:
	D3DClass * m_D3D;
	CameraClass* m_Camera;
};

template<typename T>
inline void CollisionClass::Swap(T & a, T & b)
{
	T temp(a);
	a = b;
	b = temp;
}
