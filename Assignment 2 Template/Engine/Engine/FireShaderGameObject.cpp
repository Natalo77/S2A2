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

Modifies:	[see GameObject(), m_baseFireModel, frameTime].

Returns:	FireShaderGameObject
				the newly created FireShaderGameObject object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireShaderGameObject::FireShaderGameObject()
{
	m_baseFireModel = 0;
	
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

Args:		FireModelClass* baseModel
				a pointer to a FireModelClass object to use as this
				Gameobject's reference.

Modifies:	[see FireShaderGameObject(), m_baseFireModel, m_AABB, frameTime,
			 scrollSpeeds, scales, distortion1, distortion2, distortion3,
			 distortionScale, distortionBias].

Returns:	FireShaderGameObject
				the newly created FireShaderGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireShaderGameObject::FireShaderGameObject(FireModelClass* baseModel)
{
	FireShaderGameObject();
	this->m_baseFireModel = baseModel;
	this->m_AABB = m_baseFireModel->GetAABB();
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

Modifies:	[see GameObject.Render()].

Returns:	bool
				was the rendering successful or not?
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool FireShaderGameObject::Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
	XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	//Increment this object's stored frame time 
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	this->m_baseFireModel->Render(device);
	const XMMATRIX* newWorldMatrix = this->CalcWorldMatrix(worldMatrix);
	return shaderManager->RenderFireShader(device, this->m_baseFireModel->GetIndexCount(), *newWorldMatrix, viewMatrix, projectionMatrix,
		this->m_baseFireModel->GetTexture1(), this->m_baseFireModel->GetTexture2(), this->m_baseFireModel->GetTexture3(), frameTime, *scrollSpeeds,
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

