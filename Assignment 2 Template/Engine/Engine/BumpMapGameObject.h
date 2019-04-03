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
			1. BumpMapGameObject()
				The default constructor of a BumpMapGameObject.
			~BumpMapGameObject()
				The default deconstructor of a BumpMapGameObject.
			2. BumpMapGameObject(ModelClass* baseModel)
				Constructor for a BumpMapGameObject that sets the base model
				for the object.
			3. BumpMapGameObject(ModelClass*, LightClass*)
				Preferred constructor for the BumpMapGameObject.
				Sets a reference to the base model and the light
				to be queried.

			Render(...)
				Implementation of Render from GameObject, call to
				render this gameObject with its sizing data to
				the specified device.

			SetLight(LightClass*)
				Used to change the light being used by the BumpMapGameObject.

			==================== PRIVATE ====================
			BumpModelClass* GetModel()
				Used to return a pointer to the baseModel used by this GameObject.
				Used by Render to obtain references to the Model.

			void Setup(BumpModelClass*)
				Hides and overrides the setup function in GameObject to perform
				the same function but use a BumpModelClass* object instead.
				Used by Constructors 2 and 3 for setting up the gameObject.

Members:	==================== PRIVATE ====================
			LightClass* m_Light
				the light to be queried by this BumpMapGameObject's render function

			BumpModelClass* m_baseModel
				a Pointer to the BumpModelClass object being used as this
				gameObject's base Model.
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
	BumpModelClass* GetModel();

	void Setup(BumpModelClass* baseModel);
	
private:
	LightClass* m_Light;

	BumpModelClass* m_baseModel;

};

