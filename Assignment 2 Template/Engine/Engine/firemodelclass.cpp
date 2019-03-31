//===========================================
//		Filename: FireModelClass.cpp
//===========================================


//===========================================
//			 User Defined Headers.
//===========================================
#include "firemodelclass.h"

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		FireModelClass

Summary:	The default constructor for a FireModelClass object.

Modifies:	[m_vertexBuffer, m_indexBuffer, m_Texture1, m_Texture2, m_Texture3, m_model,
			 m_AABB, m_min, m_max].

Returns:	FireModelClass
				The constructed FireModelClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireModelClass::FireModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture1 = 0;
	m_Texture2 = 0;
	m_Texture3 = 0;
	m_model = 0;

	m_AABB = 0;
	m_min = 0;
	m_max = 0;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		FireModelClass

Summary:	The reference constructor for a FireModelClass object.

Args:		const FireModelClass& other
				a reference to the FireModelClass object to create this one
				in the image of.

Modifies:	[none].

Returns:	FireModelClass
				The constructed FireModelClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireModelClass::FireModelClass(const FireModelClass& other)
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~FireModelClass

Summary:	The default deconstructor for a FireModelClass object.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FireModelClass::~FireModelClass()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Initialize

Summary:	================= CALL AFTER CREATION =================
			Performs all the required setup on a newly created FireModelClass
			object.

Args:		ID3D11Device*  device.
				the device that this ModelClass is being created for.
			char* modelFilename
				a filepath to the .txt file containing the vertex data
				for this model.
			WCHAR* textureFilename1
				a filepath to the ARGB8 .dds file used for the texture
				of this model.
			WCHAR* textureFilename2
				a filepath to the ARGB8 .dds file used for the second
				texture of this model.
			WCHAR* textureFilename3
				a filepath for the ARGB8 .dds file used for the third
				texture of this model.

Modifies:	[none].

Returns:	bool
				was the initialization of all subparts successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool FireModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2, 
							WCHAR* textureFilename3)
{
	bool result;


	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	//Setup the bounding box
	result = SetupBoundingBox();
	if (!result)
		return false;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the textures for this model.
	result = LoadTextures(device, textureFilename1, textureFilename2, textureFilename3);
	if(!result)
	{
		return false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Shutdown()

Summary:	================= CALL BEFORE DELETION =================
			Performs all the necessary memory cleaning/releasing on
			this FireModelClass object.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireModelClass::Shutdown()
{
	// Release the model textures.
	ReleaseTextures();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	//Release the boundingBox collision data.
	ReleaseBoundingBox();

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Render

Summary:	Puts the stored vertex and index buffers on the graphics
			pipeline to draw them

Args:		ID3D11DeviceContext* deviceContext
				the device context that the model should be rendered to,

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetIndexCount

Summary:	returns the number of indices in this model.

Modifies:	[none].

Returns:	int
				The number of indices in the model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
int FireModelClass::GetIndexCount()
{
	return m_indexCount;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetAABB

Summary:	Returns the bounding box used for this model.

Returns:	BoundingBox*
				A pointer to the bounding box object used for this model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BoundingBox* FireModelClass::GetAABB()
{
	return this->m_AABB;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		InitializeBuffers

Summary:	Uses the ModelType data stored by this class to setup and
			create vertex & index buffers for this model.

Args:		ID3D11Device* device
				A pointer to the Device object to create the buffers
				with.

Modifies:	[m_vertexBuffer, m_indexBuffer].

Returns:	bool
				whether or not the initialization of the buffers
				with the ModelData was successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool FireModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ShutdownBuffers

Summary:	releases and de-points both the index and vertex buffer to
			free them from memory.

Modifies:	[m_vertexBuffer, m_indexBuffer].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		RenderBuffers

Summary:	Used to activate the index and vertex buffers so that they
			will be rendered by the deviceContext.

Args:		ID3D11DeviceContext* deviceContext
				The device context to prepare the buffers for rendering on.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		LoadTextures

Summary:	Creates and initializes 3 textureclass objects for the model
			using the specified data.

Args:		ID3D11Device* device.
				a pointer to the device object to be used for the texture
				initialization.
			WCHAR* textureFilename1
				the filepath to the ARGB8 .dds file to be used for the
				creation of the first texture for the textureClass.
			WCHAR* textureFilename2
				the filepath to the ARGB8 .dds file to be used for the
				creation of the second texture for the textureClass.
			WCHAR* textureFilename3
				the filepath to the ARGB8 .dds file to be used for the
				creation of the third texture for the textureClass.


Modifies:	[m_Texture1, m_Texture2, m_Texture3].

Returns:	bool
				was the texture loading successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool FireModelClass::LoadTextures(ID3D11Device* device, WCHAR* textureFilename1, WCHAR* textureFilename2, WCHAR* textureFilename3)
{
	bool result;


	// Create the texture object.
	m_Texture1 = new TextureClass;
	if(!m_Texture1)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture1->Initialize(device, textureFilename1);
	if(!result)
	{
		return false;
	}

	// Create the texture object.
	m_Texture2 = new TextureClass;
	if(!m_Texture2)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture2->Initialize(device, textureFilename2);
	if(!result)
	{
		return false;
	}

	// Create the texture object.
	m_Texture3 = new TextureClass;
	if(!m_Texture3)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture3->Initialize(device, textureFilename3);
	if(!result)
	{
		return false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ReleaseTexture

Summary:	Properly de-points and releases the 3 textureClass objects
			used by this Model.

Modifies:	[m_Texture1, m_Texture2, m_Texture3].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireModelClass::ReleaseTextures()
{
	// Release the texture objects.
	if(m_Texture1)
	{
		m_Texture1->Shutdown();
		delete m_Texture1;
		m_Texture1 = 0;
	}

	if(m_Texture2)
	{
		m_Texture2->Shutdown();
		delete m_Texture2;
		m_Texture2 = 0;
	}

	if(m_Texture3)
	{
		m_Texture3->Shutdown();
		delete m_Texture3;
		m_Texture3 = 0;
	}

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetTexture1

Summary:	Gets a reference to the resource view containing the first
			texture used for this Model.

Returns:	ID3D11ShaderResourceView*
				A pointer to a resource view containing the texture being
				used by the model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ID3D11ShaderResourceView* FireModelClass::GetTexture1()
{
	return m_Texture1->GetTexture();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetTexture2

Summary:	Gets a reference to the resource view containing the second
			texture used for this Model.

Returns:	ID3D11ShaderResourceView*
				A pointer to a resource view containing the texture being
				used by the model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ID3D11ShaderResourceView* FireModelClass::GetTexture2()
{
	return m_Texture2->GetTexture();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetTexture3

Summary:	Gets a reference to the resource view containing the third
			texture used for this Model.

Returns:	ID3D11ShaderResourceView*
				A pointer to a resource view containing the texture being
				used by the model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ID3D11ShaderResourceView* FireModelClass::GetTexture3()
{
	return m_Texture3->GetTexture();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		LoadModel

Summary:	Loads the Model data from a .txt file containing the vertex
				data of the model.
			Calculate the min and max points of the model.

Args:		char* filename
				a filepath to the .txt file containing the model's
				vertex data.

Modifies:	[m_vertexCount, m_indexCount, m_model, m_min, m_max].

Returns:	bool
				was the loading of the ModelType data succesful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool FireModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	//Store temporary min and maxes for each point.
	float minx = INFINITY;
	float miny = INFINITY;
	float minz = INFINITY;
	float maxx = -INFINITY;
	float maxy = -INFINITY;
	float maxz = -INFINITY;

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;

		//Update the min and maxes.
		///x
		if (m_model[i].x < minx)
			minx = m_model[i].x;
		else if (m_model[i].x > maxx)
			maxx = m_model[i].x;

		///y
		if (m_model[i].y < miny)
			miny = m_model[i].y;
		else if (m_model[i].y > maxy)
			maxy = m_model[i].y;

		///z
		if (m_model[i].z < minz)
			minz = m_model[i].z;
		else if (m_model[i].z > maxz)
			maxz = m_model[i].z;
	}

	// Close the model file.
	fin.close();

	//Initialize the min and max using the data provided.
	m_min = new XMFLOAT3(minx, miny, minz);
	m_max = new XMFLOAT3(maxx, maxy, maxz);

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ReleaseModel

Summary:	Properly releases and de-points the ModelType data used by
			this model and the min and max points.

Modifies:	[m_model, m_min, m_max].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	if (m_min)
	{
		delete m_min;
		m_min = 0;
	}

	if (m_max)
	{
		delete m_max;
		m_max = 0;
	}

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SetupBoundingBox

Summary:	Sets up the bounding box attached to this FireModelClass object
				using the precalculated min and max points.

Modifies:	[m_AABB].

Returns:	bool
				was the creation of the bounding box successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool FireModelClass::SetupBoundingBox()
{
	//Create a new BoundingBox.
	m_AABB = new BoundingBox();

	//Create the bounding box using the stored max and min points.
	BoundingBox::CreateFromPoints(*m_AABB, XMLoadFloat3(m_min), XMLoadFloat3(m_max));

	//The operation passed.
	return true;

}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ReleaseBoundingBox

Summary:	Deletes and de-points the bounding box pointer.

Modifies:	[m_AABB].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void FireModelClass::ReleaseBoundingBox()
{
	if (m_AABB)
	{
		delete m_AABB;
		m_AABB = 0;
	}
}