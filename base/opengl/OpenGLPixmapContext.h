/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/opengl/OpenGLContextImpl.h>
#include <base/Dimension.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  OpenGL pixmap rendering context.

  @short OpenGL pixmap rendering context.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OpenGLPixmapContext : public OpenGLContextImpl {
private:

  nothing prefixInitialization;
  void* screenHandle = nullptr;
  
  /**
    Initializes the pixmap rendering context.
  */
  nothing initialize(const Dimension& dimension, unsigned int flags);
public:
  
  /**
    Initializes an OpenGL pixmap rendering context.

    @param dimension The dimension of the pixmap.
    @param flags The format flags.
  */
  OpenGLPixmapContext(const Dimension& dimension, unsigned int flags);

  /**
    Destroys the OpenGL pixmap rendering context.
  */
  ~OpenGLPixmapContext() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
