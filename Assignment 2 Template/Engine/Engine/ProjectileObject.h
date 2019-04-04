#pragma once
//=================================================
//			Filename: ProjectileObject.h
//=================================================


//=================================================
//				User defined headers.
//=================================================
#include "LightGameObject.h"


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		ProjectileObject.

Summary:	A derivation of LightGameObject adding velocity as a stored
			metric, intended to be used for projectile objects within a
			scene.

Methods:	==================== PUBLIC ====================
			ProjectileObject()
				default constructor of A projectileObject.
			~ProjectileObject()
				default deconstructor of a projectileObject.
			ProjectileObject(ModelClass* baseModel)
				constructor of a projectileObject with reference to a baseModel.
			ProjectileObject(ModelClass*, LightClass*, CameraClass*)
				preferred constructor of a projectileObject with reference to the baseModel,
				light and camera to query.
				
			SetVelocity(float, float, float)
				Use to set the velocity of this projectileObject.
			AddVelocity(float, float, float)
				Use to add to the velocity of this projectileObject.

			XMFLOAT3* GetVelocity()
				Use to return a pointer to the velocity of this ProjectileObject.

			Frame(float)
				Use to update the position of this object using its velocity

Members:	==================== PRIVATE ====================
			XMFLOAT3* m_Velocity
				an XMFLOAT3 to store the velocity of this projectileObject.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class ProjectileObject : public LightGameObject
{
public:
	ProjectileObject();
	~ProjectileObject();
	ProjectileObject(ModelClass* baseModel);
	ProjectileObject(ModelClass* baseModel, LightClass* light, CameraClass* camera, XMFLOAT3* velocity);

	void SetVelocity(float x, float y, float z);
	void AddVelocity(float x, float y, float z);

	XMFLOAT3* GetVelocity();

	void Frame();

private:
	XMFLOAT3* m_Velocity;
};

