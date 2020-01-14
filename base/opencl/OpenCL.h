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

#include <base/collection/Array.h>
#include <base/Primitives.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This exception is raised by the OpenCL class.
  
  @short OpenCL exception
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OpenCLException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  OpenCLException() noexcept;

  /**
    Initializes the exception object.

    @param message The message.
  */
  OpenCLException(const char* message) noexcept;

  /**
    Initializes the exception object without an associated message.

    @param type The identity of the type.
  */
  OpenCLException(const Type& type) noexcept;

  /**
    Initializes the exception object.

    @param message An NULL-terminated string.
    @param type The identity of the type.
  */
  OpenCLException(const char* message, const Type& type) noexcept;

  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

/**
  OpenCL context.

  @short OpenCL.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OpenCL {
private:

  void* context = nullptr;
public:

  class _COM_AZURE_DEV__BASE__API HandleImpl : public ReferenceCountedObject {
  public:

    void* handle = nullptr;
  };

  typedef Reference<HandleImpl> Handle;

  enum Type {
    TYPE_CPU,
    TYPE_GPU
  };

  /** Queue. */
  class _COM_AZURE_DEV__BASE__API Queue {
    friend class OpenCL;
  private:

    Handle handle;
    // void* handle = nullptr;

    Queue(void* _handle);
  public:

    Queue();
  };

  /** Kernel. */
  class _COM_AZURE_DEV__BASE__API Kernel {
    friend class OpenCL;
  private:

    void* handle = nullptr;

    Kernel(void* _handle);
  public:

    Kernel();

    void setArgument(unsigned int index, MemorySize size, void* v);

    ~Kernel();
  };

  /** Program. */
  class _COM_AZURE_DEV__BASE__API Program {
    friend class OpenCL;
  private:

    // Reference<Handle> handle;
    void* handle = nullptr;

    Program(void* _handle);
  public:

    Program();

    void build();

    Kernel createKernel(const String& name);

    ~Program();
  };

  typedef void* DeviceId;

  /** Returns the number of devices. */
  static unsigned int getNumberOfDevices();

  enum {
    FLAG_CPU = 1,
    FLAG_GPU = 2,
    FLAG_ANY = FLAG_CPU | FLAG_GPU
  };

  /** Returns the available devices. */
  static Array<DeviceId> getDevices(unsigned int flags = FLAG_ANY);

  /** Returns the name of the given device. */
  static String getDeviceName(DeviceId device);

  /** Returns uint value of the given device. */
  static uint32 getDeviceInfoUInt(DeviceId device);
  
  /** Returns bool value of the given device. */
  static bool getDeviceInfoBool(DeviceId device);

  OpenCL();

  bool createContext(Type type);

  bool createContext(DeviceId device);

  /** Returns true if context is valid. */
  inline bool isValid() const noexcept
  {
    return context != nullptr;
  }

  enum Access {
    ACCESS_READ_ONLY,
    ACCESS_WRITE_ONLY
  };

  Queue createCommandQueue(MemorySize size);

  Program createProgram(const char* source);

  Kernel createKernel();

  Queue createQueue();

  // void* createReadBuffer(MemorySize size);
  // void* createWriteBuffer(MemorySize size);

  void* createBuffer(MemorySize size);

  // Buffer createBuffer(Access access, MemorySize size);
  // Buffer.enqueueWriteBuffer(); - read and write from buffer 
  // void wait();

  ~OpenCL();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
