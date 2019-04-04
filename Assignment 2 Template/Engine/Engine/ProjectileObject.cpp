//=============================================
//			Filename: ProjectileObject.cpp
//=============================================


//=============================================
//			User defined headers.
//=============================================
#include "ProjectileObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ProjectileObject

Summary:	Default constructor for a ProjectileObject.
			Uses the parent default constructor (LightGameObject())

Modifies:	[none].

Returns:	ProjectileObject
				the newly created object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ProjectileObject::ProjectileObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~ProjectileObject

Summary:	Default deconstructor for a ProjectileObject.
			Uses the parent default deconstructor (LightGameObject())

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ProjectileObject::~ProjectileObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ProjectileObject

Summary:	Constructor for a ProjectileObject with a reference to
				a base model.
			Sets velocity to a base value.

Args:		ModelClass* baseModel
				a pointer to a ModelClass to use as the baseModel
				for this gameObject.

Modifies:	[m_Velocity].

Returns:	ProjectileObject
				the newly created object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ProjectileObject::ProjectileObject(ModelClass * baseModel)
{
	Setup(baseModel);
	m_Velocity = new XMFLOAT3(0.f, 0.f, 0.f);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ProjectileObject

Summary:	Preferred constructor for a ProjectileObject
			Creates the object with reference to a base model,
				the camera & light to query, and an initial velocity.

Args:		ModelClass* baseModel
				a pointer to a ModelClass to use as the base model for
				this gameObject.
			LightClass* light
				a pointer to the light object to query when rendering.
			CameraClass* camera
				a pointer to the camera object being used.
			XMFLOAT3* velocity
				a pointer to an XMFLOAT3 describing the initial velocity
				of the gameObject.

Modifies:	[none].

Returns:	ProjectileObject
				the newly created object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ProjectileObject::ProjectileObject(ModelClass * baseModel, LightClass * light, CameraClass * camera, XMFLOAT3* velocity)
{
	Setup(baseModel);
	m_Light = light;
	m_Camera = camera;
	m_Velocity = new XMFLOAT3(velocity->x, velocity->y, velocity->z);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SetVelocity

Summary:	Use to set the velocity of this projectile to a desired
			value.

Args:		float x, y, z
				the respective values to set the velocity to.

Modifies:	[m_Velocity].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ProjectileObject::SetVelocity(float x, float y, float z)
{
	m_Velocity->x = x;
	m_Velocity->y = y;
	m_Velocity->z = z;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		AddVelocity

Summary:	Use to add to the velocity of this projectile with desired
			values.

Args:		float x, y, z
				the respective values to add to the velocity.

Modifies:	[m_Velocity].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ProjectileObject::AddVelocity(float x, float y, float z)
{
	m_Velocity->x += x;
	m_Velocity->y += y;
	m_Velocity->z += z;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetVelocity

Summary:	Use to get a pointer to this projectile's current velocity.

Modifies:	[none].

Returns:	XMFLOAT3*
				a pointer to this projectile's velocity XMFLOAT3
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
XMFLOAT3 * ProjectileObject::GetVelocity()
{
	return m_Velocity;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Frame

Summary:	A public method to ask this Projectile object to update its
			position using the velocity stored for this object.
			Should be called every frame that this object is in existence.

Modifies:	[m_Transform].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ProjectileObject::Frame()
{
	m_transform->x += m_Velocity->x;
	m_transform->y += m_Velocity->y;
	m_transform->z += m_Velocity->z;
}
