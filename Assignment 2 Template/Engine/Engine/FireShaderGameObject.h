#pragma once
//=============================================
//		Filename: FireShaderGameObject.h
//=============================================


//=============================================
//			User Defined Headers.
//=============================================
#include "GameObject.h"
#include "firemodelclass.h"


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		FireShaderGameObject

Summary:	A Class designed for use as a prefab like structure, with properties
			to draw, rotate, etc. and keep track of changing collision data.
			This specific Class uses an Animated Fire shader alongside the texture shader.

Methods:	==================== PUBLIC ====================
			FireShaderGameObject()
				The default constructor of a FireShaderGameObject.
			~FireShaderGameObject()
				The default deconstructor of a FireShaderGameObject.
			FireShaderGameObject(FireModelClass* baseModel)
				Preferred vonstructor for a FireShaderGameObject that sets the base model
				for the object.

			Render(...)
				Implementation of Render from GameObject, call to
				render this gameObject with its sizing data to
				the specified device.

			SetParameters(...)
				Use to set the internal parameters of the fire shader.

Members:	==================== PRIVATE ====================
			FireModelClass* m_baseFireModel
				a pointer to a fireModelClass that is used as the base model
				for this FireShaderGameObject.
			
			float frameTime

			XMFLOAT3* scrollSpeeds

			XMFLOAT3* scales

			XMFLOAT2* distortion1, distortion2, distortion3

			float distortionScale.

			float distortionBias.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class FireShaderGameObject : public GameObject
{
public:
	FireShaderGameObject();
	~FireShaderGameObject();
	FireShaderGameObject(FireModelClass* baseModel);

	virtual bool Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
		XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix) override;

	void SetParameters(XMFLOAT3* scrollSpeeds, XMFLOAT3* scales, XMFLOAT2* distortion1,
		XMFLOAT2* distortion2, XMFLOAT2* distortion3, float distortionScale, float distortionBias);

private:
	FireModelClass* m_baseFireModel;

	float frameTime;
	XMFLOAT3* scrollSpeeds;
	XMFLOAT3* scales;
	XMFLOAT2* distortion1;
	XMFLOAT2* distortion2;
	XMFLOAT2* distortion3;
	float distortionScale;
	float distortionBias;

};

