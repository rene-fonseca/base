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
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Jupyter and compatible interpreter front-ends integration. */
class Jupyter { // class InterpreterFrontEnd
public:
  
  /** Clear output field. */
  static inline void clearOutput(bool wait = true)
  {
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)
    xeus::get_interpreter().clear_output(wait);
#else
    fout << "\033[2K\r" << FLUSH;
    ferr << "\033[2K\r" << FLUSH;
#endif
  }
  
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)
  /** Display. */
  template<class TYPE>
  static void display(const TYPE& v)
  {
    xeus::get_interpreter().display_data(
      _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(v),
      ClingMimeBundleHandle(),
      ClingMimeBundleHandle()
    );
  }

  /** Display. */
  template<class TYPE>
  static void display(const TYPE& v, const String& id, bool update = false)
  {
    if (update) {
      xeus::get_interpreter().update_display_data(
        _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(v),
        ClingMimeBundleHandle(),
        cling_getAnyMimeBundle("display_id", id)
      );
    } else {
      xeus::get_interpreter().display_data(
        _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(v),
        ClingMimeBundleHandle(),
        cling_getAnyMimeBundle("display_id", id)
      );
    }
  }
#endif

  /** Display data. */
  static MediaBlob blob(const String& text, const String& mimetype);

  /** Display HTML. */
  static MediaBlob html(const String& text);
  
  /** Display JavaScript. */
  static MediaBlob script(const String& text);
  
  /** Display data in iframe. */
  static MediaBlob iframe(const String& url, unsigned width = 300, unsigned int height = 300);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
