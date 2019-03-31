//================================================
//			Filename: bumpmodelclass.h
//================================================


//================================================
//	Definitions
//================================================
#ifndef _BUMPMODELCLASS_H_
#define _BUMPMODELCLASS_H_


//================================================
// Library Headers. 
//================================================
#include <d3d11_1.h>
#include <directXMath.h>
#include <fstream>
#include <DirectXCollision.h>

//================================================
//	User Defined Headers.
//================================================
#include "textureclass.h"


//================================================
//	Namespaces.
//================================================
using namespace std;
using namespace DirectX;




/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		BumpModelClass

Summary:	A class to represent a unique bump mapped model and provide methods for
			its creation, management, and drawing.

Structs:	VertexType
				A struct to represent 5 values of each vertex on a mesh:
					position, texture and normal.
				along with
					tangent and binormal.
			ModelType
				A struct to represent co-ordinates for each vertexType value.
			TempVertexType
				A struct to represent vertexType without tangent and binormal
				before they have been calculated.
			VectorType
				A struct to represent the 3 values of a vector
					x,y,z.

Methods:	==================== PUBLIC ====================
			BumpModelClass();
				Default Constructor.
			BumpModelClass(const BumpModelClass&);
				Reference Constructor.
			~BumpModelClass();
				Deconstructor.

			bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*)
				Call after creating to set up BumpModelClass object for use.
			void Shutdown()
				Call before finished using to tear down the object.
			void Render(ID3D11DeviceContext*)
				Call during the rendering loop to draw this model to the screen.

			int GetIndexCount()
				a utility function to return the number of indicies in this model's mesh.
			ID3D11ShaderResourceView* GetColorTexture()
				a utility function to return the ColourTexture used for this model as a resource.
			ID3D11ShaderResourceView* GetNormalMapTexture()
				a utility function to return the normal map texture used for this model
				as a resource.

			==================== PRIVATE ====================
			InitializeBuffers(ID3D11Device*)
				Called by Initialize() to initialize the index and vertex buffer in memory.
			ShutdownBuffers()
				Called by Shutdown() to clear the vertex and index buffers from memory.
			RenderBuffers(ID3D11DeviceContext)
				Called by Render to activate the vertex and index buffer for rendering.

			bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*)
				Called by Initialize to create and initialize texture objects for this model
				with the specified paths to both colour and normal map textures.
			void ReleaseTextures()
				Called by Shutdown() to release the texture objects from memory.

			bool LoadModel(char*)
				Called by Initialize() to create and initialize a ModelType
				array for the vertex data of the specified model.
				Also calculates the min and max points of the model.
			void ReleaseModel()
				Called by Shutdown() to release the ModelType data and
				the min and max points from memory.

			void CalculateModelVectors()
				Called by Initialize() to calculate the Binormal, tangent
				and normal vectors for each point.
			void Calculate TangentBinormal(...)
				Called by CalculateModelVectors() to find a tangent
				and binormal for an entire face.

			bool SetupBoundingBox()
				called by Initialize() to create a bounding box structure from the 
				precalculated min and max points of the model.
			void ReleaseBoundingBox()
				called by shutdown() to release the bounding box from memory.
				

Members:	==================== PRIVATE ====================
			ID3D11Buffer *m_vertexBuffer
				a Buffer object to store the vertices of the model in memory.
			ID3D11Buffer *m_indexBuffer
				a Buffer object to store the indices of the model in memory.

			int m_vertexCount
				an integer to represent the number of vertices in the model.
			int m_indexCount
				an integer to represent the number of indices in the model.

			ModelType* m_Model.
				An array of ModelType structs to store the vertex data about this model.
			
			TextureClass* m_ColorTexture
				A texture class to keep track of and manage the colour texture used
				by this model.
			TextureClass* m_NormalMapTexture
				A texture class to keep track of and manage the normal map texture
				used by this model.

			BoundingBox* m_AABB
				A boundingBox representing the collision data of this model.

			XMFLOAT3* m_min
				A pointer to an XMFLOAT3 object to be used to store the minimum
				point of the model.
			XMFLOAT3* m_max
				A pointer to an XMFLOAT3 object to be used to store the maximum
				point of the model.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class BumpModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	BumpModelClass();
	BumpModelClass(const BumpModelClass&);
	~BumpModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetColorTexture();
	ID3D11ShaderResourceView* GetNormalMapTexture();

	BoundingBox* GetAABB();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);

	bool SetupBoundingBox();
	void ReleaseBoundingBox();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* m_model;
	TextureClass* m_ColorTexture;
	TextureClass* m_NormalMapTexture;

	BoundingBox* m_AABB;
	XMFLOAT3* m_min;
	XMFLOAT3* m_max;
};

#endif