//======================================================
// Filename: GameObject.cpp
//======================================================


//======================================================
// User Defined Headers.
//======================================================
#include "GameObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GameObject

Summary:	The Default Constructor for a gameObject.

Modifies:	[m_baseModel, m_AABB, m_transform, m_scale].

Returns:	GameObject
				the newly created GameObject object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObject::GameObject()
{
	m_baseModel = 0;
	m_AABB = new BoundingBox();
	m_transform = new XMFLOAT3(0, 0, 0);
	m_scale = new XMFLOAT3(1, 1, 1);
	m_rotation = new XMFLOAT3(0, 0, 0);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~GameObject

Modifies:	[none].

Summary:	The default deconstructor for a gameobject
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObject::~GameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GameObject

Summary:	The preferred constructor for a gameobject. Creates a
			gameObject in the image of the specified ModelClass.

Args:		ModelClass* baseModel
				the ModelClass object ussed for this model.

Modifies:	[m_baseModel, m_AABB, m_transform, m_scale].

Returns:	GameObject
				the newly created GameObject
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GameObject::GameObject(ModelClass * baseModel)
{
	m_baseModel = baseModel;
	m_AABB = new BoundingBox();
	*m_AABB = *m_baseModel->GetAABB();
	m_transform = new XMFLOAT3(0, 0, 0);
	m_scale = new XMFLOAT3(1, 1, 1);
	m_rotation = new XMFLOAT3(0, 0, 0);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		setScale

Summary:	The public method to change the scale of the gameObject.
			Updates the stored scale and collision information.

Args:		float x
				the new x scale to be used.
			float y
				the new y scale to be used.
			float z
				the new z scale to be used.

Modifies:	[m_scale].

Returns:	bool
				was the scale setting successful or not.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GameObject::setScale(float x, float y, float z)
{
	try
	{
		float prevX = m_scale->x;
		float prevY = m_scale->y;
		float prevZ = m_scale->z;

		m_scale->x = x;
		m_scale->y = y;
		m_scale->z = z;

		UpdateScale(prevX, prevY, prevZ);
	}
	catch (exception e)
	{
		return false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		setRotation

Summary:	The public method to change the rotation of the gameObject.
			Updates the stored rotation and collision information.

Args:		float x
				the new x rotation to be used.
			float y
				the new y rotation to be used.
			float z
				the new z rotation to be used.

Modifies:	[m_rotation].

Returns:	bool
				was the rotation setting successful or not.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GameObject::setRotation(float x, float y, float z)
{
	try
	{
		float prevX = m_rotation->x;
		float prevY = m_rotation->y;
		float prevZ = m_rotation->z;

		m_rotation->x = x;
		m_rotation->y = y;
		m_rotation->z = z;

		UpdateRotation(prevX, prevY, prevZ);
	}
	catch (exception e)
	{
		return false;
	}
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		setTransform

Summary:	The public method to change the transform of the gameObject.
			Updates the stored transform and collision information.

Args:		float x
				the new x transform to be used.
			float y
				the new y transform to be used.
			float z
				the new z transform to be used.

Modifies:	[m_transform].

Returns:	bool
				was the transform setting successful or not.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GameObject::setTransform(float x, float y, float z)
{
	try
	{
		float prevX = m_transform->x;
		float prevY = m_transform->y;
		float prevZ = m_transform->z;

		m_transform->x = x;
		m_transform->y = y;
		m_transform->z = z;

		UpdateTransform(prevX, prevY, prevZ);
	}
	catch (exception e)
	{
		return false;
	}
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		addRotation

Summary:	The public method to add to the rotation of the gameObject.
			Updates the stored rotation and collision information.

Args:		float x
				the x rotation to be added.
			float y
				the y rotation to be added.
			float z
				the z rotation to be added.

Modifies:	[m_rotation].

Returns:	bool
				was the rotation adding successful or not.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GameObject::addRotation(float x, float y, float z)
{
	try
	{
		float prevX = m_rotation->x;
		float prevY = m_rotation->y;
		float prevZ = m_rotation->z;

		m_rotation->x += x;
		m_rotation->y += y;
		m_rotation->z += z;

		UpdateRotation(prevX, prevY, prevZ);
	}
	catch (exception e)
	{
		return false;
	}
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		addTransform

Summary:	The public method to add to the transform of the gameObject.
			Updates the stored rotation and collision information.

Args:		float x
				the x transform to be added.
			float y
				the y transform to be added.
			float z
				the z transform to be added.

Modifies:	[m_transform].

Returns:	bool
				was the transform adding successful or not.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GameObject::addTransform(float x, float y, float z)
{
	try
	{
		float prevX = m_transform->x;
		float prevY = m_transform->y;
		float prevZ = m_transform->z;

		m_transform->x += x;
		m_transform->y += y;
		m_transform->z += z;

		UpdateTransform(prevX, prevY, prevZ);
	}
	catch (exception e)
	{
		return false;
	}
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		UpdateScale

Summary:	uses the known previous scale information to update the extents
			of the bounding box.

Args:		float prevX
				the previous X scale of the object.
			float prevY
				the previous Y scale of the object.
			float prevZ
				the previous Z scale of the object.

Modifies:	[m_AABB].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObject::UpdateScale(float prevX, float prevY, float prevZ)
{
	m_AABB->Extents.x *= (m_scale->x / prevX);
	m_AABB->Extents.y *= (m_scale->y / prevY);
	m_AABB->Extents.z *= (m_scale->z / prevZ);

}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		UpdateRotation

Summary:	uses the known previous rotation information to update the extents
			of the bounding box.

Args:		float prevX
				the previous X rotation of the object.
			float prevY
				the previous Y rotation of the object.
			float prevZ
				the previous Z rotation of the object.

Modifies:	[m_AABB].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObject::UpdateRotation(float prevX, float prevY, float prevZ)
{
	XMVECTOR rotDiff;
	rotDiff = XMVectorSet(m_rotation->x - prevX, m_rotation->y - prevY, m_rotation->z - prevZ, 1.0f);
	rotDiff = XMQuaternionRotationRollPitchYawFromVector(rotDiff);
	XMVECTOR* vector = new XMVECTOR();
	*vector = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	
	m_AABB->Transform(*m_AABB, 1.0f, rotDiff, *vector);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		UpdateTransform

Summary:	uses the known previous transform information to update the extents
			of the bounding box.

Args:		float prevX
				the previous X transform of the object.
			float prevY
				the previous Y transform of the object.
			float prevZ
				the previous Z transform of the object.

Modifies:	[m_AABB].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObject::UpdateTransform(float prevX, float prevY, float prevZ)
{
	XMVECTOR* transDiff = new XMVECTOR();
	*transDiff = XMVectorSet(m_transform->x - prevX, m_transform->y - prevY, m_transform->z - prevZ, 1.0f);
	XMVECTOR* vector = new XMVECTOR();
	*vector = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	*vector = XMQuaternionRotationRollPitchYawFromVector(*vector);
	m_AABB->Transform(*m_AABB, 1.0f, *vector, *transDiff);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		CalcWorldMatrix

Summary:	Uses all data about this object to construct a resultant
			world matrix for this object.

Args:		XMMATRIX &initialWorldMatrix
				an Initial world matrix to be copied and used for
				the resultant calculation.

Modifies:	[none].

Returns:	XMMATRIX* a pointer to the calculated XMMATRIX object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
XMMATRIX * GameObject::CalcWorldMatrix(XMMATRIX &initialWorldMatrix)
{
	XMMATRIX* worldMatrix = new XMMATRIX(initialWorldMatrix);

	*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixScaling(m_scale->x, m_scale->y, m_scale->z));
	*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixTranslation(m_transform->x, m_transform->y, m_transform->z));
	*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationRollPitchYaw(m_rotation->x, m_rotation->y, m_rotation->z));
	

	return worldMatrix;
}

