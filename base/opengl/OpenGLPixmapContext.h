/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_OPENGL__OPEN_GL_PIXMAP_CONTEXT_H
#define _DK_SDU_MIP__BASE_OPENGL__OPEN_GL_PIXMAP_CONTEXT_H

#include <base/opengl/OpenGLContextImpl.h>
#include <base/Dimension.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  OpenGL pixmap rendering context.

  @short OpenGL pixmap rendering context.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OpenGLPixmapContext : public OpenGLContextImpl {
private:

  nothing prefixInitialization;
  void* screenHandle;
  
  /**
    Initializes the pixmap rendering context.
  */
  nothing initialize(const Dimension& dimension, unsigned int flags) throw(OpenGLException);
public:
  
  /**
    Initializes an OpenGL pixmap rendering context.

    @param dimension The dimension of the pixmap.
    @param flags The format flags.
  */
  OpenGLPixmapContext(const Dimension& dimension, unsigned int flags) throw(OpenGLException);

  /**
    Destroys the OpenGL pixmap rendering context.
  */
  ~OpenGLPixmapContext() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
