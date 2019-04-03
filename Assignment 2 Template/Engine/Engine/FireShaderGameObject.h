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
			1. FireShaderGameObject()
				The default constructor of a FireShaderGameObject.
			~FireShaderGameObject()
				The default deconstructor of a FireShaderGameObject.
			2. FireShaderGameObject(FireModelClass* baseModel)
				Preferred vonstructor for a FireShaderGameObject that sets the base model
				for the object.

			Render(...)
				Implementation of Render from GameObject, call to
				render this gameObject with its sizing data to
				the specified device.

			SetParameters(...)
				Use to set the internal parameters of the fire shader.

			==================== PRIVATE ====================
			GetModel()
				A utility function to return a reference to the baseModel
					used by this GameObject.
				Used by the Render function to get data about the model.

			Setup(FireModelClass*)
				A hide-override of the Setup function in GameObject that
					performs the same function but uses a FireModelClass instead.
				Used by constrctor 2 to setup the GameObject and its
					collision data.

Members:	==================== PRIVATE ====================
			FireModelClass* m_baseFireModel
				a pointer to a fireModelClass that is used as the base model
				for this FireShaderGameObject.
			
			float frameTime
				A float representing the current time of the fire animation.
			XMFLOAT3* scrollSpeeds
				An XMFLOAT3 used by the fire animation.
			XMFLOAT3* scales
				An XMFLOAT3 used by the fire animation.
			XMFLOAT2* distortion1, distortion2, distortion3
				XMFLOAT2s used by the fire animation.
			float distortionScale.
				a float used by the fire animation.
			float distortionBias.
				a float used by the fire animation.
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
	FireModelClass* GetModel();

	void Setup(FireModelClass* baseModel);

private:
	FireModelClass* m_baseModel;

	float frameTime;
	XMFLOAT3* scrollSpeeds;
	XMFLOAT3* scales;
	XMFLOAT2* distortion1;
	XMFLOAT2* distortion2;
	XMFLOAT2* distortion3;
	float distortionScale;
	float distortionBias;

};

