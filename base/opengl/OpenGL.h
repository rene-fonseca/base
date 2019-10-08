/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__OPENGL__OPENGL_H
#define _DK_SDU_MIP__OPENGL__OPENGL_H

#include <base/string/String.h>
#include <base/mathematics/Vector3D.h>
#include <base/mathematics/Matrix4x4.h>
#include <base/NotSupported.h>

#undef DOMAIN

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define CALL_OPENGL __stdcall
#else
#  define CALL_OPENGL
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

  // differences
  // no GL_ prefix
  // no GLvoid
  // no TRUE
  // no FALSE
  // 2_BYTES renamed to BYTES_2
  // 3_BYTES renamed to BYTES_3
  // 4_BYTES renamed to BYTES_4
  // FEED_BACK_2D
  // FEED_BACK_3D
  // FEED_BACK_3D_COLOR
  // FEED_BACK_3D_COLOR_TEXTURE
  // FEED_BACK_4D_COLOR_TEXTURE

/**
  OpenGL namespace.
  
  @short This namespace provides OpenGL specific types.
*/

namespace opengl {

  typedef unsigned int GLenum;
  typedef unsigned char GLboolean;
  typedef unsigned int GLbitfield;
  typedef signed char GLbyte;
  typedef short GLshort;
  typedef int GLint;
  typedef int GLsizei;
  typedef unsigned char GLubyte;
  typedef unsigned short GLushort;
  typedef unsigned int GLuint;
  typedef float GLfloat;
  typedef float GLclampf;
  typedef double GLdouble;
  typedef double GLclampd;
  
  // OpenGL 1.1 specification

#if (defined(_DK_SDU_MIP__BASE__EXPORT_OPEN_GL_1_1))
  extern "C" void CALL_OPENGL glAccum(GLenum op, GLfloat value);
  extern "C" void CALL_OPENGL glAlphaFunc(GLenum func, GLclampf ref);
  extern "C" GLboolean CALL_OPENGL glAreTexturesResident(GLsizei n, const GLuint* textures, GLboolean* residences);
  extern "C" void CALL_OPENGL glArrayElement(GLint i);
  extern "C" void CALL_OPENGL glBegin(GLenum mode);
  extern "C" void CALL_OPENGL glBindTexture(GLenum target, GLuint texture);
  extern "C" void CALL_OPENGL glBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte* bitmap);
  extern "C" void CALL_OPENGL glBlendFunc(GLenum sfactor, GLenum dfactor);
  extern "C" void CALL_OPENGL glCallList(GLuint list);
  extern "C" void CALL_OPENGL glCallLists(GLsizei n, GLenum type, const void* lists);
  extern "C" void CALL_OPENGL glClear(GLbitfield mask);
  extern "C" void CALL_OPENGL glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  extern "C" void CALL_OPENGL glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  extern "C" void CALL_OPENGL glClearDepth(GLclampd depth);
  extern "C" void CALL_OPENGL glClearIndex(GLfloat c);
  extern "C" void CALL_OPENGL glClearStencil(GLint s);
  extern "C" void CALL_OPENGL glClipPlane(GLenum plane, const GLdouble* equation);
  extern "C" void CALL_OPENGL glColor3b(GLbyte red, GLbyte green, GLbyte blue);
  extern "C" void CALL_OPENGL glColor3bv(const GLbyte* v);
  extern "C" void CALL_OPENGL glColor3d(GLdouble red, GLdouble green, GLdouble blue);
  extern "C" void CALL_OPENGL glColor3dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glColor3f(GLfloat red, GLfloat green, GLfloat blue);
  extern "C" void CALL_OPENGL glColor3fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glColor3i(GLint red, GLint green, GLint blue);
  extern "C" void CALL_OPENGL glColor3iv(const GLint* v);
  extern "C" void CALL_OPENGL glColor3s(GLshort red, GLshort green, GLshort blue);
  extern "C" void CALL_OPENGL glColor3sv(const GLshort* v);
  extern "C" void CALL_OPENGL glColor3ub(GLubyte red, GLubyte green, GLubyte blue);
  extern "C" void CALL_OPENGL glColor3ubv(const GLubyte* v);
  extern "C" void CALL_OPENGL glColor3ui(GLuint red, GLuint green, GLuint blue);
  extern "C" void CALL_OPENGL glColor3uiv(const GLuint* v);
  extern "C" void CALL_OPENGL glColor3us(GLushort red, GLushort green, GLushort blue);
  extern "C" void CALL_OPENGL glColor3usv(const GLushort* v);
  extern "C" void CALL_OPENGL glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
  extern "C" void CALL_OPENGL glColor4bv(const GLbyte* v);
  extern "C" void CALL_OPENGL glColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
  extern "C" void CALL_OPENGL glColor4dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  extern "C" void CALL_OPENGL glColor4fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glColor4i(GLint red, GLint green, GLint blue, GLint alpha);
  extern "C" void CALL_OPENGL glColor4iv(const GLint* v);
  extern "C" void CALL_OPENGL glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);
  extern "C" void CALL_OPENGL glColor4sv(const GLshort* v);
  extern "C" void CALL_OPENGL glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
  extern "C" void CALL_OPENGL glColor4ubv(const GLubyte* v);
  extern "C" void CALL_OPENGL glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha);
  extern "C" void CALL_OPENGL glColor4uiv(const GLuint* v);
  extern "C" void CALL_OPENGL glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha);
  extern "C" void CALL_OPENGL glColor4usv(const GLushort* v);
  extern "C" void CALL_OPENGL glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
  extern "C" void CALL_OPENGL glColorMaterial(GLenum face, GLenum mode);
  extern "C" void CALL_OPENGL glColorPointer(GLint size, GLenum type, GLsizei stride, const void* pointer);
  extern "C" void CALL_OPENGL glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
  extern "C" void CALL_OPENGL glCopyTexImage1D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
  extern "C" void CALL_OPENGL glCopyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
  extern "C" void CALL_OPENGL glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
  extern "C" void CALL_OPENGL glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
  extern "C" void CALL_OPENGL glCullFace(GLenum mode);
  extern "C" void CALL_OPENGL glDeleteLists(GLuint list, GLsizei range);
  extern "C" void CALL_OPENGL glDeleteTextures(GLsizei n, const GLuint* textures);
  extern "C" void CALL_OPENGL glDepthFunc(GLenum func);
  extern "C" void CALL_OPENGL glDepthMask(GLboolean flag);
  extern "C" void CALL_OPENGL glDepthRange(GLclampd zNear, GLclampd zFar);
  extern "C" void CALL_OPENGL glDisable(GLenum cap);
  extern "C" void CALL_OPENGL glDisableClientState(GLenum array);
  extern "C" void CALL_OPENGL glDrawArrays(GLenum mode, GLint first, GLsizei count);
  extern "C" void CALL_OPENGL glDrawBuffer(GLenum mode);
  extern "C" void CALL_OPENGL glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
  extern "C" void CALL_OPENGL glDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
  extern "C" void CALL_OPENGL glEdgeFlag(GLboolean flag);
  extern "C" void CALL_OPENGL glEdgeFlagPointer(GLsizei stride, const GLboolean* pointer);
  extern "C" void CALL_OPENGL glEdgeFlagv(const GLboolean* flag);
  extern "C" void CALL_OPENGL glEnable(GLenum cap);
  extern "C" void CALL_OPENGL glEnableClientState(GLenum array);
  extern "C" void CALL_OPENGL glEnd();
  extern "C" void CALL_OPENGL glEndList();
  extern "C" void CALL_OPENGL glEvalCoord1d(GLdouble u);
  extern "C" void CALL_OPENGL glEvalCoord1dv(const GLdouble* u);
  extern "C" void CALL_OPENGL glEvalCoord1f(GLfloat u);
  extern "C" void CALL_OPENGL glEvalCoord1fv(const GLfloat* u);
  extern "C" void CALL_OPENGL glEvalCoord2d(GLdouble u, GLdouble v);
  extern "C" void CALL_OPENGL glEvalCoord2dv(const GLdouble* u);
  extern "C" void CALL_OPENGL glEvalCoord2f(GLfloat u, GLfloat v);
  extern "C" void CALL_OPENGL glEvalCoord2fv(const GLfloat* u);
  extern "C" void CALL_OPENGL glEvalMesh1(GLenum mode, GLint i1, GLint i2);
  extern "C" void CALL_OPENGL glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
  extern "C" void CALL_OPENGL glEvalPoint1(GLint i);
  extern "C" void CALL_OPENGL glEvalPoint2(GLint i, GLint j);
  extern "C" void CALL_OPENGL glFeedbackBuffer(GLsizei size, GLenum type, GLfloat* buffer);
  extern "C" void CALL_OPENGL glFinish();
  extern "C" void CALL_OPENGL glFlush();
  extern "C" void CALL_OPENGL glFogf(GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glFogfv(GLenum pname, const GLfloat* params);
  extern "C" void CALL_OPENGL glFogi(GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glFogiv(GLenum pname, const GLint* params);
  extern "C" void CALL_OPENGL glFrontFace(GLenum mode);
  extern "C" void CALL_OPENGL glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
  extern "C" GLuint CALL_OPENGL glGenLists(GLsizei range);
  extern "C" void CALL_OPENGL glGenTextures(GLsizei n, GLuint* textures);
  extern "C" void CALL_OPENGL glGetBooleanv(GLenum pname, GLboolean* params);
  extern "C" void CALL_OPENGL glGetClipPlane(GLenum plane, GLdouble* equation);
  extern "C" void CALL_OPENGL glGetDoublev(GLenum pname, GLdouble* params);
  extern "C" GLenum CALL_OPENGL glGetError();
  extern "C" void CALL_OPENGL glGetFloatv(GLenum pname, GLfloat* params);
  extern "C" void CALL_OPENGL glGetIntegerv(GLenum pname, GLint* params);
  extern "C" void CALL_OPENGL glGetLightfv(GLenum light, GLenum pname, GLfloat* params);
  extern "C" void CALL_OPENGL glGetLightiv(GLenum light, GLenum pname, GLint* params);
  extern "C" void CALL_OPENGL glGetMapdv(GLenum target, GLenum query, GLdouble* v);
  extern "C" void CALL_OPENGL glGetMapfv(GLenum target, GLenum query, GLfloat* v);
  extern "C" void CALL_OPENGL glGetMapiv(GLenum target, GLenum query, GLint* v);
  extern "C" void CALL_OPENGL glGetMaterialfv(GLenum face, GLenum pname, GLfloat* params);
  extern "C" void CALL_OPENGL glGetMaterialiv(GLenum face, GLenum pname, GLint* params);
  extern "C" void CALL_OPENGL glGetPixelMapfv(GLenum map, GLfloat* values);
  extern "C" void CALL_OPENGL glGetPixelMapuiv(GLenum map, GLuint* values);
  extern "C" void CALL_OPENGL glGetPixelMapusv(GLenum map, GLushort* values);
  extern "C" void CALL_OPENGL glGetPointerv(GLenum pname, void** params);
  extern "C" void CALL_OPENGL glGetPolygonStipple(GLubyte* mask);
  extern "C" const GLubyte* CALL_OPENGL glGetString(GLenum name);
  extern "C" void CALL_OPENGL glGetTexEnvfv(GLenum target, GLenum pname, GLfloat* params);
  extern "C" void CALL_OPENGL glGetTexEnviv(GLenum target, GLenum pname, GLint* params);
  extern "C" void CALL_OPENGL glGetTexGendv(GLenum coord, GLenum pname, GLdouble* params);
  extern "C" void CALL_OPENGL glGetTexGenfv(GLenum coord, GLenum pname, GLfloat* params);
  extern "C" void CALL_OPENGL glGetTexGeniv(GLenum coord, GLenum pname, GLint* params);
  extern "C" void CALL_OPENGL glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
  extern "C" void CALL_OPENGL glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat* params);
  extern "C" void CALL_OPENGL glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint* params);
  extern "C" void CALL_OPENGL glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params);
  extern "C" void CALL_OPENGL glGetTexParameteriv(GLenum target, GLenum pname, GLint* params);
  extern "C" void CALL_OPENGL glHint(GLenum target, GLenum mode);
  extern "C" void CALL_OPENGL glIndexMask(GLuint mask);
  extern "C" void CALL_OPENGL glIndexPointer(GLenum type, GLsizei stride, const void* pointer);
  extern "C" void CALL_OPENGL glIndexd(GLdouble c);
  extern "C" void CALL_OPENGL glIndexdv(const GLdouble* c);
  extern "C" void CALL_OPENGL glIndexf(GLfloat c);
  extern "C" void CALL_OPENGL glIndexfv(const GLfloat* c);
  extern "C" void CALL_OPENGL glIndexi(GLint c);
  extern "C" void CALL_OPENGL glIndexiv(const GLint* c);
  extern "C" void CALL_OPENGL glIndexs(GLshort c);
  extern "C" void CALL_OPENGL glIndexsv(const GLshort* c);
  extern "C" void CALL_OPENGL glIndexub(GLubyte c);
  extern "C" void CALL_OPENGL glIndexubv(const GLubyte* c);
  extern "C" void CALL_OPENGL glInitNames();
  extern "C" void CALL_OPENGL glInterleavedArrays(GLenum format, GLsizei stride, const void* pointer);
  extern "C" GLboolean CALL_OPENGL glIsEnabled(GLenum cap);
  extern "C" GLboolean CALL_OPENGL glIsList(GLuint list);
  extern "C" GLboolean CALL_OPENGL glIsTexture(GLuint texture);
  extern "C" void CALL_OPENGL glLightModelf(GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glLightModelfv(GLenum pname, const GLfloat* params);
  extern "C" void CALL_OPENGL glLightModeli(GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glLightModeliv(GLenum pname, const GLint* params);
  extern "C" void CALL_OPENGL glLightf(GLenum light, GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glLightfv(GLenum light, GLenum pname, const GLfloat* params);
  extern "C" void CALL_OPENGL glLighti(GLenum light, GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glLightiv(GLenum light, GLenum pname, const GLint* params);
  extern "C" void CALL_OPENGL glLineStipple(GLint factor, GLushort pattern);
  extern "C" void CALL_OPENGL glLineWidth(GLfloat width);
  extern "C" void CALL_OPENGL glListBase(GLuint base);
  extern "C" void CALL_OPENGL glLoadIdentity();
  extern "C" void CALL_OPENGL glLoadMatrixd(const GLdouble* m);
  extern "C" void CALL_OPENGL glLoadMatrixf(const GLfloat* m);
  extern "C" void CALL_OPENGL glLoadName(GLuint name);
  extern "C" void CALL_OPENGL glLogicOp(GLenum opcode);
  extern "C" void CALL_OPENGL glMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points);
  extern "C" void CALL_OPENGL glMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat* points);
  extern "C" void CALL_OPENGL glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points);
  extern "C" void CALL_OPENGL glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat* points);
  extern "C" void CALL_OPENGL glMapGrid1d(GLint un, GLdouble u1, GLdouble u2);
  extern "C" void CALL_OPENGL glMapGrid1f(GLint un, GLfloat u1, GLfloat u2);
  extern "C" void CALL_OPENGL glMapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
  extern "C" void CALL_OPENGL glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
  extern "C" void CALL_OPENGL glMaterialf(GLenum face, GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glMaterialfv(GLenum face, GLenum pname, const GLfloat* params);
  extern "C" void CALL_OPENGL glMateriali(GLenum face, GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glMaterialiv(GLenum face, GLenum pname, const GLint* params);
  extern "C" void CALL_OPENGL glMatrixMode(GLenum mode);
  extern "C" void CALL_OPENGL glMultMatrixd(const GLdouble* m);
  extern "C" void CALL_OPENGL glMultMatrixf(const GLfloat* m);
  extern "C" void CALL_OPENGL glNewList(GLuint list, GLenum mode);
  extern "C" void CALL_OPENGL glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz);
  extern "C" void CALL_OPENGL glNormal3bv(const GLbyte* v);
  extern "C" void CALL_OPENGL glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz);
  extern "C" void CALL_OPENGL glNormal3dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);
  extern "C" void CALL_OPENGL glNormal3fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glNormal3i(GLint nx, GLint ny, GLint nz);
  extern "C" void CALL_OPENGL glNormal3iv(const GLint* v);
  extern "C" void CALL_OPENGL glNormal3s(GLshort nx, GLshort ny, GLshort nz);
  extern "C" void CALL_OPENGL glNormal3sv(const GLshort* v);
  extern "C" void CALL_OPENGL glNormalPointer(GLenum type, GLsizei stride, const void* pointer);
  extern "C" void CALL_OPENGL glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
  extern "C" void CALL_OPENGL glPassThrough(GLfloat token);
  extern "C" void CALL_OPENGL glPixelMapfv(GLenum map, GLint mapsize, const GLfloat* values);
  extern "C" void CALL_OPENGL glPixelMapuiv(GLenum map, GLint mapsize, const GLuint* values);
  extern "C" void CALL_OPENGL glPixelMapusv(GLenum map, GLint mapsize, const GLushort* values);
  extern "C" void CALL_OPENGL glPixelStoref(GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glPixelStorei(GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glPixelTransferf(GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glPixelTransferi(GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glPixelZoom(GLfloat xfactor, GLfloat yfactor);
  extern "C" void CALL_OPENGL glPointSize(GLfloat size);
  extern "C" void CALL_OPENGL glPolygonMode(GLenum face, GLenum mode);
  extern "C" void CALL_OPENGL glPolygonOffset(GLfloat factor, GLfloat units);
  extern "C" void CALL_OPENGL glPolygonStipple(const GLubyte* mask);
  extern "C" void CALL_OPENGL glPopAttrib();
  extern "C" void CALL_OPENGL glPopClientAttrib();
  extern "C" void CALL_OPENGL glPopMatrix();
  extern "C" void CALL_OPENGL glPopName();
  extern "C" void CALL_OPENGL glPrioritizeTextures(GLsizei n, const GLuint* textures, const GLclampf* priorities);
  extern "C" void CALL_OPENGL glPushAttrib(GLbitfield mask);
  extern "C" void CALL_OPENGL glPushClientAttrib(GLbitfield mask);
  extern "C" void CALL_OPENGL glPushMatrix();
  extern "C" void CALL_OPENGL glPushName(GLuint name);
  extern "C" void CALL_OPENGL glRasterPos2d(GLdouble x, GLdouble y);
  extern "C" void CALL_OPENGL glRasterPos2dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glRasterPos2f(GLfloat x, GLfloat y);
  extern "C" void CALL_OPENGL glRasterPos2fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glRasterPos2i(GLint x, GLint y);
  extern "C" void CALL_OPENGL glRasterPos2iv(const GLint* v);
  extern "C" void CALL_OPENGL glRasterPos2s(GLshort x, GLshort y);
  extern "C" void CALL_OPENGL glRasterPos2sv(const GLshort* v);
  extern "C" void CALL_OPENGL glRasterPos3d(GLdouble x, GLdouble y, GLdouble z);
  extern "C" void CALL_OPENGL glRasterPos3dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glRasterPos3f(GLfloat x, GLfloat y, GLfloat z);
  extern "C" void CALL_OPENGL glRasterPos3fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glRasterPos3i(GLint x, GLint y, GLint z);
  extern "C" void CALL_OPENGL glRasterPos3iv(const GLint* v);
  extern "C" void CALL_OPENGL glRasterPos3s(GLshort x, GLshort y, GLshort z);
  extern "C" void CALL_OPENGL glRasterPos3sv(const GLshort* v);
  extern "C" void CALL_OPENGL glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
  extern "C" void CALL_OPENGL glRasterPos4dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  extern "C" void CALL_OPENGL glRasterPos4fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glRasterPos4i(GLint x, GLint y, GLint z, GLint w);
  extern "C" void CALL_OPENGL glRasterPos4iv(const GLint* v);
  extern "C" void CALL_OPENGL glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w);
  extern "C" void CALL_OPENGL glRasterPos4sv(const GLshort* v);
  extern "C" void CALL_OPENGL glReadBuffer(GLenum mode);
  extern "C" void CALL_OPENGL glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
  extern "C" void CALL_OPENGL glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
  extern "C" void CALL_OPENGL glRectdv(const GLdouble* v1, const GLdouble* v2);
  extern "C" void CALL_OPENGL glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
  extern "C" void CALL_OPENGL glRectfv(const GLfloat* v1, const GLfloat* v2);
  extern "C" void CALL_OPENGL glRecti(GLint x1, GLint y1, GLint x2, GLint y2);
  extern "C" void CALL_OPENGL glRectiv(const GLint* v1, const GLint* v2);
  extern "C" void CALL_OPENGL glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
  extern "C" void CALL_OPENGL glRectsv(const GLshort* v1, const GLshort* v2);
  extern "C" GLint CALL_OPENGL glRenderMode(GLenum mode);
  extern "C" void CALL_OPENGL glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
  extern "C" void CALL_OPENGL glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
  extern "C" void CALL_OPENGL glScaled(GLdouble x, GLdouble y, GLdouble z);
  extern "C" void CALL_OPENGL glScalef(GLfloat x, GLfloat y, GLfloat z);
  extern "C" void CALL_OPENGL glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
  extern "C" void CALL_OPENGL glSelectBuffer(GLsizei size, GLuint* buffer);
  extern "C" void CALL_OPENGL glShadeModel(GLenum mode);
  extern "C" void CALL_OPENGL glStencilFunc(GLenum func, GLint ref, GLuint mask);
  extern "C" void CALL_OPENGL glStencilMask(GLuint mask);
  extern "C" void CALL_OPENGL glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
  extern "C" void CALL_OPENGL glTexCoord1d(GLdouble s);
  extern "C" void CALL_OPENGL glTexCoord1dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glTexCoord1f(GLfloat s);
  extern "C" void CALL_OPENGL glTexCoord1fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glTexCoord1i(GLint s);
  extern "C" void CALL_OPENGL glTexCoord1iv(const GLint* v);
  extern "C" void CALL_OPENGL glTexCoord1s(GLshort s);
  extern "C" void CALL_OPENGL glTexCoord1sv(const GLshort* v);
  extern "C" void CALL_OPENGL glTexCoord2d(GLdouble s, GLdouble t);
  extern "C" void CALL_OPENGL glTexCoord2dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glTexCoord2f(GLfloat s, GLfloat t);
  extern "C" void CALL_OPENGL glTexCoord2fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glTexCoord2i(GLint s, GLint t);
  extern "C" void CALL_OPENGL glTexCoord2iv(const GLint* v);
  extern "C" void CALL_OPENGL glTexCoord2s(GLshort s, GLshort t);
  extern "C" void CALL_OPENGL glTexCoord2sv(const GLshort* v);
  extern "C" void CALL_OPENGL glTexCoord3d(GLdouble s, GLdouble t, GLdouble r);
  extern "C" void CALL_OPENGL glTexCoord3dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glTexCoord3f(GLfloat s, GLfloat t, GLfloat r);
  extern "C" void CALL_OPENGL glTexCoord3fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glTexCoord3i(GLint s, GLint t, GLint r);
  extern "C" void CALL_OPENGL glTexCoord3iv(const GLint* v);
  extern "C" void CALL_OPENGL glTexCoord3s(GLshort s, GLshort t, GLshort r);
  extern "C" void CALL_OPENGL glTexCoord3sv(const GLshort* v);
  extern "C" void CALL_OPENGL glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
  extern "C" void CALL_OPENGL glTexCoord4dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
  extern "C" void CALL_OPENGL glTexCoord4fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glTexCoord4i(GLint s, GLint t, GLint r, GLint q);
  extern "C" void CALL_OPENGL glTexCoord4iv(const GLint* v);
  extern "C" void CALL_OPENGL glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q);
  extern "C" void CALL_OPENGL glTexCoord4sv(const GLshort* v);
  extern "C" void CALL_OPENGL glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const void* pointer);
  extern "C" void CALL_OPENGL glTexEnvf(GLenum target, GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glTexEnvfv(GLenum target, GLenum pname, const GLfloat* params);
  extern "C" void CALL_OPENGL glTexEnvi(GLenum target, GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glTexEnviv(GLenum target, GLenum pname, const GLint* params);
  extern "C" void CALL_OPENGL glTexGend(GLenum coord, GLenum pname, GLdouble param);
  extern "C" void CALL_OPENGL glTexGendv(GLenum coord, GLenum pname, const GLdouble* params);
  extern "C" void CALL_OPENGL glTexGenf(GLenum coord, GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glTexGenfv(GLenum coord, GLenum pname, const GLfloat* params);
  extern "C" void CALL_OPENGL glTexGeni(GLenum coord, GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glTexGeniv(GLenum coord, GLenum pname, const GLint* params);
  extern "C" void CALL_OPENGL glTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
  extern "C" void CALL_OPENGL glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
  extern "C" void CALL_OPENGL glTexParameterf(GLenum target, GLenum pname, GLfloat param);
  extern "C" void CALL_OPENGL glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params);
  extern "C" void CALL_OPENGL glTexParameteri(GLenum target, GLenum pname, GLint param);
  extern "C" void CALL_OPENGL glTexParameteriv(GLenum target, GLenum pname, const GLint* params);
  extern "C" void CALL_OPENGL glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
  extern "C" void CALL_OPENGL glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
  extern "C" void CALL_OPENGL glTranslated(GLdouble x, GLdouble y, GLdouble z);
  extern "C" void CALL_OPENGL glTranslatef(GLfloat x, GLfloat y, GLfloat z);
  extern "C" void CALL_OPENGL glVertex2d(GLdouble x, GLdouble y);
  extern "C" void CALL_OPENGL glVertex2dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glVertex2f(GLfloat x, GLfloat y);
  extern "C" void CALL_OPENGL glVertex2fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glVertex2i(GLint x, GLint y);
  extern "C" void CALL_OPENGL glVertex2iv(const GLint* v);
  extern "C" void CALL_OPENGL glVertex2s(GLshort x, GLshort y);
  extern "C" void CALL_OPENGL glVertex2sv(const GLshort* v);
  extern "C" void CALL_OPENGL glVertex3d(GLdouble x, GLdouble y, GLdouble z);
  extern "C" void CALL_OPENGL glVertex3dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glVertex3f(GLfloat x, GLfloat y, GLfloat z);
  extern "C" void CALL_OPENGL glVertex3fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glVertex3i(GLint x, GLint y, GLint z);
  extern "C" void CALL_OPENGL glVertex3iv(const GLint* v);
  extern "C" void CALL_OPENGL glVertex3s(GLshort x, GLshort y, GLshort z);
  extern "C" void CALL_OPENGL glVertex3sv(const GLshort* v);
  extern "C" void CALL_OPENGL glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
  extern "C" void CALL_OPENGL glVertex4dv(const GLdouble* v);
  extern "C" void CALL_OPENGL glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  extern "C" void CALL_OPENGL glVertex4fv(const GLfloat* v);
  extern "C" void CALL_OPENGL glVertex4i(GLint x, GLint y, GLint z, GLint w);
  extern "C" void CALL_OPENGL glVertex4iv(const GLint* v);
  extern "C" void CALL_OPENGL glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
  extern "C" void CALL_OPENGL glVertex4sv(const GLshort* v);
  extern "C" void CALL_OPENGL glVertexPointer(GLint size, GLenum type, GLsizei stride, const void* pointer);
  extern "C" void CALL_OPENGL glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
#endif // _DK_SDU_MIP__BASE__EXPORT_OPEN_GL_1_1

#if (defined(_DK_SDU_MIP__BASE__EXPORT_OPEN_GL_1_2))
  // TAG: fixme
#endif // _DK_SDU_MIP__BASE__EXPORT_OPEN_GL_1_2

#if (defined(_DK_SDU_MIP__BASE__EXPORT_OPEN_GL_1_3))
  // TAG: fixme
#endif // _DK_SDU_MIP__BASE__EXPORT_OPEN_GL_1_3

#if (defined(_DK_SDU_MIP__BASE__EXPORT_OPEN_GL_1_4))
  // TAG: fixme
#endif // _DK_SDU_MIP__BASE__EXPORT_OPEN_GL_1_4

}; // end of opengl namespace

/**
  This exception is raised by the OpenGL class.
  
  @short OpenGL exception
  @ingroup opengl ui exceptions
  @version 1.0
*/

class OpenGLException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline OpenGLException() throw() {
  }
  
  /**
    Initializes the exception object.
    
    @param message The message.
  */
  inline OpenGLException(const char* message) throw() : Exception(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline OpenGLException(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string.
    @param type The identity of the type.
  */
  inline OpenGLException(const char* message, Type type) throw() : Exception(message, type) {
  }
};

/**
  This class provides access to a specific OpenGL implementation.
  
  @short OpenGL implementation.
  @ingroup opengl ui
  @version 1.0
*/

class OpenGL {
public:

  typedef void (*Function)();
  typedef void (*OpenGL::*FunctionPointer)();
  
  struct Descriptor {
    /** Name of extension. */
    Literal name;
    /** Storage. */
    FunctionPointer function;
  };
private:  
  
  /** Specification supported. */
  unsigned int specification;  
  
  /**
    Returns the extension function.
  */
  Function getFunction(const Literal& name) throw();
  
  /**
    Returns the extension function.
  */
  Function getFunction(const String& name) throw();
private:
  
  /**
    Loads the specified functions.
  */
  void loadFunctions(Descriptor* descriptor, unsigned int size) throw();

  /**
    Loads the handler for missing functions.
  */
  void fixMissing(Descriptor* descriptor, unsigned int size) throw();
public:

  typedef unsigned int GLenum;
  typedef unsigned char GLboolean;
  typedef unsigned int GLbitfield;
  typedef signed char GLbyte;
  typedef short GLshort;
  typedef int GLint;
  typedef int GLsizei;
  typedef unsigned char GLubyte;
  typedef unsigned short GLushort;
  typedef unsigned int GLuint;
  typedef float GLfloat;
  typedef float GLclampf;
  typedef double GLdouble;
  typedef double GLclampd;
  
  enum {
    
    // version
    VERSION_1_1 = 0x0101,
    VERSION_1_2 = 0x0102,
    VERSION_1_3 = 0x0103,
    VERSION_1_4 = 0x0104,

    // AccumOp
    ACCUM = 0x0100,
    LOAD = 0x0101,
    RETURN = 0x0102,
    MULT = 0x0103,
    ADD = 0x0104,

    // AlphaFunction
    NEVER = 0x0200,
    LESS = 0x0201,
    EQUAL = 0x0202,
    LEQUAL = 0x0203,
    GREATER = 0x0204,
    NOTEQUAL = 0x0205,
    GEQUAL = 0x0206,
    ALWAYS = 0x0207,
    
    // AttribMask
    CURRENT_BIT = 0x00000001,
    POINT_BIT = 0x00000002,
    LINE_BIT = 0x00000004,
    POLYGON_BIT = 0x00000008,
    POLYGON_STIPPLE_BIT = 0x00000010,
    PIXEL_MODE_BIT = 0x00000020,
    LIGHTING_BIT = 0x00000040,
    FOG_BIT = 0x00000080,
    DEPTH_BUFFER_BIT = 0x00000100,
    ACCUM_BUFFER_BIT = 0x00000200,
    STENCIL_BUFFER_BIT = 0x00000400,
    VIEWPORT_BIT = 0x00000800,
    TRANSFORM_BIT = 0x00001000,
    ENABLE_BIT = 0x00002000,
    COLOR_BUFFER_BIT = 0x00004000,
    HINT_BIT = 0x00008000,
    EVAL_BIT = 0x00010000,
    LIST_BIT = 0x00020000,
    TEXTURE_BIT = 0x00040000,
    SCISSOR_BIT = 0x00080000,
    ALL_ATTRIB_BITS = 0x000fffff,

    // BeginMode
    POINTS = 0x0000,
    LINES = 0x0001,
    LINE_LOOP = 0x0002,
    LINE_STRIP = 0x0003,
    TRIANGLES = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN = 0x0006,
    QUADS = 0x0007,
    QUAD_STRIP = 0x0008,
    POLYGON = 0x0009,
    
    // BlendingFactorDest
    ZERO = 0,
    ONE = 1,
    SRC_COLOR = 0x0300,
    ONE_MINUS_SRC_COLOR = 0x0301,
    SRC_ALPHA = 0x0302,
    ONE_MINUS_SRC_ALPHA = 0x0303,
    DST_ALPHA = 0x0304,
    ONE_MINUS_DST_ALPHA = 0x0305,

    // BlendingFactorSrc
    // ZERO
    // ONE
    DST_COLOR = 0x0306,
    ONE_MINUS_DST_COLOR = 0x0307,
    SRC_ALPHA_SATURATE = 0x0308,
    // SRC_ALPHA
    // ONE_MINUS_SRC_ALPHA
    // DST_ALPHA
    // ONE_MINUS_DST_ALPHA
    
    // ClearBufferMask
    // COLOR_BUFFER_BIT
    // ACCUM_BUFFER_BIT
    // STENCIL_BUFFER_BIT
    // DEPTH_BUFFER_BIT
    
    // ClientArrayType
    // VERTEX_ARRAY
    // NORMAL_ARRAY
    // COLOR_ARRAY
    // INDEX_ARRAY
    // TEXTURE_COORD_ARRAY
    // EDGE_FLAG_ARRAY
    
    // ClipPlaneName
    CLIP_PLANE0 = 0x3000,
    CLIP_PLANE1 = 0x3001,
    CLIP_PLANE2 = 0x3002,
    CLIP_PLANE3 = 0x3003,
    CLIP_PLANE4 = 0x3004,
    CLIP_PLANE5 = 0x3005,
    
    // ColorMaterialFace
    // FRONT
    // BACK
    // FRONT_AND_BACK
    
    // ColorMaterialParameter
    // AMBIENT
    // DIFFUSE
    // SPECULAR
    // EMISSION
    // AMBIENT_AND_DIFFUSE
    
    // ColorPointerType
    // BYTE
    // UNSIGNED_BYTE
    // SHORT
    // UNSIGNED_SHORT
    // INT
    // UNSIGNED_INT
    // FLOAT
    // DOUBLE

    // CullFaceMode
    // FRONT
    // BACK
    // FRONT_AND_BACK

    // DataType
    BYTE = 0x1400,
    UNSIGNED_BYTE = 0x1401,
    SHORT = 0x1402,
    UNSIGNED_SHORT = 0x1403,
    INT = 0x1404,
    UNSIGNED_INT = 0x1405,
    FLOAT = 0x1406,
    BYTES_2 = 0x1407,
    BYTES_3 = 0x1408,
    BYTES_4 = 0x1409,
    DOUBLE = 0x140a,

    // DepthFunction
    // NEVER
    // LESS
    // EQUAL
    // LEQUAL
    // GREATER
    // NOTEQUAL
    // GEQUAL
    // ALWAYS
    
    // DrawBufferMode
    NONE = 0,
    FRONT_LEFT = 0x0400,
    FRONT_RIGHT = 0x0401,
    BACK_LEFT = 0x0402,
    BACK_RIGHT = 0x0403,
    FRONT = 0x0404,
    BACK = 0x0405,
    LEFT = 0x0406,
    RIGHT = 0x0407,
    FRONT_AND_BACK = 0x0408,
    AUX0 = 0x0409,
    AUX1 = 0x040a,
    AUX2 = 0x040b,
    AUX3 = 0x040c,

    // Enable
    // FOG
    // LIGHTING
    // TEXTURE_1D
    // TEXTURE_2D
    // LINE_STIPPLE
    // POLYGON_STIPPLE
    // CULL_FACE
    // ALPHA_TEST
    // BLEND
    // INDEX_LOGIC_OP
    // COLOR_LOGIC_OP
    // DITHER
    // STENCIL_TEST
    // DEPTH_TEST
    // CLIP_PLANE0
    // CLIP_PLANE1
    // CLIP_PLANE2
    // CLIP_PLANE3
    // CLIP_PLANE4
    // CLIP_PLANE5
    // LIGHT0
    // LIGHT1
    // LIGHT2
    // LIGHT3
    // LIGHT4
    // LIGHT5
    // LIGHT6
    // LIGHT7
    // TEXTURE_GEN_S
    // TEXTURE_GEN_T
    // TEXTURE_GEN_R
    // TEXTURE_GEN_Q
    // MAP1_VERTEX_3
    // MAP1_VERTEX_4
    // MAP1_COLOR_4
    // MAP1_INDEX
    // MAP1_NORMAL
    // MAP1_TEXTURE_COORD_1
    // MAP1_TEXTURE_COORD_2
    // MAP1_TEXTURE_COORD_3
    // MAP1_TEXTURE_COORD_4
    // MAP2_VERTEX_3
    // MAP2_VERTEX_4
    // MAP2_COLOR_4
    // MAP2_INDEX
    // MAP2_NORMAL
    // MAP2_TEXTURE_COORD_1
    // MAP2_TEXTURE_COORD_2
    // MAP2_TEXTURE_COORD_3
    // MAP2_TEXTURE_COORD_4
    // POINT_SMOOTH
    // LINE_SMOOTH
    // POLYGON_SMOOTH
    // SCISSOR_TEST
    // COLOR_MATERIAL
    // NORMALIZE
    // AUTO_NORMAL
    // VERTEX_ARRAY
    // NORMAL_ARRAY
    // COLOR_ARRAY
    // INDEX_ARRAY
    // TEXTURE_COORD_ARRAY
    // EDGE_FLAG_ARRAY
    // POLYGON_OFFSET_POINT
    // POLYGON_OFFSET_LINE
    // POLYGON_OFFSET_FILL

    // ErrorCode
    NO_ERROR = 0,
    INVALID_ENUM = 0x0500,
    INVALID_VALUE = 0x0501,
    INVALID_OPERATION = 0x0502,
    STACK_OVERFLOW = 0x0503,
    STACK_UNDERFLOW = 0x0504,
    OUT_OF_MEMORY = 0x0505,

    // FeedBackMode
    FEED_BACK_2D = 0x0600,
    FEED_BACK_3D = 0x0601,
    FEED_BACK_3D_COLOR = 0x0602,
    FEED_BACK_3D_COLOR_TEXTURE = 0x0603,
    FEED_BACK_4D_COLOR_TEXTURE = 0x0604,

    // FeedBackToken
    PASS_THROUGH_TOKEN = 0x0700,
    POINT_TOKEN = 0x0701,
    LINE_TOKEN = 0x0702,
    POLYGON_TOKEN = 0x0703,
    BITMAP_TOKEN = 0x0704,
    DRAW_PIXEL_TOKEN = 0x0705,
    COPY_PIXEL_TOKEN = 0x0706,
    LINE_RESET_TOKEN = 0x0707,

    // FogMode
    // LINEAR
    EXP = 0x0800,
    EXP2 = 0x0801,
    
    // FogParameter
    // FOG_COLOR
    // FOG_DENSITY
    // FOG_END
    // FOG_INDEX
    // FOG_MODE
    // FOG_START
    
    // FrontFaceDirection
    CW = 0x0900,
    CCW = 0x0901,

    // GetMapTarget
    COEFF = 0x0a00,
    ORDER = 0x0a01,
    DOMAIN = 0x0a02,

    // GetPixelMap
    // PIXEL_MAP_I_TO_I
    // PIXEL_MAP_S_TO_S
    // PIXEL_MAP_I_TO_R
    // PIXEL_MAP_I_TO_G
    // PIXEL_MAP_I_TO_B
    // PIXEL_MAP_I_TO_A
    // PIXEL_MAP_R_TO_R
    // PIXEL_MAP_G_TO_G
    // PIXEL_MAP_B_TO_B
    // PIXEL_MAP_A_TO_A

    // GetPointerTarget
    // VERTEX_ARRAY_POINTER
    // NORMAL_ARRAY_POINTER
    // COLOR_ARRAY_POINTER
    // INDEX_ARRAY_POINTER
    // TEXTURE_COORD_ARRAY_POINTER
    // EDGE_FLAG_ARRAY_POINTER

    // GetTarget
    CURRENT_COLOR = 0x0b00,
    CURRENT_INDEX = 0x0b01,
    CURRENT_NORMAL = 0x0b02,
    CURRENT_TEXTURE_COORDS = 0x0b03,
    CURRENT_RASTER_COLOR = 0x0b04,
    CURRENT_RASTER_INDEX = 0x0b05,
    CURRENT_RASTER_TEXTURE_COORDS= 0x0b06,
    CURRENT_RASTER_POSITION = 0x0b07,
    CURRENT_RASTER_POSITION_VALID= 0x0b08,
    CURRENT_RASTER_DISTANCE = 0x0b09,
    POINT_SMOOTH = 0x0b10,
    POINT_SIZE = 0x0b11,
    POINT_SIZE_RANGE = 0x0b12,
    POINT_SIZE_GRANULARITY = 0x0b13,
    LINE_SMOOTH = 0x0b20,
    LINE_WIDTH = 0x0b21,
    LINE_WIDTH_RANGE = 0x0b22,
    LINE_WIDTH_GRANULARITY = 0x0b23,
    LINE_STIPPLE = 0x0b24,
    LINE_STIPPLE_PATTERN = 0x0b25,
    LINE_STIPPLE_REPEAT = 0x0b26,
    LIST_MODE = 0x0b30,
    MAX_LIST_NESTING = 0x0b31,
    LIST_BASE = 0x0b32,
    LIST_INDEX = 0x0b33,
    POLYGON_MODE = 0x0b40,
    POLYGON_SMOOTH = 0x0b41,
    POLYGON_STIPPLE = 0x0b42,
    EDGE_FLAG = 0x0b43,
    CULL_FACE = 0x0b44,
    CULL_FACE_MODE = 0x0b45,
    FRONT_FACE = 0x0b46,
    LIGHTING = 0x0b50,
    LIGHT_MODEL_LOCAL_VIEWER = 0x0b51,
    LIGHT_MODEL_TWO_SIDE = 0x0b52,
    LIGHT_MODEL_AMBIENT = 0x0b53,
    SHADE_MODEL = 0x0b54,
    COLOR_MATERIAL_FACE = 0x0b55,
    COLOR_MATERIAL_PARAMETER = 0x0b56,
    COLOR_MATERIAL = 0x0b57,
    FOG = 0x0b60,
    FOG_INDEX = 0x0b61,
    FOG_DENSITY = 0x0b62,
    FOG_START = 0x0b63,
    FOG_END = 0x0b64,
    FOG_MODE = 0x0b65,
    FOG_COLOR = 0x0b66,
    DEPTH_RANGE = 0x0b70,
    DEPTH_TEST = 0x0b71,
    DEPTH_WRITEMASK = 0x0b72,
    DEPTH_CLEAR_VALUE = 0x0b73,
    DEPTH_FUNC = 0x0b74,
    ACCUM_CLEAR_VALUE = 0x0b80,
    STENCIL_TEST = 0x0b90,
    STENCIL_CLEAR_VALUE = 0x0b91,
    STENCIL_FUNC = 0x0b92,
    STENCIL_VALUE_MASK = 0x0b93,
    STENCIL_FAIL = 0x0b94,
    STENCIL_PASS_DEPTH_FAIL = 0x0b95,
    STENCIL_PASS_DEPTH_PASS = 0x0b96,
    STENCIL_REF = 0x0b97,
    STENCIL_WRITEMASK = 0x0b98,
    MATRIX_MODE = 0x0ba0,
    NORMALIZE = 0x0ba1,
    VIEWPORT = 0x0ba2,
    MODELVIEW_STACK_DEPTH = 0x0ba3,
    PROJECTION_STACK_DEPTH = 0x0ba4,
    TEXTURE_STACK_DEPTH = 0x0ba5,
    MODELVIEW_MATRIX = 0x0ba6,
    PROJECTION_MATRIX = 0x0ba7,
    TEXTURE_MATRIX = 0x0ba8,
    ATTRIB_STACK_DEPTH = 0x0bb0,
    CLIENT_ATTRIB_STACK_DEPTH = 0x0bb1,
    ALPHA_TEST = 0x0bc0,
    ALPHA_TEST_FUNC = 0x0bc1,
    ALPHA_TEST_REF = 0x0bc2,
    DITHER = 0x0bd0,
    BLEND_DST = 0x0be0,
    BLEND_SRC = 0x0be1,
    BLEND = 0x0be2,
    LOGIC_OP_MODE = 0x0bf0,
    INDEX_LOGIC_OP = 0x0bf1,
    COLOR_LOGIC_OP = 0x0bf2,
    AUX_BUFFERS = 0x0c00,
    DRAW_BUFFER = 0x0c01,
    READ_BUFFER = 0x0c02,
    SCISSOR_BOX = 0x0c10,
    SCISSOR_TEST = 0x0c11,
    INDEX_CLEAR_VALUE = 0x0c20,
    INDEX_WRITEMASK = 0x0c21,
    COLOR_CLEAR_VALUE = 0x0c22,
    COLOR_WRITEMASK = 0x0c23,
    INDEX_MODE = 0x0c30,
    RGBA_MODE = 0x0c31,
    DOUBLEBUFFER = 0x0c32,
    STEREO = 0x0c33,
    RENDER_MODE = 0x0c40,
    PERSPECTIVE_CORRECTION_HINT = 0x0c50,
    POINT_SMOOTH_HINT = 0x0c51,
    LINE_SMOOTH_HINT = 0x0c52,
    POLYGON_SMOOTH_HINT = 0x0c53,
    FOG_HINT = 0x0c54,
    TEXTURE_GEN_S = 0x0c60,
    TEXTURE_GEN_T = 0x0c61,
    TEXTURE_GEN_R = 0x0c62,
    TEXTURE_GEN_Q = 0x0c63,
    PIXEL_MAP_I_TO_I = 0x0c70,
    PIXEL_MAP_S_TO_S = 0x0c71,
    PIXEL_MAP_I_TO_R = 0x0c72,
    PIXEL_MAP_I_TO_G = 0x0c73,
    PIXEL_MAP_I_TO_B = 0x0c74,
    PIXEL_MAP_I_TO_A = 0x0c75,
    PIXEL_MAP_R_TO_R = 0x0c76,
    PIXEL_MAP_G_TO_G = 0x0c77,
    PIXEL_MAP_B_TO_B = 0x0c78,
    PIXEL_MAP_A_TO_A = 0x0c79,
    PIXEL_MAP_I_TO_I_SIZE = 0x0cb0,
    PIXEL_MAP_S_TO_S_SIZE = 0x0cb1,
    PIXEL_MAP_I_TO_R_SIZE = 0x0cb2,
    PIXEL_MAP_I_TO_G_SIZE = 0x0cb3,
    PIXEL_MAP_I_TO_B_SIZE = 0x0cb4,
    PIXEL_MAP_I_TO_A_SIZE = 0x0cb5,
    PIXEL_MAP_R_TO_R_SIZE = 0x0cb6,
    PIXEL_MAP_G_TO_G_SIZE = 0x0cb7,
    PIXEL_MAP_B_TO_B_SIZE = 0x0cb8,
    PIXEL_MAP_A_TO_A_SIZE = 0x0cb9,
    UNPACK_SWAP_BYTES = 0x0cf0,
    UNPACK_LSB_FIRST = 0x0cf1,
    UNPACK_ROW_LENGTH = 0x0cf2,
    UNPACK_SKIP_ROWS = 0x0cf3,
    UNPACK_SKIP_PIXELS = 0x0cf4,
    UNPACK_ALIGNMENT = 0x0cf5,
    PACK_SWAP_BYTES = 0x0d00,
    PACK_LSB_FIRST = 0x0d01,
    PACK_ROW_LENGTH = 0x0d02,
    PACK_SKIP_ROWS = 0x0d03,
    PACK_SKIP_PIXELS = 0x0d04,
    PACK_ALIGNMENT = 0x0d05,
    MAP_COLOR = 0x0d10,
    MAP_STENCIL = 0x0d11,
    INDEX_SHIFT = 0x0d12,
    INDEX_OFFSET = 0x0d13,
    RED_SCALE = 0x0d14,
    RED_BIAS = 0x0d15,
    ZOOM_X = 0x0d16,
    ZOOM_Y = 0x0d17,
    GREEN_SCALE = 0x0d18,
    GREEN_BIAS = 0x0d19,
    BLUE_SCALE = 0x0d1a,
    BLUE_BIAS = 0x0d1b,
    ALPHA_SCALE = 0x0d1c,
    ALPHA_BIAS = 0x0d1d,
    DEPTH_SCALE = 0x0d1e,
    DEPTH_BIAS = 0x0d1f,
    MAX_EVAL_ORDER = 0x0d30,
    MAX_LIGHTS = 0x0d31,
    MAX_CLIP_PLANES = 0x0d32,
    MAX_TEXTURE_SIZE = 0x0d33,
    MAX_PIXEL_MAP_TABLE = 0x0d34,
    MAX_ATTRIB_STACK_DEPTH = 0x0d35,
    MAX_MODELVIEW_STACK_DEPTH = 0x0d36,
    MAX_NAME_STACK_DEPTH = 0x0d37,
    MAX_PROJECTION_STACK_DEPTH = 0x0d38,
    MAX_TEXTURE_STACK_DEPTH = 0x0d39,
    MAX_VIEWPORT_DIMS = 0x0d3a,
    MAX_CLIENT_ATTRIB_STACK_DEPTH= 0x0d3b,
    SUBPIXEL_BITS = 0x0d50,
    INDEX_BITS = 0x0d51,
    RED_BITS = 0x0d52,
    GREEN_BITS = 0x0d53,
    BLUE_BITS = 0x0d54,
    ALPHA_BITS = 0x0d55,
    DEPTH_BITS = 0x0d56,
    STENCIL_BITS = 0x0d57,
    ACCUM_RED_BITS = 0x0d58,
    ACCUM_GREEN_BITS = 0x0d59,
    ACCUM_BLUE_BITS = 0x0d5a,
    ACCUM_ALPHA_BITS = 0x0d5b,
    NAME_STACK_DEPTH = 0x0d70,
    AUTO_NORMAL = 0x0d80,
    MAP1_COLOR_4 = 0x0d90,
    MAP1_INDEX = 0x0d91,
    MAP1_NORMAL = 0x0d92,
    MAP1_TEXTURE_COORD_1 = 0x0d93,
    MAP1_TEXTURE_COORD_2 = 0x0d94,
    MAP1_TEXTURE_COORD_3 = 0x0d95,
    MAP1_TEXTURE_COORD_4 = 0x0d96,
    MAP1_VERTEX_3 = 0x0d97,
    MAP1_VERTEX_4 = 0x0d98,
    MAP2_COLOR_4 = 0x0db0,
    MAP2_INDEX = 0x0db1,
    MAP2_NORMAL = 0x0db2,
    MAP2_TEXTURE_COORD_1 = 0x0db3,
    MAP2_TEXTURE_COORD_2 = 0x0db4,
    MAP2_TEXTURE_COORD_3 = 0x0db5,
    MAP2_TEXTURE_COORD_4 = 0x0db6,
    MAP2_VERTEX_3 = 0x0db7,
    MAP2_VERTEX_4 = 0x0db8,
    MAP1_GRID_DOMAIN = 0x0dd0,
    MAP1_GRID_SEGMENTS = 0x0dd1,
    MAP2_GRID_DOMAIN = 0x0dd2,
    MAP2_GRID_SEGMENTS = 0x0dd3,
    TEXTURE_1D = 0x0dee,
    TEXTURE_2D = 0x0de1,
    FEEDBACK_BUFFER_POINTER = 0x0df0,
    FEEDBACK_BUFFER_SIZE = 0x0df1,
    FEEDBACK_BUFFER_TYPE = 0x0df2,
    SELECTION_BUFFER_POINTER = 0x0df3,
    SELECTION_BUFFER_SIZE = 0x0df4,
    // TEXTURE_BINDING_1D
    // TEXTURE_BINDING_2D
    // VERTEX_ARRAY
    // NORMAL_ARRAY
    // COLOR_ARRAY
    // INDEX_ARRAY
    // TEXTURE_COORD_ARRAY
    // EDGE_FLAG_ARRAY
    // VERTEX_ARRAY_SIZE
    // VERTEX_ARRAY_TYPE
    // VERTEX_ARRAY_STRIDE
    // NORMAL_ARRAY_TYPE
    // NORMAL_ARRAY_STRIDE
    // COLOR_ARRAY_SIZE
    // COLOR_ARRAY_TYPE
    // COLOR_ARRAY_STRIDE
    // INDEX_ARRAY_TYPE
    // INDEX_ARRAY_STRIDE
    // TEXTURE_COORD_ARRAY_SIZE
    // TEXTURE_COORD_ARRAY_TYPE
    // TEXTURE_COORD_ARRAY_STRIDE
    // EDGE_FLAG_ARRAY_STRIDE
    // POLYGON_OFFSET_FACTOR
    // POLYGON_OFFSET_UNITS
    
    // GetTextureParameter
    // TEXTURE_MAG_FILTER
    // TEXTURE_MIN_FILTER
    // TEXTURE_WRAP_S
    // TEXTURE_WRAP_T
    TEXTURE_WIDTH = 0x1000,
    TEXTURE_HEIGHT = 0x1001,
    TEXTURE_INTERNAL_FORMAT = 0x1003,
    TEXTURE_BORDER_COLOR = 0x1004,
    TEXTURE_BORDER = 0x1005,
    // TEXTURE_RED_SIZE
    // TEXTURE_GREEN_SIZE
    // TEXTURE_BLUE_SIZE
    // TEXTURE_ALPHA_SIZE
    // TEXTURE_LUMINANCE_SIZE
    // TEXTURE_INTENSITY_SIZE
    // TEXTURE_PRIORITY
    // TEXTURE_RESIDENT
    DEPTH_TEXTURE_MODE = 0x884b,
    
    // HintMode
    DONT_CARE = 0x1100,
    FASTEST = 0x1101,
    NICEST = 0x1102,

    // HintTarget
    // PERSPECTIVE_CORRECTION_HINT
    // POINT_SMOOTH_HINT
    // LINE_SMOOTH_HINT
    // POLYGON_SMOOTH_HINT
    // FOG_HINT

    // IndexPointerType
    // SHORT
    // INT
    // FLOAT
    // DOUBLE

    // LightModelParameter
    // LIGHT_MODEL_AMBIENT
    // LIGHT_MODEL_LOCAL_VIEWER
    // LIGHT_MODEL_TWO_SIDE

    // LightName
    LIGHT0 = 0x4000,
    LIGHT1 = 0x4001,
    LIGHT2 = 0x4002,
    LIGHT3 = 0x4003,
    LIGHT4 = 0x4004,
    LIGHT5 = 0x4005,
    LIGHT6 = 0x4006,
    LIGHT7 = 0x4007,

    // LightParameter
    AMBIENT = 0x1200,
    DIFFUSE = 0x1201,
    SPECULAR = 0x1202,
    POSITION = 0x1203,
    SPOT_DIRECTION = 0x1204,
    SPOT_EXPONENT = 0x1205,
    SPOT_CUTOFF = 0x1206,
    CONSTANT_ATTENUATION = 0x1207,
    LINEAR_ATTENUATION = 0x1208,
    QUADRATIC_ATTENUATION = 0x1209,
    
    // ListMode
    COMPILE = 0x1300,
    COMPILE_AND_EXECUTE = 0x1301,

    // ListNameType
    // BYTE
    // UNSIGNED_BYTE
    // SHORT
    // UNSIGNED_SHORT
    // INT
    // UNSIGNED_INT
    // FLOAT
    // 2_BYTES
    // 3_BYTES
    // 4_BYTES

    // LogicOp
    CLEAR = 0x1500,
    AND = 0x1501,
    AND_REVERSE = 0x1502,
    COPY = 0x1503,
    AND_INVERTED = 0x1504,
    NOOP = 0x1505,
    XOR = 0x1506,
    OR = 0x1507,
    NOR = 0x1508,
    EQUIV = 0x1509,
    INVERT = 0x150a,
    OR_REVERSE = 0x150b,
    COPY_INVERTED = 0x150c,
    OR_INVERTED = 0x150d,
    NAND = 0x150e,
    SET = 0x150f,

    // MapTarget
    // MAP1_COLOR_4
    // MAP1_INDEX
    // MAP1_NORMAL
    // MAP1_TEXTURE_COORD_1
    // MAP1_TEXTURE_COORD_2
    // MAP1_TEXTURE_COORD_3
    // MAP1_TEXTURE_COORD_4
    // MAP1_VERTEX_3
    // MAP1_VERTEX_4
    // MAP2_COLOR_4
    // MAP2_INDEX
    // MAP2_NORMAL
    // MAP2_TEXTURE_COORD_1
    // MAP2_TEXTURE_COORD_2
    // MAP2_TEXTURE_COORD_3
    // MAP2_TEXTURE_COORD_4
    // MAP2_VERTEX_3
    // MAP2_VERTEX_4

    // MaterialFace
    // FRONT
    // BACK
    // FRONT_AND_BACK

    // MaterialParameter
    EMISSION = 0x1600,
    SHININESS = 0x1601,
    AMBIENT_AND_DIFFUSE = 0x1602,
    COLOR_INDEXES = 0x1603,
    // AMBIENT
    // DIFFUSE
    // SPECULAR

    // MatrixMode
    MODELVIEW = 0x1700,
    PROJECTION = 0x1701,
    TEXTURE = 0x1702,

    // MeshMode1
    // POINT
    // LINE

    // MeshMode2
    // POINT
    // LINE
    // FILL

    // NormalPointerType
    // BYTE
    // SHORT
    // INT
    // FLOAT
    // DOUBLE

    // PixelCopyType
    COLOR = 0x1800,
    DEPTH = 0x1801,
    STENCIL = 0x1802,

    // PixelFormat
    COLOR_INDEX = 0x1900,
    STENCIL_INDEX = 0x1901,
    DEPTH_COMPONENT = 0x1902,
    RED = 0x1903,
    GREEN = 0x1904,
    BLUE = 0x1905,
    ALPHA = 0x1906,
    RGB = 0x1907,
    RGBA = 0x1908,
    LUMINANCE = 0x1909,
    LUMINANCE_ALPHA = 0x190a,

    // PixelMap
    // PIXEL_MAP_I_TO_I
    // PIXEL_MAP_S_TO_S
    // PIXEL_MAP_I_TO_R
    // PIXEL_MAP_I_TO_G
    // PIXEL_MAP_I_TO_B
    // PIXEL_MAP_I_TO_A
    // PIXEL_MAP_R_TO_R
    // PIXEL_MAP_G_TO_G
    // PIXEL_MAP_B_TO_B
    // PIXEL_MAP_A_TO_A

    // PixelStore
    // UNPACK_SWAP_BYTES
    // UNPACK_LSB_FIRST
    // UNPACK_ROW_LENGTH
    // UNPACK_SKIP_ROWS
    // UNPACK_SKIP_PIXELS
    // UNPACK_ALIGNMENT
    // PACK_SWAP_BYTES
    // PACK_LSB_FIRST
    // PACK_ROW_LENGTH
    // PACK_SKIP_ROWS
    // PACK_SKIP_PIXELS
    // PACK_ALIGNMENT

    // PixelTransfer
    // MAP_COLOR
    // MAP_STENCIL
    // INDEX_SHIFT
    // INDEX_OFFSET
    // RED_SCALE
    // RED_BIAS
    // GREEN_SCALE
    // GREEN_BIAS
    // BLUE_SCALE
    // BLUE_BIAS
    // ALPHA_SCALE
    // ALPHA_BIAS
    // DEPTH_SCALE
    // DEPTH_BIAS

    // PixelType
    BITMAP = 0x1a00,
    // BYTE
    // UNSIGNED_BYTE
    // SHORT
    // UNSIGNED_SHORT
    // INT
    // UNSIGNED_INT
    // FLOAT

    // PolygonMode
    POINT = 0x1b00,
    LINE = 0x1b01,
    FILL = 0x1b02,

    // ReadBufferMode
    // FRONT_LEFT
    // FRONT_RIGHT
    // BACK_LEFT
    // BACK_RIGHT
    // FRONT
    // BACK
    // LEFT
    // RIGHT
    // AUX0
    // AUX1
    // AUX2
    // AUX3

    // RenderingMode
    RENDER = 0x1c00,
    FEEDBACK = 0x1c01,
    SELECT = 0x1c02,

    // ShadingModel
    FLAT = 0x1d00,
    SMOOTH = 0x1d01,
    
    // StencilFunction
    // NEVER
    // LESS
    // EQUAL
    // LEQUAL
    // GREATER
    // NOTEQUAL
    // GEQUAL
    // ALWAYS

    // StencilOp
    // ZERO
    KEEP = 0x1e00,
    REPLACE = 0x1e01,
    INCR = 0x1e02,
    DECR = 0x1e03,
    // INVERT

    // StringName
    VENDOR = 0x1f00,
    RENDERER = 0x1f01,
    VERSION = 0x1f02,
    EXTENSIONS = 0x1f03,

    // TextureCoordName
    S = 0x2000,
    T = 0x2001,
    R = 0x2002,
    Q = 0x2003,

    // TexCoordPointerType
    // SHORT
    // INT
    // FLOAT
    // DOUBLE

    // TextureEnvMode
    MODULATE = 0x2100,
    DECAL = 0x2101,
    // BLEND
    // REPLACE

    // TextureEnvParameter
    TEXTURE_ENV_MODE = 0x2200,
    TEXTURE_ENV_COLOR = 0x2201,

    // TextureEnvTarget
    TEXTURE_ENV = 0x2300,

    // TextureGenMode
    EYE_LINEAR = 0x2400,
    OBJECT_LINEAR = 0x2401,
    SPHERE_MA = 0x2402,

    // TextureGenParameter
    TEXTURE_GEN_MODE = 0x2500,
    OBJECT_PLANE = 0x2501,
    EYE_PLANE = 0x2502,
    
    // TextureMagFilter
    NEAREST = 0x2600,
    LINEAR = 0x2601,

    // TextureMinFilter
    // NEAREST
    // LINEAR
    NEAREST_MIPMAP_NEAREST = 0x2700,
    LINEAR_MIPMAP_NEAREST = 0x2701,
    NEAREST_MIPMAP_LINEAR = 0x2702,
    LINEAR_MIPMAP_LINEAR = 0x2703,
    
    // TextureParameterName
    TEXTURE_MAG_FILTER = 0x2800,
    TEXTURE_MIN_FILTER = 0x2801,
    TEXTURE_WRAP_S = 0x2802,
    TEXTURE_WRAP_T = 0x2803,
    // TEXTURE_BORDER_COLOR
    // TEXTURE_PRIORITY

    // TextureTarget
    // TEXTURE_1D
    // TEXTURE_2D
    // PROXY_TEXTURE_1D
    // PROXY_TEXTURE_2D

    // TextureWrapMode
    CLAMP = 0x2900,
    REPEAT = 0x2901,

    // VertexPointerType
    // SHORT
    // INT
    // FLOAT
    // DOUBLE

    // ClientAttribMask
    CLIENT_PIXEL_STORE_BIT = 0x00000001,
    CLIENT_VERTEX_ARRAY_BIT = 0x00000002,
    CLIENT_ALL_ATTRIB_BITS = 0xffffffff,

    // polygon_offset
    POLYGON_OFFSET_FACTOR = 0x8038,
    POLYGON_OFFSET_UNITS = 0x2a00,
    POLYGON_OFFSET_POINT = 0x2a01,
    POLYGON_OFFSET_LINE = 0x2a02,
    POLYGON_OFFSET_FILL = 0x8037,

    // texture
    ALPHA4 = 0x803b,
    ALPHA8 = 0x803c,
    ALPHA12 = 0x803d,
    ALPHA16 = 0x803e,
    LUMINANCE4 = 0x803f,
    LUMINANCE8 = 0x8040,
    LUMINANCE12 = 0x8041,
    LUMINANCE16 = 0x8042,
    LUMINANCE4_ALPHA4 = 0x8043,
    LUMINANCE6_ALPHA2 = 0x8044,
    LUMINANCE8_ALPHA8 = 0x8045,
    LUMINANCE12_ALPHA4 = 0x8046,
    LUMINANCE12_ALPHA12 = 0x8047,
    LUMINANCE16_ALPHA16 = 0x8048,
    INTENSITY = 0x8049,
    INTENSITY4 = 0x804a,
    INTENSITY8 = 0x804b,
    INTENSITY12 = 0x804c,
    INTENSITY16 = 0x804d,
    R3_G3_B2 = 0x2a10,
    RGB4 = 0x804f,
    RGB5 = 0x8050,
    RGB8 = 0x8051,
    RGB10 = 0x8052,
    RGB12 = 0x8053,
    RGB16 = 0x8054,
    RGBA2 = 0x8055,
    RGBA4 = 0x8056,
    RGB5_A1 = 0x8057,
    RGBA8 = 0x8058,
    RGB10_A2 = 0x8059,
    RGBA12 = 0x805a,
    RGBA16 = 0x805b,
    TEXTURE_RED_SIZE = 0x805c,
    TEXTURE_GREEN_SIZE = 0x805d,
    TEXTURE_BLUE_SIZE = 0x805e,
    TEXTURE_ALPHA_SIZE = 0x805f,
    TEXTURE_LUMINANCE_SIZE = 0x8060,
    TEXTURE_INTENSITY_SIZE = 0x8061,
    PROXY_TEXTURE_1D = 0x8063,
    PROXY_TEXTURE_2D = 0x8064,

    // texture_object
    TEXTURE_PRIORITY = 0x8066,
    TEXTURE_RESIDENT = 0x8067,
    TEXTURE_BINDING_1D = 0x8068,
    TEXTURE_BINDING_2D = 0x8069,

    // vertex_array
    VERTEX_ARRAY = 0x8074,
    NORMAL_ARRAY = 0x8075,
    COLOR_ARRAY = 0x8076,
    INDEX_ARRAY = 0x8077,
    TEXTURE_COORD_ARRAY = 0x8078,
    EDGE_FLAG_ARRAY = 0x8079,
    VERTEX_ARRAY_SIZE = 0x807a,
    VERTEX_ARRAY_TYPE = 0x807b,
    VERTEX_ARRAY_STRIDE = 0x807c,
    NORMAL_ARRAY_TYPE = 0x807e,
    NORMAL_ARRAY_STRIDE = 0x807f,
    COLOR_ARRAY_SIZE = 0x8081,
    COLOR_ARRAY_TYPE = 0x8082,
    COLOR_ARRAY_STRIDE = 0x8083,
    INDEX_ARRAY_TYPE = 0x8085,
    INDEX_ARRAY_STRIDE = 0x8086,
    TEXTURE_COORD_ARRAY_SIZE = 0x8088,
    TEXTURE_COORD_ARRAY_TYPE = 0x8089,
    TEXTURE_COORD_ARRAY_STRIDE = 0x808a,
    EDGE_FLAG_ARRAY_STRIDE = 0x808c,
    VERTEX_ARRAY_POINTER = 0x808e,
    NORMAL_ARRAY_POINTER = 0x808f,
    COLOR_ARRAY_POINTER = 0x8090,
    INDEX_ARRAY_POINTER = 0x8091,
    TEXTURE_COORD_ARRAY_POINTER = 0x8092,
    EDGE_FLAG_ARRAY_POINTER = 0x8093,
    V2F = 0x2a20,
    V3F = 0x2a21,
    C4UB_V2F = 0x2a22,
    C4UB_V3F = 0x2a23,
    C3F_V3F = 0x2a24,
    N3F_V3F = 0x2a25,
    C4F_N3F_V3F = 0x2a26,
    T2F_V3F = 0x2a27,
    T4F_V4F = 0x2a28,
    T2F_C4UB_V3F = 0x2a29,
    T2F_C3F_V3F = 0x2a2a,
    T2F_N3F_V3F = 0x2a2b,
    T2F_C4F_N3F_V3F = 0x2a2c,
    T4F_C4F_N3F_V4F = 0x2a2d,

    // Extensions
    EXT_vertex_array = 1,
    EXT_bgra = 1,
    EXT_paletted_texture = 1,
    WIN_swap_hint = 1,
    WIN_draw_range_elements = 1,
    // WIN_phong_shading = 1,
    // WIN_specular_fog = 1,

    // EXT_vertex_array
    VERTEX_ARRAY_EXT = 0x8074,
    NORMAL_ARRAY_EXT = 0x8075,
    COLOR_ARRAY_EXT = 0x8076,
    INDEX_ARRAY_EXT = 0x8077,
    TEXTURE_COORD_ARRAY_EXT = 0x8078,
    EDGE_FLAG_ARRAY_EXT = 0x8079,
    VERTEX_ARRAY_SIZE_EXT = 0x807a,
    VERTEX_ARRAY_TYPE_EXT = 0x807b,
    VERTEX_ARRAY_STRIDE_EXT = 0x807c,
    VERTEX_ARRAY_COUNT_EXT = 0x807d,
    NORMAL_ARRAY_TYPE_EXT = 0x807e,
    NORMAL_ARRAY_STRIDE_EXT = 0x807f,
    NORMAL_ARRAY_COUNT_EXT = 0x8080,
    COLOR_ARRAY_SIZE_EXT = 0x8081,
    COLOR_ARRAY_TYPE_EXT = 0x8082,
    COLOR_ARRAY_STRIDE_EXT = 0x8083,
    COLOR_ARRAY_COUNT_EXT = 0x8084,
    INDEX_ARRAY_TYPE_EXT = 0x8085,
    INDEX_ARRAY_STRIDE_EXT = 0x8086,
    INDEX_ARRAY_COUNT_EXT = 0x8087,
    TEXTURE_COORD_ARRAY_SIZE_EXT = 0x8088,
    TEXTURE_COORD_ARRAY_TYPE_EXT = 0x8089,
    TEXTURE_COORD_ARRAY_STRIDE_EXT = 0x808a,
    TEXTURE_COORD_ARRAY_COUNT_EXT = 0x808b,
    EDGE_FLAG_ARRAY_STRIDE_EXT = 0x808c,
    EDGE_FLAG_ARRAY_COUNT_EXT = 0x808d,
    VERTEX_ARRAY_POINTER_EXT = 0x808e,
    NORMAL_ARRAY_POINTER_EXT = 0x808f,
    COLOR_ARRAY_POINTER_EXT = 0x8090,
    INDEX_ARRAY_POINTER_EXT = 0x8091,
    TEXTURE_COORD_ARRAY_POINTER_EXT = 0x8092,
    EDGE_FLAG_ARRAY_POINTER_EXT = 0x8093,
    DOUBLE_EXT = DOUBLE, // FIXME
    // EXT_bgra
    BGR_EXT = 0x80e0,
    BGRA_EXT = 0x80e1,

    // EXT_paletted_texture

    // These must match the COLOR_TABLE_*_SGI enumerants
    COLOR_TABLE_FORMAT_EXT = 0x80d8,
    COLOR_TABLE_WIDTH_EXT = 0x80d9,
    COLOR_TABLE_RED_SIZE_EXT = 0x80da,
    COLOR_TABLE_GREEN_SIZE_EXT = 0x80db,
    COLOR_TABLE_BLUE_SIZE_EXT = 0x80dc,
    COLOR_TABLE_ALPHA_SIZE_EXT = 0x80dd,
    COLOR_TABLE_LUMINANCE_SIZE_EXT = 0x80de,
    COLOR_TABLE_INTENSITY_SIZE_EXT = 0x80df,
    
    COLOR_INDEX1_EXT = 0x80e2,
    COLOR_INDEX2_EXT = 0x80e3,
    COLOR_INDEX4_EXT = 0x80e4,
    COLOR_INDEX8_EXT = 0x80e5,
    COLOR_INDEX12_EXT = 0x80e6,
    COLOR_INDEX16_EXT = 0x80e7,

    // WIN_draw_range_elements
    MAX_ELEMENTS_VERTICES_WIN = 0x80e8,
    MAX_ELEMENTS_INDICES_WIN = 0x80e9,

    // WIN_phong_shading
    PHONG_WIN = 0x80EA,
    PHONG_HINT_WIN = 0x80eb,

    // WIN_specular_fog
    FOG_SPECULAR_TEXTURE_WIN = 0x80ec
  };

  enum {
      
    // Extensions
    ARB_imaging = 1,
    ARB_multitexture = 1,

    // BlendEquationMode
    // LOGIC_OP
    // FUNC_ADD
    // MIN
    // MAX
    // FUNC_SUBTRACT
    // FUNC_REVERSE_SUBTRACT
      
    // BlendingFactorDest
    // CONSTANT_COLOR
    // ONE_MINUS_CONSTANT_COLOR
    // CONSTANT_ALPHA
    // ONE_MINUS_CONSTANT_ALPHA
      
    // BlendingFactorSrc
    // CONSTANT_COLOR
    // ONE_MINUS_CONSTANT_COLOR
    // CONSTANT_ALPHA
    // ONE_MINUS_CONSTANT_ALPHA
      
    // ColorTableParameterPName
    // COLOR_TABLE_SCALE
    // COLOR_TABLE_BIAS
      
    // ColorTableTarget
    // COLOR_TABLE
    // POST_CONVOLUTION_COLOR_TABLE
    // POST_COLOR_MATRIX_COLOR_TABLE
    // PROXY_COLOR_TABLE
    // PROXY_POST_CONVOLUTION_COLOR_TABLE
    // PROXY_POST_COLOR_MATRIX_COLOR_TABLE
      
    // ConvolutionBorderMode
    // REDUCE
    // IGNORE_BORDER
    // CONSTANT_BORDER
      
    // ConvolutionParameter
    // CONVOLUTION_BORDER_MODE
    // CONVOLUTION_FILTER_SCALE
    // CONVOLUTION_FILTER_BIAS
      
    // ConvolutionTarget
    // CONVOLUTION_1D
    // CONVOLUTION_2D
      
    // Enable
    // COLOR_TABLE
    // POST_CONVOLUTION_COLOR_TABLE
    // POST_COLOR_MATRIX_COLOR_TABLE
    // CONVOLUTION_1D
    // CONVOLUTION_2D
    // SEPARABLE_2D
    // HISTOGRAM
    // MINMAX
    // RESCALE_NORMAL
    // TEXTURE_3D
      
    // ErrorCode
    // TABLE_TOO_LARGE
      
    // GetColorTableParameterPName
    // COLOR_TABLE_SCALE
    // COLOR_TABLE_BIAS
    // COLOR_TABLE_FORMAT
    // COLOR_TABLE_WIDTH
    // COLOR_TABLE_RED_SIZE
    // COLOR_TABLE_GREEN_SIZE
    // COLOR_TABLE_BLUE_SIZE
    // COLOR_TABLE_ALPHA_SIZE
    // COLOR_TABLE_LUMINANCE_SIZE
    // COLOR_TABLE_INTENSITY_SIZE
      
    // GetConvolutionParameterPName
    // CONVOLUTION_BORDER_COLOR
    // CONVOLUTION_BORDER_MODE
    // CONVOLUTION_FILTER_SCALE
    // CONVOLUTION_FILTER_BIAS
    // CONVOLUTION_FORMAT
    // CONVOLUTION_WIDTH
    // CONVOLUTION_HEIGHT
    // MAX_CONVOLUTION_WIDTH
    // MAX_CONVOLUTION_HEIGHT
      
    // GetHistogramParameterPName
    // HISTOGRAM_WIDTH
    // HISTOGRAM_FORMAT
    // HISTOGRAM_RED_SIZE
    // HISTOGRAM_GREEN_SIZE
    // HISTOGRAM_BLUE_SIZE
    // HISTOGRAM_ALPHA_SIZE
    // HISTOGRAM_LUMINANCE_SIZE
    // HISTOGRAM_SINK
      
    // GetMinmaxParameterPName
    // MINMAX_FORMAT
    // MINMAX_SINK
      
    // GetTarget
    // SMOOTH_POINT_SIZE_RANGE
    // SMOOTH_POINT_SIZE_GRANULARITY
    // SMOOTH_LINE_WIDTH_RANGE
    // SMOOTH_LINE_WIDTH_GRANULARITY
    // ALIASED_POINT_SIZE_RANGE
    // ALIASED_LINE_WIDTH_RANGE
    // TEXTURE_BINDING_3D
    // COLOR_TABLE
    // POST_CONVOLUTION_COLOR_TABLE
    // POST_COLOR_MATRIX_COLOR_TABLE
    // CONVOLUTION_1D
    // CONVOLUTION_2D
    // SEPARABLE_2D
    // POST_CONVOLUTION_RED_SCALE
    // POST_CONVOLUTION_GREEN_SCALE
    // POST_CONVOLUTION_BLUE_SCALE
    // POST_CONVOLUTION_ALPHA_SCALE
    // POST_CONVOLUTION_RED_BIAS
    // POST_CONVOLUTION_GREEN_BIAS
    // POST_CONVOLUTION_BLUE_BIAS
    // POST_CONVOLUTION_ALPHA_BIAS
    // COLOR_MATRIX
    // COLOR_MATRIX_STACK_DEPTH
    // MAX_COLOR_MATRIX_STACK_DEPTH
    // POST_COLOR_MATRIX_RED_SCALE
    // POST_COLOR_MATRIX_GREEN_SCALE
    // POST_COLOR_MATRIX_BLUE_SCALE
    // POST_COLOR_MATRIX_ALPHA_SCALE
    // POST_COLOR_MATRIX_RED_BIAS
    // POST_COLOR_MATRIX_GREEN_BIAS
    // POST_COLOR_MATRIX_BLUE_BIAS
    // POST_COLOR_MATRIX_ALPHA_BIAS
    // HISTOGRAM
    // MINMAX
    // MAX_ELEMENTS_VERTICES
    // MAX_ELEMENTS_INDICES
    // RESCALE_NORMAL
    // LIGHT_MODEL_COLOR_CONTROL
    // PACK_SKIP_IMAGES
    // PACK_IMAGE_HEIGHT
    // UNPACK_SKIP_IMAGES
    // UNPACK_IMAGE_HEIGHT
    // TEXTURE_3D
    // MAX_3D_TEXTURE_SIZE
    // BLEND_COLOR
    // BLEND_EQUATION
    // ACTIVE_TEXTURE_ARB
    // CLIENT_ACTIVE_TEXTURE_ARB
    // MAX_TEXTURE_UNITS_ARB
      
    // GetTextureParameter
    // TEXTURE_DEPTH
    // TEXTURE_WRAP_R
    // TEXTURE_MIN_LOD
    // TEXTURE_MAX_LOD
    // TEXTURE_BASE_LEVEL
    // TEXTURE_MAX_LEVEL
      
    // HistogramTarget
    // HISTOGRAM
    // PROXY_HISTOGRAM
      
    // LightModelColorControl
    // SINGLE_COLOR
    // SEPARATE_SPECULAR_COLOR
      
    // LightModelParameter
    // LIGHT_MODEL_COLOR_CONTROL
      
    // MinmaxTarget
    // MINMAX
      
    // PixelFormat
    // ABGR
      
    // PixelInternalFormat
    // ALPHA4
    // ALPHA8
    // ALPHA12
    // ALPHA16
    // LUMINANCE4
    // LUMINANCE8
    // LUMINANCE12
    // LUMINANCE16
    // LUMINANCE4_ALPHA4
    // LUMINANCE6_ALPHA2
    // LUMINANCE8_ALPHA8
    // LUMINANCE12_ALPHA4
    // LUMINANCE12_ALPHA12
    // LUMINANCE16_ALPHA16
    // INTENSITY
    // INTENSITY4
    // INTENSITY8
    // INTENSITY12
    // INTENSITY16
    // R3_G3_B2
    // RGB4
    // RGB5
    // RGB8
    // RGB10
    // RGB12
    // RGB16
    // RGBA2
    // RGBA4
    // RGB5_A1
    // RGBA8
    // RGB10_A2
    // RGBA12
    // RGBA16

    // PixelStore
    // PACK_SKIP_IMAGES
    // PACK_IMAGE_HEIGHT
    // UNPACK_SKIP_IMAGES
    // UNPACK_IMAGE_HEIGHT
      
    // PixelTransfer
    // POST_CONVOLUTION_RED_SCALE
    // POST_CONVOLUTION_GREEN_SCALE
    // POST_CONVOLUTION_BLUE_SCALE
    // POST_CONVOLUTION_ALPHA_SCALE
    // POST_CONVOLUTION_RED_BIAS
    // POST_CONVOLUTION_GREEN_BIAS
    // POST_CONVOLUTION_BLUE_BIAS
    // POST_CONVOLUTION_ALPHA_BIAS
    // POST_COLOR_MATRIX_RED_SCALE
    // POST_COLOR_MATRIX_GREEN_SCALE
    // POST_COLOR_MATRIX_BLUE_SCALE
    // POST_COLOR_MATRIX_ALPHA_SCALE
    // POST_COLOR_MATRIX_RED_BIAS
    // POST_COLOR_MATRIX_GREEN_BIAS
    // POST_COLOR_MATRIX_BLUE_BIAS
    // POST_COLOR_MATRIX_ALPHA_BIAS

    // PixelType
    // BGR
    // BGRA
    // UNSIGNED_BYTE_3_3_2
    // UNSIGNED_SHORT_4_4_4_4
    // UNSIGNED_SHORT_5_5_5_1
    // UNSIGNED_INT_8_8_8_8
    // UNSIGNED_INT_10_10_10_2
    // UNSIGNED_SHORT_5_6_5
    // UNSIGNED_BYTE_2_3_3_REV
    // UNSIGNED_SHORT_5_6_5_REV
    // UNSIGNED_SHORT_4_4_4_4_REV
    // UNSIGNED_SHORT_1_5_5_5_REV
    // UNSIGNED_INT_8_8_8_8_REV
    // UNSIGNED_INT_2_10_10_10_REV
      
    // SeparableTarget
    // SEPARABLE_2D

    // TextureParameterName
    // TEXTURE_WRAP_R
    // TEXTURE_MIN_LOD
    // TEXTURE_MAX_LOD
    // TEXTURE_BASE_LEVEL
    // TEXTURE_MAX_LEVEL
      
    // TextureTarget
    // TEXTURE_3D
    // PROXY_TEXTURE_3D
      
    // TextureUnit
    // TEXTURE0_ARB
    // TEXTURE1_ARB
    // TEXTURE2_ARB
    // TEXTURE3_ARB
    // TEXTURE4_ARB
    // TEXTURE5_ARB
    // TEXTURE6_ARB
    // TEXTURE7_ARB
    // TEXTURE8_ARB
    // TEXTURE9_ARB
    // TEXTURE10_ARB
    // TEXTURE11_ARB
    // TEXTURE12_ARB
    // TEXTURE13_ARB
    // TEXTURE14_ARB
    // TEXTURE15_ARB
    // TEXTURE16_ARB
    // TEXTURE17_ARB
    // TEXTURE18_ARB
    // TEXTURE19_ARB
    // TEXTURE20_ARB
    // TEXTURE21_ARB
    // TEXTURE22_ARB
    // TEXTURE23_ARB
    // TEXTURE24_ARB
    // TEXTURE25_ARB
    // TEXTURE26_ARB
    // TEXTURE27_ARB
    // TEXTURE28_ARB
    // TEXTURE29_ARB
    // TEXTURE30_ARB
    // TEXTURE31_ARB
      
    // TextureWrapMode
    // CLAMP_TO_EDGE
      
    // texture_object
    TEXTURE_BINDING_3D = 0x806a,
      
    // bgra
    BGR = 0x80e0,
    BGRA = 0x80e1,
      
    // blend_color
    CONSTANT_COLOR = 0x8001,
    ONE_MINUS_CONSTANT_COLOR = 0x8002,
    CONSTANT_ALPHA = 0x8003,
    ONE_MINUS_CONSTANT_ALPHA = 0x8004,
    BLEND_COLOR = 0x8005,
      
    // blend_minmax
    FUNC_ADD = 0x8006,
    MIN = 0x8007,
    MAX = 0x8008,
    BLEND_EQUATION = 0x8009,
      
    // blend_subtract
    FUNC_SUBTRACT = 0x800a,
    FUNC_REVERSE_SUBTRACT = 0x800b,
      
    // color_matrix
    COLOR_MATRIX = 0x80b1,
    COLOR_MATRIX_STACK_DEPTH = 0x80b2,
    MAX_COLOR_MATRIX_STACK_DEPTH = 0x80b3,
    POST_COLOR_MATRIX_RED_SCALE = 0x80b4,
    POST_COLOR_MATRIX_GREEN_SCALE = 0x80b5,
    POST_COLOR_MATRIX_BLUE_SCALE = 0x80b6,
    POST_COLOR_MATRIX_ALPHA_SCALE = 0x80b7,
    POST_COLOR_MATRIX_RED_BIAS = 0x80b8,
    POST_COLOR_MATRIX_GREEN_BIAS = 0x80b9,
    POST_COLOR_MATRIX_BLUE_BIAS = 0x80ba,
    POST_COLOR_MATRIX_ALPHA_BIAS = 0x80bb,
      
    // color_table
    COLOR_TABLE = 0x80d0,
    POST_CONVOLUTION_COLOR_TABLE = 0x80d1,
    POST_COLOR_MATRIX_COLOR_TABLE = 0x80d2,
    PROXY_COLOR_TABLE = 0x80d3,
    PROXY_POST_CONVOLUTION_COLOR_TABLE = 0x80d4,
    PROXY_POST_COLOR_MATRIX_COLOR_TABLE = 0x80d5,
    COLOR_TABLE_SCALE = 0x80d6,
    COLOR_TABLE_BIAS = 0x80d7,
    COLOR_TABLE_FORMAT = 0x80d8,
    COLOR_TABLE_WIDTH = 0x80d9,
    COLOR_TABLE_RED_SIZE = 0x80da,
    COLOR_TABLE_GREEN_SIZE = 0x80db,
    COLOR_TABLE_BLUE_SIZE = 0x80dc,
    COLOR_TABLE_ALPHA_SIZE = 0x80dd,
    COLOR_TABLE_LUMINANCE_SIZE = 0x80de,
    COLOR_TABLE_INTENSITY_SIZE = 0x80df,

    // convolution
    CONVOLUTION_1D = 0x8010,
    CONVOLUTION_2D = 0x8011,
    SEPARABLE_2D = 0x8012,
    CONVOLUTION_BORDER_MODE = 0x8013,
    CONVOLUTION_FILTER_SCALE = 0x8014,
    CONVOLUTION_FILTER_BIAS = 0x8015,
    REDUCE = 0x8016,
    CONVOLUTION_FORMAT = 0x8017,
    CONVOLUTION_WIDTH = 0x8018,
    CONVOLUTION_HEIGHT = 0x8019,
    MAX_CONVOLUTION_WIDTH = 0x801a,
    MAX_CONVOLUTION_HEIGHT = 0x801b,
    POST_CONVOLUTION_RED_SCALE = 0x801c,
    POST_CONVOLUTION_GREEN_SCALE = 0x801d,
    POST_CONVOLUTION_BLUE_SCALE = 0x801e,
    POST_CONVOLUTION_ALPHA_SCALE = 0x801f,
    POST_CONVOLUTION_RED_BIAS = 0x8020,
    POST_CONVOLUTION_GREEN_BIAS = 0x8021,
    POST_CONVOLUTION_BLUE_BIAS = 0x8022,
    POST_CONVOLUTION_ALPHA_BIAS = 0x8023,
    CONSTANT_BORDER = 0x8151,
    REPLICATE_BORDER = 0x8153,
    CONVOLUTION_BORDER_COLOR = 0x8154,
      
    // draw_range_elements
    MAX_ELEMENTS_VERTICES = 0x80e8,
    MAX_ELEMENTS_INDICES = 0x80e9,
      
    // histogram
    HISTOGRAM = 0x8024,
    PROXY_HISTOGRAM = 0x8025,
    HISTOGRAM_WIDTH = 0x8026,
    HISTOGRAM_FORMAT = 0x8027,
    HISTOGRAM_RED_SIZE = 0x8028,
    HISTOGRAM_GREEN_SIZE = 0x8029,
    HISTOGRAM_BLUE_SIZE = 0x802a,
    HISTOGRAM_ALPHA_SIZE = 0x802b,
    HISTOGRAM_LUMINANCE_SIZE = 0x802c,
    HISTOGRAM_SINK = 0x802d,
    MINMAX = 0x802e,
    MINMAX_FORMAT = 0x802f,
    MINMAX_SINK = 0x8030,
    TABLE_TOO_LARGE = 0x8031,

    // packed_pixels
    UNSIGNED_BYTE_3_3_2 = 0x8032,
    UNSIGNED_SHORT_4_4_4_4 = 0x8033,
    UNSIGNED_SHORT_5_5_5_1 = 0x8034,
    UNSIGNED_INT_8_8_8_8 = 0x8035,
    UNSIGNED_INT_10_10_10_2 = 0x8036,
    UNSIGNED_BYTE_2_3_3_REV = 0x8362,
    UNSIGNED_SHORT_5_6_5 = 0x8363,
    UNSIGNED_SHORT_5_6_5_REV = 0x8364,
    UNSIGNED_SHORT_4_4_4_4_REV = 0x8365,
    UNSIGNED_SHORT_1_5_5_5_REV = 0x8366,
    UNSIGNED_INT_8_8_8_8_REV = 0x8367,
    UNSIGNED_INT_2_10_10_10_REV = 0x8368,

    // rescale_normal
    RESCALE_NORMAL = 0x803a,

    // separate_specular_color
    LIGHT_MODEL_COLOR_CONTROL = 0x81f8,
    SINGLE_COLOR = 0x81f9,
    SEPARATE_SPECULAR_COLOR = 0x81fa,

    // texture3D
    PACK_SKIP_IMAGES = 0x806b,
    PACK_IMAGE_HEIGHT = 0x806c,
    UNPACK_SKIP_IMAGES = 0x806d,
    UNPACK_IMAGE_HEIGHT = 0x806e,
    TEXTURE_3D = 0x806f,
    PROXY_TEXTURE_3D = 0x8070,
    TEXTURE_DEPTH = 0x8071,
    TEXTURE_WRAP_R = 0x8072,
    MAX_3D_TEXTURE_SIZE = 0x8073,
      
    // texture_edge_clamp
    CLAMP_TO_EDGE = 0x812f,

    // texture_lod
    TEXTURE_MIN_LOD = 0x813a,
    TEXTURE_MAX_LOD = 0x813b,
    TEXTURE_BASE_LEVEL = 0x813c,
    TEXTURE_MAX_LEVEL = 0x813d,
      
    // GetTarget1_2
    SMOOTH_POINT_SIZE_RANGE = 0x0b12,
    SMOOTH_POINT_SIZE_GRANULARITY = 0x0b13,
    SMOOTH_LINE_WIDTH_RANGE = 0x0b22,
    SMOOTH_LINE_WIDTH_GRANULARITY = 0x0b23,
    ALIASED_POINT_SIZE_RANGE = 0x846d,
    ALIASED_LINE_WIDTH_RANGE = 0x846e,

    // multitexture
    TEXTURE0_ARB = 0x84c0,
    TEXTURE1_ARB = 0x84c1,
    TEXTURE2_ARB = 0x84c2,
    TEXTURE3_ARB = 0x84c3,
    TEXTURE4_ARB = 0x84c4,
    TEXTURE5_ARB = 0x84c5,
    TEXTURE6_ARB = 0x84c6,
    TEXTURE7_ARB = 0x84c7,
    TEXTURE8_ARB = 0x84c8,
    TEXTURE9_ARB = 0x84c9,
    TEXTURE10_ARB = 0x84ca,
    TEXTURE11_ARB = 0x84cb,
    TEXTURE12_ARB = 0x84cc,
    TEXTURE13_ARB = 0x84cd,
    TEXTURE14_ARB = 0x84ce,
    TEXTURE15_ARB = 0x84cf,
    TEXTURE16_ARB = 0x84d0,
    TEXTURE17_ARB = 0x84d1,
    TEXTURE18_ARB = 0x84d2,
    TEXTURE19_ARB = 0x84d3,
    TEXTURE20_ARB = 0x84d4,
    TEXTURE21_ARB = 0x84d5,
    TEXTURE22_ARB = 0x84d6,
    TEXTURE23_ARB = 0x84d7,
    TEXTURE24_ARB = 0x84d8,
    TEXTURE25_ARB = 0x84d9,
    TEXTURE26_ARB = 0x84da,
    TEXTURE27_ARB = 0x84db,
    TEXTURE28_ARB = 0x84dc,
    TEXTURE29_ARB = 0x84dd,
    TEXTURE30_ARB = 0x84de,
    TEXTURE31_ARB = 0x84df,
    ACTIVE_TEXTURE_ARB = 0x84e0,
    CLIENT_ACTIVE_TEXTURE_ARB = 0x84e1,
    MAX_TEXTURE_UNITS_ARB = 0x84e2,

    // Extensions
    EXT_abgr = 1,
    EXT_blend_color = 1,
    EXT_blend_minmax = 1,
    EXT_blend_subtract = 1,
    EXT_texture_env_combine = 1,
    EXT_texture_env_add = 1,
      
    // EXT_abgr
    ABGR_EXT = 0x8000,
      
    // EXT_blend_color
    CONSTANT_COLOR_EXT = 0x8001,
    ONE_MINUS_CONSTANT_COLOR_EXT = 0x8002,
    CONSTANT_ALPHA_EXT = 0x8003,
    ONE_MINUS_CONSTANT_ALPHA_EXT = 0x8004,
    BLEND_COLOR_EXT = 0x8005,
      
    // EXT_blend_minmax
    FUNC_ADD_EXT = 0x8006,
    MIN_EXT = 0x8007,
    MAX_EXT = 0x8008,
    BLEND_EQUATION_EXT = 0x8009,
      
    // EXT_blend_subtract
    FUNC_SUBTRACT_EXT = 0x800a,
    FUNC_REVERSE_SUBTRACT_EXT = 0x800b,
      
    // EXT_texture_env_combine
    COMBINE_EXT = 0x8570,
    COMBINE_RGB_EXT = 0x8571,
    COMBINE_ALPHA_EXT = 0x8572,
    RGB_SCALE_EXT = 0x8573,
    ADD_SIGNED_EXT = 0x8574,
    INTERPOLATE_EXT = 0x8575,
    CONSTANT_EXT = 0x8576,
    PRIMARY_COLOR_EXT = 0x8577,
    PREVIOUS_EXT = 0x8578,
    SOURCE0_RGB_EXT = 0x8580,
    SOURCE1_RGB_EXT = 0x8581,
    SOURCE2_RGB_EXT = 0x8582,
    SOURCE0_ALPHA_EXT = 0x8588,
    SOURCE1_ALPHA_EXT = 0x8589,
    SOURCE2_ALPHA_EXT = 0x858a,
    OPERAND0_RGB_EXT = 0x8590,
    OPERAND1_RGB_EXT = 0x8591,
    OPERAND2_RGB_EXT = 0x8592,
    OPERAND0_ALPHA_EXT = 0x8598,
    OPERAND1_ALPHA_EXT = 0x8599,
    OPERAND2_ALPHA_EXT = 0x859a
  };
    
  // OpenGL 1.1 specification

  typedef void (CALL_OPENGL *Accum)(GLenum op, GLfloat value);
  Accum glAccum;
  typedef void (CALL_OPENGL *AlphaFunc)(GLenum func, GLclampf ref);
  AlphaFunc glAlphaFunc;
  typedef GLboolean (CALL_OPENGL *AreTexturesResident)(GLsizei n, const GLuint* textures, GLboolean* residences);
  AreTexturesResident glAreTexturesResident;
  typedef void (CALL_OPENGL *ArrayElement)(GLint i);
  ArrayElement glArrayElement;
  typedef void (CALL_OPENGL *Begin)(GLenum mode);
  Begin glBegin;
  typedef void (CALL_OPENGL *BindTexture)(GLenum target, GLuint texture);
  BindTexture glBindTexture;
  typedef void (CALL_OPENGL *Bitmap)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte* bitmap);
  Bitmap glBitmap;
  typedef void (CALL_OPENGL *BlendFunc)(GLenum sfactor, GLenum dfactor);
  BlendFunc glBlendFunc;
  typedef void (CALL_OPENGL *CallList)(GLuint list);
  CallList glCallList;
  typedef void (CALL_OPENGL *CallLists)(GLsizei n, GLenum type, const void* lists);
  CallLists glCallLists;
  typedef void (CALL_OPENGL *Clear)(GLbitfield mask);
  Clear glClear;
  typedef void (CALL_OPENGL *ClearAccum)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  ClearAccum glClearAccum;
  typedef void (CALL_OPENGL *ClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  ClearColor glClearColor;
  typedef void (CALL_OPENGL *ClearDepth)(GLclampd depth);
  ClearDepth glClearDepth;
  typedef void (CALL_OPENGL *ClearIndex)(GLfloat c);
  ClearIndex glClearIndex;
  typedef void (CALL_OPENGL *ClearStencil)(GLint s);
  ClearStencil glClearStencil;
  typedef void (CALL_OPENGL *ClipPlane)(GLenum plane, const GLdouble* equation);
  ClipPlane glClipPlane;
  typedef void (CALL_OPENGL *Color3b)(GLbyte red, GLbyte green, GLbyte blue);
  Color3b glColor3b;
  typedef void (CALL_OPENGL *Color3bv)(const GLbyte* v);
  Color3bv glColor3bv;
  typedef void (CALL_OPENGL *Color3d)(GLdouble red, GLdouble green, GLdouble blue);
  Color3d glColor3d;
  typedef void (CALL_OPENGL *Color3dv)(const GLdouble* v);
  Color3dv glColor3dv;
  typedef void (CALL_OPENGL *Color3f)(GLfloat red, GLfloat green, GLfloat blue);
  Color3f glColor3f;
  typedef void (CALL_OPENGL *Color3fv)(const GLfloat* v);
  Color3fv glColor3fv;
  typedef void (CALL_OPENGL *Color3i)(GLint red, GLint green, GLint blue);
  Color3i glColor3i;
  typedef void (CALL_OPENGL *Color3iv)(const GLint* v);
  Color3iv glColor3iv;
  typedef void (CALL_OPENGL *Color3s)(GLshort red, GLshort green, GLshort blue);
  Color3s glColor3s;
  typedef void (CALL_OPENGL *Color3sv)(const GLshort* v);
  Color3sv glColor3sv;
  typedef void (CALL_OPENGL *Color3ub)(GLubyte red, GLubyte green, GLubyte blue);
  Color3ub glColor3ub;
  typedef void (CALL_OPENGL *Color3ubv)(const GLubyte* v);
  Color3ubv glColor3ubv;
  typedef void (CALL_OPENGL *Color3ui)(GLuint red, GLuint green, GLuint blue);
  Color3ui glColor3ui;
  typedef void (CALL_OPENGL *Color3uiv)(const GLuint* v);
  Color3uiv glColor3uiv;
  typedef void (CALL_OPENGL *Color3us)(GLushort red, GLushort green, GLushort blue);
  Color3us glColor3us;
  typedef void (CALL_OPENGL *Color3usv)(const GLushort* v);
  Color3usv glColor3usv;
  typedef void (CALL_OPENGL *Color4b)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
  Color4b glColor4b;
  typedef void (CALL_OPENGL *Color4bv)(const GLbyte* v);
  Color4bv glColor4bv;
  typedef void (CALL_OPENGL *Color4d)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
  Color4d glColor4d;
  typedef void (CALL_OPENGL *Color4dv)(const GLdouble* v);
  Color4dv glColor4dv;
  typedef void (CALL_OPENGL *Color4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  Color4f glColor4f;
  typedef void (CALL_OPENGL *Color4fv)(const GLfloat* v);
  Color4fv glColor4fv;
  typedef void (CALL_OPENGL *Color4i)(GLint red, GLint green, GLint blue, GLint alpha);
  Color4i glColor4i;
  typedef void (CALL_OPENGL *Color4iv)(const GLint* v);
  Color4iv glColor4iv;
  typedef void (CALL_OPENGL *Color4s)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
  Color4s glColor4s;
  typedef void (CALL_OPENGL *Color4sv)(const GLshort* v);
  Color4sv glColor4sv;
  typedef void (CALL_OPENGL *Color4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
  Color4ub glColor4ub;
  typedef void (CALL_OPENGL *Color4ubv)(const GLubyte* v);
  Color4ubv glColor4ubv;
  typedef void (CALL_OPENGL *Color4ui)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
  Color4ui glColor4ui;
  typedef void (CALL_OPENGL *Color4uiv)(const GLuint* v);
  Color4uiv glColor4uiv;
  typedef void (CALL_OPENGL *Color4us)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
  Color4us glColor4us;
  typedef void (CALL_OPENGL *Color4usv)(const GLushort* v);
  Color4usv glColor4usv;
  typedef void (CALL_OPENGL *ColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
  ColorMask glColorMask;
  typedef void (CALL_OPENGL *ColorMaterial)(GLenum face, GLenum mode);
  ColorMaterial glColorMaterial;
  typedef void (CALL_OPENGL *ColorPointer)(GLint size, GLenum type, GLsizei stride, const void* pointer);
  ColorPointer glColorPointer;
  typedef void (CALL_OPENGL *CopyPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
  CopyPixels glCopyPixels;
  typedef void (CALL_OPENGL *CopyTexImage1D)(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
  CopyTexImage1D glCopyTexImage1D;
  typedef void (CALL_OPENGL *CopyTexImage2D)(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
  CopyTexImage2D glCopyTexImage2D;
  typedef void (CALL_OPENGL *CopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
  CopyTexSubImage1D glCopyTexSubImage1D;
  typedef void (CALL_OPENGL *CopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
  CopyTexSubImage2D glCopyTexSubImage2D;
  typedef void (CALL_OPENGL *CullFace)(GLenum mode);
  CullFace glCullFace;
  typedef void (CALL_OPENGL *DeleteLists)(GLuint list, GLsizei range);
  DeleteLists glDeleteLists;
  typedef void (CALL_OPENGL *DeleteTextures)(GLsizei n, const GLuint* textures);
  DeleteTextures glDeleteTextures;
  typedef void (CALL_OPENGL *DepthFunc)(GLenum func);
  DepthFunc glDepthFunc;
  typedef void (CALL_OPENGL *DepthMask)(GLboolean flag);
  DepthMask glDepthMask;
  typedef void (CALL_OPENGL *DepthRange)(GLclampd zNear, GLclampd zFar);
  DepthRange glDepthRange;
  typedef void (CALL_OPENGL *Disable)(GLenum cap);
  Disable glDisable;
  typedef void (CALL_OPENGL *DisableClientState)(GLenum array);
  DisableClientState glDisableClientState;
  typedef void (CALL_OPENGL *DrawArrays)(GLenum mode, GLint first, GLsizei count);
  DrawArrays glDrawArrays;
  typedef void (CALL_OPENGL *DrawBuffer)(GLenum mode);
  DrawBuffer glDrawBuffer;
  typedef void (CALL_OPENGL *DrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
  DrawElements glDrawElements;
  typedef void (CALL_OPENGL *DrawPixels)(GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
  DrawPixels glDrawPixels;
  typedef void (CALL_OPENGL *EdgeFlag)(GLboolean flag);
  EdgeFlag glEdgeFlag;
  typedef void (CALL_OPENGL *EdgeFlagPointer)(GLsizei stride, const GLboolean* pointer);
  EdgeFlagPointer glEdgeFlagPointer;
  typedef void (CALL_OPENGL *EdgeFlagv)(const GLboolean* flag);
  EdgeFlagv glEdgeFlagv;
  typedef void (CALL_OPENGL *Enable)(GLenum cap);
  Enable glEnable;
  typedef void (CALL_OPENGL *EnableClientState)(GLenum array);
  EnableClientState glEnableClientState;
  typedef void (CALL_OPENGL *End)();
  End glEnd;
  typedef void (CALL_OPENGL *EndList)();
  EndList glEndList;
  typedef void (CALL_OPENGL *EvalCoord1d)(GLdouble u);
  EvalCoord1d glEvalCoord1d;
  typedef void (CALL_OPENGL *EvalCoord1dv)(const GLdouble* u);
  EvalCoord1dv glEvalCoord1dv;
  typedef void (CALL_OPENGL *EvalCoord1f)(GLfloat u);
  EvalCoord1f glEvalCoord1f;
  typedef void (CALL_OPENGL *EvalCoord1fv)(const GLfloat* u);
  EvalCoord1fv glEvalCoord1fv;
  typedef void (CALL_OPENGL *EvalCoord2d)(GLdouble u, GLdouble v);
  EvalCoord2d glEvalCoord2d;
  typedef void (CALL_OPENGL *EvalCoord2dv)(const GLdouble* u);
  EvalCoord2dv glEvalCoord2dv;
  typedef void (CALL_OPENGL *EvalCoord2f)(GLfloat u, GLfloat v);
  EvalCoord2f glEvalCoord2f;
  typedef void (CALL_OPENGL *EvalCoord2fv)(const GLfloat* u);
  EvalCoord2fv glEvalCoord2fv;
  typedef void (CALL_OPENGL *EvalMesh1)(GLenum mode, GLint i1, GLint i2);
  EvalMesh1 glEvalMesh1;
  typedef void (CALL_OPENGL *EvalMesh2)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
  EvalMesh2 glEvalMesh2;
  typedef void (CALL_OPENGL *EvalPoint1)(GLint i);
  EvalPoint1 glEvalPoint1;
  typedef void (CALL_OPENGL *EvalPoint2)(GLint i, GLint j);
  EvalPoint2 glEvalPoint2;
  typedef void (CALL_OPENGL *FeedbackBuffer)(GLsizei size, GLenum type, GLfloat* buffer);
  FeedbackBuffer glFeedbackBuffer;
  typedef void (CALL_OPENGL *Finish)();
  Finish glFinish;
  typedef void (CALL_OPENGL *Flush)();
  Flush glFlush;
  typedef void (CALL_OPENGL *Fogf)(GLenum pname, GLfloat param);
  Fogf glFogf;
  typedef void (CALL_OPENGL *Fogfv)(GLenum pname, const GLfloat* params);
  Fogfv glFogfv;
  typedef void (CALL_OPENGL *Fogi)(GLenum pname, GLint param);
  Fogi glFogi;
  typedef void (CALL_OPENGL *Fogiv)(GLenum pname, const GLint* params);
  Fogiv glFogiv;
  typedef void (CALL_OPENGL *FrontFace)(GLenum mode);
  FrontFace glFrontFace;
  typedef void (CALL_OPENGL *Frustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
  Frustum glFrustum;
  typedef GLuint (CALL_OPENGL *GenLists)(GLsizei range);
  GenLists glGenLists;
  typedef void (CALL_OPENGL *GenTextures)(GLsizei n, GLuint* textures);
  GenTextures glGenTextures;
  typedef void (CALL_OPENGL *GetBooleanv)(GLenum pname, GLboolean* params);
  GetBooleanv glGetBooleanv;
  typedef void (CALL_OPENGL *GetClipPlane)(GLenum plane, GLdouble* equation);
  GetClipPlane glGetClipPlane;
  typedef void (CALL_OPENGL *GetDoublev)(GLenum pname, GLdouble* params);
  GetDoublev glGetDoublev;
  typedef GLenum (CALL_OPENGL *GetError)();
  GetError glGetError;
  typedef void (CALL_OPENGL *GetFloatv)(GLenum pname, GLfloat* params);
  GetFloatv glGetFloatv;
  typedef void (CALL_OPENGL *GetIntegerv)(GLenum pname, GLint* params);
  GetIntegerv glGetIntegerv;
  typedef void (CALL_OPENGL *GetLightfv)(GLenum light, GLenum pname, GLfloat* params);
  GetLightfv glGetLightfv;
  typedef void (CALL_OPENGL *GetLightiv)(GLenum light, GLenum pname, GLint* params);
  GetLightiv glGetLightiv;
  typedef void (CALL_OPENGL *GetMapdv)(GLenum target, GLenum query, GLdouble* v);
  GetMapdv glGetMapdv;
  typedef void (CALL_OPENGL *GetMapfv)(GLenum target, GLenum query, GLfloat* v);
  GetMapfv glGetMapfv;
  typedef void (CALL_OPENGL *GetMapiv)(GLenum target, GLenum query, GLint* v);
  GetMapiv glGetMapiv;
  typedef void (CALL_OPENGL *GetMaterialfv)(GLenum face, GLenum pname, GLfloat* params);
  GetMaterialfv glGetMaterialfv;
  typedef void (CALL_OPENGL *GetMaterialiv)(GLenum face, GLenum pname, GLint* params);
  GetMaterialiv glGetMaterialiv;
  typedef void (CALL_OPENGL *GetPixelMapfv)(GLenum map, GLfloat* values);
  GetPixelMapfv glGetPixelMapfv;
  typedef void (CALL_OPENGL *GetPixelMapuiv)(GLenum map, GLuint* values);
  GetPixelMapuiv glGetPixelMapuiv;
  typedef void (CALL_OPENGL *GetPixelMapusv)(GLenum map, GLushort* values);
  GetPixelMapusv glGetPixelMapusv;
  typedef void (CALL_OPENGL *GetPointerv)(GLenum pname, void** params);
  GetPointerv glGetPointerv;
  typedef void (CALL_OPENGL *GetPolygonStipple)(GLubyte* mask);
  GetPolygonStipple glGetPolygonStipple;
  typedef const GLubyte* (CALL_OPENGL *GetString)(GLenum name);
  GetString glGetString;
  typedef void (CALL_OPENGL *GetTexEnvfv)(GLenum target, GLenum pname, GLfloat* params);
  GetTexEnvfv glGetTexEnvfv;
  typedef void (CALL_OPENGL *GetTexEnviv)(GLenum target, GLenum pname, GLint* params);
  GetTexEnviv glGetTexEnviv;
  typedef void (CALL_OPENGL *GetTexGendv)(GLenum coord, GLenum pname, GLdouble* params);
  GetTexGendv glGetTexGendv;
  typedef void (CALL_OPENGL *GetTexGenfv)(GLenum coord, GLenum pname, GLfloat* params);
  GetTexGenfv glGetTexGenfv;
  typedef void (CALL_OPENGL *GetTexGeniv)(GLenum coord, GLenum pname, GLint* params);
  GetTexGeniv glGetTexGeniv;
  typedef void (CALL_OPENGL *GetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
  GetTexImage glGetTexImage;
  typedef void (CALL_OPENGL *GetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat* params);
  GetTexLevelParameterfv glGetTexLevelParameterfv;
  typedef void (CALL_OPENGL *GetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint* params);
  GetTexLevelParameteriv glGetTexLevelParameteriv;
  typedef void (CALL_OPENGL *GetTexParameterfv)(GLenum target, GLenum pname, GLfloat* params);
  GetTexParameterfv glGetTexParameterfv;
  typedef void (CALL_OPENGL *GetTexParameteriv)(GLenum target, GLenum pname, GLint* params);
  GetTexParameteriv glGetTexParameteriv;
  typedef void (CALL_OPENGL *Hint)(GLenum target, GLenum mode);
  Hint glHint;
  typedef void (CALL_OPENGL *IndexMask)(GLuint mask);
  IndexMask glIndexMask;
  typedef void (CALL_OPENGL *IndexPointer)(GLenum type, GLsizei stride, const void* pointer);
  IndexPointer glIndexPointer;
  typedef void (CALL_OPENGL *Indexd)(GLdouble c);
  Indexd glIndexd;
  typedef void (CALL_OPENGL *Indexdv)(const GLdouble* c);
  Indexdv glIndexdv;
  typedef void (CALL_OPENGL *Indexf)(GLfloat c);
  Indexf glIndexf;
  typedef void (CALL_OPENGL *Indexfv)(const GLfloat* c);
  Indexfv glIndexfv;
  typedef void (CALL_OPENGL *Indexi)(GLint c);
  Indexi glIndexi;
  typedef void (CALL_OPENGL *Indexiv)(const GLint* c);
  Indexiv glIndexiv;
  typedef void (CALL_OPENGL *Indexs)(GLshort c);
  Indexs glIndexs;
  typedef void (CALL_OPENGL *Indexsv)(const GLshort* c);
  Indexsv glIndexsv;
  typedef void (CALL_OPENGL *Indexub)(GLubyte c);
  Indexub glIndexub;
  typedef void (CALL_OPENGL *Indexubv)(const GLubyte* c);
  Indexubv glIndexubv;
  typedef void (CALL_OPENGL *InitNames)();
  InitNames glInitNames;
  typedef void (CALL_OPENGL *InterleavedArrays)(GLenum format, GLsizei stride, const void* pointer);
  InterleavedArrays glInterleavedArrays;
  typedef GLboolean (CALL_OPENGL *IsEnabled)(GLenum cap);
  IsEnabled glIsEnabled;
  typedef GLboolean (CALL_OPENGL *IsList)(GLuint list);
  IsList glIsList;
  typedef GLboolean (CALL_OPENGL *IsTexture)(GLuint texture);
  IsTexture glIsTexture;
  typedef void (CALL_OPENGL *LightModelf)(GLenum pname, GLfloat param);
  LightModelf glLightModelf;
  typedef void (CALL_OPENGL *LightModelfv)(GLenum pname, const GLfloat* params);
  LightModelfv glLightModelfv;
  typedef void (CALL_OPENGL *LightModeli)(GLenum pname, GLint param);
  LightModeli glLightModeli;
  typedef void (CALL_OPENGL *LightModeliv)(GLenum pname, const GLint* params);
  LightModeliv glLightModeliv;
  typedef void (CALL_OPENGL *Lightf)(GLenum light, GLenum pname, GLfloat param);
  Lightf glLightf;
  typedef void (CALL_OPENGL *Lightfv)(GLenum light, GLenum pname, const GLfloat* params);
  Lightfv glLightfv;
  typedef void (CALL_OPENGL *Lighti)(GLenum light, GLenum pname, GLint param);
  Lighti glLighti;
  typedef void (CALL_OPENGL *Lightiv)(GLenum light, GLenum pname, const GLint* params);
  Lightiv glLightiv;
  typedef void (CALL_OPENGL *LineStipple)(GLint factor, GLushort pattern);
  LineStipple glLineStipple;
  typedef void (CALL_OPENGL *LineWidth)(GLfloat width);
  LineWidth glLineWidth;
  typedef void (CALL_OPENGL *ListBase)(GLuint base);
  ListBase glListBase;
  typedef void (CALL_OPENGL *LoadIdentity)();
  LoadIdentity glLoadIdentity;
  typedef void (CALL_OPENGL *LoadMatrixd)(const GLdouble* m);
  LoadMatrixd glLoadMatrixd;
  typedef void (CALL_OPENGL *LoadMatrixf)(const GLfloat* m);
  LoadMatrixf glLoadMatrixf;
  typedef void (CALL_OPENGL *LoadName)(GLuint name);
  LoadName glLoadName;
  typedef void (CALL_OPENGL *LogicOp)(GLenum opcode);
  LogicOp glLogicOp;
  typedef void (CALL_OPENGL *Map1d)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points);
  Map1d glMap1d;
  typedef void (CALL_OPENGL *Map1f)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat* points);
  Map1f glMap1f;
  typedef void (CALL_OPENGL *Map2d)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points);
  Map2d glMap2d;
  typedef void (CALL_OPENGL *Map2f)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat* points);
  Map2f glMap2f;
  typedef void (CALL_OPENGL *MapGrid1d)(GLint un, GLdouble u1, GLdouble u2);
  MapGrid1d glMapGrid1d;
  typedef void (CALL_OPENGL *MapGrid1f)(GLint un, GLfloat u1, GLfloat u2);
  MapGrid1f glMapGrid1f;
  typedef void (CALL_OPENGL *MapGrid2d)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
  MapGrid2d glMapGrid2d;
  typedef void (CALL_OPENGL *MapGrid2f)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
  MapGrid2f glMapGrid2f;
  typedef void (CALL_OPENGL *Materialf)(GLenum face, GLenum pname, GLfloat param);
  Materialf glMaterialf;
  typedef void (CALL_OPENGL *Materialfv)(GLenum face, GLenum pname, const GLfloat* params);
  Materialfv glMaterialfv;
  typedef void (CALL_OPENGL *Materiali)(GLenum face, GLenum pname, GLint param);
  Materiali glMateriali;
  typedef void (CALL_OPENGL *Materialiv)(GLenum face, GLenum pname, const GLint* params);
  Materialiv glMaterialiv;
  typedef void (CALL_OPENGL *MatrixMode)(GLenum mode);
  MatrixMode glMatrixMode;
  typedef void (CALL_OPENGL *MultMatrixd)(const GLdouble* m);
  MultMatrixd glMultMatrixd;
  typedef void (CALL_OPENGL *MultMatrixf)(const GLfloat* m);
  MultMatrixf glMultMatrixf;
  typedef void (CALL_OPENGL *NewList)(GLuint list, GLenum mode);
  NewList glNewList;
  typedef void (CALL_OPENGL *Normal3b)(GLbyte nx, GLbyte ny, GLbyte nz);
  Normal3b glNormal3b;
  typedef void (CALL_OPENGL *Normal3bv)(const GLbyte* v);
  Normal3bv glNormal3bv;
  typedef void (CALL_OPENGL *Normal3d)(GLdouble nx, GLdouble ny, GLdouble nz);
  Normal3d glNormal3d;
  typedef void (CALL_OPENGL *Normal3dv)(const GLdouble* v);
  Normal3dv glNormal3dv;
  typedef void (CALL_OPENGL *Normal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
  Normal3f glNormal3f;
  typedef void (CALL_OPENGL *Normal3fv)(const GLfloat* v);
  Normal3fv glNormal3fv;
  typedef void (CALL_OPENGL *Normal3i)(GLint nx, GLint ny, GLint nz);
  Normal3i glNormal3i;
  typedef void (CALL_OPENGL *Normal3iv)(const GLint* v);
  Normal3iv glNormal3iv;
  typedef void (CALL_OPENGL *Normal3s)(GLshort nx, GLshort ny, GLshort nz);
  Normal3s glNormal3s;
  typedef void (CALL_OPENGL *Normal3sv)(const GLshort* v);
  Normal3sv glNormal3sv;
  typedef void (CALL_OPENGL *NormalPointer)(GLenum type, GLsizei stride, const void* pointer);
  NormalPointer glNormalPointer;
  typedef void (CALL_OPENGL *Ortho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
  Ortho glOrtho;
  typedef void (CALL_OPENGL *PassThrough)(GLfloat token);
  PassThrough glPassThrough;
  typedef void (CALL_OPENGL *PixelMapfv)(GLenum map, GLint mapsize, const GLfloat* values);
  PixelMapfv glPixelMapfv;
  typedef void (CALL_OPENGL *PixelMapuiv)(GLenum map, GLint mapsize, const GLuint* values);
  PixelMapuiv glPixelMapuiv;
  typedef void (CALL_OPENGL *PixelMapusv)(GLenum map, GLint mapsize, const GLushort* values);
  PixelMapusv glPixelMapusv;
  typedef void (CALL_OPENGL *PixelStoref)(GLenum pname, GLfloat param);
  PixelStoref glPixelStoref;
  typedef void (CALL_OPENGL *PixelStorei)(GLenum pname, GLint param);
  PixelStorei glPixelStorei;
  typedef void (CALL_OPENGL *PixelTransferf)(GLenum pname, GLfloat param);
  PixelTransferf glPixelTransferf;
  typedef void (CALL_OPENGL *PixelTransferi)(GLenum pname, GLint param);
  PixelTransferi glPixelTransferi;
  typedef void (CALL_OPENGL *PixelZoom)(GLfloat xfactor, GLfloat yfactor);
  PixelZoom glPixelZoom;
  typedef void (CALL_OPENGL *PointSize)(GLfloat size);
  PointSize glPointSize;
  typedef void (CALL_OPENGL *PolygonMode)(GLenum face, GLenum mode);
  PolygonMode glPolygonMode;
  typedef void (CALL_OPENGL *PolygonOffset)(GLfloat factor, GLfloat units);
  PolygonOffset glPolygonOffset;
  typedef void (CALL_OPENGL *PolygonStipple)(const GLubyte* mask);
  PolygonStipple glPolygonStipple;
  typedef void (CALL_OPENGL *PopAttrib)();
  PopAttrib glPopAttrib;
  typedef void (CALL_OPENGL *PopClientAttrib)();
  PopClientAttrib glPopClientAttrib;
  typedef void (CALL_OPENGL *PopMatrix)();
  PopMatrix glPopMatrix;
  typedef void (CALL_OPENGL *PopName)();
  PopName glPopName;
  typedef void (CALL_OPENGL *PrioritizeTextures)(GLsizei n, const GLuint* textures, const GLclampf* priorities);
  PrioritizeTextures glPrioritizeTextures;
  typedef void (CALL_OPENGL *PushAttrib)(GLbitfield mask);
  PushAttrib glPushAttrib;
  typedef void (CALL_OPENGL *PushClientAttrib)(GLbitfield mask);
  PushClientAttrib glPushClientAttrib;
  typedef void (CALL_OPENGL *PushMatrix)();
  PushMatrix glPushMatrix;
  typedef void (CALL_OPENGL *PushName)(GLuint name);
  PushName glPushName;
  typedef void (CALL_OPENGL *RasterPos2d)(GLdouble x, GLdouble y);
  RasterPos2d glRasterPos2d;
  typedef void (CALL_OPENGL *RasterPos2dv)(const GLdouble* v);
  RasterPos2dv glRasterPos2dv;
  typedef void (CALL_OPENGL *RasterPos2f)(GLfloat x, GLfloat y);
  RasterPos2f glRasterPos2f;
  typedef void (CALL_OPENGL *RasterPos2fv)(const GLfloat* v);
  RasterPos2fv glRasterPos2fv;
  typedef void (CALL_OPENGL *RasterPos2i)(GLint x, GLint y);
  RasterPos2i glRasterPos2i;
  typedef void (CALL_OPENGL *RasterPos2iv)(const GLint* v);
  RasterPos2iv glRasterPos2iv;
  typedef void (CALL_OPENGL *RasterPos2s)(GLshort x, GLshort y);
  RasterPos2s glRasterPos2s;
  typedef void (CALL_OPENGL *RasterPos2sv)(const GLshort* v);
  RasterPos2sv glRasterPos2sv;
  typedef void (CALL_OPENGL *RasterPos3d)(GLdouble x, GLdouble y, GLdouble z);
  RasterPos3d glRasterPos3d;
  typedef void (CALL_OPENGL *RasterPos3dv)(const GLdouble* v);
  RasterPos3dv glRasterPos3dv;
  typedef void (CALL_OPENGL *RasterPos3f)(GLfloat x, GLfloat y, GLfloat z);
  RasterPos3f glRasterPos3f;
  typedef void (CALL_OPENGL *RasterPos3fv)(const GLfloat* v);
  RasterPos3fv glRasterPos3fv;
  typedef void (CALL_OPENGL *RasterPos3i)(GLint x, GLint y, GLint z);
  RasterPos3i glRasterPos3i;
  typedef void (CALL_OPENGL *RasterPos3iv)(const GLint* v);
  RasterPos3iv glRasterPos3iv;
  typedef void (CALL_OPENGL *RasterPos3s)(GLshort x, GLshort y, GLshort z);
  RasterPos3s glRasterPos3s;
  typedef void (CALL_OPENGL *RasterPos3sv)(const GLshort* v);
  RasterPos3sv glRasterPos3sv;
  typedef void (CALL_OPENGL *RasterPos4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
  RasterPos4d glRasterPos4d;
  typedef void (CALL_OPENGL *RasterPos4dv)(const GLdouble* v);
  RasterPos4dv glRasterPos4dv;
  typedef void (CALL_OPENGL *RasterPos4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  RasterPos4f glRasterPos4f;
  typedef void (CALL_OPENGL *RasterPos4fv)(const GLfloat* v);
  RasterPos4fv glRasterPos4fv;
  typedef void (CALL_OPENGL *RasterPos4i)(GLint x, GLint y, GLint z, GLint w);
  RasterPos4i glRasterPos4i;
  typedef void (CALL_OPENGL *RasterPos4iv)(const GLint* v);
  RasterPos4iv glRasterPos4iv;
  typedef void (CALL_OPENGL *RasterPos4s)(GLshort x, GLshort y, GLshort z, GLshort w);
  RasterPos4s glRasterPos4s;
  typedef void (CALL_OPENGL *RasterPos4sv)(const GLshort* v);
  RasterPos4sv glRasterPos4sv;
  typedef void (CALL_OPENGL *ReadBuffer)(GLenum mode);
  ReadBuffer glReadBuffer;
  typedef void (CALL_OPENGL *ReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
  ReadPixels glReadPixels;
  typedef void (CALL_OPENGL *Rectd)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
  Rectd glRectd;
  typedef void (CALL_OPENGL *Rectdv)(const GLdouble* v1, const GLdouble* v2);
  Rectdv glRectdv;
  typedef void (CALL_OPENGL *Rectf)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
  Rectf glRectf;
  typedef void (CALL_OPENGL *Rectfv)(const GLfloat* v1, const GLfloat* v2);
  Rectfv glRectfv;
  typedef void (CALL_OPENGL *Recti)(GLint x1, GLint y1, GLint x2, GLint y2);
  Recti glRecti;
  typedef void (CALL_OPENGL *Rectiv)(const GLint* v1, const GLint* v2);
  Rectiv glRectiv;
  typedef void (CALL_OPENGL *Rects)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
  Rects glRects;
  typedef void (CALL_OPENGL *Rectsv)(const GLshort* v1, const GLshort* v2);
  Rectsv glRectsv;
  typedef GLint (CALL_OPENGL *RenderMode)(GLenum mode);
  RenderMode glRenderMode;
  typedef void (CALL_OPENGL *Rotated)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
  Rotated glRotated;
  typedef void (CALL_OPENGL *Rotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
  Rotatef glRotatef;
  typedef void (CALL_OPENGL *Scaled)(GLdouble x, GLdouble y, GLdouble z);
  Scaled glScaled;
  typedef void (CALL_OPENGL *Scalef)(GLfloat x, GLfloat y, GLfloat z);
  Scalef glScalef;
  typedef void (CALL_OPENGL *Scissor)(GLint x, GLint y, GLsizei width, GLsizei height);
  Scissor glScissor;
  typedef void (CALL_OPENGL *SelectBuffer)(GLsizei size, GLuint* buffer);
  SelectBuffer glSelectBuffer;
  typedef void (CALL_OPENGL *ShadeModel)(GLenum mode);
  ShadeModel glShadeModel;
  typedef void (CALL_OPENGL *StencilFunc)(GLenum func, GLint ref, GLuint mask);
  StencilFunc glStencilFunc;
  typedef void (CALL_OPENGL *StencilMask)(GLuint mask);
  StencilMask glStencilMask;
  typedef void (CALL_OPENGL *StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
  StencilOp glStencilOp;
  typedef void (CALL_OPENGL *TexCoord1d)(GLdouble s);
  TexCoord1d glTexCoord1d;
  typedef void (CALL_OPENGL *TexCoord1dv)(const GLdouble* v);
  TexCoord1dv glTexCoord1dv;
  typedef void (CALL_OPENGL *TexCoord1f)(GLfloat s);
  TexCoord1f glTexCoord1f;
  typedef void (CALL_OPENGL *TexCoord1fv)(const GLfloat* v);
  TexCoord1fv glTexCoord1fv;
  typedef void (CALL_OPENGL *TexCoord1i)(GLint s);
  TexCoord1i glTexCoord1i;
  typedef void (CALL_OPENGL *TexCoord1iv)(const GLint* v);
  TexCoord1iv glTexCoord1iv;
  typedef void (CALL_OPENGL *TexCoord1s)(GLshort s);
  TexCoord1s glTexCoord1s;
  typedef void (CALL_OPENGL *TexCoord1sv)(const GLshort* v);
  TexCoord1sv glTexCoord1sv;
  typedef void (CALL_OPENGL *TexCoord2d)(GLdouble s, GLdouble t);
  TexCoord2d glTexCoord2d;
  typedef void (CALL_OPENGL *TexCoord2dv)(const GLdouble* v);
  TexCoord2dv glTexCoord2dv;
  typedef void (CALL_OPENGL *TexCoord2f)(GLfloat s, GLfloat t);
  TexCoord2f glTexCoord2f;
  typedef void (CALL_OPENGL *TexCoord2fv)(const GLfloat* v);
  TexCoord2fv glTexCoord2fv;
  typedef void (CALL_OPENGL *TexCoord2i)(GLint s, GLint t);
  TexCoord2i glTexCoord2i;
  typedef void (CALL_OPENGL *TexCoord2iv)(const GLint* v);
  TexCoord2iv glTexCoord2iv;
  typedef void (CALL_OPENGL *TexCoord2s)(GLshort s, GLshort t);
  TexCoord2s glTexCoord2s;
  typedef void (CALL_OPENGL *TexCoord2sv)(const GLshort* v);
  TexCoord2sv glTexCoord2sv;
  typedef void (CALL_OPENGL *TexCoord3d)(GLdouble s, GLdouble t, GLdouble r);
  TexCoord3d glTexCoord3d;
  typedef void (CALL_OPENGL *TexCoord3dv)(const GLdouble* v);
  TexCoord3dv glTexCoord3dv;
  typedef void (CALL_OPENGL *TexCoord3f)(GLfloat s, GLfloat t, GLfloat r);
  TexCoord3f glTexCoord3f;
  typedef void (CALL_OPENGL *TexCoord3fv)(const GLfloat* v);
  TexCoord3fv glTexCoord3fv;
  typedef void (CALL_OPENGL *TexCoord3i)(GLint s, GLint t, GLint r);
  TexCoord3i glTexCoord3i;
  typedef void (CALL_OPENGL *TexCoord3iv)(const GLint* v);
  TexCoord3iv glTexCoord3iv;
  typedef void (CALL_OPENGL *TexCoord3s)(GLshort s, GLshort t, GLshort r);
  TexCoord3s glTexCoord3s;
  typedef void (CALL_OPENGL *TexCoord3sv)(const GLshort* v);
  TexCoord3sv glTexCoord3sv;
  typedef void (CALL_OPENGL *TexCoord4d)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
  TexCoord4d glTexCoord4d;
  typedef void (CALL_OPENGL *TexCoord4dv)(const GLdouble* v);
  TexCoord4dv glTexCoord4dv;
  typedef void (CALL_OPENGL *TexCoord4f)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
  TexCoord4f glTexCoord4f;
  typedef void (CALL_OPENGL *TexCoord4fv)(const GLfloat* v);
  TexCoord4fv glTexCoord4fv;
  typedef void (CALL_OPENGL *TexCoord4i)(GLint s, GLint t, GLint r, GLint q);
  TexCoord4i glTexCoord4i;
  typedef void (CALL_OPENGL *TexCoord4iv)(const GLint* v);
  TexCoord4iv glTexCoord4iv;
  typedef void (CALL_OPENGL *TexCoord4s)(GLshort s, GLshort t, GLshort r, GLshort q);
  TexCoord4s glTexCoord4s;
  typedef void (CALL_OPENGL *TexCoord4sv)(const GLshort* v);
  TexCoord4sv glTexCoord4sv;
  typedef void (CALL_OPENGL *TexCoordPointer)(GLint size, GLenum type, GLsizei stride, const void* pointer);
  TexCoordPointer glTexCoordPointer;
  typedef void (CALL_OPENGL *TexEnvf)(GLenum target, GLenum pname, GLfloat param);
  TexEnvf glTexEnvf;
  typedef void (CALL_OPENGL *TexEnvfv)(GLenum target, GLenum pname, const GLfloat* params);
  TexEnvfv glTexEnvfv;
  typedef void (CALL_OPENGL *TexEnvi)(GLenum target, GLenum pname, GLint param);
  TexEnvi glTexEnvi;
  typedef void (CALL_OPENGL *TexEnviv)(GLenum target, GLenum pname, const GLint* params);
  TexEnviv glTexEnviv;
  typedef void (CALL_OPENGL *TexGend)(GLenum coord, GLenum pname, GLdouble param);
  TexGend glTexGend;
  typedef void (CALL_OPENGL *TexGendv)(GLenum coord, GLenum pname, const GLdouble* params);
  TexGendv glTexGendv;
  typedef void (CALL_OPENGL *TexGenf)(GLenum coord, GLenum pname, GLfloat param);
  TexGenf glTexGenf;
  typedef void (CALL_OPENGL *TexGenfv)(GLenum coord, GLenum pname, const GLfloat* params);
  TexGenfv glTexGenfv;
  typedef void (CALL_OPENGL *TexGeni)(GLenum coord, GLenum pname, GLint param);
  TexGeni glTexGeni;
  typedef void (CALL_OPENGL *TexGeniv)(GLenum coord, GLenum pname, const GLint* params);
  TexGeniv glTexGeniv;
  typedef void (CALL_OPENGL *TexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
  TexImage1D glTexImage1D;
  typedef void (CALL_OPENGL *TexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
  TexImage2D glTexImage2D;
  typedef void (CALL_OPENGL *TexParameterf)(GLenum target, GLenum pname, GLfloat param);
  TexParameterf glTexParameterf;
  typedef void (CALL_OPENGL *TexParameterfv)(GLenum target, GLenum pname, const GLfloat* params);
  TexParameterfv glTexParameterfv;
  typedef void (CALL_OPENGL *TexParameteri)(GLenum target, GLenum pname, GLint param);
  TexParameteri glTexParameteri;
  typedef void (CALL_OPENGL *TexParameteriv)(GLenum target, GLenum pname, const GLint* params);
  TexParameteriv glTexParameteriv;
  typedef void (CALL_OPENGL *TexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
  TexSubImage1D glTexSubImage1D;
  typedef void (CALL_OPENGL *TexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
  TexSubImage2D glTexSubImage2D;
  typedef void (CALL_OPENGL *Translated)(GLdouble x, GLdouble y, GLdouble z);
  Translated glTranslated;
  typedef void (CALL_OPENGL *Translatef)(GLfloat x, GLfloat y, GLfloat z);
  Translatef glTranslatef;
  typedef void (CALL_OPENGL *Vertex2d)(GLdouble x, GLdouble y);
  Vertex2d glVertex2d;
  typedef void (CALL_OPENGL *Vertex2dv)(const GLdouble* v);
  Vertex2dv glVertex2dv;
  typedef void (CALL_OPENGL *Vertex2f)(GLfloat x, GLfloat y);
  Vertex2f glVertex2f;
  typedef void (CALL_OPENGL *Vertex2fv)(const GLfloat* v);
  Vertex2fv glVertex2fv;
  typedef void (CALL_OPENGL *Vertex2i)(GLint x, GLint y);
  Vertex2i glVertex2i;
  typedef void (CALL_OPENGL *Vertex2iv)(const GLint* v);
  Vertex2iv glVertex2iv;
  typedef void (CALL_OPENGL *Vertex2s)(GLshort x, GLshort y);
  Vertex2s glVertex2s;
  typedef void (CALL_OPENGL *Vertex2sv)(const GLshort* v);
  Vertex2sv glVertex2sv;
  typedef void (CALL_OPENGL *Vertex3d)(GLdouble x, GLdouble y, GLdouble z);
  Vertex3d glVertex3d;
  typedef void (CALL_OPENGL *Vertex3dv)(const GLdouble* v);
  Vertex3dv glVertex3dv;
  typedef void (CALL_OPENGL *Vertex3f)(GLfloat x, GLfloat y, GLfloat z);
  Vertex3f glVertex3f;
  typedef void (CALL_OPENGL *Vertex3fv)(const GLfloat* v);
  Vertex3fv glVertex3fv;
  typedef void (CALL_OPENGL *Vertex3i)(GLint x, GLint y, GLint z);
  Vertex3i glVertex3i;
  typedef void (CALL_OPENGL *Vertex3iv)(const GLint* v);
  Vertex3iv glVertex3iv;
  typedef void (CALL_OPENGL *Vertex3s)(GLshort x, GLshort y, GLshort z);
  Vertex3s glVertex3s;
  typedef void (CALL_OPENGL *Vertex3sv)(const GLshort* v);
  Vertex3sv glVertex3sv;
  typedef void (CALL_OPENGL *Vertex4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
  Vertex4d glVertex4d;
  typedef void (CALL_OPENGL *Vertex4dv)(const GLdouble* v);
  Vertex4dv glVertex4dv;
  typedef void (CALL_OPENGL *Vertex4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  Vertex4f glVertex4f;
  typedef void (CALL_OPENGL *Vertex4fv)(const GLfloat* v);
  Vertex4fv glVertex4fv;
  typedef void (CALL_OPENGL *Vertex4i)(GLint x, GLint y, GLint z, GLint w);
  Vertex4i glVertex4i;
  typedef void (CALL_OPENGL *Vertex4iv)(const GLint* v);
  Vertex4iv glVertex4iv;
  typedef void (CALL_OPENGL *Vertex4s)(GLshort x, GLshort y, GLshort z, GLshort w);
  Vertex4s glVertex4s;
  typedef void (CALL_OPENGL *Vertex4sv)(const GLshort* v);
  Vertex4sv glVertex4sv;
  typedef void (CALL_OPENGL *VertexPointer)(GLint size, GLenum type, GLsizei stride, const void* pointer);
  VertexPointer glVertexPointer;
  typedef void (CALL_OPENGL *Viewport)(GLint x, GLint y, GLsizei width, GLsizei height);
  Viewport glViewport;
  
  // OpenGL 1.2 specification
    
  typedef void (CALL_OPENGL *ColorTable)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* table);
  ColorTable glColorTable;
  typedef void (CALL_OPENGL *ColorSubTable)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void* data);
  ColorSubTable glColorSubTable;
  typedef void (CALL_OPENGL *ColorTableParameteriv)(GLenum target, GLenum pname, const GLint* params);
  ColorTableParameteriv glColorTableParameteriv;
  typedef void (CALL_OPENGL *ColorTableParameterfv)(GLenum target, GLenum pname, const GLfloat* params);
  ColorTableParameterfv glColorTableParameterfv;
  typedef void (CALL_OPENGL *CopyColorSubTable)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
  CopyColorSubTable glCopyColorSubTable;
  typedef void (CALL_OPENGL *CopyColorTable)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
  CopyColorTable glCopyColorTable;
  typedef void (CALL_OPENGL *GetColorTable)(GLenum target, GLenum format, GLenum type, void* table);
  GetColorTable glGetColorTable;
  typedef void (CALL_OPENGL *GetColorTableParameterfv)(GLenum target, GLenum pname, GLfloat* params);
  GetColorTableParameterfv glGetColorTableParameterfv;
  typedef void (CALL_OPENGL *GetColorTableParameteriv)(GLenum target, GLenum pname, GLint* params);
  GetColorTableParameteriv glGetColorTableParameteriv;
  typedef void (CALL_OPENGL *BlendEquation)(GLenum mode);
  BlendEquation glBlendEquation;
  typedef void (CALL_OPENGL *BlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  BlendColor glBlendColor;
  typedef void (CALL_OPENGL *Histogram)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
  Histogram glHistogram;
  typedef void (CALL_OPENGL *ResetHistogram)(GLenum target);
  ResetHistogram glResetHistogram;
  typedef void (CALL_OPENGL *GetHistogram)(GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
  GetHistogram glGetHistogram;
  typedef void (CALL_OPENGL *GetHistogramParameterfv)(GLenum target, GLenum pname, GLfloat* params);
  GetHistogramParameterfv glGetHistogramParameterfv;
  typedef void (CALL_OPENGL *GetHistogramParameteriv)(GLenum target, GLenum pname, GLint* params);
  GetHistogramParameteriv glGetHistogramParameteriv;
  typedef void (CALL_OPENGL *Minmax)(GLenum target, GLenum internalformat, GLboolean sink);
  Minmax glMinmax;
  typedef void (CALL_OPENGL *ResetMinmax)(GLenum target);
  ResetMinmax glResetMinmax;
  typedef void (CALL_OPENGL *GetMinmax)(GLenum target, GLboolean reset, GLenum format, GLenum types, void* values);
  GetMinmax glGetMinmax;
  typedef void (CALL_OPENGL *GetMinmaxParameterfv)(GLenum target, GLenum pname, GLfloat* params);
  GetMinmaxParameterfv glGetMinmaxParameterfv;
  typedef void (CALL_OPENGL *GetMinmaxParameteriv)(GLenum target, GLenum pname, GLint* params);
  GetMinmaxParameteriv glGetMinmaxParameteriv;
  typedef void (CALL_OPENGL *ConvolutionFilter1D)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* image);
  ConvolutionFilter1D glConvolutionFilter1D;
  typedef void (CALL_OPENGL *ConvolutionFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* image);
  ConvolutionFilter2D glConvolutionFilter2D;
  typedef void (CALL_OPENGL *ConvolutionParameterf)(GLenum target, GLenum pname, GLfloat params);
  ConvolutionParameterf glConvolutionParameterf;
  typedef void (CALL_OPENGL *ConvolutionParameterfv)(GLenum target, GLenum pname, const GLfloat* params);
  ConvolutionParameterfv glConvolutionParameterfv;
  typedef void (CALL_OPENGL *ConvolutionParameteri)(GLenum target, GLenum pname, GLint params);
  ConvolutionParameteri glConvolutionParameteri;
  typedef void (CALL_OPENGL *ConvolutionParameteriv)(GLenum target, GLenum pname, const GLint* params);
  ConvolutionParameteriv glConvolutionParameteriv;
  typedef void (CALL_OPENGL *CopyConvolutionFilter1D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
  CopyConvolutionFilter1D glCopyConvolutionFilter1D;
  typedef void (CALL_OPENGL *CopyConvolutionFilter2D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
  CopyConvolutionFilter2D glCopyConvolutionFilter2D;
  typedef void (CALL_OPENGL *GetConvolutionFilter)(GLenum target, GLenum format, GLenum type, void* image);
  GetConvolutionFilter glGetConvolutionFilter;
  typedef void (CALL_OPENGL *GetConvolutionParameterfv)(GLenum target, GLenum pname, GLfloat* params);
  GetConvolutionParameterfv glGetConvolutionParameterfv;
  typedef void (CALL_OPENGL *GetConvolutionParameteriv)(GLenum target, GLenum pname, GLint* params);
  GetConvolutionParameteriv glGetConvolutionParameteriv;
  typedef void (CALL_OPENGL *SeparableFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* row, const void* column);
  SeparableFilter2D glSeparableFilter2D;
  typedef void (CALL_OPENGL *GetSeparableFilter)(GLenum target, GLenum format, GLenum type, void* row, void* column, void* span);
  GetSeparableFilter glGetSeparableFilter;
  typedef void (CALL_OPENGL *DrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
  DrawRangeElements glDrawRangeElements;
  typedef void (CALL_OPENGL *TexImage3D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
  TexImage3D glTexImage3D;
  typedef void (CALL_OPENGL *TexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
  TexSubImage3D glTexSubImage3D;
  typedef void (CALL_OPENGL *CopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
  CopyTexSubImage3D glCopyTexSubImage3D;
    
  // OpenGL 1.2.1 specification
    
  // OpenGL 1.3 specification
    
  typedef void (CALL_OPENGL *ActiveTexture)(GLenum texture);
  ActiveTexture glActiveTexture;
  typedef void (CALL_OPENGL *ClientActiveTexture)(GLenum texture);
  ClientActiveTexture glClientActiveTexture;
  typedef void (CALL_OPENGL *CompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
  CompressedTexImage1D glCompressedTexImage1D;
  typedef void (CALL_OPENGL *CompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
  CompressedTexImage2D glCompressedTexImage2D;
  typedef void (CALL_OPENGL *CompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
  CompressedTexImage3D glCompressedTexImage3D;
  typedef void (CALL_OPENGL *CompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
  CompressedTexSubImage1D glCompressedTexSubImage1D;
  typedef void (CALL_OPENGL *CompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
  CompressedTexSubImage2D glCompressedTexSubImage2D;
  typedef void (CALL_OPENGL *CompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
  CompressedTexSubImage3D glCompressedTexSubImage3D;
  typedef void (CALL_OPENGL *GetCompressedTexImage)(GLenum target, GLint lod, void* img);
  GetCompressedTexImage glGetCompressedTexImage;
  typedef void (CALL_OPENGL *MultiTexCoord1d)(GLenum target, GLdouble s);
  MultiTexCoord1d glMultiTexCoord1d;
  typedef void (CALL_OPENGL *MultiTexCoord1dv)(GLenum target, const GLdouble* v);
  MultiTexCoord1dv glMultiTexCoord1dv;
  typedef void (CALL_OPENGL *MultiTexCoord1f)(GLenum target, GLfloat s);
  MultiTexCoord1f glMultiTexCoord1f;
  typedef void (CALL_OPENGL *MultiTexCoord1fv)(GLenum target, const GLfloat* v);
  MultiTexCoord1fv glMultiTexCoord1fv;
  typedef void (CALL_OPENGL *MultiTexCoord1i)(GLenum target, GLint s);
  MultiTexCoord1i glMultiTexCoord1i;
  typedef void (CALL_OPENGL *MultiTexCoord1iv)(GLenum target, const GLint* v);
  MultiTexCoord1iv glMultiTexCoord1iv;
  typedef void (CALL_OPENGL *MultiTexCoord1s)(GLenum target, GLshort s);
  MultiTexCoord1s glMultiTexCoord1s;
  typedef void (CALL_OPENGL *MultiTexCoord1sv)(GLenum target, const GLshort* v);
  MultiTexCoord1sv glMultiTexCoord1sv;
  typedef void (CALL_OPENGL *MultiTexCoord2d)(GLenum target, GLdouble s, GLdouble t);
  MultiTexCoord2d glMultiTexCoord2d;
  typedef void (CALL_OPENGL *MultiTexCoord2dv)(GLenum target, const GLdouble* v);
  MultiTexCoord2dv glMultiTexCoord2dv;
  typedef void (CALL_OPENGL *MultiTexCoord2f)(GLenum target, GLfloat s, GLfloat t);
  MultiTexCoord2f glMultiTexCoord2f;
  typedef void (CALL_OPENGL *MultiTexCoord2fv)(GLenum target, const GLfloat* v);
  MultiTexCoord2fv glMultiTexCoord2fv;
  typedef void (CALL_OPENGL *MultiTexCoord2i)(GLenum target, GLint s, GLint t);
  MultiTexCoord2i glMultiTexCoord2i;
  typedef void (CALL_OPENGL *MultiTexCoord2iv)(GLenum target, const GLint* v);
  MultiTexCoord2iv glMultiTexCoord2iv;
  typedef void (CALL_OPENGL *MultiTexCoord2s)(GLenum target, GLshort s, GLshort t);
  MultiTexCoord2s glMultiTexCoord2s;
  typedef void (CALL_OPENGL *MultiTexCoord2sv)(GLenum target, const GLshort* v);
  MultiTexCoord2sv glMultiTexCoord2sv;
  typedef void (CALL_OPENGL *MultiTexCoord3d)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
  MultiTexCoord3d glMultiTexCoord3d;
  typedef void (CALL_OPENGL *MultiTexCoord3dv)(GLenum target, const GLdouble* v);
  MultiTexCoord3dv glMultiTexCoord3dv;
  typedef void (CALL_OPENGL *MultiTexCoord3f)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
  MultiTexCoord3f glMultiTexCoord3f;
  typedef void (CALL_OPENGL *MultiTexCoord3fv)(GLenum target, const GLfloat* v);
  MultiTexCoord3fv glMultiTexCoord3fv;
  typedef void (CALL_OPENGL *MultiTexCoord3i)(GLenum target, GLint s, GLint t, GLint r);
  MultiTexCoord3i glMultiTexCoord3i;
  typedef void (CALL_OPENGL *MultiTexCoord3iv)(GLenum target, const GLint* v);
  MultiTexCoord3iv glMultiTexCoord3iv;
  typedef void (CALL_OPENGL *MultiTexCoord3s)(GLenum target, GLshort s, GLshort t, GLshort r);
  MultiTexCoord3s glMultiTexCoord3s;
  typedef void (CALL_OPENGL *MultiTexCoord3sv)(GLenum target, const GLshort* v);
  MultiTexCoord3sv glMultiTexCoord3sv;
  typedef void (CALL_OPENGL *MultiTexCoord4d)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
  MultiTexCoord4d glMultiTexCoord4d;
  typedef void (CALL_OPENGL *MultiTexCoord4dv)(GLenum target, const GLdouble* v);
  MultiTexCoord4dv glMultiTexCoord4dv;
  typedef void (CALL_OPENGL *MultiTexCoord4f)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
  MultiTexCoord4f glMultiTexCoord4f;
  typedef void (CALL_OPENGL *MultiTexCoord4fv)(GLenum target, const GLfloat* v);
  MultiTexCoord4fv glMultiTexCoord4fv;
  typedef void (CALL_OPENGL *MultiTexCoord4i)(GLenum target, GLint s, GLint t, GLint r, GLint q);
  MultiTexCoord4i glMultiTexCoord4i;
  typedef void (CALL_OPENGL *MultiTexCoord4iv)(GLenum target, const GLint* v);
  MultiTexCoord4iv glMultiTexCoord4iv;
  typedef void (CALL_OPENGL *MultiTexCoord4s)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
  MultiTexCoord4s glMultiTexCoord4s;
  typedef void (CALL_OPENGL *MultiTexCoord4sv)(GLenum target, const GLshort* v);
  MultiTexCoord4sv glMultiTexCoord4sv;
  typedef void (CALL_OPENGL *LoadTransposeMatrixd)(const GLdouble m[16]);
  LoadTransposeMatrixd glLoadTransposeMatrixd;
  typedef void (CALL_OPENGL *LoadTransposeMatrixf)(const GLfloat m[16]);
  LoadTransposeMatrixf glLoadTransposeMatrixf;
  typedef void (CALL_OPENGL *MultTransposeMatrixd)(const GLdouble m[16]);
  MultTransposeMatrixd glMultTransposeMatrixd;
  typedef void (CALL_OPENGL *MultTransposeMatrixf)(const GLfloat m[16]);
  MultTransposeMatrixf glMultTransposeMatrixf;
  typedef void (CALL_OPENGL *SampleCoverage)(GLclampf value, GLboolean invert);
  SampleCoverage glSampleCoverage;
    
  // OpenGL 1.4 specification

  typedef void (CALL_OPENGL *FogCoordf)(GLfloat coord);
  FogCoordf glFogCoordf;
  typedef void (CALL_OPENGL *FogCoordfv)(const GLfloat* coord);
  FogCoordfv glFogCoordfv;
  typedef void (CALL_OPENGL *FogCoordd)(GLdouble coord);
  FogCoordd glFogCoordd;
  typedef void (CALL_OPENGL *FogCoorddv)(const GLdouble* coord);
  FogCoorddv glFogCoorddv;
  typedef void (CALL_OPENGL *FogCoordPointer)(GLenum type, GLsizei stride, const void* pointer);
  FogCoordPointer glFogCoordPointer;
  typedef void (CALL_OPENGL *MultiDrawArrays)(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount);
  MultiDrawArrays glMultiDrawArrays;
  typedef void (CALL_OPENGL *MultiDrawElements)(GLenum mode, GLsizei* count, GLenum type, const void** indices, GLsizei primcount);
  MultiDrawElements glMultiDrawElements;
  typedef void (CALL_OPENGL *PointParameterf)(GLenum pname, GLfloat param);
  PointParameterf glPointParameterf;
  typedef void (CALL_OPENGL *PointParameterfv)(GLenum pname, GLfloat* params);
  PointParameterfv glPointParameterfv;
  typedef void (CALL_OPENGL *SecondaryColor3b)(GLbyte red, GLbyte green, GLbyte blue);
  SecondaryColor3b glSecondaryColor3b;
  typedef void (CALL_OPENGL *SecondaryColor3bv)(const GLbyte* v);
  SecondaryColor3bv glSecondaryColor3bv;
  typedef void (CALL_OPENGL *SecondaryColor3d)(GLdouble red, GLdouble green, GLdouble blue);
  SecondaryColor3d glSecondaryColor3d;
  typedef void (CALL_OPENGL *SecondaryColor3dv)(const GLdouble* v);
  SecondaryColor3dv glSecondaryColor3dv;
  typedef void (CALL_OPENGL *SecondaryColor3f)(GLfloat red, GLfloat green, GLfloat blue);
  SecondaryColor3f glSecondaryColor3f;
  typedef void (CALL_OPENGL *SecondaryColor3fv)(const GLfloat* v);
  SecondaryColor3fv glSecondaryColor3fv;
  typedef void (CALL_OPENGL *SecondaryColor3i)(GLint red, GLint green, GLint blue);
  SecondaryColor3i glSecondaryColor3i;
  typedef void (CALL_OPENGL *SecondaryColor3iv)(const GLint* v);
  SecondaryColor3iv glSecondaryColor3iv;
  typedef void (CALL_OPENGL *SecondaryColor3s)(GLshort red, GLshort green, GLshort blue);
  SecondaryColor3s glSecondaryColor3s;
  typedef void (CALL_OPENGL *SecondaryColor3sv)(const GLshort* v);
  SecondaryColor3sv glSecondaryColor3sv;
  typedef void (CALL_OPENGL *SecondaryColor3ub)(GLubyte red, GLubyte green, GLubyte blue);
  SecondaryColor3ub glSecondaryColor3ub;
  typedef void (CALL_OPENGL *SecondaryColor3ubv)(const GLubyte* v);
  SecondaryColor3ubv glSecondaryColor3ubv;
  typedef void (CALL_OPENGL *SecondaryColor3ui)(GLuint red, GLuint green, GLuint blue);
  SecondaryColor3ui glSecondaryColor3ui;
  typedef void (CALL_OPENGL *SecondaryColor3uiv)(const GLuint* v);
  SecondaryColor3uiv glSecondaryColor3uiv;
  typedef void (CALL_OPENGL *SecondaryColor3us)(GLushort red, GLushort green, GLushort blue);
  SecondaryColor3us glSecondaryColor3us;
  typedef void (CALL_OPENGL *SecondaryColor3usv)(const GLushort* v);
  SecondaryColor3usv glSecondaryColor3usv;
  typedef void (CALL_OPENGL *SecondaryColorPointer)(GLint size, GLenum type, GLsizei stride, void* pointer);
  SecondaryColorPointer glSecondaryColorPointer;
  typedef void (CALL_OPENGL *BlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
  BlendFuncSeparate glBlendFuncSeparate;
  typedef void (CALL_OPENGL *WindowPos2d)(GLdouble x, GLdouble y);
  WindowPos2d glWindowPos2d;
  typedef void (CALL_OPENGL *WindowPos2f)(GLfloat x, GLfloat y);
  WindowPos2f glWindowPos2f;
  typedef void (CALL_OPENGL *WindowPos2i)(GLint x, GLint y);
  WindowPos2i glWindowPos2i;
  typedef void (CALL_OPENGL *WindowPos2s)(GLshort x, GLshort y);
  WindowPos2s glWindowPos2s;
  typedef void (CALL_OPENGL *WindowPos2dv)(const GLdouble* p);
  WindowPos2dv glWindowPos2dv;
  typedef void (CALL_OPENGL *WindowPos2fv)(const GLfloat* p);
  WindowPos2fv glWindowPos2fv;
  typedef void (CALL_OPENGL *WindowPos2iv)(const GLint* p);
  WindowPos2iv glWindowPos2iv;
  typedef void (CALL_OPENGL *WindowPos2sv)(const GLshort* p);
  WindowPos2sv glWindowPos2sv;
  typedef void (CALL_OPENGL *WindowPos3d)(GLdouble x, GLdouble y, GLdouble z);
  WindowPos3d glWindowPos3d;
  typedef void (CALL_OPENGL *WindowPos3f)(GLfloat x, GLfloat y, GLfloat z);
  WindowPos3f glWindowPos3f;
  typedef void (CALL_OPENGL *WindowPos3i)(GLint x, GLint y, GLint z);
  WindowPos3i glWindowPos3i;
  typedef void (CALL_OPENGL *WindowPos3s)(GLshort x, GLshort y, GLshort z);
  WindowPos3s glWindowPos3s;
  typedef void (CALL_OPENGL *WindowPos3dv)(const GLdouble* p);
  WindowPos3dv glWindowPos3dv;
  typedef void (CALL_OPENGL *WindowPos3fv)(const GLfloat* p);
  WindowPos3fv glWindowPos3fv;
  typedef void (CALL_OPENGL *WindowPos3iv)(const GLint* p);
  WindowPos3iv glWindowPos3iv;
  typedef void (CALL_OPENGL *WindowPos3sv)(const GLshort* p);
  WindowPos3sv glWindowPos3sv;
  
  /**
    Defines the 2D orthographic projection matrix.
  */
  inline void ortho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top) throw() {
    glOrtho(left, right, bottom, top, -1, 1);
  }

  /**
    Defines a viewing transformation.
  */
  void lookAt(const Vector3D<double>& eye, const Vector3D<double>& center, const Vector3D<double>& up) throw();

  /**
    Sets the perspective projection matrix.

    @param fovy The field of view angle in degrees.
    @param aspectRatio The aspect ratio.
    @param zNear Distance from the viewer to the near clipping plane.
    @param zFar Distance from the viewer to the far clipping plane.
  */
  void perspective(GLdouble fovy, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar) throw();
  
  Vector3D<double> project(Vector3D<double> object, const Matrix4x4<double> model, const Matrix4x4<double> projection, const GLint viewport[4]) throw(OpenGLException);
  
  void pickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height, GLint viewport[4]) throw();
  
  Vector3D<double> unProject(const Vector3D<double>& window, const Matrix4x4<double>& model, const Matrix4x4<double>& projection, const GLint viewport[4]) throw(OpenGLException);

  /**
    Draws a box.
    
    @param width The width of the box.
    @param length The length of the box.
    @param height The height of the box.
    @param flags The flags.
  */
  void box(double width, double length, double height, unsigned int flags) throw();
  
  void cylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height, unsigned int slices, unsigned int stacks) throw();
  
  inline void cone(GLdouble baseRadius, GLdouble height, unsigned int slices, unsigned int stacks) throw() {
    cylinder(baseRadius, 0, height, slices, stacks);
  }

  void disk(double innerRadius, double outerRadius, unsigned int slices, unsigned int loops) throw();

  void partialDisk(GLdouble innerRadius, GLdouble outerRadius, unsigned int slices, unsigned int loops, GLdouble startAngle, GLdouble sweepAngle) throw();

  /**
    Sphere.

    @param radius The radius of the sphere.
    @param slices The number of slices.
    @param stacks The number of stacks.
  */
  void sphere(double radius, unsigned int slices, unsigned int stacks) throw();

  /**
    Torus.

    @param innerRadius The inner radius.
    @param outerRadius The outer radius.
    @param numberOfRings The number of rings.
    @param numberOfSides The number of sides.
  */
  void torus(GLdouble innerRadius, GLdouble outerRadius, unsigned int numberOfRings, unsigned int numberOfSides) throw();
  
  /**
    Initializes the OpenGL interface.
    
    @param lastest Selects the latest specification to be loaded. The default is version 1.4 (i.e. 0x010400).
  */
  OpenGL(unsigned int latest = 0x010400) throw(OpenGLException);

  /**
    Returns the OpenGL specification version (0x010100, 0x010200, 0x010201, 0x010300, or 0x010400).
  */
  inline unsigned int getSpecification() throw() {
    return specification;
  }

  /**
    Returns the vendor.
  */
  inline String getVendor() throw() {
    const GLubyte* vendor = glGetString(OpenGL::VENDOR);
    return NativeString(Cast::pointer<const char*>(vendor));
  }

  /**
    Returns the renderer.
  */
  inline String getRenderer() throw() {
    const GLubyte* renderer = glGetString(OpenGL::RENDERER);
    return NativeString(Cast::pointer<const char*>(renderer));
  }
  
  /**
    Returns the version.
  */
  inline String getVersion() throw() {
    const GLubyte* version = glGetString(OpenGL::VERSION);
    return NativeString(Cast::pointer<const char*>(version));
  }

  /**
    Returns the extensions.
  */
  inline String getExtensions() throw() {
    const GLubyte* extensions = glGetString(OpenGL::EXTENSIONS);
    return NativeString(Cast::pointer<const char*>(extensions));
  }
  
  /**
    Returns true if the extension is supported.
  */
  bool isSupported(const String& name) throw(OpenGLException);

  class Block {
  private:
    
    OpenGL& openGL;
  public:
    
    inline Block(OpenGL& _openGL, GLenum mode) throw() : openGL(_openGL) {
      openGL.glBegin(mode);
    }
    
    inline ~Block() throw() {
      openGL.glEnd();
    }
  };
  
  class PushPop {
  private:
    
    OpenGL& openGL;
  public:
    
    inline PushPop(OpenGL& _openGL) throw() : openGL(_openGL) {
      openGL.glPushMatrix();
    }
    
    inline ~PushPop() throw() {
      openGL.glPopMatrix();
    }
  };

  class DisplayList {
  private:
    
    OpenGL& openGL;
  public:
    
    inline DisplayList(OpenGL& _openGL, GLuint list, GLenum mode = OpenGL::COMPILE) throw()
      : openGL(_openGL) {
      openGL.glNewList(list, mode);
    }
    
    inline ~DisplayList() throw() {
      openGL.glEndList();
    }
  };

  class ReserveDisplayLists : public Object {
  private:
    
    OpenGL& openGL;
    unsigned int offset;
    unsigned int numberOfLists;
    Allocator<unsigned int> objects;
  public:
    
    inline ReserveDisplayLists(OpenGL& _openGL, unsigned int _numberOfLists) throw(OutOfDomain, OpenGLException)
      : openGL(_openGL),
        numberOfLists(_numberOfLists),
        objects(_numberOfLists) {
      bassert(numberOfLists > 0, OutOfDomain(this));
      offset = openGL.glGenLists(numberOfLists);
      bassert(offset > 0, OpenGLException("Display lists exhausted", this));
      unsigned int* dest = objects.getElements();
      const unsigned int* end = dest + numberOfLists;
      for (unsigned int id = offset; dest < end; ++id, ++dest) {
        *dest = id;
      }
    }
    
    inline unsigned int getOffset() const throw() {
      return offset;
    }
    
    inline void execute() const throw() {
      openGL.glCallLists(numberOfLists, UNSIGNED_INT, objects.getElements());
    }
    
    inline ~ReserveDisplayLists() throw() {
      openGL.glDeleteLists(offset, numberOfLists);
    }
  };
  
  class ReserveTexture : public Object {
  private:
    
    OpenGL& openGL;
    unsigned int name;
  public:
    
    inline ReserveTexture(OpenGL& _openGL) throw(OutOfDomain)
      : openGL(_openGL) {
      openGL.glGenTextures(1, &name);
    }
    
    inline unsigned int getName() const throw() {
      return name;
    }
    
    inline ~ReserveTexture() throw() {
      openGL.glDeleteTextures(1, &name);
    }
  };
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#undef CALL_OPENGL

#endif
