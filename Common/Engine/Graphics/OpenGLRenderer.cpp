#include "OpenGLRenderer.hpp"

#pragma region OpenGL_Function_Pointer_Definitions
//-----------------------------------------------------------------------------------------------
//Vertex Buffers
PFNGLBINDBUFFERPROC				  glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC				  glBufferData = nullptr;
PFNGLDELETEBUFFERSPROC			  glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC				  glGenBuffers = nullptr;
PFNGLPRIMITIVERESTARTINDEXPROC	  glPrimitiveRestartIndex = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC	  glVertexAttribPointer = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray = nullptr;

//Framebuffers
PFNGLBINDFRAMEBUFFERPROC		 glBindFramebuffer = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC	 glCheckFramebufferStatus = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC		 glDeleteFramebuffers = nullptr;
PFNGLDRAWBUFFERSPROC			 glDrawBuffers = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC	 glFramebufferTexture2D = nullptr;
PFNGLGENFRAMEBUFFERSPROC		 glGenFramebuffers = nullptr;

//Mipmaps
PFNGLGENERATEMIPMAPPROC	glGenerateMipmap = nullptr;

//Shaders
PFNGLBINDATTRIBLOCATIONPROC  glBindAttribLocation = nullptr;
PFNGLCOMPILESHADERPROC		  glCompileShader = nullptr;
PFNGLCREATESHADERPROC		  glCreateShader = nullptr;
PFNGLDELETEPROGRAMPROC		  glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC		  glDeleteShader = nullptr;
PFNGLDETACHSHADERPROC		  glDetachShader = nullptr;
PFNGLGETACTIVEATTRIBPROC	  glGetActiveAttrib = nullptr;
PFNGLGETACTIVEUNIFORMPROC	  glGetActiveUniform = nullptr;
PFNGLGETATTRIBLOCATIONPROC	  glGetAttribLocation = nullptr;
PFNGLGETSHADERINFOLOGPROC	  glGetShaderInfoLog = nullptr;
PFNGLGETSHADERIVPROC		  glGetShaderiv = nullptr;
PFNGLSHADERSOURCEPROC		  glShaderSource = nullptr;

//Shader Programs
PFNGLATTACHSHADERPROC		glAttachShader = nullptr;
PFNGLCREATEPROGRAMPROC		glCreateProgram = nullptr;
PFNGLGETPROGRAMIVPROC		glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC	glGetProgramInfoLog = nullptr;
PFNGLLINKPROGRAMPROC		glLinkProgram = nullptr;
PFNGLUSEPROGRAMPROC			glUseProgram = nullptr;

//Shader Uniform binding
PFNGLGETUNIFORMLOCATIONPROC	glGetUniformLocation = nullptr;

PFNGLUNIFORM1FPROC			glUniform1f = nullptr;
PFNGLUNIFORM2FPROC			glUniform2f = nullptr;
PFNGLUNIFORM3FPROC			glUniform3f = nullptr;
PFNGLUNIFORM4FPROC			glUniform4f = nullptr;

PFNGLUNIFORM1FVPROC			glUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC			glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC			glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC			glUniform4fv = nullptr;

PFNGLUNIFORM1IPROC			glUniform1i = nullptr;
PFNGLUNIFORM2IPROC			glUniform2i = nullptr;
PFNGLUNIFORM3IPROC			glUniform3i = nullptr;
PFNGLUNIFORM4IPROC			glUniform4i = nullptr;

PFNGLUNIFORM1IVPROC			glUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC			glUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC			glUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC			glUniform4iv = nullptr;

PFNGLUNIFORMMATRIX4FVPROC	glUniformMatrix4fv = nullptr;

//Textures
PFNGLACTIVETEXTUREPROC		glActiveTexture = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = nullptr;
#pragma endregion

#pragma region Renderer_to_OpenGL_Constant_Definitions
//-----------------------------------------------------------------------------------------------
STATIC const Renderer::ArrayType Renderer::COLOR_ARRAYS			= GL_COLOR_ARRAY;
STATIC const Renderer::ArrayType Renderer::TEXTURE_COORD_ARRAYS = GL_TEXTURE_COORD_ARRAY;
STATIC const Renderer::ArrayType Renderer::VERTEX_ARRAYS		= GL_VERTEX_ARRAY;

STATIC const Renderer::BufferType Renderer::ARRAY_BUFFER = GL_ARRAY_BUFFER;
STATIC const Renderer::BufferType Renderer::INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER;

STATIC const Renderer::ColorComponents Renderer::COMPONENTS_DEPTH	= GL_DEPTH_COMPONENT;
STATIC const Renderer::ColorComponents Renderer::COMPONENTS_STENCIL = GL_STENCIL_COMPONENTS;
STATIC const Renderer::ColorComponents Renderer::RGB				= GL_RGB;
STATIC const Renderer::ColorComponents Renderer::RGBA				= GL_RGBA;

STATIC const Renderer::ColorBlendingMode Renderer::NO_COLOR						= GL_ZERO;
STATIC const Renderer::ColorBlendingMode Renderer::ONE_MINUS_DESTINATION_COLOR	= GL_ONE_MINUS_DST_COLOR;
STATIC const Renderer::ColorBlendingMode Renderer::ONE_MINUS_SOURCE_ALPHA		= GL_ONE_MINUS_SRC_ALPHA;
STATIC const Renderer::ColorBlendingMode Renderer::SOURCE_ALPHA					= GL_SRC_ALPHA;

STATIC const Renderer::CoordinateType Renderer::SHORT_TYPE		= GL_SHORT;
STATIC const Renderer::CoordinateType Renderer::INTEGER_TYPE	= GL_INT;
STATIC const Renderer::CoordinateType Renderer::FLOAT_TYPE		= GL_FLOAT;
STATIC const Renderer::CoordinateType Renderer::DOUBLE_TYPE		= GL_DOUBLE;
STATIC const Renderer::CoordinateType Renderer::UNSIGNED_BYTE	= GL_UNSIGNED_BYTE;
STATIC const Renderer::CoordinateType Renderer::UNSIGNED_INTEGER	= GL_UNSIGNED_INT;
STATIC const Renderer::CoordinateType Renderer::UNSIGNED_SHORT_TYPE	= GL_UNSIGNED_SHORT;

STATIC const Renderer::Feature Renderer::COLOR_BLENDING	= GL_BLEND;
STATIC const Renderer::Feature Renderer::DEPTH_TESTING	= GL_DEPTH_TEST;
STATIC const Renderer::Feature Renderer::FACE_CULLING	= GL_CULL_FACE;
STATIC const Renderer::Feature Renderer::SHAPE_RESTART_INDEXING	= GL_PRIMITIVE_RESTART;
STATIC const Renderer::Feature Renderer::TEXTURES_2D	= GL_TEXTURE_2D;

STATIC const Renderer::QualityLevel Renderer::FASTEST = GL_FASTEST;

STATIC const Renderer::Shader Renderer::GEOMETRY_SHADER = GL_GEOMETRY_SHADER;
STATIC const Renderer::Shader Renderer::PIXEL_FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
STATIC const Renderer::Shader Renderer::VERTEX_SHADER = GL_VERTEX_SHADER;

STATIC const Renderer::Shape Renderer::POINTS			= GL_POINT;
STATIC const Renderer::Shape Renderer::LINES			= GL_LINES;
STATIC const Renderer::Shape Renderer::LINE_LOOP		= GL_LINE_LOOP;
STATIC const Renderer::Shape Renderer::LINE_STRIP		= GL_LINE_STRIP;
STATIC const Renderer::Shape Renderer::TRIANGLES		= GL_TRIANGLES;
STATIC const Renderer::Shape Renderer::TRIANGLE_STRIP	= GL_TRIANGLE_STRIP;
STATIC const Renderer::Shape Renderer::QUADS			= GL_QUADS;

STATIC const Renderer::TextureFilteringMethod Renderer::NEAREST_NEIGHBOR = GL_NEAREST;
STATIC const Renderer::TextureFilteringMethod Renderer::LINEAR_INTERPOLATION = GL_LINEAR;
STATIC const Renderer::TextureFilteringMethod Renderer::NEAREST_MIPMAP_NEAREST_TEXTURE = GL_NEAREST_MIPMAP_NEAREST;
STATIC const Renderer::TextureFilteringMethod Renderer::NEAREST_MIPMAP_INTERPOLATE_TEXTURES = GL_LINEAR_MIPMAP_NEAREST;
STATIC const Renderer::TextureFilteringMethod Renderer::INTERPOLATE_MIPMAPS_NEAREST_TEXTURE = GL_NEAREST_MIPMAP_LINEAR;
STATIC const Renderer::TextureFilteringMethod Renderer::INTERPOLATE_MIPMAPS_INTERPOLATE_TEXTURES = GL_LINEAR_MIPMAP_LINEAR;

STATIC const Renderer::TextureWrapMode Renderer::CLAMP_TO_EDGE		  = GL_CLAMP;
STATIC const Renderer::TextureWrapMode Renderer::REPEAT_OVER_GEOMETRY = GL_REPEAT;
#pragma endregion

//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::InitializeOpenGLFunctionPointers()
{
	//Vertex Buffers
	glBindBuffer				= ( PFNGLBINDBUFFERPROC ) wglGetProcAddress( "glBindBuffer" );
	glBufferData				= ( PFNGLBUFFERDATAPROC ) wglGetProcAddress( "glBufferData" );
	glDeleteBuffers				= ( PFNGLDELETEBUFFERSPROC ) wglGetProcAddress( "glDeleteBuffers" );
	glGenBuffers				= ( PFNGLGENBUFFERSPROC ) wglGetProcAddress( "glGenBuffers" );
	glPrimitiveRestartIndex		= ( PFNGLPRIMITIVERESTARTINDEXPROC ) wglGetProcAddress( "glPrimitiveRestartIndex" );
	glVertexAttribPointer		= ( PFNGLVERTEXATTRIBPOINTERPROC ) wglGetProcAddress( "glVertexAttribPointer" );
	glDisableVertexAttribArray	= ( PFNGLDISABLEVERTEXATTRIBARRAYPROC ) wglGetProcAddress( "glDisableVertexAttribArray" );
	glEnableVertexAttribArray	= ( PFNGLENABLEVERTEXATTRIBARRAYPROC ) wglGetProcAddress( "glEnableVertexAttribArray" );

	//Framebuffers
	glBindFramebuffer			= ( PFNGLBINDFRAMEBUFFERPROC ) wglGetProcAddress( "glBindFramebuffer" );
	glCheckFramebufferStatus	= ( PFNGLCHECKFRAMEBUFFERSTATUSPROC ) wglGetProcAddress( "glCheckFramebufferStatus" );
	glDeleteFramebuffers		= ( PFNGLDELETEFRAMEBUFFERSPROC ) wglGetProcAddress( "glDeleteFramebuffers" );
	glDrawBuffers				= ( PFNGLDRAWBUFFERSPROC ) wglGetProcAddress( "glDrawBuffers" );
	glFramebufferRenderbuffer	= ( PFNGLFRAMEBUFFERRENDERBUFFERPROC ) wglGetProcAddress( "glFramebufferRenderbuffer" );
	glFramebufferTexture2D		= ( PFNGLFRAMEBUFFERTEXTURE2DPROC ) wglGetProcAddress( "glFramebufferTexture2D" );
	glGenFramebuffers			= ( PFNGLGENFRAMEBUFFERSPROC ) wglGetProcAddress( "glGenFramebuffers" );

	//Mipmaps
	glGenerateMipmap = ( PFNGLGENERATEMIPMAPPROC ) wglGetProcAddress( "glGenerateMipmap" );

	//Shaders
	glCompileShader		= ( PFNGLCOMPILESHADERPROC ) wglGetProcAddress( "glCompileShader" );
	glCreateShader		= ( PFNGLCREATESHADERPROC ) wglGetProcAddress( "glCreateShader" );
	glDeleteProgram		= ( PFNGLDELETEPROGRAMPROC ) wglGetProcAddress( "glDeleteProgram" );
	glDeleteShader		= ( PFNGLDELETESHADERPROC ) wglGetProcAddress( "glDeleteShader" );
	glDetachShader		= ( PFNGLDETACHSHADERPROC ) wglGetProcAddress( "glDetachShader" );
	glGetShaderInfoLog	= ( PFNGLGETSHADERINFOLOGPROC ) wglGetProcAddress( "glGetShaderInfoLog" );
	glGetShaderiv		= ( PFNGLGETSHADERIVPROC ) wglGetProcAddress( "glGetShaderiv" );
	glShaderSource		= ( PFNGLSHADERSOURCEPROC ) wglGetProcAddress( "glShaderSource" );

	//Shader Programs
	glAttachShader		= ( PFNGLATTACHSHADERPROC ) wglGetProcAddress( "glAttachShader" );
	glBindAttribLocation = ( PFNGLBINDATTRIBLOCATIONPROC ) wglGetProcAddress( "glBindAttribLocation" );
	glCreateProgram		= ( PFNGLCREATEPROGRAMPROC ) wglGetProcAddress( "glCreateProgram" );
	glGetActiveAttrib	= ( PFNGLGETACTIVEATTRIBPROC ) wglGetProcAddress( "glGetActiveAttrib" );
	glGetActiveUniform	= ( PFNGLGETACTIVEUNIFORMPROC ) wglGetProcAddress( "glGetActiveUniform" );
	glGetAttribLocation = ( PFNGLGETATTRIBLOCATIONPROC ) wglGetProcAddress( "glGetAttribLocation" );
	glGetProgramiv		= ( PFNGLGETPROGRAMIVPROC ) wglGetProcAddress( "glGetProgramiv" );
	glGetUniformLocation = ( PFNGLGETUNIFORMLOCATIONPROC ) wglGetProcAddress( "glGetUniformLocation" );
	glGetProgramInfoLog	= ( PFNGLGETPROGRAMINFOLOGPROC ) wglGetProcAddress( "glGetProgramInfoLog" );
	glLinkProgram		= ( PFNGLLINKPROGRAMPROC ) wglGetProcAddress( "glLinkProgram" );
	glUseProgram		= ( PFNGLUSEPROGRAMPROC ) wglGetProcAddress( "glUseProgram" );

	glUniform1f = ( PFNGLUNIFORM1FPROC ) wglGetProcAddress( "glUniform1f" );
	glUniform2f = ( PFNGLUNIFORM2FPROC ) wglGetProcAddress( "glUniform2f" );
	glUniform3f = ( PFNGLUNIFORM3FPROC ) wglGetProcAddress( "glUniform3f" );
	glUniform4f = ( PFNGLUNIFORM4FPROC ) wglGetProcAddress( "glUniform4f" );

	glUniform1fv = ( PFNGLUNIFORM1FVPROC ) wglGetProcAddress( "glUniform1fv" );
	glUniform2fv = ( PFNGLUNIFORM2FVPROC ) wglGetProcAddress( "glUniform2fv" );
	glUniform3fv = ( PFNGLUNIFORM3FVPROC ) wglGetProcAddress( "glUniform3fv" );
	glUniform4fv = ( PFNGLUNIFORM4FVPROC ) wglGetProcAddress( "glUniform4fv" );

	glUniform1i = ( PFNGLUNIFORM1IPROC ) wglGetProcAddress( "glUniform1i" );
	glUniform2i = ( PFNGLUNIFORM2IPROC ) wglGetProcAddress( "glUniform2i" );
	glUniform3i = ( PFNGLUNIFORM3IPROC ) wglGetProcAddress( "glUniform3i" );
	glUniform4i = ( PFNGLUNIFORM4IPROC ) wglGetProcAddress( "glUniform4i" );

	glUniform1iv = ( PFNGLUNIFORM1IVPROC ) wglGetProcAddress( "glUniform1iv" );
	glUniform2iv = ( PFNGLUNIFORM2IVPROC ) wglGetProcAddress( "glUniform2iv" );
	glUniform3iv = ( PFNGLUNIFORM3IVPROC ) wglGetProcAddress( "glUniform3iv" );
	glUniform4iv = ( PFNGLUNIFORM4IVPROC ) wglGetProcAddress( "glUniform4iv" );

	glUniformMatrix4fv = ( PFNGLUNIFORMMATRIX4FVPROC ) wglGetProcAddress( "glUniformMatrix4fv" );

	//Textures
	glActiveTexture = ( PFNGLACTIVETEXTUREPROC ) wglGetProcAddress( "glActiveTexture" );
	glCompressedTexImage2D = ( PFNGLCOMPRESSEDTEXIMAGE2DPROC ) wglGetProcAddress( "glCompressedTexImage2D" );
}

//-----------------------------------------------------------------------------------------------
int OpenGLRenderer::CompileShader( Shader shaderType, const char* sourceString, std::string& out_errors )
{
	GLuint shaderID = glCreateShader( shaderType );

	static const GLsizei NUMBER_OF_STRINGS_TO_LOAD = 1;
	static const GLint* LENGTHS_OF_EACH_SOURCE_STRING = NULL;
	glShaderSource( shaderID, NUMBER_OF_STRINGS_TO_LOAD, ( const GLchar** )&sourceString, LENGTHS_OF_EACH_SOURCE_STRING );

	GLint compilationResult = GL_FALSE;
	glCompileShader( shaderID );
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compilationResult );

	if( compilationResult == GL_FALSE )
	{
		GLint errorStringLength = 0;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &errorStringLength );

		char* errorString = new char[ errorStringLength ];
		glGetShaderInfoLog( shaderID, errorStringLength, NULL, errorString );

		out_errors.assign( errorString, errorStringLength );
		delete[] errorString;
		return -1;
	}
	return shaderID;
}

//-----------------------------------------------------------------------------------------------
int OpenGLRenderer::CreateProgramFromShaders( int vertexShaderID, int geometryShaderID, int pixelFragmentShaderID, std::string& out_errors )
{
	GLuint programID = glCreateProgram();

	glAttachShader( programID, vertexShaderID );

	if( geometryShaderID != -1 )
		glAttachShader( programID, geometryShaderID );

	glAttachShader( programID, pixelFragmentShaderID );

	glBindAttribLocation( programID, Renderer::LOCATION_Vertex,	   "i_vertexWorldPosition" );
	glBindAttribLocation( programID, Renderer::LOCATION_Color,	   "i_vertexColor" );
	glBindAttribLocation( programID, Renderer::LOCATION_TextureUV, "i_textureCoordinates" );
	glBindAttribLocation( programID, Renderer::LOCATION_Normal,    "i_textureNormal" );
	glBindAttribLocation( programID, Renderer::LOCATION_Tangent,   "i_textureTangent" );
	glBindAttribLocation( programID, Renderer::LOCATION_Bitangent, "i_textureBitangent" );

	glBindAttribLocation( programID, Renderer::LOCATION_BoneIndex0,  "i_tendons[0].boneIndex"  );
	glBindAttribLocation( programID, Renderer::LOCATION_BoneWeight0, "i_tendons[0].boneWeight" );

	glBindAttribLocation( programID, Renderer::LOCATION_BoneIndex1,  "i_tendons[1].boneIndex"  );
	glBindAttribLocation( programID, Renderer::LOCATION_BoneWeight1, "i_tendons[1].boneWeight" );

	glBindAttribLocation( programID, Renderer::LOCATION_BoneIndex2,  "i_tendons[2].boneIndex"  );
	glBindAttribLocation( programID, Renderer::LOCATION_BoneWeight2, "i_tendons[2].boneWeight" );

	glBindAttribLocation( programID, Renderer::LOCATION_BoneIndex3,  "i_tendons[3].boneIndex"  );
	glBindAttribLocation( programID, Renderer::LOCATION_BoneWeight3, "i_tendons[3].boneWeight" );

	GLint linkingResult = GL_FALSE;
	glLinkProgram( programID );
	glGetProgramiv( programID, GL_LINK_STATUS, &linkingResult );

	if( linkingResult == GL_FALSE )
	{
		GLint errorStringLength = 0;
		glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &errorStringLength );

		char* errorString = new char[ errorStringLength ];
		glGetProgramInfoLog( programID, errorStringLength, NULL, errorString );

		out_errors.assign( errorString, errorStringLength );
		delete[] errorString;
		return -1;
	}

	return programID;
}
