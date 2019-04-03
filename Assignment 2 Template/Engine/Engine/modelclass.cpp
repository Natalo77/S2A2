//===========================================
//			Filename: modelclass.cpp
//===========================================


//===========================================
//			 User Defined Headers.
//===========================================
#include "modelclass.h"

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ModelClass

Summary:	The default constructor for a ModelClass object.

Modifies:	[m_vertexBuffer, m_indexBuffer, m_Texture, m_model, m_AABB, m_min, m_max].

Returns:	ModelClass
				The constructed ModelClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	m_AABB = 0;
	m_min = 0;
	m_max = 0;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ModelClass

Summary:	The reference constructor for a ModelClass object.

Args:		const ModelClass& other
				a reference to the ModelClass object to create this one
				in the image of.

Modifies:	[none].

Returns:	ModelClass
				The constructed ModelClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ModelClass::ModelClass(const ModelClass& other)
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~ModelClass

Summary:	The default deconstructor for the ModelClass object.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ModelClass::~ModelClass()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Initialize

Summary:	An overload of Initialize that uses a boundingbox to create
			a model (using the center and extents of the boundingbox).

Args:		ID3D11Device* device
				a pointer to the ID3D11Device object that should be used
				to load the boundingBoxModel texture with.
			BoundingBox* AABB
				a pointer to the bounding box which contains the data
				with which to query for the points of the bounding box.

Modifies:	[none].

Returns:	bool
				was the initialization of all the subparts successful?
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ModelClass::Initialize(ID3D11Device* device, BoundingBox* AABB)
{
	bool result;

	//Calculate all 8 points of the bounding box.
	XMFLOAT3 p1{ AABB->Center.x + AABB->Extents.x, AABB->Center.y + AABB->Extents.y, AABB->Center.z + AABB->Extents.z };
	XMFLOAT3 p2{ AABB->Center.x + AABB->Extents.x, AABB->Center.y + AABB->Extents.y, AABB->Center.z - AABB->Extents.z };
	XMFLOAT3 p3{ AABB->Center.x + AABB->Extents.x, AABB->Center.y - AABB->Extents.y, AABB->Center.z + AABB->Extents.z };
	XMFLOAT3 p4{ AABB->Center.x - AABB->Extents.x, AABB->Center.y + AABB->Extents.y, AABB->Center.z + AABB->Extents.z };
	XMFLOAT3 p5{ AABB->Center.x + AABB->Extents.x, AABB->Center.y - AABB->Extents.y, AABB->Center.z - AABB->Extents.z };
	XMFLOAT3 p6{ AABB->Center.x - AABB->Extents.x, AABB->Center.y - AABB->Extents.y, AABB->Center.z + AABB->Extents.z };
	XMFLOAT3 p7{ AABB->Center.x - AABB->Extents.x, AABB->Center.y + AABB->Extents.y, AABB->Center.z - AABB->Extents.z };
	XMFLOAT3 p8{ AABB->Center.x - AABB->Extents.x, AABB->Center.y - AABB->Extents.y, AABB->Center.z - AABB->Extents.z };

	//push them back onto a vector.
	std::vector<XMFLOAT3*>* list = new std::vector<XMFLOAT3*>();
	list->push_back(&p7);
	list->push_back(&p4);
	list->push_back(&p1);
	list->push_back(&p2);
	list->push_back(&p8);
	list->push_back(&p5);
	list->push_back(&p3);
	list->push_back(&p6);

	//Load the model using the calculated points.
	result = LoadModel(list);
	if (!result)
		return false;

	//Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
		return false;

	//Initialize the texture
	result = LoadTexture(device, L"../Engine/Data/blue.dds");
	if (!result)
		return false;

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Initialize

Summary:	================= CALL AFTER CREATION =================
			Performs all the required setup on a newly created ModelClass
			object.

Args:		ID3D11Device*  device.
				the device that this ModelClass is being created for.
			char* modelFilename
				a filepath to the .txt file containing the vertex data
				for this model.
			WCHAR* textureFilename
				a filepath to the ARGB8 .dds file used for the texture
				of this model.

Modifies:	[none].

Returns:	bool
				was the initialization of all subparts successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	//Setup the bounding box.
	result = SetupBoundingBox();
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
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
			this ModelClass object.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

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
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
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
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetTexture

Summary:	Gets a reference to the resource view containing the texture
			used for this Model.

Returns:	ID3D11ShaderResourceView*
				A pointer to a resource view containing the texture being
				used by the model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetAABB

Summary:	Returns the bounding box used for this model.

Returns:	BoundingBox*
				A pointer to the bounding box object used for this model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BoundingBox* ModelClass::GetAABB()
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
bool ModelClass::InitializeBuffers(ID3D11Device* device)
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
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

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
void ModelClass::ShutdownBuffers()
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
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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
Method:		LoadTexture

Summary:	Creates and initializes a textureclass object for the model
			using the specified data.

Args:		ID3D11Device* device.
				a pointer to the device object to be used for the texture
				initialization.
			WCHAR* filename
				the filepath to the ARGB8 .dds file to be used for the
				creation of the texture for the textureClass.

Modifies:	[m_Texture].

Returns:	bool
				was the texture loading successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ReleaseTexture

Summary:	Properly de-points and releases the textureClass object
			used by this Model.

Modifies:	[m_Texture].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
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
bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
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
Method:		LoadModel

Summary:	Creates an array of ModelType structs to represent the model
			using the data passed in by a list of float3s.

Args:		vector<XMFLOAT3*>* list
				a pointer to a list of XMFLOAT3s representing a list of vertices
				to create the model data with.

Modifies:	[m_vertexCount, m_indexCount, m_model].

Returns:	bool
				was the loading of the ModelType data succesful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ModelClass::LoadModel(std::vector<XMFLOAT3*>* list)
{
	//the number of vertices is the size of the list.
	m_vertexCount = list->size();

	//The number of indices is the same as the number of vertices.
	m_indexCount = list->size();

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	//Setup each of the ModelType structs.
	for (int i = 0; i < m_vertexCount; i++)
	{
		m_model[i].x = list->at(i)->x;
		m_model[i].y = list->at(i)->y;
		m_model[i].z = list->at(i)->z;

		m_model[i].nx = 1.0f;
		m_model[i].ny = 1.0f;
		m_model[i].nz = 1.0f;

		m_model[i].tu = 1.0f;
		m_model[i].tv = 1.0f;
	}

	//If this point was reached, the model was created successfully.
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ReleaseModel

Summary:	Properly releases and de-points the ModelType data used by
			this model and the min and max points.

Modifies:	[m_model, m_min, m_max].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ModelClass::ReleaseModel()
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

Summary:	Sets up the bounding box attached to this ModelClass object
				using the precalculated min and max points.

Modifies:	[m_AABB].

Returns:	bool
				was the creation of the bounding box successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ModelClass::SetupBoundingBox()
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
void ModelClass::ReleaseBoundingBox()
{
	if (m_AABB)
	{
		delete m_AABB;
		m_AABB = 0;
	}
}