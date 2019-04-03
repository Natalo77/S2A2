//=============================================
//		Filename: FireShaderGameObject.cpp
//=============================================


//=============================================
//			User Defined Headers.
//=============================================
#include "FireShaderGameObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		FireShaderGameObject

Summary:	The Default Constructor for a FireShaderGameObject.
			Simply uses the default constructor for its parent GameObject

Modifies:	[see GameObject()].

Returns:	FireShaderGameObject
				the newly created FireShaderGameObject object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireShaderGameObject::FireShaderGameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		FireShaderGameObject

Summary:	The Default deconstructor for a FireShaderGameObject.

Modifies:	[see ~GameObject()].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireShaderGameObject::~FireShaderGameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		FireShaderGameObject

Summary:	A constructor for a FireShaderGameObject.
			Constructs a FireShaderGameObject with the specified FireModelClass
			as its base model object.
			Sets up default values for the member variables related to the 
			fire animation.

Args:		FireModelClass* baseModel
				a pointer to a FireModelClass object to use as this
				Gameobject's reference.

Modifies:	[frameTime, scrollSpeeds, scales, distortion1, 
			 distortion2, distortion3, distortionScale, distortionBias].

Returns:	FireShaderGameObject
				the newly created FireShaderGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireShaderGameObject::FireShaderGameObject(FireModelClass* baseModel)
{
	Setup(baseModel);

	frameTime = 0.0f;
	scrollSpeeds = new XMFLOAT3(1.3f, 2.1f, 2.3f);
	scales = new XMFLOAT3(1.0f, 2.0f, 3.0f);
	distortion1 = new XMFLOAT2(0.1f, 0.2f);
	distortion2 = new XMFLOAT2(0.1f, 0.3f);
	distortion3 = new XMFLOAT2(0.1f, 0.1f);
	distortionScale = 0.8f;
	distortionBias = 0.5f;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Render

Summary:	The implementation of render from GameObject.
			uses the FireShader within the passed in shadermanager
			to render the object to the screen.

Modifies:	[frameTime].

Returns:	bool
				was the rendering successful or not?
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool FireShaderGameObject::Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
	XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	//Increment this object's stored frame time 
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
		frameTime = 0.0f;
	
	//Render the model to the device.
	GetModel()->Render(device);

	//Calculate the worldMatrix for the object using its transform data.
	const XMMATRIX* newWorldMatrix = this->CalcWorldMatrix(worldMatrix);

	//Use the fire shader and the information stored on this device to render this model to the deviceContext
	return shaderManager->RenderFireShader(device, GetModel()->GetIndexCount(), *newWorldMatrix, viewMatrix, projectionMatrix,
		GetModel()->GetTexture1(), GetModel()->GetTexture2(), GetModel()->GetTexture3(), frameTime, *scrollSpeeds,
		*scales, *distortion1, *distortion2, *distortion3, distortionScale, distortionBias);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SetParameters

Summary:	Used to set the internal parameters used for rendering when
			the fire shader is being calculated.

Args:		XMFLOAT3* scrollSpeeds

			XMFLOAT3* scales

			XMFLOAT2* distortion1, distortion2, distortion3

			float distortionBias.

			float distortionScale.

Modifies:	[same as args].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireShaderGameObject::SetParameters(XMFLOAT3* scrollSpeeds, XMFLOAT3* scales, XMFLOAT2* distortion1, XMFLOAT2* distortion2, XMFLOAT2* distortion3, float distortionScale, float distortionBias)
{
	this->scrollSpeeds = scrollSpeeds;
	this->scales = scales;
	this->distortion1 = distortion1;
	this->distortion2 = distortion2;
	this->distortion3 = distortion3;
	this->distortionBias = distortionBias;
	this->distortionScale = distortionScale;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetModel

Summary:	A utility function to return a pointer to the FireModelClass
			object being used as the baseModel by this gameObject.

Returns:	FireModelClass*
				a pointer to a FireModelClass object being used as
				the baseModel for this class.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireModelClass * FireShaderGameObject::GetModel()
{
	return this->m_baseModel;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Setup

Summary:	A hide-override of the Setup function in GameObject that
			performs the same function but uses a FireModelClass
			as the baseModel instead.

Args:		FireModelClass* baseModel
				a pointer to the FireModelClass object to use as this
				gameObject's base model.

Modifies:	[m_baseModel, m_min, m_max, m_transform, m_scale, m_rotation].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireShaderGameObject::Setup(FireModelClass * baseModel)
{
	m_baseModel = baseModel;
	m_min = new XMFLOAT3(m_baseModel->m_min->x, m_baseModel->m_min->y, m_baseModel->m_min->z);
	m_max = new XMFLOAT3(m_baseModel->m_max->x, m_baseModel->m_max->y, m_baseModel->m_max->z);
	BoundingBox::CreateFromPoints(*m_AABB, XMLoadFloat3(m_min), XMLoadFloat3(m_max));
	m_transform = new XMFLOAT3(0, 0, 0);
	m_scale = new XMFLOAT3(1, 1, 1);
	m_rotation = new XMFLOAT3(0, 0, 0);
}

