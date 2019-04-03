//======================================================
// Filename: GameObject.cpp
//======================================================


//======================================================
// User Defined Headers.
//======================================================
#include "GameObject.h"
#include "d3dclass.h"
#include "cameraclass.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GameObject

Summary:	The Default Constructor for a gameObject.

Modifies:	[m_baseModel, m_AABB, m_transform, m_scale, m_rotation].

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
	Setup(baseModel);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		RenderAABB

Summary:	Use to render the AABB of this gameObject to the screen.

Args:		ShaderManagerClass* shaderManager
				a pointer to the ShaderManagerClass object currently
				being used.
			D3DClass* d3d
				A pointer to the D3DClass object currently being used.
			CameraClass* cam
				A pointer to the CameraClass object being used to
				represent the current user camera.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObject::RenderAABB(ShaderManagerClass* shaderManager, D3DClass* d3d, CameraClass* cam)
{
	//Turn on wireframe drawing in the d3d class.
	d3d->TurnOnWireframe();

	//Create a model to represent the boundingBox
	ModelClass* aabb = new ModelClass();

	//Initialize the model using the boundingBox stored within this model.
	aabb->Initialize(d3d->GetDevice(), m_AABB);

	//Render the boundingBoxModel to the device.
	aabb->Render(d3d->GetDeviceContext());

	//Get the world, view and projection matrices for drawing.
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	d3d->GetWorldMatrix(worldMatrix);
	cam->GetViewMatrix(viewMatrix);
	d3d->GetProjectionMatrix(projectionMatrix);
	
	//use the shaderManager and the texture shader to render the boundingBoxModel to the device.
	bool result = shaderManager->RenderTextureShader(d3d->GetDeviceContext(), aabb->GetIndexCount(), 
		worldMatrix, viewMatrix, projectionMatrix, aabb->GetTexture());

	//Turn off wireframe drawing in the d3d class.
	d3d->TurnOffWireframe();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		setScale

Summary:	The public method to change the stored scale of the gameObject.

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
	}
	catch (exception e)
	{
		return false;
	}
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetAABB

Summary:	Public method to return a pointer to this GameObject's AABB.

Modifies:	[none].

Returns:	BoundingBox*
				a pointer to the boundingbox being used by this gameobject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BoundingBox * GameObject::GetAABB()
{
	return this->m_AABB;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetPosition

Summary:	Public method to return a pointer to this GameObject's
			XMFLOAT3 to represent transform.

Modifies:	[none].

Returns:	XMFLOAT3*
				a pointer to the XMFLOAT3 being used by this gameobject
				to describe its position.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
XMFLOAT3 * GameObject::GetPosition()
{
	return this->m_transform;
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
[[deprecated("CalcWorldMatrix now handles all manipulations of the collision data.")]]
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
[[deprecated("CalcWorldMatrix now handles all manipulations of the collision data.")]]
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
[[deprecated("CalcWorldMatrix now handles all manipulations of the collision data.")]]
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
			Then uses that worldMatrix to update and position the AABB
				for this gameObject.

Args:		XMMATRIX &initialWorldMatrix
				an Initial world matrix to be copied and used for
				the resultant calculation.

Modifies:	[m_AABB].

Returns:	XMMATRIX* 
				a pointer to the calculated XMMATRIX object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
XMMATRIX * GameObject::CalcWorldMatrix(XMMATRIX &initialWorldMatrix)
{
	//Create an initial XMMATRIX out of the initialWorldMatrix
	XMMATRIX* worldMatrix = new XMMATRIX(initialWorldMatrix);

	//Scale, rotate and translate it.
	*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixScaling(m_scale->x, m_scale->y, m_scale->z));
	*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationRollPitchYaw(m_rotation->x, m_rotation->y, m_rotation->z));
	*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixTranslation(m_transform->x, m_transform->y, m_transform->z));
	
	//Remake the bounding box and transform it using the new worldMatrix.
	BoundingBox::CreateFromPoints(*m_AABB, XMLoadFloat3(m_min), XMLoadFloat3(m_max));
	m_AABB->Transform(*m_AABB, *worldMatrix);

	return worldMatrix;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetModel

Summary:	A utility function to return a pointer to the bae model
			used by this gameobject.

Returns:	ModelClass*
				a pointer to a modelClass object being used as the
				base model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ModelClass * GameObject::GetModel()
{
	return this->m_baseModel;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Setup

Summary:	The base function to perform all the required setup
			for a GameObject that can be done using either no information
			and/or just the baseModel.

			This should be hidden by derived classes if other ModelClass
			types need to be used.

Args:		ModelClass* baseModel
				a pointer to the ModelClass object that this gameObject
				should use as its baseModel.

Modifies:	[m_baseModel, m_min, m_max, m_transform, m_scale, m_rotation].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GameObject::Setup(ModelClass* baseModel)
{
	//Point the baseModel correctly.
	m_baseModel = baseModel;

	//Initialize the min and max points of the base gameObject using the information in baseModel.
	m_min = new XMFLOAT3(m_baseModel->m_min->x, m_baseModel->m_min->y, m_baseModel->m_min->z);
	m_max = new XMFLOAT3(m_baseModel->m_max->x, m_baseModel->m_max->y, m_baseModel->m_max->z);

	//Create an initial bounding box using this data.
	BoundingBox::CreateFromPoints(*m_AABB, XMLoadFloat3(m_min), XMLoadFloat3(m_max));

	//Initialize the transform, scale and rotation to acceptable default values.
	m_transform = new XMFLOAT3(0, 0, 0);
	m_scale = new XMFLOAT3(1, 1, 1);
	m_rotation = new XMFLOAT3(0, 0, 0);
}

