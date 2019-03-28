#pragma once
//======================================================
//					Filename: GameObject.h
//======================================================


//======================================================
//					User Defined Headers.
//======================================================
#include "modelclass.h"


//======================================================
//						Library Headers.
//======================================================
#include <DirectXCollision.h>
#include <DirectXMath.h>


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		GameObject

Summary:	A Class designed for use as a prefab like structure, with properties
			to draw, rotate, etc. and keep track of changing collision data.

Methods:	==================== PUBLIC ====================
			GameObject()
				The default constructor for a gameObject, should not be used
				unless reserving space for a later changed gameObject.
			~GameObject()
				The default constructor for a gameObject. Deletes both
				member variable pointers.
			GameObject(ModelClass* baseModel)
				The preferred constructor for a GameObject. Creates the
				GameObject using references to the model data.

			SetScale(float x, float y, float z)
				Use to change the scale of the gameobject at runtime.
			SetRotation(float x, float y, float z)
				Use to change the rotation of the gameObject at runtime.
			SetTransform(float x, float y, float z)
				Use to change the transform of the gameObject at runtime.

			AddRotation(float x, float y, float z)
				Use to add to the rotation data of the gameObject at runtime.
			AddTransform(float x, float y, float z)
				Use to add to the transform data of the gameObject at runtime.

			==================== PRIVATE ====================
			UpdateScale(float prevX, float prevY, float prevZ)
				Called by SetScale() to change the Boundingbox data
				according to the change in scale.
			UpdateRotation(float prevX, float prevY, float prevZ)
				Called by SetRotation() and AddRotation() to update
				the bounding box data according to the change in rotation.
			UpdateTransform(float prevX, float prevY, float prevZ)
				Called by SetTransform() and AddTransform() to update
				the bounding box data according to the dhange in rotation.

Members:	==================== PRIVATE ====================
			ModelClass* m_baseModel
				a pointer to the baseModel data used for this GameObject.
			BoundingBox* m_AABB
				a pointer to the bounding box used for this gameobject.

			XMFLOAT3* m_transform
				an XMFLOAT3 keeping track of the current position of the
				gameobject.
			XMFLOAT3* m_scale
				an XMFLOAT3 keeping track of the current scale of the
				gameObject.
			XMFLOAT3* m_rotation
				an XMFLOAT3 keeping track of the current rotation of
				the gameObject.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class GameObject
{
public:
	GameObject();
	~GameObject();
	GameObject(ModelClass* baseModel);

	bool setScale(float x, float y, float z);
	bool setRotation(float x, float y, float z);
	bool setTransform(float x, float y, float z);

	bool addRotation(float x, float y, float z);
	bool addTransform(float x, float y, float z);

private:
	void UpdateScale(float prevX, float prevY, float prevZ);
	void UpdateRotation(float prevX, float prevY, float prevZ);
	void UpdateTransform(float prevX, float prevY, float prevZ);
	

private:
	ModelClass * m_baseModel;
	BoundingBox* m_AABB;

	XMFLOAT3* m_transform;
	XMFLOAT3* m_scale;
	XMFLOAT3* m_rotation;
};

