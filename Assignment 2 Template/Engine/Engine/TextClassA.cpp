//================================================================
//                   Filename: TextClassA.cpp
//================================================================

//================================================================
//					   User Defined Headers.
//================================================================
#include "TextClassA.h"
#include "GameObject.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/CFW1Factory.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/CFW1FontWrapper.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/FW1FontWrapper.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/FW1Precompiled.h"


//================================================================
//						  Library Headers.
//================================================================
#include <sstream>

//================================================================
//						Library Definitions
//================================================================
#pragma comment(lib, "TextLibA.lib")

//================================================================
//						Functions Source Code
//================================================================
#pragma region Functions

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		TextClassA

Summary:	The default constructor of the TextClassA object.

Modifies:	[m_sentence1].

Returns:	TextClassA
				the constructed TextClassA object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
TextClassA::TextClassA()
{
	m_sentence1 = 0;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		TextClassA

Summary:	The reference constructor for this TextClassA Object.

Args:		const TextClassA& other
				a reference to a TextClass object to construct this one
				in the image of.

Modifies:	[None].

Returns:	TextClassA
				the constructed TextClassA object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
TextClassA::TextClassA(const TextClassA& other)
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~TextClassA

Summary:	The default deconstructor of the the TextClassA object.

Modifies:	[None].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
TextClassA::~TextClassA()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Initialize

Summary:	Initializes the member variables/objects of this TextClassA object
			and set default values.

Args:		ID3D11Device* device
				A pointer to the device that is current being used to render to.
			ID3D11DeviceContext* deviceContext
				A pointer to the device context currently being used for rendering.
			HWND hwnd
				A Handle to the window current being used.

Modifies:	[None].

Returns:	bool
				representing whether or not the TextClassA object was Initialized successfully.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool TextClassA::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd)
{
	bool result;

	// Initialize the first sentence.
	result = InitializeSentence(&m_sentence1);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence1, "Score = 0", 1.0f, 0.0f, 0.0f, deviceContext);
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

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Shutdown.

Summary:	Releases all member variables and objects from memory.

Modifies:	[m_pFontWrapper, m_pFW1FontFactory].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void TextClassA::Shutdown()
{
	//Release the FontWrapper object.
	///Deleting this object is not required (handled within release)
	if (m_pFontWrapper)
	{
		m_pFontWrapper->Release();
		m_pFontWrapper = 0;
	}

	//Release the FWFactory object.
	///Deleting this object is not required (handled within release)
	if (m_pFW1Factory)
	{
		m_pFW1Factory->Release();
		m_pFW1Factory = 0;
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
bool TextClassA::Render(ID3D11DeviceContext* deviceContext)
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

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		InitializeSentence

Summary:	Initializes the intersection sentence.

Args:		SentenceType** sentence.
				A pointer to a block of memory to be used for initializing
				a SentenceType struct.

Modifies:	[None].

Returns:	bool
				representing whether or not the sentence was Initialized successfully.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool TextClassA::InitializeSentence(SentenceType** sentence)
{
	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	//initial sentence
	(*sentence)->sentence = (L"Score = 0");

	//initial score = 0.
	(*sentence)->score = 0;

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		UpdateSentence

Summary:	Updates the text and colour information of the intersection text
			at the top left of the screen.

Args:		SentenceType* sentence
				A pointer to the sentence object that is being used for the top left
				text.
			char* text
				A char array = the text to be written.
			float red
				A float value : 0.0f - 1.0f : the percentage of red in the colour.
			float green
				A float value : 0.0f - 1.0f : the percentage of green in the colour.
			float blue
				A float value : 0.0f - 1.0f : the percentage of blue in the colour.
			ID3D11DeviceContext* deviceContext
				A pointer to the device Context that this update was called because of

Modifies:	[None].

Returns:	bool
				representing whether or not the sentence was updated successfully.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool TextClassA::UpdateSentence(SentenceType* sentence, char* text, float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	//Convert the Char array to a WCHAR for use with FW1FontWrapper
	size_t newsize = strlen(text) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, text, _TRUNCATE);

	//Store the converted WCHAR in the sentence object.
	sentence->sentence = wcstring;

	return true;
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


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		getColour

Summary:	Returns a UINT32 value in the format 0xAaBbGgRr created from
				the float values for each colour store in param sentence.

Args:		SentenceType* sentence.
				A pointer to a SentenceType object containing the float
				colour values to convert from.

Modifies:	[None.].

Returns:	UINT32
				in the format 0xAaBbGgRr with Aa as FF
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
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
	UINT32 u32finalcolorb = 0x00000000 | (u32colorb << 24);
	UINT32 u32finalcolorg = 0x00000000 | (u32colorg << 12);
	UINT32 u32finalcolorr = 0x00000000 | (u32colorr);

	//0xAaBbGgRr
	//Push each hex back onto a UNIT32
	UINT32 color = 0xFF000000 | u32finalcolorb | u32finalcolorg | u32finalcolorr;

	return color;
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SetIntersection

Summary:	Changes the intersection text depending on the value of param intersection
			Intended to be used from outside of this class as an interfacing method
			adds the scoreToAdd if intersection, subtracts otherwise

Args:		bool intersection
				Whether or not an intersection has been recorded.
			ID3D11DeviceContext* deviceContext
				A pointer to the device context that recorded the intersection.
			float scoreToAdd
				the score to add or subtract from the total score.


Modifies:	[m_sentence1].

Returns:	bool
				representing whether or not the sentence was updated successfully.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool TextClassA::SetIntersection(bool intersection, ID3D11DeviceContext* deviceContext, float scoreToAdd)
{
	char intersectionString[32];
	bool result;

	

	std::string string = "Score = ";


	if (intersection)
	{
		m_sentence1->score += scoreToAdd;
		std::ostringstream ss;
		ss << m_sentence1->score;
		std::string s(ss.str());
		string += s;
		strcpy_s(intersectionString, string.c_str());
		result = UpdateSentence(m_sentence1, intersectionString, 0.0f, 1.0f, 0.0f, deviceContext);
	}
	else
	{
		m_sentence1->score -= scoreToAdd;
		std::ostringstream ss;
		ss << m_sentence1->score;
		std::string s(ss.str());
		string += s;
		strcpy_s(intersectionString, string.c_str());
		result = UpdateSentence(m_sentence1, intersectionString, 1.0f, 0.0f, 0.0f, deviceContext);
	}

	return result;
}

#pragma endregion