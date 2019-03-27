#include "CollisionClass.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		CollisionClass

Summary:	Short summary of purpose and content of MyMethodOne.
				Short summary of purpose and content of MyMethodOne.

Args:		MYTYPE MyArgOne
				Short description of argument MyArgOne.
			MYTYPE MyArgTwo
				Short description of argument MyArgTwo.

Modifies:	[list of member data variables modified by this method].

Returns:	MYRETURNTYPE
				Short description of meaning of the return type values.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		CollisionClass::CollisionClass

Summary:	Default constructor for a Collision Class.

Modifies:	[m_D3D].

Returns:	A new CollisionClass Object.
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

Args:		const CollisionClass&
				a reference to a CollisionClass object to create this
				object from.

Returns:	A CollisionClass object with the data stored at PARAM other.
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

Modifies:	[Currently Nothing].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void CollisionClass::Shutdown()
{
	//De-point the pointer.
	m_D3D = 0;

	//De point the camera class pointer.
	m_Camera = 0;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		TestIntersection

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
bool CollisionClass::TestIntersection(int mouseX, int mouseY)
{
	float pointX, pointY;
	XMMATRIX mprojectionMatrix, mviewMatrix, minverseViewMatrix, mworldMatrix, mtranslateMatrix, minverseWorldMatrix;
	XMFLOAT4X4 projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	XMFLOAT3 direction, origin, rayOrigin, rayDirection;
	bool intersect, result;


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
	mtranslateMatrix = XMMatrixTranslation(-5.0f, 1.0f, 5.0f);
	mworldMatrix = XMMatrixMultiply(mworldMatrix, mtranslateMatrix);

	// Now get the inverse of the translated world matrix.
	XMVECTOR worldDeterminant = XMMatrixDeterminant(mworldMatrix);
	minverseWorldMatrix = XMMatrixInverse(&worldDeterminant, mworldMatrix);


	// Now transform the ray origin and the ray direction from view space to world space.
	//D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);

	XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMLoadFloat3(&origin), minverseWorldMatrix));
	//D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);
	XMStoreFloat3(&rayDirection, XMVector3TransformNormal(XMLoadFloat3(&direction), minverseWorldMatrix));

	// Normalize the ray direction.
	//D3DXVec3Normalize(&rayDirection, &rayDirection);
	XMStoreFloat3(&rayDirection, XMVector3Normalize(XMLoadFloat3(&rayDirection)));

	// Now perform the ray-sphere intersection test.
	intersect = RaySphereIntersect(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), 1.0f);

	//return this result.
	return intersect;
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
bool CollisionClass::rayAABBIntersect(FXMVECTOR rayOrigin, FXMVECTOR rayDirection, BoundingBox AABB)
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

	float* infinity;
	*infinity = INFINITY;
	return AABB.Intersects(rayOrigin, rayDirection, *infinity);

	//If this point is reached then the intersection must be true.
	return true;
}



