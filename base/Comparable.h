/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_COMPARABLE_H
#define _DK_SDU_MIP_BASE_COMPARABLE_H

/**
  Comparable interface.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class Comparable {
public:

  /**
    Equality operator.

    @param object Object to be compared.
    @return True if the object are equal.
  */
  virtual bool operator==(const Comparable& comparable) const {return (this == &comparable);};
};

#endif
