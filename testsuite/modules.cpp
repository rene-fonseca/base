/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/Module.h>

using namespace com::azure::dev::base;

class ModulesApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ModulesApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) noexcept
    : Application("modules", numberOfArguments, arguments, environment)
  {
  }

  void main()
  {
    auto& manager = ModuleManager::getManager();
    if (!manager.traverseModules()) {
      setExitCode(1);
      return;
    }

    // TAG: dump
    // TAG: connect to online service to check if recalled
    // TAG: dump graph
  }
};

APPLICATION_STUB(ModulesApplication);
