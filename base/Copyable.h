/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_COPYABLE_H
#define _BASE_COPYABLE_H

/**
  Copyable interface.

  @author René Møller Fonseca
  @version 1.0
*/

class Copyable {
public:

  /**
    Initializes object.
  */
//  Copyable();

  /**
    Initializes object based on existing object. The default implementation is used if this is not implemented.

    @param copyable The desired object.
  */
//  Copyable(const Copyable& copyable);

  /**
    Assignment of object to object. The default implementation is used if this is not implemented.

    @param object The desired object.
  */
//  Copyable& operator=(const Copyable& copyable);
};

#endif
