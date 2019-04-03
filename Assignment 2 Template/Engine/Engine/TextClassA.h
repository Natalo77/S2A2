//===================================================
//				 Filename: textclass.h
//===================================================

//===================================================
//					Def statements.
//===================================================
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

//===================================================
//			     User Defined Headers.
//===================================================
#include "FW1Font/sourceCode/FW1FontWrapper/Source/FW1Precompiled.h"


//====================================================
//					Forward declarations
//====================================================
class GameObject;


//===================================================
//					Class Declaration.
//===================================================
/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		TextClassA

Summary:	A class designed to provide an implementation of an intersection
			status string at the top left of the screen.

Methods:	================== PUBLIC ==================
			TextClass();
				Default Constructor.
			TextClass(const TextClass&);
				Reference Constructor.
			~TextClass();
				Deconstructor.

			Initialize(ID3D11Device* ID3D11DeviceContext* HWND)
				USE AFTER CONSTRUCTION: 
					Initializes the member variables and objects of this TextClassA object.
			Shutdown()
				USE BEFORE PROGRAM SHUTDOWN: 
					Deletes and cleans up memory used by this TextClassA object.
			Render(ID3D11DeviceContext*)
				CALL EVERY FRAME:
					Renders the current sentence object to the DeviceContext.
			SetIntersection(bool, ID3D11DeviceContext*)
				CALL WHEN INTERSECTION STATUS CHANGES:
					Changes the sentence appropriately to reflect the bool(intersection) passed in.

			================== PRIVATE ==================
			InitializeSentence(SentenceType**)
				Used by the InitializeMethod to Initialize a sentence Struct for the text to be written.
			UpdateSentence(SentenceType*, char*, float, float, floar, ID3D11DeviceContext*)
				Used by the SetIntersection method to perform changes on the data of the Sentence Struct being used.
			RenderSenctence(ID3D11DeviceContext* SentenceType*)
				Used by the render method to Render the sentenceType being used.
			getColour(SentenceType*)
				Used to obtain a UINT32 Colour value in the Format 0xAaBbGgRr from 0.0f - 1.0f float values.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class TextClassA
{
private:
	struct SentenceType
	{
		float red, green, blue;
		WCHAR* sentence;
	};


public:
	TextClassA();
	TextClassA(const TextClassA&);
	~TextClassA();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND);
	void Shutdown();

	bool Render(ID3D11DeviceContext*);
	bool SetIntersection(bool, ID3D11DeviceContext*, GameObject*);


private:
	bool InitializeSentence(SentenceType**);
	bool UpdateSentence(SentenceType*, char*, float, float, float, ID3D11DeviceContext*);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*);
	UINT32 getColour(SentenceType* sentence);

private:
	SentenceType* m_sentence1;

	IFW1Factory* m_pFW1Factory;
	IFW1FontWrapper* m_pFontWrapper;
};

#endif