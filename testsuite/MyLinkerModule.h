/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/dl/LinkerModule.h>
#include <base/string/FormatOutputStream.h>

using namespace dk::sdu::mip::base;

class MyLinkerModule : public LinkerModule {
public:

  MyLinkerModule(const String& identity, const String& name, const String& version) throw();

  virtual void myFunction() throw();
};
