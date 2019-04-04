#pragma once
//=============================================
//		Filename: LightGameObject.h
//=============================================


//=============================================
//			User Defined Headers.
//=============================================
#include "GameObject.h"
#include "lightclass.h"
#include "cameraclass.h"


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		LightGameObject

Summary:	A Class designed for use as a prefab like structure, with properties
			to draw, rotate, etc. and keep track of changing collision data.
			This specific Class uses a Light shader alongside the texture shader.

Methods:	==================== PUBLIC ====================
			LightGameObject()
				The default constructor of a LightGameObject.
			~LightGameObject()
				The default deconstructor of a LightGameObject.
			LightGameObject(ModelClass* baseModel)
				Constructor for a LightGameObject that sets the base model
				for the object.
			LightGameObject(ModelClass*, LightClass*, CameraClass*)
				Preferred constructor for the LightGameObject.
				Sets a reference to the base model, the light to be queried
				and the Camera being used by the user.

			Render(...)
				Implementation of Render from GameObject, call to
				render this gameObject with its sizing data to
				the specified device.

			SetLight(LightClass*)
				Used to change the light being used by the LightGameObject.

			SetCamera(CameraClass*)
				Used to change the Camera currently being used by the User.

Members:	==================== PROTECTED ====================
			LightClass* m_Light
				the light to be queried by this LightGameObject's render function
			CameraClass* m_Camera
				the Camera to be queried by this LightGameObject's render function.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class LightGameObject : public GameObject
{
public:
	LightGameObject();
	~LightGameObject();
	LightGameObject(ModelClass* baseModel);
	LightGameObject(ModelClass* baseModel, LightClass* light, CameraClass* camera);

	virtual bool Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
		XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix) override;

	void SetLight(LightClass* light);

	void SetCamera(CameraClass* camera);

protected:
	LightClass* m_Light;
	CameraClass* m_Camera;

};

