/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_UI__BITMAP_H
#define _DK_SDU_MIP__BASE_UI__BITMAP_H

#include <base/Dimension.h>
#include <base/mem/ReferenceCountedObject.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A bitmap stored in native format.
  
  @short Bitmap.
  @ingroup ui
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Bitmap : public Object {
private:

  class Handle : public ReferenceCountedObject {
  public:
  };

  ReferenceCountedObjectPointer<Handle> handle;
public:

  Bitmap() throw();

  inline Bitmap(const Bitmap& copy) throw()
    : handle(copy.handle) {
  }

  inline Bitmap& operator=(const Bitmap& eq) throw() {
    handle = eq.handle;
    return *this;
  }
  
  Dimension getDimension() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
