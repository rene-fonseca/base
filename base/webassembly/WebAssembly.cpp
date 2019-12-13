/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/webassembly/WebAssembly.h>
#include <base/io/File.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_WASM)
#  include <wasm.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class WebAssembly::Handle : public ReferenceCountedObject {
private:

  MemorySize maximumMemoryUsage = 0;
public:

  String getEngine()
  {
    return String("WASM 1.0");
  }

  MemorySize getMaximumMemoryUsage() const
  {
    return maximumMemoryUsage;
  }

  void setMaximumMemoryUsage(MemorySize size)
  {
    maximumMemoryUsage = size;
  }

  MemorySize getMemoryUsage() const
  {
    return 0;
  }

  uint64 getProcessingTime() const
  {
    return 0;
  }

  void garbageCollect()
  {
  }

  bool load(const uint8* wasm, MemorySize size)
  {
    return true;
  }
  
  Array<Symbol> getExports()
  {
    Array<Symbol> result;
    return result;
  }

  Symbol getSymbol(const String& id)
  {
    return Symbol();
  }

  AnyValue call(const String& id, const Array<AnyValue>& arguments)
  {
    // TAG: handle exceptions
    fout << "Call " << id << "(";
    bool first = true;
    for (auto a : arguments) {
      if (!first) {
        fout << ", ";
      }
      first = false;
      fout << a;
    }
    fout << ")" << ENDL;
    return AnyValue();
  }

  void async(const String& id)
  {
#if 0
    auto thread = new Thread();
    thread->start();
    thread->run();
#endif
  }
};

WebAssembly::WebAssembly()
{
  handle = new Handle();
}

String WebAssembly::getEngine()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getEngine();
}

uint64 WebAssembly::getProcessingTime() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getProcessingTime();
}

MemorySize WebAssembly::getMaximumMemoryUsage() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getMaximumMemoryUsage();
}

void WebAssembly::setMaximumMemoryUsage(MemorySize size)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->setMaximumMemoryUsage(size);
}

MemorySize WebAssembly::getMemoryUsage() const
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getMemoryUsage();
}

void WebAssembly::garbageCollect()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->garbageCollect();
}

bool WebAssembly::load(const String& path)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  
  Allocator<uint8> buffer;
  {
    File file(path, File::READ, 0);
    if (file.getSize() > (128 * 1024 * 1024)) {
      return false;
    }
    buffer.setSize(file.getSize());
    file.read(buffer.getElements(), static_cast<unsigned int>(file.getSize()));
  }
  return load(buffer.getElements(), buffer.getSize());
}

bool WebAssembly::load(const uint8* wasm, MemorySize size)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->load(wasm, size);
}

Array<WebAssembly::Symbol> WebAssembly::getExports()
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getExports();
}

WebAssembly::Symbol WebAssembly::getSymbol(const String& id)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->getSymbol(id);
}

AnyValue WebAssembly::call(const String& id, const Array<AnyValue>& arguments)
{
  auto handle = this->handle.cast<WebAssembly::Handle>();
  return handle->call(id, arguments);
}

WebAssembly::~WebAssembly()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
