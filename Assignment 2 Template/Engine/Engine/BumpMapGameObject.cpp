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

Modifies:	[see GameObject(), m_baseBumpModel].

Returns:	BumpMapGameObject
				the newly created BumpMapGameObject object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpMapGameObject::BumpMapGameObject()
{
	m_baseModel = 0;
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

Modifies:	[ m_baseModel, m_AABB].

Returns:	BumpMapGameObject
				the newly created BumpMapGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpMapGameObject::BumpMapGameObject(BumpModelClass* baseModel)
{
	this->m_baseModel = baseModel;
	this->m_AABB = m_baseModel->GetAABB();
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
			CameraClass* camera
				a pointer to the CameraClass object that this
				BumpMapGameObject should use when rendering.

Modifies:	[m_baseModel, m_AABB, m_Light, m_Camera].

Returns:	BumpMapGameObject
				the newly created BumpMapGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpMapGameObject::BumpMapGameObject(BumpModelClass* baseModel, LightClass* light)
{
	this->m_baseModel = baseModel;
	this->m_AABB = m_baseModel->GetAABB();
	this->m_Light = light;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Render

Summary:	The implementation of render from GameObject.
			uses the textureshader within the passed in shadermanager
			to render the object to the screen.

Modifies:	[see GameObject.Render()].

Returns:	bool
				was the rendering successful or not?
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool BumpMapGameObject::Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
	XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	GetModel()->Render(device);
	const XMMATRIX* newWorldMatrix = this->CalcWorldMatrix(worldMatrix);
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

Modifies:	BumpModelClass*
				a pointer to the Modeltype class object being used by this
				gameObject as its base model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpModelClass * BumpMapGameObject::GetModel()
{
	return this->m_baseModel;
}

