//================================================
//		  Filename: bumpmodelclass.cpp
//================================================


//================================================
//			  User Defined Headers.
//================================================
#include "bumpmodelclass.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		BumpModelClass

Summary:	Creates a new BumpModelClass object.
			points all initial pointer objects to zero.

Modifies:	[m_vertexBuffer, m_indexBuffer, m_model, m_ColorTexture, 
			 m_NormalMapTexture, m_AABB, m_min, m_max].

Returns:	BumpModelClass
				The newly created bumpModelClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpModelClass::BumpModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_model = 0;
	m_ColorTexture = 0;
	m_NormalMapTexture = 0;

	m_AABB = 0;
	m_min = 0;
	m_max = 0;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		BumpModelClass

Summary:	The reference constructor for a BumpModelClass object.

Args:		const BumpModelClass& other
				a reference to a BumpModelClass object to create this
				one in the image of.

Modifies:	[none].

Returns:	BumpModelClass
				The newly created bumpModelClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpModelClass::BumpModelClass(const BumpModelClass& other)
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~BumpModelClass

Summary:	The default deconstructor for a BumpModelClass object.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BumpModelClass::~BumpModelClass()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Initialize()

Summary:	Sets up all member variables and objects of this BumpModelClass
			object.
			Call after creation to set the object up for use.

Args:		ID3D11Device* device
				the device that this BumpModelClass object is being created
				for
			char* modelFilename
				a filepath to the model file to be used for this model.
			WCHAR* textureFilename1
				a filepath to the colour texture to be used for this model.
			WCHAR* textureFilename2
				a filepath to the normal map texture to be used for this model.

Modifies:	[none].

Returns:	bool
				whether the initialization of all the submembers was successful
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool BumpModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Calculate the tangent and binormal vectors for the model.
	CalculateModelVectors();

	//Setup the boundingbox.
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
	result = LoadTextures(device, textureFilename1, textureFilename2);
	if(!result)
	{
		return false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Shutdown

Summary:	Releases and frees up memory used by this BumpModelClass
			object.
			Call before deletion to ensure memory is freed correctly.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void BumpModelClass::Shutdown()
{
	// Release the model textures.
	ReleaseTextures();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	//Release the boundingbox collision data.
	ReleaseBoundingBox();

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Render

Summary:	Puts the vertex and index buffers on the graphics pipeline
			to prepare them for drawing.

Args:		ID3D11DeviceContext* deviceContext
				the deviceContext that the model will be rendered to.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void BumpModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetIndexCount

Summary:	Returns the number of indices in this model.

Returns:	int
				Representing the number of indices in the model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
int BumpModelClass::GetIndexCount()
{
	return m_indexCount;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetColorTexture()

Summary:	Returns the ColorTexture used by this Model as a
			ShaderResourceView

Returns:	ID3D11ShaderResourceView*
				a ShaderResourceView containing the colour texture
				of this model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ID3D11ShaderResourceView* BumpModelClass::GetColorTexture()
{
	return m_ColorTexture->GetTexture();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetNormalMapTexture()

Summary:	Returns the NormalMapTexture used by this Model as a
			ShaderResourceView

Returns:	ID3D11ShaderResourceView*
				a ShaderResourceView containing the normal map texture
				of this model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
ID3D11ShaderResourceView* BumpModelClass::GetNormalMapTexture()
{
	return m_NormalMapTexture->GetTexture();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GetAABB

Summary:	Returns a pointer to the boundingbox used to represent the
			collision data of this base model.

Returns:	BoundingBox*
				A pointer to the bounding box being used for this model.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
BoundingBox * BumpModelClass::GetAABB()
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
bool BumpModelClass::InitializeBuffers(ID3D11Device* device)
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
		vertices[i].tangent = XMFLOAT3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertices[i].binormal = XMFLOAT3(m_model[i].bx, m_model[i].by, m_model[i].bz);

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
void BumpModelClass::ShutdownBuffers()
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
void BumpModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

Summary:	Creates and initializes 2 textureclass objects for the model
			using the specified texture and normal maps.

Args:		ID3D11Device* device.
				a pointer to the device object to be used for the texture
				initialization.
			WCHAR* filename1
				the filepath to the ARGB8 .dds file to be used for the
				creation of the texture for the textureClass.
			WCHAR* filename2
				the filepath to the ARGB8 .dds file to be used for the
				creation of the normalmap texture for the second TextureClass

Modifies:	[m_ColorTexture, m_NormalMapTexture].

Returns:	bool
				was the texture loading successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool BumpModelClass::LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	bool result;


	// Create the color texture object.
	m_ColorTexture = new TextureClass;
	if(!m_ColorTexture)
	{
		return false;
	}

	// Initialize the color texture object.
	result = m_ColorTexture->Initialize(device, filename1);
	if(!result)
	{
		return false;
	}

	// Create the normal map texture object.
	m_NormalMapTexture = new TextureClass;
	if(!m_NormalMapTexture)
	{
		return false;
	}

	// Initialize the normal map texture object.
	result = m_NormalMapTexture->Initialize(device, filename2);
	if(!result)
	{
		return false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ReleaseTextures

Summary:	Properly de-points and releases the 2 textureClass objects
			used by this Model.

Modifies:	[m_ColorTexture, m_NormalMapTexture].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void BumpModelClass::ReleaseTextures()
{
	// Release the texture objects.
	if(m_ColorTexture)
	{
		m_ColorTexture->Shutdown();
		delete m_ColorTexture;
		m_ColorTexture = 0;
	}

	if(m_NormalMapTexture)
	{
		m_NormalMapTexture->Shutdown();
		delete m_NormalMapTexture;
		m_NormalMapTexture = 0;
	}

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		LoadModel

Summary:	Loads the Model data from a .txt file containing the vertex
				data of the model.
			Calculates the min and max points of the model.

Args:		char* filename
				a filepath to the .txt file containing the model's
				vertex data.

Modifies:	[m_vertexCount, m_indexCount, m_model, m_min, m_max].

Returns:	bool
				was the loading of the ModelType data succesful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool BumpModelClass::LoadModel(char* filename)
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
void BumpModelClass::ReleaseModel()
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
Method:		CalculateModelVectors

Summary:	Uses the existing vertex normal data to calculate a normal
			for each face of the mesh. (Tris)

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void BumpModelClass::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal;


	// Calculate the number of faces in the model.
	faceCount = m_vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for(i=0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		m_model[index-1].tx = tangent.x;
		m_model[index-1].ty = tangent.y;
		m_model[index-1].tz = tangent.z;
		m_model[index-1].bx = binormal.x;
		m_model[index-1].by = binormal.y;
		m_model[index-1].bz = binormal.z;

		m_model[index-2].tx = tangent.x;
		m_model[index-2].ty = tangent.y;
		m_model[index-2].tz = tangent.z;
		m_model[index-2].bx = binormal.x;
		m_model[index-2].by = binormal.y;
		m_model[index-2].bz = binormal.z;

		m_model[index-3].tx = tangent.x;
		m_model[index-3].ty = tangent.y;
		m_model[index-3].tz = tangent.z;
		m_model[index-3].bx = binormal.x;
		m_model[index-3].by = binormal.y;
		m_model[index-3].bz = binormal.z;
	}

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		CalculateTangentBinormal

Summary:	Calculate a resultant tangent and Binormal to the three
			specified TempVertexTypes and store the result in
			tangent and bionormal.

Args:		TempVertexType vertex1, vertex2, vertex3
				the three vertices comprising a triangle within
				the model mesh.
			VectorType& tangent
				A reference to the vectorType struct instance to store
				the result tangent in.
			VectorType& binormal
				A reference to the vectorType struct instance to store
				the result binormal in.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void BumpModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
										  VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

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
bool BumpModelClass::SetupBoundingBox()
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
void BumpModelClass::ReleaseBoundingBox()
{
	if (m_AABB)
	{
		delete m_AABB;
		m_AABB = 0;
	}
}
