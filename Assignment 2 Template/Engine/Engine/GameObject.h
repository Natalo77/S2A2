#pragma once
//======================================================
//					Filename: GameObject.h
//======================================================


//======================================================
//					Include Guards.
//======================================================
#ifndef _GAMEOBJECT_GUARD
#define _GAMEOBJECT_GUARD

//======================================================
//					User Defined Headers.
//======================================================
#include "modelclass.h"
#include "shadermanagerclass.h"


//======================================================
//					Forward declarations
//======================================================
class D3DClass;
class CameraClass;


//======================================================
//						Library Headers.
//======================================================
#include <DirectXCollision.h>
#include <DirectXMath.h>


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		GameObject

Summary:	A Class designed for use as a prefab like structure, with properties
			to draw, rotate, etc. and keep track of changing collision data.

Methods:	==================== PURE VIRTUAL ====================
			Render()
				A method that must be implemented to render the object
				using whatever shader is needed.
				The given parameters are used by every Render Method regardless
				of shader.

			==================== PUBLIC ====================
			1. GameObject()
				The default constructor for a gameObject, should not be used
				unless reserving space for a later changed gameObject.
			~GameObject()
				The default constructor for a gameObject. Deletes both
				member variable pointers.
			2. GameObject(ModelClass* baseModel)
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

			GetAABB()
				Use to get a pointer to the AABB being used by this GameObject.
			RenderAABB(ShaderManagerClass*, D3DClass*, CameraClass*)
				Use to render this GameObjects BoundingBox to the specified
				D3D's device context.

			GetPosition()
				Use to get a pointer to the XMFLOAT3 this GameObject uses to
				store its position in world space.

			==================== PROTECTED ====================

			=====================================================================
			==================== DEPRECATED =====================================
			=====================================================================
			UpdateScale(float prevX, float prevY, float prevZ)
				Called by SetScale() to change the Boundingbox data
				according to the change in scale.
			UpdateRotation(float prevX, float prevY, float prevZ)
				Called by SetRotation() and AddRotation() to update
				the bounding box data according to the change in rotation.
			UpdateTransform(float prevX, float prevY, float prevZ)
				Called by SetTransform() and AddTransform() to update
				the bounding box data according to the dhange in rotation.
			=====================================================================
			==================== DEPRECATED =====================================
			=====================================================================

			XMMATRIX* CalcWorldMatrix(XMMATRIX &initialWorldMatrix)
				Uses a reference to an initial world matrix to produce a resultant
					world matrix using the scaling, rotation and transformation data
					of this model.
				Used by Render during the positioning stage.

			ModelClass* GetModel()
				Returns a pointer to the Model Used by this GameObject.
				Hide this function in derivations for use with other model types.
				Used by Render() to get data about the model.

			void Setup(ModelClass*)
				Used by constructor 2 to perform basic setup on the gameObject.
				Hide-Override this function in derived classes for use with other
					Model Types.

Members:	==================== PROTECTED ====================
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

			XMFLOAT3* m_min
				an XMFLOAT3 object to represent the minimum point of
				this gameObject's base model.
			XMFLOAT3* m_max
				an XMFLOAT3 object to represent the maximum point of
				this gameOBject's base model.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class GameObject
{
public:
	virtual bool Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
		XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix) = 0;
public:
	GameObject();
	~GameObject();
	GameObject(ModelClass* baseModel);

	bool setScale(float x, float y, float z);
	bool setRotation(float x, float y, float z);
	bool setTransform(float x, float y, float z);

	bool addRotation(float x, float y, float z);
	bool addTransform(float x, float y, float z);

	BoundingBox* GetAABB();
	void RenderAABB(ShaderManagerClass* shaderManager, D3DClass* d3d, CameraClass* cam);

	XMFLOAT3* GetPosition();

	

protected:
	void UpdateScale(float prevX, float prevY, float prevZ);
	void UpdateRotation(float prevX, float prevY, float prevZ);
	void UpdateTransform(float prevX, float prevY, float prevZ);

	XMMATRIX* CalcWorldMatrix(XMMATRIX &initialWorldMatrix);

	ModelClass* GetModel();

	void Setup(ModelClass* baseModel);

protected:
	ModelClass * m_baseModel;

	BoundingBox* m_AABB;

	XMFLOAT3* m_transform;
	XMFLOAT3* m_scale;
	XMFLOAT3* m_rotation;

protected:
	XMFLOAT3* m_min;
	XMFLOAT3* m_max;
};

#endif

