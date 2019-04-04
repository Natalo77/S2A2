//==============================================
//		 Filename: graphicsclass.cpp
//==============================================


//==============================================
//			   User Defined Headers
//==============================================
#include "graphicsclass.h"
#include "ProjectileObject.h"


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GraphicsClass

Summary:	Default constructor for a GraphicsClass object.

Modifies:	[m_Input, m_D3D, m_Timer, m_ShaderManager, m_Light, m_Position,
			 m_Camera, m_Text, m_Bitmap, m_CollisionObject,
			 m_renderingList, m_GameObjectManager, bumpCube, metalNinja,
			 m_BulletModel].

Returns:	GraphicsClass
				the new GraphicsClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GraphicsClass::GraphicsClass()
{
	m_Input = 0;
	m_D3D = 0;
	m_Timer = 0;
	m_ShaderManager = 0;
	m_Light = 0;
	m_Position = 0;
	m_Camera = 0;

	m_Text = 0;
	m_Bitmap = 0;

	bumpCube = 0;
	metalNinja = 0;

	m_BulletModel = 0;

	m_CollisionObject = 0;
	m_GameObjectManager = new GameObjectManager();
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		GraphicsClass

Summary:	The reference constructor for a GraphicsClass object.

Args:		const GraphicsClass& other
				the GraphicsClass object to create this one in the image of.

Modifies:	[none].

Returns:	GraphicsClass
				the new GraphicsClass object.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		~GraphicsClass

Summary:	The default deconstructor for a GraphicsClass object.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
GraphicsClass::~GraphicsClass()
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Initialize

Summary:	Initializes and sets up all member variables of the
			GraphicsClass object.

Args:		HINSTANCE hinstance
				the screen instance being used.
			HWND hwnd
				a handle to the window being used.
			int screenWidth
				the screenWidth in pixels.
			int screenHeight
				the screenHeight in pixels.

Modifies:	[m_Input, m_D3D, m_ShaderManager, m_Timer, m_Position,
			 m_Camera, m_Light, m_Text, m_Bitmap,
			 m_CollisionObject, m_GameObjectManager, m_beginCheck,
			 metalNinja, bumpCube, m_BulletModel, m_BeginSpawn].

Returns:	bool
				was the initialization of all member variables successful.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GraphicsClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	XMMATRIX baseViewMatrix;

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	m_Position->SetPosition(0.0f, 0.0f, -10.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	// Create the camera object.
	m_Camera = new CameraClass;
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// Create the light object.
	m_Light = new LightClass;
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(64.0f);

	// Create the text object.
	m_Text = new TextClassA;
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClassA;
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/mouse.dds", 32, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	//Create the collision class object.
	m_CollisionObject = new CollisionClass;
	result = m_CollisionObject->Initialize(m_D3D, m_Camera);
	if (!result)
	{
		MessageBox(hwnd, L"Failed to initialize CollisionClass object", L"ERROR", MB_OK);
		return false;
	}

	//Initialize a blue cube
	ModelClass* testCube = new ModelClass();
	result = testCube->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/blue.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Failed to initialize GameoOject", L"ERROR", MB_OK);
		return false;
	}

	//Initialize the BulletModel.
	m_BulletModel = new ModelClass();
	result = m_BulletModel->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/bullet.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Failed to initialize bullet model", L"ERROR", MB_OK);
		return false;
	}

	//Create and add objects to the GameObject manager.
	{
		//Add 3 new Objects to the gameObjectManager.
		m_GameObjectManager->AddItem(GameObjectManager::OBJECTTYPE_STATIC, new TextureGameObject(testCube), 
			new XMFLOAT3(5.0f, 1.0f, 1.0f), new XMFLOAT3(0.0f, 45.0f, 0.0f), new XMFLOAT3(2.0f, 2.0f, 1.0f));
		m_GameObjectManager->AddItem(GameObjectManager::OBJECTTYPE_STATIC, new LightGameObject(testCube, m_Light, m_Camera),
			new XMFLOAT3(5.0f, 5.0f, 0.0f), new XMFLOAT3(45.0f, 0.0f, 0.0f), new XMFLOAT3(1.0f, 0.5, 1.0f));
		m_GameObjectManager->AddItem(GameObjectManager::OBJECTTYPE_STATIC, new LightGameObject(testCube, m_Light, m_Camera),
			new XMFLOAT3(-5.0f, 1.0f, 5.0f), new XMFLOAT3(0.0f, 0.0f, 0.0f), new XMFLOAT3(1.0f, 1.0f, 1.0f));

		//Create and add a bumpmap cube to the gameobjectmanager.
		BumpModelClass* testCubeBump = new BumpModelClass();
		result = testCubeBump->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/stone.dds",
			L"../Engine/data/normal.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Failed to initialize BumpGameObject", L"ERROR", MB_OK);
			return false;
		}
		m_GameObjectManager->AddItem(GameObjectManager::OBJECTTYPE_STATIC, new BumpMapGameObject(testCubeBump, m_Light),
			new XMFLOAT3(0.0f, 0.0f, -5.0f), new XMFLOAT3(45.0f, 0.0f, 0.0f), new XMFLOAT3(1.0f, 1.0f, 1.0f));

		//Create and add a fire animation ninja head to the gameObjectManager.
		FireModelClass* testCubeFire = new FireModelClass();
		result = testCubeFire->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/fire01.dds", //square or cube
			L"../Engine/data/noise01.dds", L"../Engine/data/alpha01.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Failed to initialize the firecube", L"ERROR", MB_OK);
			return false;
		}
		m_GameObjectManager->AddItem(GameObjectManager::OBJECTTYPE_STATIC, new FireShaderGameObject(testCubeFire),
			new XMFLOAT3(0.0f, 7.5f, 0.0f), new XMFLOAT3(0.0f, 0.0f, 45.0f), new XMFLOAT3(1.0f, 2.0f, 1.0f));

		//Create and add a firemodel cube to the gameObjectManager.
		FireModelClass* m_Model4 = new FireModelClass();
		result = m_Model4->Initialize(m_D3D->GetDevice(), "../Engine/data/new-ninjaHead.txt", L"../Engine/data/fire01.dds", //square or cube
			L"../Engine/data/noise01.dds", L"../Engine/data/alpha01.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the fourth model object.", L"Error", MB_OK);
			return false;
		}
		m_GameObjectManager->AddItem(GameObjectManager::OBJECTTYPE_STATIC, new FireShaderGameObject(m_Model4),
			new XMFLOAT3(0.0f, 2.0f, -1.0f), new XMFLOAT3(0.0f, 0.0f, 0.0f), new XMFLOAT3(0.03f, 0.03f, 0.03f));

		//Create and add a metal ninja head to the gameObjectManager.
		ModelClass* m_MetalNinja = new ModelClass;
		result = m_MetalNinja->Initialize(m_D3D->GetDevice(), "../Engine/data/new-ninjaHead.txt", L"../Engine/data/metal.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the second model object.", L"Error", MB_OK);
			return false;
		}
		metalNinja = new LightGameObject(m_MetalNinja, m_Light, m_Camera);
		m_GameObjectManager->AddItem(GameObjectManager::OBJECTTYPE_DYNAMIC, metalNinja,
			new XMFLOAT3(0.0f, -2.0f, 0.0f), new XMFLOAT3(0.0f, 0.0f, 0.0f), new XMFLOAT3(0.03f, 0.03f, 0.03f));

		//Create and add a stone cube to the gameObjectManager.
		BumpModelClass* m_StoneCube = new BumpModelClass();
		result = m_StoneCube->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/stone.dds",
			L"../Engine/data/normal.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the third model object.", L"Error", MB_OK);
			return false;
		}
		bumpCube = new BumpMapGameObject(m_StoneCube, m_Light);
		m_GameObjectManager->AddItem(GameObjectManager::OBJECTTYPE_DYNAMIC, bumpCube,
			new XMFLOAT3(3.5f, 0.0f, 0.0f), new XMFLOAT3(0.0f, 0.0f, 0.0f), new XMFLOAT3(1.0f, 1.0f, 1.0f));
	}

	// Initialize that the user has not clicked on the screen to try an intersection test yet.
	m_beginCheck = false;
	m_BeginSpawn = false;

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Shutdown

Summary:	Releases and depoints all member variables of the
			GraphicsClass object.

Modifies:	[m_Light, m_Camera,
			 m_Position, m_ShaderManager, m_Timer, m_D3D,
			 m_Input, m_Bitmap, m_Text, m_CollisionObject
			 m_GameObjectManager, metalNinja, bumpCube, m_BulletModel].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GraphicsClass::Shutdown()
{

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the shader manager object.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	//release the collision object.
	if (m_CollisionObject)
	{
		m_CollisionObject->Shutdown();
		delete m_CollisionObject;
		m_CollisionObject = 0;
	}

	//Release the GameOBjectManager
	if (m_GameObjectManager)
	{
		m_GameObjectManager->Shutdown();
		delete m_GameObjectManager;
		m_GameObjectManager = 0;
	}

	//Release the metalNinja gameObject.
	if (metalNinja)
	{
		delete metalNinja;
		metalNinja = 0;
	}
	
	//Release the bumpCube gameObject.
	if (bumpCube)
	{
		delete bumpCube;
		bumpCube = 0;
	}

	//Release the BulletModel ModelClass*
	if (m_BulletModel)
	{
		m_BulletModel->Shutdown();
		delete m_BulletModel;
		m_BulletModel = 0;
	}

	return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Frame

Summary:	Performs the actions required every frame:
				Timer update.
				Input handling.
				Rendering.

Modifies:	[none].

Returns:	bool
				did the frame execute successfully.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GraphicsClass::Frame()
{
	bool result;

	// Update the system stats.
	m_Timer->Frame();

	// Read the user input.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleMovementInput(m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		HandleMovementInput

Summary:	Handles all inputs appropriately.

Args:		float frameTime
				the amount of time that has passed between the last frame
				and this one.

Modifies:	[m_Position, m_beginCheck, m_BeginSpawn].

Returns:	bool
				was all movement handled succesfully.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GraphicsClass::HandleMovementInput(float frameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;
	int mouseX, mouseY;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsAPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsZPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Check if the left mouse button has been pressed.
	if (m_Input->IsLeftMouseButtonDown() == true)
	{
		// If they have clicked on the screen with the mouse then perform an intersection test.
		if (m_beginCheck == false)
		{
			m_beginCheck = true;

			//Get the mouse location on the screen.
			m_Input->GetMouseLocation(mouseX, mouseY);
				
			//Begin a ray checking loop.
			GameObject* collidedWith = m_CollisionObject->CollisionTestLoop(mouseX, mouseY, m_GameObjectManager, XMLoadFloat3(&m_Camera->GetPosition()));
			if (collidedWith != nullptr)
			{
				SetIntersectionText(true, 1);
				m_GameObjectManager->Delete(collidedWith);
			}
			else
				SetIntersectionText(false, 1);


		}
	}

	//If the right Mouse button is pressed, shoot a projectile.
	if (m_Input->IsRightMouseButtonDown() == true)
	{
		if (m_BeginSpawn == false)
		{

			m_BeginSpawn = true;

			ShootProjectile();
		}
	}

	// Check if the left/right mouse button has been released.
	if (m_Input->IsLeftMouseButtonDown() == false)
	{
		m_beginCheck = false;
	}
	if (m_Input->IsRightMouseButtonDown() == false)
	{
		m_BeginSpawn = false;
	}

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		Render

Summary:	Translates and renders each model.
			Requests the gameObjectManager to render all models it is
			concerned by, then renders models the GraphicsClass is
			responsible for.

Modifies:	[m_D3D].

Returns:	bool
				was the frame rendered succesfully.
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix, orthoMatrix;
	bool result;
	int mouseX, mouseY;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.0005f * m_Timer->GetTime();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the view, projection and ortho matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//Dynamic object alteration.
	GameObject* metalNinjaRef = m_GameObjectManager->SearchFor(GameObjectManager::OBJECTTYPE_DYNAMIC, metalNinja);
	if (metalNinjaRef != nullptr)
		metalNinjaRef->setRotation(0.0f, rotation, 0.0f);
	GameObject* bumpCubeRef = m_GameObjectManager->SearchFor(GameObjectManager::OBJECTTYPE_DYNAMIC, bumpCube);
	if (bumpCubeRef != nullptr)
		bumpCubeRef->setRotation(rotation / 3.0f, 0.0f, 0.0f);

	//Turn on alpha blending
	m_D3D->TurnOnAlphaBlending();

	//Use the gameObjectManager to render all the objects it holds.
	m_GameObjectManager->RenderAll(m_ShaderManager, m_D3D, m_Camera, viewMatrix, projectionMatrix, m_Text);

	// Get the location of the mouse from the input object and the ortho matrix.
	m_Input->GetMouseLocation(mouseX, mouseY);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Render the mouse cursor with the texture shader.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), mouseX, mouseY);  
	if (!result) 
		return false;
	result = m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), XMMatrixIdentity(), XMMatrixIdentity(), orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
		return false;

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext());
	if (!result)
		return false;

	// Turn off alpha blending.
	m_D3D->TurnOffAlphaBlending();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		SetIntersectionText

Summary:	Used to set the intersection test of the sphere-ray
			collision text in the top left corner.

Args:		bool intersection
				was there an intersection or not?
			GameObject* collided
				a pointer to the GameObject that was collided with.

Modifies:	[m_Text].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GraphicsClass::SetIntersectionText(bool intersection, float scoreToAdd)
{
	m_Text->SetIntersection(intersection, m_D3D->GetDeviceContext(), scoreToAdd);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
Method:		ShootProjectile

Summary:	Adds a new bullet LightGameObject to the gameObjetManager
			at the position of the camera.

Modifies:	[none].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void GraphicsClass::ShootProjectile()
{
	//Create an XMFLOAT3 for velocity and get the mouse co-ordinates.
	XMFLOAT3 mouseRayVelocity;
	int mouseX, mouseY;
	m_Input->GetMouseLocation(mouseX, mouseY);

	//Use the collision class to get a direction vector from the camera to the mouse co-ordinates.
	CollisionClass::GetRay(m_D3D, m_Camera, mouseRayVelocity, mouseX, mouseY);

	//Add a projectile into consideration by the gameObject using the calculated velocity.
	m_GameObjectManager->AddProjectile(new ProjectileObject(m_BulletModel, m_Light, m_Camera, &mouseRayVelocity), &m_Camera->GetPosition(), &m_Camera->GetRotation());
}

