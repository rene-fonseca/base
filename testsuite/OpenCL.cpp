/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/opencl/OpenCL.h>

using namespace com::azure::dev::base;

class OpenCLApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  OpenCLApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("OpenCL", numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    fout << "Number of OpenCL devices: " << OpenCL::getNumberOfDevices() << ENDL;

    auto devices = OpenCL::getDevices();
    fout << "Devices: " << devices << ENDL;

    const OpenCL::DeviceId device = devices[0];
    const String name = OpenCL::getDeviceName(device);
    fout << "Device name: " << name << ENDL;

    OpenCL openCL;
    if (!openCL.createContext(OpenCL::TYPE_GPU)) {
      fout << "Failed to create OpenCL context." << ENDL;
      return;
    }
    fout << "Context: " << openCL.isValid() << ENDL;

    //openCL.createBuffer();

    const char* source = "kernel void square("
         "          global float* input,"
        "           global float* output) "
       "{"
      "size_t i = get_global_id(0);"
      "output[i] = input[i] * input[i];"
      " }";
    OpenCL::Program program = openCL.createProgram(source);
    program.build();
    OpenCL::Kernel kernel = program.createKernel("square");
    kernel.setArgument(0, 4, nullptr);
  }
};

STUB(OpenCLApplication);
