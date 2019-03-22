///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "TextClassA.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/CFW1Factory.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/CFW1FontWrapper.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/FW1FontWrapper.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/FW1Precompiled.h"

#pragma comment(lib, "TextLibA.lib")


TextClassA::TextClassA()
{
	m_Font = 0;
	m_FontShader = 0;
	m_sentence1 = 0;
}


TextClassA::TextClassA(const TextClassA& other)
{
}


TextClassA::~TextClassA()
{
}


bool TextClassA::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight,
	XMMATRIX baseViewMatrix)
{
	bool result;


	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClassA;
	if (!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, "../Engine/data/fontdata.txt", L"../Engine/data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClassA;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_sentence1, 32, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence1, "Intersection: No", 20, 20, 1.0f, 0.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	//Create an FW1 text factory.
	result = FW1CreateFactory(FW1_VERSION, &m_pFW1Factory);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Fukd up the text object fam.", L"Error", MB_OK);
		return false;
	}

	//Create a font wrapper for ARIAL using the FW1 text factory.
	result = m_pFW1Factory->CreateFontWrapper(device, (LPCWSTR)("ARIAL"), &m_pFontWrapper);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Fukd the font wrapper fam.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void TextClassA::Shutdown()
{
	// Release the first sentence.
	ReleaseSentence(&m_sentence1);

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	if (m_pFontWrapper)
	{
		delete m_pFontWrapper;
		m_pFontWrapper = 0;
	}

	if (m_pFW1Factory)
	{
		delete m_pFW1Factory;
		m_pFW1Factory = 0;
	}

	return;
}


bool TextClassA::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix)
{
	bool result;


	// Draw the first sentence.
	result = RenderSentence(deviceContext, m_sentence1);
	if (!result)
	{
		return false;
	}

	return true;
}


bool TextClassA::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for (i = 0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	//initial sentence

	(*sentence)->sentence = (L"Intersection: No");

	return true;
}


bool TextClassA::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
	ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;


	size_t newsize = strlen(text) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, text, _TRUNCATE);
	sentence->sentence = wcstring;
	
	return true;
}


void TextClassA::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		RenderSentence

Summary:	Renders the sentence stored in param 'sentence' to the top left of the screen.

Args:		ID3D11DeviceContext deviceContext
				A pointer to the current DeviceContext being used.
			SentenceType* sentence.
				A pointer to a sentence struct containing the text to be written.

Modifies:	[None.].

Returns:	bool
				representing whether or not the sentence was rendered successfully.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool TextClassA::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence)
{
	UINT32 colour;
	
	//color = 0xAABBGGRR
	//colour = 0xFF0000FF;
	//colour = 0xFF + (blue << 6) + (green << 4) + (blue << 2);
	

	colour = getColour(sentence);

	

	m_pFontWrapper->DrawString(deviceContext, (WCHAR*)sentence->sentence, 50.0f, 10.0f, 10.0f, colour, FW1_TOP | FW1_LEFT | FW1_RESTORESTATE);

	

	return true;
}

UINT32 TextClassA::getColour(SentenceType* sentence)
{
	//Convert each float value to its percentage of 255
	int colorb = 255 * sentence->blue;
	int colorg = 255 * sentence->green;
	int colorr = 255 * sentence->red;

	//convert each int to 8 bit Hex
	UINT8 ucolorb = 0x00 + colorb;
	UINT8 ucolorg = 0x00 + colorg;
	UINT8 ucolorr = 0x00 + colorr;

	//Convert each UINT8 to a UINT32
	UINT32 u32colorb = ucolorb;
	UINT32 u32colorg = ucolorg;
	UINT32 u32colorr = ucolorr;

	//Create final UINT32s and push the converted UINT8s back onto each.
	UINT32 u32finalcolorb = 0x00000000 | (u32colorb << 16);
	UINT32 u32finalcolorg = 0x00000000 | (u32colorg << 8);
	UINT32 u32finalcolorr = 0x00000000 | (u32colorr);

	//0xAaBbGgRr
	//Push each hex back onto a UNIT32
	UINT32 color = 0xFF000000 | u32finalcolorb | u32finalcolorg | u32finalcolorr;

	return color;
}


bool TextClassA::SetIntersection(bool intersection, ID3D11DeviceContext* deviceContext)
{
	char intersectionString[32];
	bool result;


	if (intersection)
	{
		strcpy_s(intersectionString, "Intersection: Yes");
		result = UpdateSentence(m_sentence1, intersectionString, 20, 20, 0.0f, 1.0f, 0.0f, deviceContext);
	}
	else
	{
		strcpy_s(intersectionString, "Intersection: No");
		result = UpdateSentence(m_sentence1, intersectionString, 20, 20, 1.0f, 0.0f, 0.0f, deviceContext);
	}

	return result;
}