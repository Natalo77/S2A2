//=============================================
//		Filename: BumpMapGameObject.cpp
//=============================================


//=============================================
//			User Defined Headers.
//=============================================
#include "BumpMapGameObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		BumpMapGameObject

Summary:	The Default Constructor for a BumpMapGameObject.
			Simply uses the default constructor for its parent GameObject

Modifies:	[see GameObject()].

Returns:	BumpMapGameObject
				the newly created BumpMapGameObject object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpMapGameObject::BumpMapGameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		BumpMapGameObject

Summary:	The Default deconstructor for a BumpMapGameObject.

Modifies:	[see ~GameObject()].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpMapGameObject::~BumpMapGameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		BumpMapGameObject

Summary:	A constructor for a BumpMapGameObject.
			Constructs a BumpMapGameObject with the specified ModelClass
			as its base model object.

Args:		ModelClass* baseModel
				a pointer to a modelClass object to use as this
				Gameobject's reference.

Modifies:	[none].

Returns:	BumpMapGameObject
				the newly created BumpMapGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpMapGameObject::BumpMapGameObject(BumpModelClass* baseModel)
{
	Setup(baseModel);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		BumpMapGameObject

Summary:	The preferred constructor for a BumpMapGameObject.
			Constructs a BumpMapGameObject with the specified ModelClass
			as its base model object and sets the LightClass and
			CameraClass objects to query.

Args:		ModelClass* baseModel
				a pointer to a modelClass object to use as this
				Gameobject's reference.
			LightClass* light
				a pointer to the LightClass object that this
				BumpMapGameObject should use when rendering.

Modifies:	[m_Light].

Returns:	BumpMapGameObject
				the newly created BumpMapGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpMapGameObject::BumpMapGameObject(BumpModelClass* baseModel, LightClass* light)
{
	Setup(baseModel);
	this->m_Light = light;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Render

Summary:	The implementation of render from GameObject.
			Uses the textureshader within the passed in shadermanager
				to render the object to the screen.

Modifies:	[none].

Returns:	bool
				was the rendering successful or not?
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool BumpMapGameObject::Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
	XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	//Render the model to the device.
	GetModel()->Render(device);

	//Calculate the worldMatrix based off this GameObject's transform data.
	XMMATRIX* newWorldMatrix = this->CalcWorldMatrix(worldMatrix);

	//Use the shaderManager's bumpMap shader to render this object.
	return shaderManager->RenderBumpMapShader(device, GetModel()->GetIndexCount(), *newWorldMatrix, viewMatrix, projectionMatrix,
		GetModel()->GetColorTexture(), GetModel()->GetNormalMapTexture(), m_Light->GetDirection(),
		m_Light->GetDiffuseColor());
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SetLight

Summary:	Sets this BumpMapGameObject's LightClass object to be used
			when rendering.

Args:		LightClass* light
				a Pointer to the lightclass object to set.

Modifies:	[m_Light].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void BumpMapGameObject::SetLight(LightClass* light)
{
	this->m_Light = light;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetModel

Summary:	A utility function to return a pointer to the baseModel used
			by this gameObject.

Returns:	BumpModelClass*
				a pointer to the Modeltype class object being used by this
				gameObject as its base model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpModelClass * BumpMapGameObject::GetModel()
{
	return this->m_baseModel;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Setup

Summary:	A hiding-override of Setup from GameObject.h
			Performs the same functions of setting up the model, however
			uses a BumpModelClass object instead.

Args:		BumpModelClass* baseModel
				a pointer to a BumpModelClass object to setup this
				gameObject with.

Modifies:	[m_baseModel, m_min, m_max, m_transform, m_scale, m_rotation].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void BumpMapGameObject::Setup(BumpModelClass * baseModel)
{
	m_baseModel = baseModel;
	m_min = new XMFLOAT3(m_baseModel->m_min->x, m_baseModel->m_min->y, m_baseModel->m_min->z);
	m_max = new XMFLOAT3(m_baseModel->m_max->x, m_baseModel->m_max->y, m_baseModel->m_max->z);
	BoundingBox::CreateFromPoints(*m_AABB, XMLoadFloat3(m_min), XMLoadFloat3(m_max));
	m_transform = new XMFLOAT3(0, 0, 0);
	m_scale = new XMFLOAT3(1, 1, 1);
	m_rotation = new XMFLOAT3(0, 0, 0);
}

