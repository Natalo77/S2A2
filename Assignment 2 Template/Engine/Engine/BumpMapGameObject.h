#pragma once
//=============================================
//		Filename: BumpMapGameObject.h
//=============================================


//=============================================
//			User Defined Headers.
//=============================================
#include "GameObject.h"
#include "lightclass.h"
#include "bumpmodelclass.h"


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		BumpMapGameObject

Summary:	A Class designed for use as a prefab like structure, with properties
			to draw, rotate, etc. and keep track of changing collision data.
			This specific Class uses a Bump Map shader alongside the texture shader.

Methods:	==================== PUBLIC ====================
			BumpMapGameObject()
				The default constructor of a BumpMapGameObject.
			~BumpMapGameObject()
				The default deconstructor of a BumpMapGameObject.
			BumpMapGameObject(ModelClass* baseModel)
				Constructor for a BumpMapGameObject that sets the base model
				for the object.
			BumpMapGameObject(ModelClass*, LightClass*)
				Preferred constructor for the BumpMapGameObject.
				Sets a reference to the base model and the light
				to be queried.

			Render(...)
				Implementation of Render from GameObject, call to
				render this gameObject with its sizing data to
				the specified device.

			SetLight(LightClass*)
				Used to change the light being used by the BumpMapGameObject.

Members:	==================== PRIVATE ====================
			LightClass* m_Light
				the light to be queried by this BumpMapGameObject's render function
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class BumpMapGameObject : public GameObject
{
public:
	BumpMapGameObject();
	~BumpMapGameObject();
	BumpMapGameObject(BumpModelClass* baseModel);
	BumpMapGameObject(BumpModelClass* baseModel, LightClass* light);

	virtual bool Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
		XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix) override;

	void SetLight(LightClass* light);
private:
	LightClass* m_Light;

	BumpModelClass* m_baseBumpModel;

};

