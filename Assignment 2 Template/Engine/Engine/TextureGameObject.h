#pragma once
//=============================================
//		Filename: TextureGameObject.h
//=============================================


//=============================================
//			User Defined Headers.
//=============================================
#include "GameObject.h"


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		TextureGameObject

Summary:	A Class designed for use as a prefab like structure, with properties
			to draw, rotate, etc. and keep track of changing collision data.
			This specific class uses just a texture shader.

Methods:	==================== PUBLIC ====================
			TextureGameObject()
				The default constructor of a TextureGameObject.
			~TextureGameObject()
				The default deconstructor of a TextureGameObject.
			TextureGameObject(ModelClass*)
				The preferred constructor of a TextureGameObject. Constructs the
				gameObject using the ModelClass pointer as the base Model for
				this gameobject.

			Render
				Implementation of Render from GameObject, call to
				render this gameObject with its sizing data to
				the specified device.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class TextureGameObject : public GameObject
{
public:
	TextureGameObject();
	~TextureGameObject();
	TextureGameObject(ModelClass* baseModel);

	virtual bool Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
		XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix) override;
};

