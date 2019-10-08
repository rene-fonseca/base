/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Color.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Color::Color(NamedColor color) throw() {
  static uint32 NAMED_COLORS[] = {
    0xfffccc, // Almond(Blanched)
    0x00ffff, // Aqua
    0x60cc93, // Aquamarine
    0x6fffc3, // Aquamarine(Light)
    0x33cc99, // Aquamarine(Med)
    0xf6f6cc, // Beige
    0xfff3c3, // Bisque
    0x000000, // Black
    0x0000ff, // Blue
    0x6f9f9f, // Blue(Cadet)
    0x6f9f90, // Blue(Cadet3)
    0x33336f, // Blue(CornFlower)
    0x6396fc, // Blue(CornFlower-Light)
    0x00009c, // Blue(Dark)
    0x6c339f, // Blue(DarkSlate)
    0x99ccff, // Blue(Light)
    0x3333cc, // Blue(Med)
    0x3f33f3, // Blue(Midnight)
    0x33339f, // Blue(Navy)
    0x3c3cff, // Blue(Neon)
    0x3399cc, // Blue(Sky)
    0x336c9f, // Blue(Steel)
    0x993cf3, // Blue(Violet)
    0xc69633, // Brass
    0x9c6963, // Bronze
    0x963939, // Brown
    0x663033, // Brown(Dark)
    0xf6ccc0, // Brown(Faded)
    0x6fff00, // Chartruse
    0xc3690f, // Chocolate
    0x6c3306, // Chocolate(Bakers)
    0x6c3336, // Chocolate(SemiSweet)
    0xc96333, // Copper
    0x39666f, // Copper(DarkGreen)
    0x636f66, // Copper(Green)
    0xff6f60, // Coral
    0xff6f00, // Coral(Bright)
    0xfff9cc, // Cornsilk
    0xcc033c, // Crimson
    0x00ffff, // Cyan
    0x009c9c, // Cyan(Dark)
    0x9f3333, // Firebrick
    0xff00ff, // Fushia
    0xffcc00, // Gold
    0xc9c909, // Gold(Bright)
    0xc6c63c, // Gold(Old)
    0xcccc60, // Goldenrod
    0xc9960c, // Goldenrod(Dark)
    0xf9f99f, // Goldenrod(Med)
    0x909090, // Gray
    0x3f3f3f, // Gray(DarkSlate)
    0x666666, // Gray(Dark)
    0x999999, // Gray(Light)
    0xc0c0c0, // Gray(Lighter)
    0x009000, // Green
    0x006300, // Green(Dark)
    0x3f3f3f, // Green(Forest)
    0x9fcc9f, // Green(Pale)
    0x99cc33, // Green(Yellow)
    0x9f9f6f, // Khaki
    0xccc66c, // Khaki(Light)
    0x00ff00, // Lime
    0xff00ff, // Magenta
    0xcc009c, // Magenta(Dark)
    0x900000, // Maroon
    0x9f336c, // Maroon3
    0x000090, // Navy
    0x909000, // Olive
    0x666c3f, // Olive(Dark)
    0x3f3f3f, // Olive(Darker)
    0xff6f00, // Orange
    0xff9c00, // Orange(Light)
    0xcc60cc, // Orchid
    0x9933cc, // Orchid(Dark)
    0xf99cf9, // Pink
    0xcc9f9f, // Pink(Dusty)
    0x900090, // Purple
    0x960669, // Purple(Dark)
    0xc9c9f3, // Quartz
    0xff0000, // Red
    0x9c0000, // Red(Dark)
    0xff3300, // Red(Orange)
    0x966363, // Rose(Dusty)
    0x6f3333, // Salmon
    0x9c0606, // Scarlet
    0x9f6c33, // Sienna
    0xf6f9f9, // Silver
    0x39c0cf, // Sky(Summer)
    0xcc9360, // Tan
    0x96693f, // Tan(Dark)
    0x009090, // Teal
    0xc9cfc9, // Thistle
    0x9cf9f9, // Turquoise
    0x00cfc0, // Turquoise(Dark)
    0x9900ff, // Violet
    0x6600cc, // Violet(Blue)
    0xcc3399, // Violet(Red)
    0xffffff, // White
    0xf9fcc6, // White(Antique)
    0xcfc996, // Wood(Curly)
    0x966f33, // Wood(Dark)
    0xf9c396, // Wood(Light)
    0x969063, // Wood(Med)
    0xffff00 // Yellow
  };
  uint32 temp = NAMED_COLORS[color];
  value = ((temp & 0xff) << 16) | (temp & 0x00ff00) | (temp >> 16);
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Color& value) throw(IOException) {
  FormatOutputStream::PushContext push(stream);
  if (stream.getBase() == FormatOutputStream::Symbols::HEXADECIMAL) {
    return stream << value; // e.g. 0x123456
  } else {
    return stream << '('
                  << value.getRed() << ','
                  << value.getGreen() << ','
                  << value.getBlue() << ')';
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
