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

#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  RGB color.
  
  @short Color.
  @ingroup ui
  @version 1.0
*/

_COM_AZURE_DEV__BASE__PACKED__BEGIN
class _COM_AZURE_DEV__BASE__API Color {
private:

  /** The color value (red, green, and blue). */
  uint32 value = 0;
public:

  /** Named color. */
  enum NamedColor {
    ALMOND_BLANCHED,
    AQUA,
    AQUAMARINE,
    AQUAMARINE_LIGHT,
    AQUAMARINE_MED,
    BEIGE
    // TAG: fixme
  };
  
  /**
    Initializes color with unspecified value.
  */
  inline Color() throw() {
  }

  /**
    Initializes color from name of color (e.g. YELLOW).
  */
  Color(NamedColor color) throw();

  /**
    Initializes color from rgb value (e.g. 0x123456).
  */
  inline Color(uint32 _value) throw() : value(_value) {
  }

  /**
    Initializes color as gray.
  */
  inline Color(uint8 gray) throw()
    : value((static_cast<uint32>(gray) << 16) |
            (static_cast<uint32>(gray) << 8) |
            static_cast<uint32>(gray)) {
  }

  /**
    @param red The red component.
    @param green The green component.
    @param blue The blue component.
  */
  inline Color(uint8 red, uint8 green, uint8 blue) throw()
    : value((static_cast<uint32>(blue) << 16) |
            (static_cast<uint32>(green) << 8) |
            static_cast<uint32>(red)) {
  }

  /**
    Initializes color form other color.
  */
  inline Color(const Color& copy) throw()
    : value(copy.value) {
  }

  /**
    Assignment of color by color.
  */
  inline Color& operator=(const Color& assign) throw() {
    value = assign.value;
    return *this;
  }

  /**
    Returns true if the colors are equal.
  */
  inline bool operator==(const Color color) const throw() {
    return value == color.value;
  }
  
  /**
    Returns true if the colors are unequal.
  */
  inline bool operator!=(const Color color) const throw() {
    return value != color.value;
  }

  /**
    Returns the red component.
  */
  inline uint8 getRed() const throw() {
    return static_cast<uint8>(value >> 0);
  }
  
  /**
    Sets the red component.
  */
  inline void setRed(uint8 value) throw() {
    value = (value & 0xffff00) | (static_cast<unsigned int>(value) << 0);
  }

  /**
    Returns the green component.
  */
  inline uint8 getGreen() const throw() {
    return static_cast<uint8>(value >> 8);
  }

  /**
    Sets the green component.
  */
  inline void setGreen(uint8 value) throw() {
    value = (value & 0xff00ff) | (static_cast<unsigned int>(value) << 8);
  }
  
  /**
    Returns the blue component.
  */
  inline uint8 getBlue() const throw() {
    return static_cast<uint8>(value >> 16);
  }

  /**
    Sets the blue component.
  */
  inline void setBlue(uint8 value) throw() {
    value = (value & 0x00ffff) | (static_cast<unsigned int>(value) << 16);
  }

  /**
    Returns the gray value of the color (i.e. (red + green + blue + 1)/3).
  */
  inline unsigned int getGray() const throw() {
    return (static_cast<unsigned int>(value >> 16) +
            static_cast<unsigned int>(value >> 8) +
            static_cast<unsigned int>(value >> 0) + 1)/3;
  }

  /**
    Sets the color as gray.
  */
  inline void setGray(uint8 value) throw() {
    value = (static_cast<uint32>(value) << 16) |
      (static_cast<uint32>(value) << 8) |
      static_cast<uint32>(value);
  }

  /**
    Returns the color as a value.
  */
  inline uint32 getValue() const throw() {
    return value;
  }
  
  /**
    Returns the color as a value.
  */
  inline operator uint32() const throw() {
    return value;
  }
} _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

/**
  Writes a string representation of the color object to the specified format stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Color& value) throw(IOException);

template<>
class IsUninitializeable<Color> : public IsUninitializeable<decltype(Color::value)> {
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
