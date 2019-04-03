//=============================================
//		Filename: TextureGameObject.cpp
//=============================================


//=============================================
//			User Defined Headers.
//=============================================
#include "TextureGameObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		TextureGameObject

Summary:	The Default Constructor for a TextureGameObject.
			Simply uses the default constructor for its parent GameObject

Modifies:	[see GameObject()].

Returns:	TextureGameObject
				the newly created TextureGameObject object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
TextureGameObject::TextureGameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		TextureGameObject

Summary:	The Default deconstructor for a TextureGameObject.

Modifies:	[see ~GameObject()].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
TextureGameObject::~TextureGameObject()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		TextureGameObject

Summary:	The preferred constructor for a textureGameObject.
			Constructs a TextureGameObject with the specified ModelClass
			as its base model object.

Args:		ModelClass* baseModel
				a pointer to a modelClass object to use as this
				Gameobject's reference.

Modifies:	[none].

Returns:	TextureGameObject
				the newly created textureGameObject.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
TextureGameObject::TextureGameObject(ModelClass* baseModel)
{
	Setup(baseModel);
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
bool TextureGameObject::Render(ShaderManagerClass* shaderManager, ID3D11DeviceContext* device,
	XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	//Render the model to the deviceContext.
	GetModel()->Render(device);

	//Calculate the worldMatrix of the Model.
	const XMMATRIX* newWorldMatrix = this->CalcWorldMatrix(worldMatrix);

	//Render the model using the textureshader.
	return shaderManager->RenderTextureShader(device, GetModel()->GetIndexCount(), *newWorldMatrix, viewMatrix, projectionMatrix,
		GetModel()->GetTexture());
}
