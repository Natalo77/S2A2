//===========================================
//	      Filename: modelclass.h
//===========================================
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//===========================================
//				Library headers. 
//===========================================
#include <d3d11_1.h>
#include <DirectXMath.h> 
#include <DirectXCollision.h>
#include <fstream>


//===========================================
//				User defined headers.
//===========================================
#include "textureclass.h"

//===========================================
//					Namespaces.
//===========================================
using namespace std;
using namespace DirectX;


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		ModelClass

Summary:	A class to represent a unique model and provide methods for
			its creation, management, and drawing.

Structs:	VertexType
				A struct to represent 3 values of each vertex on a mesh:
					position, texture and normal.
			ModelType
				A struct to represent co-ordinates for each vertexType value.

Methods:	==================== PUBLIC ====================
			ModelClass();
				Default Constructor.
			ModelClass(const ModelClass&);
				Reference Constructor.
			~ModelClass();
				Deconstructor.

			Initialize(ID3D11Device*, char*, WCHAR*);
				Call after creating to setup Model Class for use.
			Shutdown();
				Call when finished using to tear down the object.

			Render(ID3D11DeviceContext*)
				Call during the rendering loop to draw this model to the screen.

			GetIndexCount();
				A utility function to return the number of vertices in the mesh.
			GetTexture()
				A utility function to return the Texture used for this resource
				as a resource.
			GetAABB()
				a utility function to return the Bounding box used for this model.

			==================== PRIVATE ====================
			InitializeBuffers(ID3D11Device*)
				Called by Initialize() to initialize the index and vertex buffer in memory.
			ShutdownBuffers()
				Called by Shutdown() to clear the vertex and index buffers from memory.
			RenderBuffers(ID3D11DeviceContext)
				Called by Render to activate the vertex and index buffer for rendering.

			LoadTexture(ID3D11Device*, WCHAR*)
				Called by Initialize() to create and initialize a texture object for this
				model with the specified texture.
			ReleaseTexture()
				Called on Shutdown() to release the texture object from memory.

			LoadModel(char*)
				Called by Initialize() to create and initialize a ModelType array for
				the vertex data of the specified model. Also calculates the min and max
				points of the model.
			ReleaseModel()
				Called by Shutdown() to release the ModelType data and the min and max
				points from memory.

			SetupBoundingBox()
				Called by Initialize() to create a bounding box structure from the
				precalculated min and max points of the model.
			ReleaseBoundingBox()
				Called by Shutdown() to release the bounding box from memory.

Members:	==================== PRIVATE ====================
			ID3D11Buffer *m_vertexBuffer
				a Buffer object to store the vertices of the model in memory.
			ID3D11Buffer *m_indexBuffer
				a Buffer object to store the indices of the model in memory.

			int m_vertexCount
				an integer to represent the number of vertices of this model.
			int m_indexCount
				an integer to represent the number of indices of this model.

			TextureClass* m_Texture
				a Texture object to hold the texture of this model.

			ModelType* m_model
				an Array of ModelType structs to store the vertex data about this model.

			BoundingBox* m_AABB
				a BoundingBox object representing the collision data of this model.

			XMFLOAT3* m_min
				a pointer to an XMFLOAT3 object to be used to store the minimum
				point of the model.
			XMFLOAT3* m_max
				a pointer to an XMFLOAT3 object to be used to store the maximum
				point of the model.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3  position;
		XMFLOAT2  texture;
		XMFLOAT3  normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();

	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	BoundingBox* GetAABB();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	bool SetupBoundingBox();
	void ReleaseBoundingBox();



private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;

	ModelType* m_model;

	BoundingBox* m_AABB;

	XMFLOAT3* m_min;
	XMFLOAT3* m_max;
};

#endif