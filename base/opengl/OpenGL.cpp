/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/opengl/OpenGL.h>
#include <base/UnsignedInteger.h>
#include <base/concurrency/SpinLock.h>
#include <base/dl/DynamicLinker.h>
#include <base/mathematics/Math.h>
#include <base/mathematics/Vector3D.h>

// TAG: functions imported with glXGetProcAddressARB are context independent (GLX 1.3) (may be static)
// TAG: this means that we can use dynamic linker class
// TAG: on win32: functions are context-dependent (must be non-static)

// TAG: put in static description
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define _DK_SDU_MIP__BASE__OPENGL_LIBRARY "OPENGL32.DLL"
#else // win32
#  define _DK_SDU_MIP__BASE__OPENGL_LIBRARY "libGL.so"
#endif // flavor

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define CALL_OPENGL _DK_SDU_MIP__BASE__CALL_PASCAL
#else
#  define CALL_OPENGL
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: how should we cast to functions and methods
template<class RESULT>
inline RESULT method_cast(void* value) throw() {
  // sizeof(RESULT) == sizeof(void*)
  // static_cast<bool>(static_cast<RESULT>(0))
  union {
    void* value;
    RESULT method;
  } _DK_SDU_MIP__BASE__PACKED temp;
  temp.method = 0;
  temp.value = value;
  return temp.method;
}

#define GL(name) {STRINGIFY(gl##name), (OpenGL::FunctionPointer)&OpenGL::gl##name}

namespace opengl {

  // TAG: pthread_once functionality is better
  SpinLock spinLock;
  DynamicLinker* dynamicLinker = 0;
  
  typedef OpenGL::Function (CALL_OPENGL *GetFunction)(const char* name);
  GetFunction getFunction = 0;  

  void CALL_OPENGL missing() throw(OpenGLException) {
    // must always raise exception (to work with any calling convention/argument list)
    throw OpenGLException("Function not supported", Type::getType<OpenGL>());
  }
  
  OpenGL::Descriptor OPEN_GL_1_1[] = {
    GL(Accum),
    GL(AlphaFunc),
    GL(AreTexturesResident),
    GL(ArrayElement),
    GL(Begin),
    GL(BindTexture),
    GL(Bitmap),
    GL(BlendFunc),
    GL(CallList),
    GL(CallLists),
    GL(Clear),
    GL(ClearAccum),
    GL(ClearColor),
    GL(ClearDepth),
    GL(ClearIndex),
    GL(ClearStencil),
    GL(ClipPlane),
    GL(Color3b),
    GL(Color3bv),
    GL(Color3d),
    GL(Color3dv),
    GL(Color3f),
    GL(Color3fv),
    GL(Color3i),
    GL(Color3iv),
    GL(Color3s),
    GL(Color3sv),
    GL(Color3ub),
    GL(Color3ubv),
    GL(Color3ui),
    GL(Color3uiv),
    GL(Color3us),
    GL(Color3usv),
    GL(Color4b),
    GL(Color4bv),
    GL(Color4d),
    GL(Color4dv),
    GL(Color4f),
    GL(Color4fv),
    GL(Color4i),
    GL(Color4iv),
    GL(Color4s),
    GL(Color4sv),
    GL(Color4ub),
    GL(Color4ubv),
    GL(Color4ui),
    GL(Color4uiv),
    GL(Color4us),
    GL(Color4usv),
    GL(ColorMask),
    GL(ColorMaterial),
    GL(ColorPointer),
    GL(CopyPixels),
    GL(CopyTexImage1D),
    GL(CopyTexImage2D),
    GL(CopyTexSubImage1D),
    GL(CopyTexSubImage2D),
    GL(CullFace),
    GL(DeleteLists),
    GL(DeleteTextures),
    GL(DepthFunc),
    GL(DepthMask),
    GL(DepthRange),
    GL(Disable),
    GL(DisableClientState),
    GL(DrawArrays),
    GL(DrawBuffer),
    GL(DrawElements),
    GL(DrawPixels),
    GL(EdgeFlag),
    GL(EdgeFlagPointer),
    GL(EdgeFlagv),
    GL(Enable),
    GL(EnableClientState),
    GL(End),
    GL(EndList),
    GL(EvalCoord1d),
    GL(EvalCoord1dv),
    GL(EvalCoord1f),
    GL(EvalCoord1fv),
    GL(EvalCoord2d),
    GL(EvalCoord2dv),
    GL(EvalCoord2f),
    GL(EvalCoord2fv),
    GL(EvalMesh1),
    GL(EvalMesh2),
    GL(EvalPoint1),
    GL(EvalPoint2),
    GL(FeedbackBuffer),
    GL(Finish),
    GL(Flush),
    GL(Fogf),
    GL(Fogfv),
    GL(Fogi),
    GL(Fogiv),
    GL(FrontFace),
    GL(Frustum),
    GL(GenLists),
    GL(GenTextures),
    GL(GetBooleanv),
    GL(GetClipPlane),
    GL(GetDoublev),
    GL(GetError),
    GL(GetFloatv),
    GL(GetIntegerv),
    GL(GetLightfv),
    GL(GetLightiv),
    GL(GetMapdv),
    GL(GetMapfv),
    GL(GetMapiv),
    GL(GetMaterialfv),
    GL(GetMaterialiv),
    GL(GetPixelMapfv),
    GL(GetPixelMapuiv),
    GL(GetPixelMapusv),
    GL(GetPointerv),
    GL(GetPolygonStipple),
    GL(GetString),
    GL(GetTexEnvfv),
    GL(GetTexEnviv),
    GL(GetTexGendv),
    GL(GetTexGenfv),
    GL(GetTexGeniv),
    GL(GetTexImage),
    GL(GetTexLevelParameterfv),
    GL(GetTexLevelParameteriv),
    GL(GetTexParameterfv),
    GL(GetTexParameteriv),
    GL(Hint),
    GL(IndexMask),
    GL(IndexPointer),
    GL(Indexd),
    GL(Indexdv),
    GL(Indexf),
    GL(Indexfv),
    GL(Indexi),
    GL(Indexiv),
    GL(Indexs),
    GL(Indexsv),
    GL(Indexub),
    GL(Indexubv),
    GL(InitNames),
    GL(InterleavedArrays),
    GL(IsEnabled),
    GL(IsList),
    GL(IsTexture),
    GL(LightModelf),
    GL(LightModelfv),
    GL(LightModeli),
    GL(LightModeliv),
    GL(Lightf),
    GL(Lightfv),
    GL(Lighti),
    GL(Lightiv),
    GL(LineStipple),
    GL(LineWidth),
    GL(ListBase),
    GL(LoadIdentity),
    GL(LoadMatrixd),
    GL(LoadMatrixf),
    GL(LoadName),
    GL(LogicOp),
    GL(Map1d),
    GL(Map1f),
    GL(Map2d),
    GL(Map2f),
    GL(MapGrid1d),
    GL(MapGrid1f),
    GL(MapGrid2d),
    GL(MapGrid2f),
    GL(Materialf),
    GL(Materialfv),
    GL(Materiali),
    GL(Materialiv),
    GL(MatrixMode),
    GL(MultMatrixd),
    GL(MultMatrixf),
    GL(NewList),
    GL(Normal3b),
    GL(Normal3bv),
    GL(Normal3d),
    GL(Normal3dv),
    GL(Normal3f),
    GL(Normal3fv),
    GL(Normal3i),
    GL(Normal3iv),
    GL(Normal3s),
    GL(Normal3sv),
    GL(NormalPointer),
    GL(Ortho),
    GL(PassThrough),
    GL(PixelMapfv),
    GL(PixelMapuiv),
    GL(PixelMapusv),
    GL(PixelStoref),
    GL(PixelStorei),
    GL(PixelTransferf),
    GL(PixelTransferi),
    GL(PixelZoom),
    GL(PointSize),
    GL(PolygonMode),
    GL(PolygonOffset),
    GL(PolygonStipple),
    GL(PopAttrib),
    GL(PopClientAttrib),
    GL(PopMatrix),
    GL(PopName),
    GL(PrioritizeTextures),
    GL(PushAttrib),
    GL(PushClientAttrib),
    GL(PushMatrix),
    GL(PushName),
    GL(RasterPos2d),
    GL(RasterPos2dv),
    GL(RasterPos2f),
    GL(RasterPos2fv),
    GL(RasterPos2i),
    GL(RasterPos2iv),
    GL(RasterPos2s),
    GL(RasterPos2sv),
    GL(RasterPos3d),
    GL(RasterPos3dv),
    GL(RasterPos3f),
    GL(RasterPos3fv),
    GL(RasterPos3i),
    GL(RasterPos3iv),
    GL(RasterPos3s),
    GL(RasterPos3sv),
    GL(RasterPos4d),
    GL(RasterPos4dv),
    GL(RasterPos4f),
    GL(RasterPos4fv),
    GL(RasterPos4i),
    GL(RasterPos4iv),
    GL(RasterPos4s),
    GL(RasterPos4sv),
    GL(ReadBuffer),
    GL(ReadPixels),
    GL(Rectd),
    GL(Rectdv),
    GL(Rectf),
    GL(Rectfv),
    GL(Recti),
    GL(Rectiv),
    GL(Rects),
    GL(Rectsv),
    GL(RenderMode),
    GL(Rotated),
    GL(Rotatef),
    GL(Scaled),
    GL(Scalef),
    GL(Scissor),
    GL(SelectBuffer),
    GL(ShadeModel),
    GL(StencilFunc),
    GL(StencilMask),
    GL(StencilOp),
    GL(TexCoord1d),
    GL(TexCoord1dv),
    GL(TexCoord1f),
    GL(TexCoord1fv),
    GL(TexCoord1i),
    GL(TexCoord1iv),
    GL(TexCoord1s),
    GL(TexCoord1sv),
    GL(TexCoord2d),
    GL(TexCoord2dv),
    GL(TexCoord2f),
    GL(TexCoord2fv),
    GL(TexCoord2i),
    GL(TexCoord2iv),
    GL(TexCoord2s),
    GL(TexCoord2sv),
    GL(TexCoord3d),
    GL(TexCoord3dv),
    GL(TexCoord3f),
    GL(TexCoord3fv),
    GL(TexCoord3i),
    GL(TexCoord3iv),
    GL(TexCoord3s),
    GL(TexCoord3sv),
    GL(TexCoord4d),
    GL(TexCoord4dv),
    GL(TexCoord4f),
    GL(TexCoord4fv),
    GL(TexCoord4i),
    GL(TexCoord4iv),
    GL(TexCoord4s),
    GL(TexCoord4sv),
    GL(TexCoordPointer),
    GL(TexEnvf),
    GL(TexEnvfv),
    GL(TexEnvi),
    GL(TexEnviv),
    GL(TexGend),
    GL(TexGendv),
    GL(TexGenf),
    GL(TexGenfv),
    GL(TexGeni),
    GL(TexGeniv),
    GL(TexImage1D),
    GL(TexImage2D),
    GL(TexParameterf),
    GL(TexParameterfv),
    GL(TexParameteri),
    GL(TexParameteriv),
    GL(TexSubImage1D),
    GL(TexSubImage2D),
    GL(Translated),
    GL(Translatef),
    GL(Vertex2d),
    GL(Vertex2dv),
    GL(Vertex2f),
    GL(Vertex2fv),
    GL(Vertex2i),
    GL(Vertex2iv),
    GL(Vertex2s),
    GL(Vertex2sv),
    GL(Vertex3d),
    GL(Vertex3dv),
    GL(Vertex3f),
    GL(Vertex3fv),
    GL(Vertex3i),
    GL(Vertex3iv),
    GL(Vertex3s),
    GL(Vertex3sv),
    GL(Vertex4d),
    GL(Vertex4dv),
    GL(Vertex4f),
    GL(Vertex4fv),
    GL(Vertex4i),
    GL(Vertex4iv),
    GL(Vertex4s),
    GL(Vertex4sv),
    GL(VertexPointer),
    GL(Viewport),
    GL(ColorTable),
    GL(ColorSubTable),
    GL(ColorTableParameteriv),
    GL(ColorTableParameterfv),
    GL(CopyColorSubTable),
    GL(CopyColorTable),
    GL(GetColorTable),
    GL(GetColorTableParameterfv),
    GL(GetColorTableParameteriv),
    GL(BlendEquation),
    GL(BlendColor),
    GL(Histogram),
    GL(ResetHistogram),
    GL(GetHistogram),
    GL(GetHistogramParameterfv),
    GL(GetHistogramParameteriv),
    GL(Minmax),
    GL(ResetMinmax),
    GL(GetMinmax),
    GL(GetMinmaxParameterfv),
    GL(GetMinmaxParameteriv),
    GL(ConvolutionFilter1D),
    GL(ConvolutionFilter2D),
    GL(ConvolutionParameterf),
    GL(ConvolutionParameterfv),
    GL(ConvolutionParameteri),
    GL(ConvolutionParameteriv),
    GL(CopyConvolutionFilter1D),
    GL(CopyConvolutionFilter2D),
    GL(GetConvolutionFilter),
    GL(GetConvolutionParameterfv),
    GL(GetConvolutionParameteriv),
    GL(SeparableFilter2D),
    GL(GetSeparableFilter),
    GL(DrawRangeElements),
    GL(TexImage3D),
    GL(TexSubImage3D),
    GL(CopyTexSubImage3D)
  };
    
  OpenGL::Descriptor OPEN_GL_1_2[] = {
    GL(BlendColor),
    GL(BlendEquation),
    GL(ColorTable),
    GL(ColorTableParameterfv),
    GL(ColorTableParameteriv),
    GL(CopyColorTable),
    GL(GetColorTable),
    GL(GetColorTableParameterfv),
    GL(GetColorTableParameteriv),
    GL(ColorSubTable),
    GL(CopyColorSubTable),
    GL(ConvolutionFilter1D),
    GL(ConvolutionFilter2D),
    GL(ConvolutionParameterf),
    GL(ConvolutionParameterfv),
    GL(ConvolutionParameteri),
    GL(ConvolutionParameteriv),
    GL(CopyConvolutionFilter1D),
    GL(CopyConvolutionFilter2D),
    GL(GetConvolutionFilter),
    GL(GetConvolutionParameterfv),
    GL(GetConvolutionParameteriv),
    GL(GetSeparableFilter),
    GL(SeparableFilter2D),
    GL(GetHistogram),
    GL(GetHistogramParameterfv),
    GL(GetHistogramParameteriv),
    GL(GetMinmax),
    GL(GetMinmaxParameterfv),
    GL(GetMinmaxParameteriv),
    GL(Histogram),
    GL(Minmax),
    GL(ResetHistogram),
    GL(ResetMinmax),
    GL(DrawRangeElements),
    GL(TexImage3D),
    GL(TexSubImage3D),
    GL(CopyTexSubImage3D)
  };

  OpenGL::Descriptor OPEN_GL_1_3[] = {
    GL(ActiveTexture),
    GL(ClientActiveTexture),
    GL(MultiTexCoord1d),
    GL(MultiTexCoord1dv),
    GL(MultiTexCoord1f),
    GL(MultiTexCoord1fv),
    GL(MultiTexCoord1i),
    GL(MultiTexCoord1iv),
    GL(MultiTexCoord1s),
    GL(MultiTexCoord1sv),
    GL(MultiTexCoord2d),
    GL(MultiTexCoord2dv),
    GL(MultiTexCoord2f),
    GL(MultiTexCoord2fv),
    GL(MultiTexCoord2i),
    GL(MultiTexCoord2iv),
    GL(MultiTexCoord2s),
    GL(MultiTexCoord2sv),
    GL(MultiTexCoord3d),
    GL(MultiTexCoord3dv),
    GL(MultiTexCoord3f),
    GL(MultiTexCoord3fv),
    GL(MultiTexCoord3i),
    GL(MultiTexCoord3iv),
    GL(MultiTexCoord3s),
    GL(MultiTexCoord3sv),
    GL(MultiTexCoord4d),
    GL(MultiTexCoord4dv),
    GL(MultiTexCoord4f),
    GL(MultiTexCoord4fv),
    GL(MultiTexCoord4i),
    GL(MultiTexCoord4iv),
    GL(MultiTexCoord4s),
    GL(MultiTexCoord4sv),
    GL(LoadTransposeMatrixf),
    GL(LoadTransposeMatrixd),
    GL(MultTransposeMatrixf),
    GL(MultTransposeMatrixd),
    GL(CompressedTexImage3D),
    GL(CompressedTexImage2D),
    GL(CompressedTexImage1D),
    GL(CompressedTexSubImage3D),
    GL(CompressedTexSubImage2D),
    GL(CompressedTexSubImage1D),
    GL(GetCompressedTexImage),
    GL(SampleCoverage)
  };

  OpenGL::Descriptor OPEN_GL_1_4[] = {
    GL(FogCoordf),
    GL(FogCoordfv),
    GL(FogCoordd),
    GL(FogCoorddv),
    GL(FogCoordPointer),
    GL(MultiDrawArrays),
    GL(MultiDrawElements),
    GL(PointParameterf),
    GL(PointParameterfv),
    GL(SecondaryColor3b),
    GL(SecondaryColor3bv),
    GL(SecondaryColor3d),
    GL(SecondaryColor3dv),
    GL(SecondaryColor3f),
    GL(SecondaryColor3fv),
    GL(SecondaryColor3i),
    GL(SecondaryColor3iv),
    GL(SecondaryColor3s),
    GL(SecondaryColor3sv),
    GL(SecondaryColor3ub),
    GL(SecondaryColor3ubv),
    GL(SecondaryColor3ui),
    GL(SecondaryColor3uiv),
    GL(SecondaryColor3us),
    GL(SecondaryColor3usv),
    GL(SecondaryColorPointer),
    GL(BlendFuncSeparate),
    GL(WindowPos2d),
    GL(WindowPos2f),
    GL(WindowPos2i),
    GL(WindowPos2s),
    GL(WindowPos2dv),
    GL(WindowPos2fv),
    GL(WindowPos2iv),
    GL(WindowPos2sv),
    GL(WindowPos3d),
    GL(WindowPos3f),
    GL(WindowPos3i),
    GL(WindowPos3s),
    GL(WindowPos3dv),
    GL(WindowPos3fv),
    GL(WindowPos3iv),
    GL(WindowPos3sv)
  };

#undef GL

}; // end of opengl namespace

OpenGL::Function OpenGL::getFunction(const StringLiteral& name) throw() {
  return opengl::getFunction(name);
}

OpenGL::Function OpenGL::getFunction(const String& name) throw() {
  return opengl::getFunction(name.getElements());
}

void OpenGL::loadFunctions(Descriptor* descriptor, unsigned int size) throw() {
  const Descriptor* end = descriptor + size;
  while (descriptor < end) {
    ASSERT(descriptor->name && !descriptor->function);
    if (opengl::getFunction) {
      this->*(descriptor->function) = opengl::getFunction(descriptor->name);
    }
    if (this->*(descriptor->function) == 0) {
      this->*(descriptor->function) =
        (OpenGL::Function)opengl::dynamicLinker->getUncertainSymbol(descriptor->name);
      if (this->*(descriptor->function) == 0) {
    fout << "gl function: " << descriptor->name << " NOT AVAILABLE" << ENDL;
        this->*(descriptor->function) = (OpenGL::Function)&opengl::missing;
      }
    }
    ++descriptor;
  }
}

void OpenGL::fixMissing(Descriptor* descriptor, unsigned int size) throw() {
  const Descriptor* end = descriptor + size;
  while (descriptor < end) {
    ASSERT(descriptor->name && !descriptor->function);
    if (opengl::getFunction) {
      this->*(descriptor->function) = opengl::getFunction(descriptor->name);
    }
    if (this->*(descriptor->function) == 0) {
      this->*(descriptor->function) = (OpenGL::Function)&opengl::missing;
    }
    ++descriptor;
  }
}

OpenGL::OpenGL(unsigned int latest) throw(OpenGLException) {
  // TAG: determine supported
  opengl::spinLock.exclusiveLock();
  specification = 0x000000;
  // library is never released
  opengl::dynamicLinker = new DynamicLinker(MESSAGE(_DK_SDU_MIP__BASE__OPENGL_LIBRARY));
  assert(opengl::dynamicLinker, OpenGLException("Unable to load OpenGL module", this));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  opengl::getFunction =
    method_cast<opengl::GetFunction>(
      opengl::dynamicLinker->getUncertainSymbol(MESSAGE("wglGetProcAddress"))
    );
  if (!opengl::getFunction) {
    latest = 0x010100;
  }
#else // unix (glx)
  // glXGetProcAddressARB is only available from GLX 1.3
  opengl::getFunction =
    method_cast<opengl::GetFunction>(
      opengl::dynamicLinker->getUncertainSymbol(MESSAGE("glXGetProcAddressARB"))
    );
  // we do not limit spec 'cause glXGetProcAddressARB would return context independent functions
#endif // flavor
  GetString glGetString =
    method_cast<GetString>(
      opengl::dynamicLinker->getUncertainSymbol(MESSAGE("glGetString"))
    );
  assert(glGetString, OpenGLException(this));
  
  const GLubyte* version = glGetString(OpenGL::VERSION); // loaded above
  
  String temp(Cast::pointer<const char*>(version));
  fout << temp << ENDL;
  int index = temp.indexOf(' ');
  if (index >= 0) {
    temp.removeFrom(index); // remove implementation specific
  }
  unsigned int major = 1;
  unsigned int minor = 0;
  unsigned int release = 0;
  unsigned int start = 0;
  index = temp.indexOf('.', start);
  if (index >= 0) {
    major = UnsignedInteger::parse(temp.substring(start, index), UnsignedInteger::DEC);
    start = index + 1;
    index = temp.indexOf('.', start);
    if (index >= 0) {
      minor = UnsignedInteger::parse(temp.substring(start, index), UnsignedInteger::DEC);
      start = index + 1;
      if (start < temp.getLength()) {
        release = UnsignedInteger::parse(temp.substring(start), UnsignedInteger::DEC);
      }
    }
  }
  specification = (major << 16) | (minor << 8) | release;
  unsigned int loadSpecification = minimum(specification, latest);
  
  if (loadSpecification >= 0x010100) {
    loadFunctions(opengl::OPEN_GL_1_1, getArraySize(opengl::OPEN_GL_1_1));
    if (loadSpecification >= 0x010200) {
      loadFunctions(opengl::OPEN_GL_1_2, getArraySize(opengl::OPEN_GL_1_2));
      if (loadSpecification >= 0x010300) {
        loadFunctions(opengl::OPEN_GL_1_3, getArraySize(opengl::OPEN_GL_1_3));
        if (loadSpecification >= 0x010400) {
          loadFunctions(opengl::OPEN_GL_1_4, getArraySize(opengl::OPEN_GL_1_4));
        }
      }
    }
  }
  if (loadSpecification < 0x010100) {
    fixMissing(opengl::OPEN_GL_1_1, getArraySize(opengl::OPEN_GL_1_1));
  }
  if (loadSpecification < 0x010200) {
    fixMissing(opengl::OPEN_GL_1_2, getArraySize(opengl::OPEN_GL_1_2));
  }
  if (loadSpecification < 0x010300) {
    fixMissing(opengl::OPEN_GL_1_3, getArraySize(opengl::OPEN_GL_1_3));
  }
  if (loadSpecification < 0x010400) {
    fixMissing(opengl::OPEN_GL_1_4, getArraySize(opengl::OPEN_GL_1_4));
  }
  opengl::spinLock.releaseLock();
}

bool OpenGL::isSupported(const String& name) throw(OpenGLException) {
  assert(name.isProper() && (name.indexOf(' ') < 0), OpenGLException(Type::getType<OpenGL>()));
  const GLubyte* extensions = glGetString(OpenGL::EXTENSIONS);
  String temp(Cast::pointer<const char*>(extensions));
  int start = 0;
  while (true) {
    int index = temp.indexOf(name, start);
    if (index < 0) {
      return false;
    }
    start = index + name.getLength();
    if (((index == 0) || (temp[index - 1] == ' ')) &&
        ((temp.getLength() == start) || (temp[start] == ' '))) {
      return true;
    }
  }
}

void OpenGL::lookAt(const Vector3D<double>& eye, const Vector3D<double>& center, const Vector3D<double>& up) throw() {
  Vector3D<double> sight = eye - center;
  sight /= sight.getModulus();
  Vector3D<double> x(cross(up, sight));
  Vector3D<double> newUp = cross(sight, x);
  newUp /= newUp.getModulus();
  x /= x.getModulus();
  
  GLdouble matrix[4][4]; // column-major order
  matrix[0][0] = x.getX();
  matrix[1][0] = x.getY();
  matrix[2][0] = x.getZ();
  matrix[3][0] = 0;

  matrix[0][1] = newUp.getX();
  matrix[1][1] = newUp.getY();
  matrix[2][1] = newUp.getZ();
  matrix[3][1] = 0;
  
  matrix[0][2] = sight.getX();
  matrix[1][2] = sight.getY();
  matrix[2][2] = sight.getZ();
  matrix[3][2] = 0;

  matrix[0][3] = 0;
  matrix[1][3] = 0;
  matrix[2][3] = 0;
  matrix[3][3] = 1;
  
  glMultMatrixd(Cast::pointer<const GLdouble*>(&matrix));
  glTranslated(-eye.getX(), -eye.getY(), -eye.getZ()); // map eye point to origin
}

#if 0
void OpenGL::frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearDistance, GLdouble farDistance) throw() {
  // see OpenGL specification
  GLdouble matrix[4][4]; // column-major order
  matrix[0][0] = (2 * nearDistance)/(right - left);
  matrix[1][0] = 0;
  matrix[2][0] = (right + left)/(right - left);
  matrix[3][0] = 0;

  matrix[0][1] = 0;
  matrix[1][1] = (2 * nearDistance)/(top - bottom);
  matrix[2][1] = (top + bottom)/(top - bottom);
  matrix[3][1] = 0;

  matrix[0][2] = 0;
  matrix[1][2] = 0;
  matrix[2][2] = -(farDistance + nearDistance)/(farDistance - nearDistance);
  matrix[3][2] = -(2 * farDistance * nearDistance)/(farDistance - nearDistance);

  matrix[0][3] = 0;
  matrix[1][3] = 0;
  matrix[2][3] = -1;
  matrix[3][3] = 0;
  glMultMatrixd(Cast::pointer<const GLdouble*>(&matrix));
}
#endif

void OpenGL::perspective(GLdouble fovy, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar) throw() {  
  GLdouble ymax = zNear * Math::tan(fovy * constant::PI/360.0);
  GLdouble ymin = -ymax;
  GLdouble xmin = ymin * aspectRatio;
  GLdouble xmax = ymax * aspectRatio;
  glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

Vector3D<double> OpenGL::project(Vector3D<double> object, const Matrix4x4<double> model, const Matrix4x4<double> projection, const GLint viewport[4]) throw(OpenGLException) {
  Quaternion<double> in(object.getX(), object.getY(), object.getZ(), 1);
  Quaternion<double> out = model * in;
  in = projection * out;
  assert(in.getW() != 0, OpenGLException(Type::getType<OpenGL>()));
  in /= in.getW();
  return Vector3D<double>(
    viewport[0] + (1 + in.getX()) * 0.5 * viewport[2],
    viewport[1] + (1 + in.getY()) * 0.5 * viewport[3],
    (1 + in.getZ())/2
  );
}

void OpenGL::pickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height, GLint viewport[4]) throw() {
  GLdouble matrix[4][4]; // column-major order
  matrix[0][0] = viewport[2]/width;
  matrix[1][0] = 0;
  matrix[2][0] = 0;
  matrix[3][0] = (viewport[2] + 2.0 * (viewport[0] - x))/width;

  matrix[0][1] = 0;
  matrix[1][1] = viewport[3]/height;
  matrix[2][1] = 0;
  matrix[3][1] = (viewport[3] + 2.0 * (viewport[1] - y))/height;

  matrix[0][2] = 0;
  matrix[1][2] = 0;
  matrix[2][2] = 1;
  matrix[3][2] = 0;

  matrix[0][3] = 0;
  matrix[1][3] = 0;
  matrix[2][3] = 0;
  matrix[3][3] = 1;
  
  glMultMatrixd(Cast::pointer<const GLdouble*>(&matrix));
}

Vector3D<double> OpenGL::unProject(const Vector3D<double>& window, const Matrix4x4<double>& model, const Matrix4x4<double>& projection, const GLint viewport[4]) throw(OpenGLException) {
  Matrix4x4<double> A = projection * model;
  Matrix4x4<double> matrix;
  matrix.inverse(A);
  
  Quaternion<double> in(
    2 * (window.getX() - viewport[0])/viewport[2] - 1,
    2 * (window.getY() - viewport[1])/viewport[3] - 1,
    2 * window.getZ() - 1,
    1
  );
  
  Quaternion<double> object = matrix * in;
  assert(object.getW() != 0, OpenGLException(Type::getType<OpenGL>()));
  return Vector3D<double>(
    object.getX()/object.getW(),
    object.getY()/object.getW(),
    object.getZ()/object.getW()
  );
}

void OpenGL::cylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height, unsigned int slices, unsigned int stacks) throw() {
  const GLdouble radiusDelta = (topRadius - baseRadius)/stacks;
  const GLdouble zDelta = height/stacks;
  const GLdouble thetaDelta = 2.0 * constant::PI/slices;
  const GLdouble zNormal = (baseRadius - topRadius)/height;
  
  GLdouble radius = baseRadius;
  GLdouble nextRadius = radius + radiusDelta;
  GLdouble z = 0;
  GLdouble zNext = zDelta;
  for (unsigned int stack = stacks; stack > 0; --stack) {
    glBegin(QUAD_STRIP);
    GLdouble theta = 0;
    for (unsigned int slice = slices + 1; slice > 0; --slice) {
      theta += thetaDelta;
      const GLdouble x = Math::cos(theta);
      const GLdouble y = Math::sin(theta);
      glNormal3f(x, y, zNormal);
      glVertex3f(radius * x, radius * y, z);
      glNormal3f(x, y, zNormal);
      glVertex3f(nextRadius * x, nextRadius * y, zNext);
    }
    glEnd();
    radius = nextRadius;
    nextRadius += radiusDelta;
    z = zNext;
    zNext += zDelta;
  }
}

void OpenGL::torus(GLdouble innerRadius, GLdouble outerRadius, unsigned int numberOfRings, unsigned int numberOfSides) throw() {
  // see http://mathworld.wolfram.com/Torus.html
  // x = (c + a cos v) cos u
  // y = (c + a cos v) sin u
  // z = a sin v
  
  const GLdouble ringDelta = 2.0 * constant::PI/numberOfRings;
  const GLdouble sideDelta = 2.0 * constant::PI/numberOfSides;
  
  GLdouble theta = 0.0;
  GLdouble cosTheta = 1.0;
  GLdouble sinTheta = 0.0;
  for (unsigned int ring = numberOfRings; ring > 0; --ring) {
    GLdouble nextTheta = theta + ringDelta;
    GLdouble cosNextTheta = Math::cos(nextTheta);
    GLdouble sinNextTheta = Math::sin(nextTheta);
    glBegin(QUAD_STRIP);
    GLdouble phi = 0.0;
    for (unsigned int side = numberOfSides + 1; side > 0; --side) {
      phi += sideDelta;
      GLdouble cosPhi = Math::cos(phi);
      GLdouble sinPhi = Math::sin(phi);
      GLdouble dist = outerRadius + innerRadius * cosPhi;
      
      glNormal3f(cosNextTheta * cosPhi, -sinNextTheta * cosPhi, sinPhi);
      glVertex3f(cosNextTheta * dist, -sinNextTheta * dist, innerRadius * sinPhi);
      glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
      glVertex3f(cosTheta * dist, -sinTheta * dist, innerRadius * sinPhi);
    }
    glEnd();
    theta = nextTheta;
    cosTheta = cosNextTheta;
    sinTheta = sinNextTheta;
  }
}

void OpenGL::disk(double innerRadius, double outerRadius, unsigned int slices, unsigned int loops) throw() {
  glNormal3d(0, 0, 1);
  const double deltaAngle = 2 * constant::PI/slices;
  const double deltaRadius = (outerRadius - innerRadius)/loops;
  
  double radius = innerRadius;
  double nextRadius = radius;
  for (unsigned int loop = loops; loop > 0; --loop) {
    glBegin(QUAD_STRIP);
    nextRadius += deltaRadius;
    double angle = 0;
    for (unsigned int slice = slices; slice > 0; --slice) {
      const double sa = Math::sin(angle);
      const double ca = Math::cos(angle);
      glVertex2f(nextRadius * sa, nextRadius * ca);
      glVertex2f(radius * sa, radius * ca);
      angle += deltaAngle;
    }
    radius = nextRadius;
    glEnd();
  }
}

void OpenGL::partialDisk(GLdouble innerRadius, GLdouble outerRadius, unsigned int slices, unsigned int loops, GLdouble startAngle, GLdouble sweepAngle) throw() {
  glNormal3f(0, 0, 1);
  const double deltaRadius = (outerRadius - innerRadius)/loops;
  const double deltaAngle = sweepAngle/slices * constant::PI/180;
  double radius = innerRadius;
  double nextRadius = radius;
  for (unsigned int loop = loops; loop > 0; --loop) {
    glBegin(QUAD_STRIP);
    nextRadius += deltaRadius;
    double angle = startAngle * constant::PI/180;
    for (unsigned int slice = slices; slice > 0; --slice) {
      const double sa = Math::sin(angle);
      const double ca = Math::cos(angle);
      glVertex2d(nextRadius * sa, nextRadius * ca);
      glVertex2d(radius * sa, radius * ca);
	    angle += deltaAngle;
    }
    glEnd();
    radius = nextRadius;
  }
}

void OpenGL::sphere(double radius, unsigned int slices, unsigned int stacks) throw() {
  const double deltaRho = constant::PI/stacks;
  const double deltaTheta = 2 * constant::PI/slices;
  
  // top
  {
    glBegin(TRIANGLE_FAN);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, radius);
    double theta = 0;
    for (unsigned int slice = slices; slice > 0; --slice) {
      const double x = -Math::sin(theta) * Math::sin(deltaRho);
      const double y = Math::cos(theta) * Math::sin(deltaRho);
      const double z = Math::cos(deltaRho);
      theta += deltaTheta;
      glNormal3f(x, y, z);
      glVertex3f(x * radius, y * radius, z * radius);
    }
    glEnd();
  }

  {
    double rho = deltaRho;
    double theta = 0;
    for (unsigned int stack = (stacks - 1); stack > 0; --stack) {
      glBegin(QUAD_STRIP);
      for (unsigned int slice = slices; slice > 0; --slice) {
        const double x = -Math::sin(theta) * Math::sin(rho);
        const double y = Math::cos(theta) * Math::sin(rho);
        const double z = Math::cos(rho);
        glNormal3f(x, y, z);
        glVertex3f(x * radius, y * radius, z * radius);
        const double x2 = -Math::sin(theta) * Math::sin(rho + deltaRho);
        const double y2 = Math::cos(theta) * Math::sin(rho + deltaRho);
        const double z2 = Math::cos(rho + deltaRho);
        glNormal3f(x2, y2, z2);
        glVertex3f(x2 * radius, y2 * radius, z2 * radius);
        theta += deltaTheta;
      }
      rho += deltaRho;
      glEnd();
    }
  }
  
  // bottom
  {
    glBegin(TRIANGLE_FAN);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(0.0, 0.0, -radius);
    double theta = 0;
    for (unsigned int slice = slices; slice > 0; --slice) {
      const double rho = constant::PI - deltaRho;
      const double x = -Math::sin(theta) * Math::sin(rho);
      const double y = Math::cos(theta) * Math::sin(rho);
      const double z = Math::cos(rho);
      theta += deltaTheta;
      glNormal3f(x, y, z);
      glVertex3f(x * radius, y * radius, z * radius);
    }
    glEnd();
  }
}

// TAG: need Dimension3D
void OpenGL::box(double width, double length, double height, unsigned int flags) throw() {
  // OpenGL::Flag POINTS
  // OpenGL::Flag LINES
  // OpenGL::Flag POLYGONS  
  
  {
    glBegin(OpenGL::LINES);
    
    // min y
    glVertex3f(0, 0, 0);
    glVertex3f(width, 0, 0);
    
    glVertex3f(width, 0, 0);
    glVertex3f(width, 0, height);
    
    glVertex3f(width, 0, height);
    glVertex3f(0, 0, height);
    
    glVertex3f(0, 0, height);
    glVertex3f(0, 0, 0);
    
    // max y
    glVertex3f(0, length, 0);
    glVertex3f(width, length, 0);
    
    glVertex3f(width, length, 0);
    glVertex3f(width, length, height);
    
    glVertex3f(width, length, height);
    glVertex3f(0, length, height);
    
    glVertex3f(0, length, height);
    glVertex3f(0, length, 0);
    
    // from min y to max y
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);
    
    glVertex3f(width, 0, 0);
    glVertex3f(width, length, 0);
    
    glVertex3f(width, 0, height);
    glVertex3f(width, length, height);

    glVertex3f(0, 0, height);
    glVertex3f(0, length, height);
    
    glEnd();
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
