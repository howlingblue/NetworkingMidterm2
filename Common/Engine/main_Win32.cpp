#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <cassert>
#include <crtdbg.h>
#include "TimeInterface.hpp"
#include "../../Common/Engine/Graphics/Renderer.hpp"
#include "../../Common/Engine/Graphics/Texture.hpp"
#include "../../Common/Engine/CommandLineManager.hpp"
#include "../../Common/Engine/StringConversion.hpp"
#include "../../Client/Code/Game/GameClient.hpp"
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma warning( disable : 4996 ) // Ignore warning about freopen (I prefer standard C++ libraries)

//-----------------------------------------------------------------------------------------------
#define UNUSED(x) (void)(x);

//-----------------------------------------------------------------------------------------------
bool g_isQuitting = false;
HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "Square Tag";
bool g_openConsole = true;

unsigned int GAME_WINDOW_WIDTH = 800;
unsigned int GAME_WINDOW_HEIGHT = 800;
unsigned int GAME_WINDOW_SCREEN_X_POS = 50;
unsigned int GAME_WINDOW_SCREEN_Y_POS = 50;

static const double LOCKED_FRAME_RATE_SECONDS = 1.0 / 60.0;
static GameClient* g_gameClient;

//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			g_isQuitting = true;
			return 0;

		case WM_KEYDOWN:
			if( asKey == VK_ESCAPE )
			{
				g_isQuitting = true;
				return 0;
			}
			else
			{
				bool wasProcessed = g_gameClient->HandleKeyDownEvent( asKey );
				if( wasProcessed ) 
					return 0;
			}
			break;
		case WM_KEYUP:
			bool wasProcessed = g_gameClient->HandleKeyUpEvent( asKey );
			if( wasProcessed ) 
				return 0;
			break;
	}

	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}

//-----------------------------------------------------------------------------------------------
HWND CreateConsoleWindow( const char* windowTitle )
{
	AllocConsole();
	AttachConsole( GetCurrentProcessId() );
	freopen( "CON", "w", stdout );
	SetConsoleTitle( windowTitle );
	HWND consoleWindow = FindWindow( nullptr, windowTitle );

	SetWindowPos( consoleWindow, nullptr, 50, 50, 1000, 600, SWP_NOZORDER );
	return consoleWindow;
}

//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow( const char* windowTitle, HINSTANCE applicationInstanceHandle )
{
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( WindowsMessageHandlingProcedure ); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" );
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );

	RECT windowRect = { GAME_WINDOW_SCREEN_X_POS, GAME_WINDOW_SCREEN_Y_POS, 
						GAME_WINDOW_SCREEN_X_POS + GAME_WINDOW_WIDTH, GAME_WINDOW_SCREEN_X_POS + GAME_WINDOW_HEIGHT };
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		TEXT( windowTitle ),
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL );

	ShowWindow( g_hWnd, SW_SHOW );
	SetForegroundWindow( g_hWnd );
	SetFocus( g_hWnd );

	g_displayDeviceContext = GetDC( g_hWnd );

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize			= sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion		= 1;
	pixelFormatDescriptor.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType	= PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits	= 24;
	pixelFormatDescriptor.cDepthBits	= 24;
	pixelFormatDescriptor.cAccumBits	= 0;
	pixelFormatDescriptor.cStencilBits	= 8;

	int pixelFormatCode = ChoosePixelFormat( g_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	g_openGLRenderingContext = wglCreateContext( g_displayDeviceContext );
	wglMakeCurrent( g_displayDeviceContext, g_openGLRenderingContext );

	//Enable blending/transparency
	glEnable( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//-----------------------------------------------------------------------------------------------
void RunMessagePump()
{
	MSG queuedMessage;
	for( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}

//-----------------------------------------------------------------------------------------------
void Update( double timeSpentLastFrameSeconds )
{
	g_gameClient->Update( timeSpentLastFrameSeconds );
}

//-----------------------------------------------------------------------------------------------
void Render()
{
	glClearColor( 0.898f, 0.792f, 0.713f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );

	g_gameClient->Render();

	SwapBuffers( g_displayDeviceContext );
}

//-----------------------------------------------------------------------------------------------
double WaitUntilNextFrameThenGiveFrameTime()
{
	static double targetTime = 0;
	targetTime = GetCurrentTimeSeconds() + LOCKED_FRAME_RATE_SECONDS;
	double timeNow = GetCurrentTimeSeconds();

	while( timeNow < targetTime )
	{
		timeNow = GetCurrentTimeSeconds();
	}

	return LOCKED_FRAME_RATE_SECONDS;
}

//-----------------------------------------------------------------------------------------------
void RunFrame()
{
	static double timeSpentLastFrameSeconds = 0.0;
	RunMessagePump();
	Update( LOCKED_FRAME_RATE_SECONDS );
	Render();
	timeSpentLastFrameSeconds = WaitUntilNextFrameThenGiveFrameTime();
}

//-----------------------------------------------------------------------------------------------
void HandleCommandLineOptions( CommandLine::OptionList options )
{
	for( unsigned int i = 0; i < options.size(); ++i )
	{
		CommandLine::Option& option = options[ i ];
		if( option.option == "gw" || option.option == "gamewindow" )
		{
			if( option.arguments.size() != 4 )
			{
				CommandLine::Manager::ReportCommandError( "Incorrect number of arguments to gamewindow option.\nUsage: --gamewindow <Screen X Position> <Screen Y Position> <Window Width> <Window Height>\n" );
				return;
			}
			
			GAME_WINDOW_SCREEN_X_POS = ConvertStringToUnsignedInt( option.arguments[ 0 ] );
			GAME_WINDOW_SCREEN_Y_POS = ConvertStringToUnsignedInt( option.arguments[ 1 ] );
			GAME_WINDOW_WIDTH = ConvertStringToUnsignedInt( option.arguments[ 2 ] );
			GAME_WINDOW_HEIGHT = ConvertStringToUnsignedInt( option.arguments[ 3 ] );
		}
		else if( option.option == "help" || option.option == "h" || option.option == "?" )
		{
			printf( "-gw\t--gamewindow\t<ScreenXPos> <ScreenYPos> <WindowWidth> <WindowHeight>");
		}
	}
}

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	InitializeTimer();

	//Console window is created first so that command line errors may be written to it
	/*HWND consoleWindow = */CreateConsoleWindow( "Vingine Console" );

	CommandLine::Manager::Create();

	CommandLine::Delegate mainCommandLineDelegate;
	mainCommandLineDelegate.Bind< &HandleCommandLineOptions >();
	CommandLine::Manager::AddObserver( mainCommandLineDelegate );
	CommandLine::Manager::RunCommandLine( commandLineString );

	if( !g_openConsole )
		FreeConsole();
		
	CreateOpenGLWindow( APP_NAME, applicationInstanceHandle );

	Renderer::CreateRenderer();
	Renderer* renderer = Renderer::GetRenderer();
	renderer->EnableFeature( Renderer::SHAPE_RESTART_INDEXING );

	g_gameClient = new GameClient( GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT );
	g_gameClient->Start( "5121", "127.0.0.1", "5000" );

	while( !g_isQuitting )	
	{
		RunFrame();
	}
	Texture::CleanUpTextureRepositories();

	if( g_openConsole )
		FreeConsole();

#if defined( _WIN32 ) && defined( _DEBUG )
	assert( _CrtCheckMemory() );
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
