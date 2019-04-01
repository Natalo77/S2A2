//==============================================
//			Filename: graphicsclass.h
//==============================================
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


//==============================================
//			  User Defined Headers. 
//==============================================
#include "inputclass.h"
#include "d3dclass.h"
#include "timerclass.h"
#include "shadermanagerclass.h"
#include "positionclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "modelclass.h"
#include "bumpmodelclass.h"
#include "firemodelclass.h"
#include "BitmapClassA.h"
#include "CollisionClass.h"
#include "FW1Font/sourceCode/FW1FontWrapper/Source/FW1Precompiled.h"
#include "TextureGameObject.h"
#include "LightGameObject.h"
#include "BumpMapGameObject.h"
#include "FireShaderGameObject.h"

//==============================================
//	  Global Constants/Program parameters 
//==============================================
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


/*C+C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C+++C
Class:		GraphicsClass

Summary:	A class intended to be used on its own to keep track of
			models, input, lighting and everything graphics related.
			Used to render all object within.

Methods:	==================== PUBLIC ====================
			GraphicsClass();
				Default Constructor.
			GraphicsClass(const ModelClass&);
				Reference Constructor.
			~GraphicsClass();
				Deconstructor.

			Initialize(HINSTANCE, HWND, int, int)
				CALL AFTER CREATION
				Used to initialize all the member variables of the
				GraphicsClass object.
			Shutdown()
				CALL BEFORE DELETION.
				Used to cleanup and free all memory used by the
				GraphicsClass object.
			Frame()
				Use to render a single frame of the scene.

			==================== PRIVATE ====================
			HandleMovementInput(float)
				Called by Frame() to handle user input every frame.
			Render()
				Called by Frame() to render every object.

			SetIntersectionText(bool intersection)
				Called by HandleMovementInput() to handle intersection testing.

Members:	==================== PRIVATE ====================
			InputClass* m_Input
				Used to handle all input for the GraphicsClass object.
			D3DClass* m_D3D
				A reference to the DirectX device and context being used.
			TimerClass* m_Timer
				A timer to be used for frametiming and other utilities.
			ShaderManagerClass* m_ShaderManager.
				An object to render objects using different shaders.
			PositionClass* m_Position.
				An object to keep track of the user's position.
			CameraClass* m_Camera
				An object to control the user's camera.
			LightClass* m_Light
				An Light object to be placed within the scene.

			ModelClass* m_Model1
				A model object within the scene.
			ModelClass* m_Model2
				A model object within the scene.
			BumpModelClass* m_Model3
				A model object within the scene.
			FireModelClass* m_Model4
				A model object within the scene.

			CollisionClass* m_CollisionObject.
				A utility object for various forms of collision checking.

			ModelClass* m_Model
				A model object within the scene.
			
			TextClassA* m_Text
				An object to keep track of all the text on the screen.
			BitmapClassA* m_Bitmap
				A class to provide various bitmapping functions.
			
			bool m_beginCheck
				A 'global' variable to handle mouse testing.

			ModelClass* m_IntersectTestCube
				A model object within the scene.
				Used to test a cube for AABB intersection.

			TextureGameObject, LightGameObject, BumpMapGameObject, FireShaderGameObject
				Used for testing the 4 types of GameObject.

			std::vector<GameObject*> renderingList.
				Used by the rendering loop.
				Items should be added to this to be rendered.
				The render function uses this to render every item.
C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C---C-C*/
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	//bool Render(float);
	//Xu
	bool HandleMovementInput(float);
	bool Render();

	void SetIntersectionText(bool intersection);

private:
	InputClass* m_Input;
	D3DClass* m_D3D;
	TimerClass* m_Timer;
	ShaderManagerClass* m_ShaderManager;
	PositionClass* m_Position;
	CameraClass* m_Camera;
	LightClass* m_Light;

	ModelClass* m_Model2;
	BumpModelClass* m_Model3;

	FireModelClass* m_Model4;

	CollisionClass* m_CollisionObject;

	TextClassA* m_Text;
	BitmapClassA* m_Bitmap;
	bool m_beginCheck;

	ModelClass* m_IntersectTestCube;

	TextureGameObject* m_GameObjectTestCube;
	LightGameObject* m_LightGameObjectTestCube;
	BumpMapGameObject* m_BumpMapGameObjectTestCube;
	FireShaderGameObject* m_FireShaderGameObjectTestCube;


	std::vector<GameObject*>* renderingList;

	

	

	
};

#endif