/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/ui/GraphicsContext.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Pixmap.

  @short Pixmap.
  @ingroup ui
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Pixmap : public GraphicsContext {
private:
  
  void destroy() throw();
public:
  
  /**
    Initializes a pixmap.
    
    @param dimension The dimension of the pixmap.
    @param flags The format flags.
  */
  Pixmap(const Dimension& dimension, unsigned int flags) throw(UserInterfaceException);

  enum Format {
    RGB
  };

  enum Encoding {
    RGB_32
  };
  
  /**
  */
  void encode(Format format, Encoding encoding, void* data) throw(UserInterfaceException);
  
  /**
    Destroys the pixmap.
  */
  ~Pixmap() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
