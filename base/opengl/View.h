/***************************************************************************
    Generic Image Processing (GIP) Framework
    A framework for developing image processing applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/ui/Position.h>
#include <base/Dimension.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A region of an image.
  
  @short A region of an image.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Region {
private:
  
  /** The offset of the region. */
  Point2D offset;
  /** The dimension of the region. */
  Dimension dimension;
public:
  
  /**
    Initializes region with offset (0, 0) and dimension (0, 0).
  */
  inline Region() noexcept : offset(0, 0), dimension(0, 0)
  {
  }
  
  /**
    Initializes the region with the specified offset and dimension.
    
    @param offset The desired offset of the region.
    @param dimension The desired dimension of the region.
  */
  Region(const Point2D& offset, const Dimension& dimension) noexcept;
  
  /**
    Initializes region from other region.
  */
  inline Region(const Region& copy) noexcept
    : offset(copy.offset), dimension(copy.dimension)
  {
  }
  
  /**
    Assignment of region by region.
  */
  inline Region& operator=(const Region& assign) noexcept
  {
    offset = assign.offset;
    dimension = assign.dimension;
    return *this;
  }
  
  /**
    Returns the offset of the region.
  */
  inline const Point2D& getOffset() const noexcept
  {
    return offset;
  }
  
  /**
    Returns the dimension of the region.
  */
  inline const Dimension& getDimension() const noexcept
  {
    return dimension;
  }
  
  /**
    Sets the offset of the region.
  */
  inline void setOffset(const Point2D& offset) noexcept
  {
    this->offset = offset;
  }
  
  /**
    Sets the dimension of the region.
  */
  inline void setDimension(const Dimension& dimension) noexcept
  {
    this->dimension = dimension;
  }
  
  /**
    Returns true if the region spans elements (i.e. the dimension is proper).
  */
  inline bool isProper() const noexcept
  {
    return dimension.isProper();
  }
};

inline Region::Region(const Point2D& _offset, const Dimension& _dimension) noexcept
  : offset(_offset), dimension(_dimension)
{
}

template<>
class IsUninitializeable<Region> :
  public BooleanConstant<IsUninitializeable<Point2D>() && IsUninitializeable<Dimension>()> {
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
