//===========================================
//		Filename: FireModelClass.h
//===========================================


//===========================================
//			   Definition Guards
//===========================================
#ifndef _FIREMODELCLASS_H_
#define _FIREMODELCLASS_H_



//===========================================
//				Library Headers.
//===========================================
#include <d3d11_1.h>
#include <directXMath.h>
#include <fstream>
#include <DirectXCollision.h>


//===========================================
//			User Defined Headers
//===========================================
#include "textureclass.h"


//===========================================
//				  Namespaces
//===========================================
using namespace std;
using namespace DirectX;



/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		FireModelClass

Summary:	A class to represent a unique model and provide methods for
			its creation, management, and drawing.

Structs:	VertexType
				A struct to represent 2 values of each vertex on a mesh:
					position and texture co-ord.
			ModelType
				A struct to represent co-ordinates for each vertexType value.
				along with an additional Normal data co-ordinate

Methods:	==================== PUBLIC ====================
			FireModelClass();
				Default Constructor.
			ForeModelClass(const FireModelClass&);
				Reference Constructor.
			~FireModelClass();
				default deconstructor.

			bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*)
				Call after creation to set up the FireModelObject for use.
			void Shutdown()
				Call before deletion to free memory used by this FireModelClass object.
			void Render(ID3D11DeviceContext*)
				Call during the rendering loop to render this FireModelClass object to
				the specified device context.

			int GetIndexCount
				A utility function to return the number of indices in the mesh.

			ID3D11ShaderResourceView* GetTexture1
				A utility function to return the first texture used by the fire shader.
			ID3D11ShaderResourceView* GetTexture2
				A utility function to return the second texture used by the fire shader.
			ID3D11ShaderResourceView* GetTexture3
				A utility function to return the third texture used by the fire shader.

			BoundingBox* GetAABB
				A utility function to return the bounding box used by this base model.

			==================== PRIVATE ====================
			bool InitializeBuffers(ID3D11Device*)
				Called by Initialize() to initialize the index and vertex buffer in memory.
			void ShutdownBuffers()
				Called by Shutdown() to clear the vertex and index buffers from memory.
			void RenderBuffers(ID3D11DeviceContext)
				Called by Render to activate the vertex and index buffer for rendering.

			bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*)
				Called by Initialize to create ShaderResourceViews from each of the textures
				passed in
			void ReleaseTextures()
				Called by shutdown to release the texture objects from memory.

			bool LoadModel(char*)
				Called by initialize to load the model file specified into an array of
				ModelType structs.
			void ReleaseModel()
				Called by shutdown to release the modelType array from memory.

			bool SetupBoundingBox()
				Called by initialize to create the bounding box using the precalculated 
				min and max points.
			void ReleaseBoundingBox()
				Called by shutdown to release the boundingbox from memory.

Members:	==================== PRIVATE ====================
			ID3D11Buffer *m_vertexBuffer
				a Buffer object to store the vertices of the model in memory.
			ID3D11Buffer *m_indexBuffer
				a Buffer object to store the indices of the model in memory.

			int m_vertexCount
				the number of vertices in the model.
			int m_indexCount
				the number of indices in the model.

			TextureClass *m_Texture1, *m_Texture2, *m_Texture3
				the three Texture class objects to represent the texture used
				by the fire shader.

			ModelType* m_model.
				an array of ModelType structs to hold the data of the model.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class FireModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	FireModelClass();
	FireModelClass(const FireModelClass&);
	~FireModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();

	BoundingBox* GetAABB();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

	bool SetupBoundingBox();
	void ReleaseBoundingBox();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass *m_Texture1, *m_Texture2, *m_Texture3;
	ModelType* m_model;

	BoundingBox* m_AABB;

	XMFLOAT3* m_min;
	XMFLOAT3* m_max;
};

#endif