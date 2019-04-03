//=============================================
//		Filename: LightGameObject.cpp
//=============================================


//=============================================
//			User Defined Headers.
//=============================================
#include "LightGameObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		LightGameObject

Summary:	The Default Constructor for a LightGameObject.
			Simply uses the default constructor for its parent GameObject

Modifies:	[see GameObject()].

Returns:	LightGameObject
				the newly created LightGameObject object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
LightGameObject::LightGameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		LightGameObject

Summary:	The Default deconstructor for a LightGameObject.

Modifies:	[see ~GameObject()].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
LightGameObject::~LightGameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		LightGameObject

Summary:	A constructor for a LightGameObject.
			Constructs a LightGameObject with the specified ModelClass
			as its base model object.

Args:		ModelClass* baseModel
				a pointer to a modelClass object to use as this
				Gameobject's reference.

Modifies:	[none].

Returns:	LightGameObject
				the newly created LightGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
LightGameObject::LightGameObject(ModelClass* baseModel)
{
	Setup(baseModel);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		LightGameObject

Summary:	The preferred constructor for a LightGameObject.
			Constructs a LightGameObject with the specified ModelClass
			as its base model object and sets the LightClass and
			CameraClass objects to query.

Args:		ModelClass* baseModel
				a pointer to a modelClass object to use as this
				Gameobject's reference.
			LightClass* light
				a pointer to the LightClass object that this
				LightGameObject should use when rendering.
			CameraClass* camera
				a pointer to the CameraClass object that this
				LightGameObject should use when rendering.

Modifies:	[m_Light, m_Camera].

Returns:	LightGameObject
				the newly created LightGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
LightGameObject::LightGameObject(ModelClass* baseModel, LightClass* light, CameraClass* camera)
{
	Setup(baseModel);
	this->m_Light = light;
	this->m_Camera = camera;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Render

Summary:	The implementation of render from GameObject.
			uses the lightshader within the passed in shadermanager
			to render the object to the screen.

Modifies:	[see GameObject.Render()].

Returns:	bool
				was the rendering successful or not?
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool LightGameObject::Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
	XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	//Render the model to the deviceContext.
	GetModel()->Render(device);

	//Calculate the worldMatrix of the model.
	const XMMATRIX* newWorldMatrix = this->CalcWorldMatrix(worldMatrix);

	//Render the model using the LightShader.
	return shaderManager->RenderLightShader(device, GetModel()->GetIndexCount(), *newWorldMatrix, viewMatrix, projectionMatrix, 
		GetModel()->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SetLight

Summary:	Sets this LightGameObject's LightClass object to be used
			when rendering.

Args:		LightClass* light
				a Pointer to the lightclass object to set.

Modifies:	[m_Light].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void LightGameObject::SetLight(LightClass* light)
{
	this->m_Light = light;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SetCamera

Summary:	Sets this LightGameObject's CameraClass object to be used
			when rendering.

Args:		CameraClass* camera
				a Pointer to the lightclass object to set.

Modifies:	[m_Camera].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void LightGameObject::SetCamera(CameraClass* camera)
{
	this->m_Camera = camera;
}
