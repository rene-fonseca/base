/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/dl/LinkerException.h>
#include <base/UnitTest.h> // annoying to do for each use

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)

class TEST_CLASS(LinkerException) : public UnitTest {
public:
  
  TEST_CLASS(LinkerException)(const String& name) : UnitTest(name) {
    // setSource(__FILE__);
  }
  
  void run()
  {  // REGISTER_HERE("A");
    try {
      LinkerException e;
      // here("A");
      throw e;
      // here("B");
    } catch (...) {
    }
    // XXX
  }
};

REGISTER_TEST(LinkerException);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
