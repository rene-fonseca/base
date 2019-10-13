/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/opencl/OpenCL.h>
#include <OpenCL/opencl.h>

// https://developer.apple.com/library/archive/samplecode/OpenCL_Hello_World_Example/Listings/hello_c.html#//apple_ref/doc/uid/DTS40008187-hello_c-DontLinkElementID_4

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#define _COM_AZURE_DEV__BASE__HAS_OPENCL

#if defined(_COM_AZURE_DEV__BASE__HAS_OPENCL)

unsigned int OpenCL::getNumberOfDevices()
{
  const cl_uint count = 1;
  // TAG: type
  int err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 0, NULL, (cl_uint*)&count);
  if (err != CL_SUCCESS) {
    return 0;
  }
  return count;
}

Array<OpenCL::DeviceId> OpenCL::getDevices(/*any,cpu,gpu*/)
{
  Array<DeviceId> result;

  const cl_uint count = 1;
  int err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 0, NULL, (cl_uint*)&count);

  PrimitiveArray<cl_device_id> devices(count + 1);
  bool gpu = true;
  err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, count, static_cast<cl_device_id*>(devices), NULL);
  if (err != CL_SUCCESS) {
    return result;
  }

  // result.setCapacity(count);
  for (unsigned int i = 0; i < count; ++i) {
    result.append(devices[i]);
  }
  
  return result;
}

String OpenCL::getDeviceName(DeviceId device)
{
  char buffer[128];
  clGetDeviceInfo((cl_device_id)device, CL_DEVICE_NAME, getArraySize(128), buffer, NULL);
  return buffer;
/*
  CL_DEVICE_EXTENSIONS
*/
}

uint32 OpenCL::getDeviceInfoUInt(DeviceId device)
{
  cl_uint result = 0;
  clGetDeviceInfo((cl_device_id)device, CL_DEVICE_ADDRESS_BITS, sizeof(result), &result, NULL);
  return buffer;
/*
  CL_DEVICE_ADDRESS_BITS
  CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE
*/
}

/*
CL_DEVICE_GLOBAL_MEM_CACHE_SIZE
CL_DEVICE_GLOBAL_MEM_SIZE
*/

bool OpenCL::getDeviceInfoBool(DeviceId device)
{
  cl_bool result = false;
  clGetDeviceInfo((cl_device_id)device, CL_DEVICE_AVAILABLE, sizeof(result), &result, NULL);

/*
  CL_DEVICE_AVAILABLE
  CL_DEVICE_COMPILER_AVAILABLE
  CL_DEVICE_ENDIAN_LITTLE
  CL_DEVICE_ERROR_CORRECTION_SUPPORT
    CL_DEVICE_IMAGE_SUPPORT

*/
}

OpenCL::OpenCL() : context(nullptr) {
}

bool OpenCL::createContext(Type type)
{
  if (context) {
    return false;
  }
  cl_context context;
  switch (type) {
  case TYPE_CPU:
    context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_CPU, NULL, NULL, NULL);
    break;
  case TYPE_GPU:
    context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
    break;
  default:
    return false;
  }
  this->context = context;
  return context != 0;
}

bool OpenCL::createContext(DeviceId _deviceId)
{
  if (context) {
    return false;
  }
  int err = 0;
  cl_device_id deviceId = (cl_device_id)_deviceId;
  cl_context context = clCreateContext(0, 1, &deviceId, NULL, NULL, &err);
  if (err != CL_SUCCESS) {
    return false;
  }
  this->context = context;
  return context != 0;
}

OpenCL::Queue OpenCL::createCommandQueue(MemorySize size)
{
  cl_context context = (cl_context)this->context;
  cl_device_id devices[1];
  cl_command_queue queue = clCreateCommandQueue(context, devices[0], 0, NULL);

  Queue q;
  return q;
}

void* OpenCL::createBuffer(MemorySize size)
{
  cl_context context = (cl_context)this->context;
  void* b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, size, NULL, NULL);
  // CL_MEM_READ_WRITE
  return b;
}

void OpenCL::Kernel::setArgument(unsigned int index, MemorySize size, void* value)
{
  cl_kernel kernel = (cl_kernel)this->handle;
  clSetKernelArg(kernel, index, size, (void*)&value); // TAG: FIXME
}

OpenCL::Queue::Queue()
{
}

OpenCL::Program::Program()
{
}

OpenCL::Kernel::Kernel()
{
}

OpenCL::Program OpenCL::createProgram(const char* source)
{  
  cl_context context = (cl_context)this->context;
  cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source, NULL, NULL);
//  clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
  Program p;
  return p;
}

void OpenCL::Program::build()
{
  // TAG: keep context so we can ensure we do not mixed contexts
/*
  if (built) {
    return;
  }
*/
  cl_program program = (cl_program)this->handle;
  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
}

OpenCL::Kernel OpenCL::Program::createKernel(const String& name)
{
  cl_program program = (cl_program)this->handle;
	cl_kernel kernel = clCreateKernel(program, name.getElements(), NULL);
  Kernel k;
  return k;
}

OpenCL::Kernel::~Kernel()
{
  if (handle) {
    clReleaseKernel((cl_kernel)handle);
    handle = nullptr;
  }
}

OpenCL::Program::~Program()
{
  if (handle) {
    clReleaseProgram((cl_program)handle);
    handle = nullptr;
  }
}

OpenCL::~OpenCL()
{
  if (context) {
    cl_context context = (cl_context)this->context;
    clReleaseContext(context);
    context = nullptr;
  }
}

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
