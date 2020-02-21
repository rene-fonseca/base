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

#include <base/media/Media.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Jupyter integration. */
class Jupyter {
public:
  
#if 0 // jupyter: not working
  /** Clear output field. */
  static inline void clearOutput()
  {
    fout << "\033[2K\r" << FLUSH;
    ferr << "\033[2K\r" << FLUSH;
  }
#endif
  
  /** Display data. */
  static inline DisplayObject display(const String& text, const String& mimetype = "text/html");

  /** Display HTML. */
  static inline DisplayObject html(const String& text);

  /** Display data in iframe. */
  static inline DisplayObject iframe(const String& url, unsigned width = 300, unsigned int height = 300);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
