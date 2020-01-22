/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/StackFrame.h>
#include <base/dl/DynamicLinker.h>
#include <base/concurrency/Thread.h>
#include <base/concurrency/ThreadLocalContext.h>
#include <base/string/FormatOutputStream.h>
#include <base/filesystem/FileSystem.h>
#include <base/TypeInfo.h>
#include <base/string/ANSIEscapeSequence.h>
#include <base/io/FileDescriptor.h>
#include <base/Application.h>
#include <base/string/Format.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX))
#  include <execinfo.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

// TAG: add support for registering stack trace in global lookup and get hash key for it
#if 0
class StackFrames {
public:

  Array<StackFrame> hashed;
  Array<StackFrame> unhashed;

  // TAG: see profiler
};
#endif

/**
  Hash performance measurement.

  You should shuffle the values before updating and do multiple runs.
*/

template<class TYPE>
class MeasureHashPerformance {
private:

  Hash<TYPE> hash;
  Array<unsigned int> slots;
  unsigned int size = 0;
  unsigned int used = 0;
  unsigned int hits = 0;
  unsigned int misses = 0;
public:

  MeasureHashPerformance(unsigned int size = 19391) // normal to use a prime
  {
    slots.setSize(size, 0);
  }

  inline unsigned int getSize() const noexcept
  {
    return size;
  }

  inline unsigned int getUsed() const noexcept
  {
    return used;
  }

  inline unsigned int getHits() const noexcept
  {
    return hits;
  }

  inline unsigned int getMisses() const noexcept
  {
    return misses;
  }

  // TAG: median is better measure
  // TAG: need distribution 5% 10%, ...

  /** Returns the average slot size. */
  unsigned int getAvgSlot() const noexcept
  {
    unsigned int result = 0;
    for (auto slot : slots) {
      result += slot;
    }
    return (result + size/2)/size;
  }

  /** Returns the minimum slot size. */
  unsigned int getMinSlot() const noexcept
  {
    unsigned int result = static_cast<unsigned int>(-1);
    for (auto slot : slots) {
      if (slot < result) {
        result = slot;
      }
    }
    return result;
  }

  /** Returns the maximum slot size. */
  unsigned int getMaxSlot() const noexcept
  {
    unsigned int result = 0;
    for (auto slot : slots) {
      if (slot > result) {
        result = slot;
      }
    }
    return result;
  }

  void dumpHist(const Array<unsigned int>& hist)
  {
    const unsigned int height = 20;
    const unsigned int width = 79;
    
    Array<unsigned int> condensed(width);
    const MemorySize W = hist.getSize();
    for (MemorySize i = 0; i < W; ++i) {
      condensed[width * (i + W / 2) / W] += hist[i];
    }

    unsigned int m = 0;
    for (MemorySize i = 0; i < condensed.getSize(); ++i) {
      m = maximum(m, condensed[i]);
    }

    // linear/logarithmic

    Array<bool> field(height * width, false);
    if (m > 0) {
      for (MemorySize i = 0; i < condensed.getSize(); ++i) {
        auto h = (condensed[i] + m/2) / m;
      }
    }

    // use color indicate subpixels? gray ~ half

    String line;
#if 0
    line.setLength(width);
    for (int h = height - 1; h >= 0; --h) {
      for (auto& ch : line) {
        line[w] = ' ';
      }
      for (unsigned int w = 0; w < width; ++w) {
        line[w] = field[h * width + w];
      }
      fout << line << ENDL;
    }
#endif
    
  }

  void getHist() const noexcept
  {
    Array<unsigned int> counts(size + 1);
    for (auto slot : slots) {
      counts[slot]++;
    }
  }

  inline void update(const TYPE& value)
  {
    auto h = hash(value);
    unsigned int slot = slots[h % size];
    if (slot != 0) {
      ++slot;
      ++hits;
    } else {
      ++misses;
      ++used;
      slot = 1;
    }
  }

  void update(const TYPE* values, MemorySize count)
  {
    const TYPE* end = values + count;
    for (; values != end; ++values) {
      update(*values);
    }
  }

  void update(const Allocator<TYPE>& values)
  {
    for (const auto& v : values) {
      update(values);
    }
  }

  void update(const Array<TYPE>& values)
  {
    // Timer timer;
    for (const auto& v : values) {
      update(values);
    }
  }
};

template<class TYPE>
class MeasureHashPerformance2 {
private:

  // TAG: generate histogram for many different sizes
  // TAG: dump stats
};

/** Check if TYPE has unsigned long getHash() const member. https://en.cppreference.com/w/cpp/language/sfinae check. */
template<typename TYPE>
class HasGetHash {
private:

  /** Member type we are looking for. */
  typedef unsigned long (TYPE::* Member)() const; // we only want const member - we dont want static either

  static_assert(IsComplete<TYPE>(), "Type must be complete.");

  template<typename POLY> static decltype(&POLY::getHash) getType(decltype(&POLY::getHash));
  template<typename POLY> static void getType(...);
public:

  static constexpr bool VALUE = std::is_same<decltype(getType<TYPE>(nullptr)), Member>(); // this works

  inline constexpr operator bool() const noexcept
  {
    return VALUE;
  }

  inline constexpr bool operator()() const noexcept
  {
    return VALUE;
  }
};



StackFrame::StackFrame()
{
}

StackFrame::StackFrame(const ConstSpan<const void*>& trace)
{
  frames.setSize(trace.getSize());
  const void** dest = frames.getElements();
  for (auto src = trace.begin(), end = trace.end(); src != end;) {
    *dest++ = *src++;
  }
}

StackFrame& StackFrame::operator=(const ConstSpan<const void*>& assign)
{
  frames.setSize(assign.getSize());
  const void** dest = frames.getElements();
  for (auto src = assign.begin(), end = assign.end(); src != end;) {
    *dest++ = *src++;
  }
  return *this;
}

namespace {

  void BASE_ADDRESS()
  {
  }

  inline void updateHash(unsigned long& state, const uint8 bits) noexcept
  {
    // needs performance testing
    if (true /*sizeof(unsigned long) == 4*/) {
      state = (state << 11) ^ (state >> 11);
      state ^= 0x98badcfe;
      state ^= (static_cast<uint32>(bits) << 24) | (static_cast<uint32>(bits) << 16) | (static_cast<uint32>(bits) << 8) | (static_cast<uint32>(bits) << 0);
      state ^= 0x10325476;
      state ^= (state << 27) ^ (state >> 27);
    }
  }

  bool useStandardOut = false;
}

bool StackFrame::getUseStandardOut() noexcept
{
  return useStandardOut;
}

void StackFrame::setUseStandardOut(bool _useStandardOut) noexcept
{
  useStandardOut = _useStandardOut;
}

// TAG: move to better class
FormatOutputStream& StackFrame::getErrorStream() noexcept
{
  return StackFrame::getUseStandardOut() ? fout : ferr;
}

unsigned long StackFrame::getHash(const ConstSpan<const void*>& trace) noexcept
{
  if (trace.getSize() == 0) {
    return 0;
  }

  const MemorySize base = reinterpret_cast<MemorySize>(&BASE_ADDRESS);

  MemorySize previous = base;
  unsigned long state = (sizeof(unsigned long) == 4) ? 0x67452301 : 0x67452301efcdab89ULL;

  MemorySize offset = trace.getSize(); // low bits only
  while (offset) { // likely only 1 iteration
    updateHash(state, static_cast<uint8>(offset & 0xff));
    offset >>= 8;
  }

  for (MemorySize i = 0; i < trace.getSize(); ++i) {
    auto v = trace[i];
    const auto address = reinterpret_cast<MemorySize>(v); // high are likely 0
    auto offset = (address >= previous) ? (address - previous) : (previous - address); // get the lower order bits that matter
    previous = address;
    while (offset) { // likely <=3 iterations
      updateHash(state, static_cast<uint8>(offset & 0xff));
      offset >>= 8;
    }
  }
  return state;
}

MemoryDiff StackFrame::find(const void* address) const noexcept
{
  const void* const* src = frames.getElements();
  for (MemorySize i = 0; i < frames.getSize(); ++i) {
    const void* ip = src[i];
    const void* symbol = DynamicLinker::getSymbolAddress(ip);
    if (symbol == address) {
      return i; // found
    }
  }
  return -1;
}

MemoryDiff StackFrame::findLast(const void* address) const noexcept
{
  const void* const* src = frames.getElements();
  for (MemoryDiff i = frames.getSize() - 1; i >= 0; --i) {
    const void* ip = src[i];
    const void* symbol = DynamicLinker::getSymbolAddress(ip);
    if (symbol == address) {
      return i; // found
    }
  }
  return -1;
}

void StackFrame::stripUntil(MemorySize index) noexcept
{
  if (index > 0) {
    const MemorySize size = frames.getSize();
    if (index >= size) {
      frames.setSize(0);
      return;
    }
    const void** dest = frames.getElements();
    move(dest, dest + index, size - index);
    frames.setSize(size - index);
  }
}

MemorySize StackFrame::stripUntil(const void* address) noexcept
{
  MemoryDiff index = find(address);
  if (index > 0) { // found
    stripUntil(index);
  }
  return index;
}

void* StackFrame::getStackFrame() noexcept
{
  void** frame = nullptr;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return nullptr; // not available for x64
#elif ((_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64) && \
       ((_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM) || \
        (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC)))
  asm("mov %%rbp, %0" : "=rm" ( frame ));
#elif ((_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86) && \
       ((_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM) || \
        (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC)))
  asm (
    "movl %%ebp,%0;\n"
    : "=m" (frame) // output
  );
#else
  // do not use assert due to recursion BASSERT(!"Not implemented");
  return nullptr;
#endif
  if (frame) {
    frame = reinterpret_cast<void**>(*frame); // get parent frame
  }
  return frame;
}

namespace {

  /** Returns the instruction pointer (return address) for the given stack frame. */
  inline void* getInstructionPointer(void* frame) noexcept
  {
    if (!frame) {
      return nullptr;
    }
    void* ip = reinterpret_cast<void**>(frame)[1]; // TAG: handle all ABIs
    return ip;
  }
}

void* StackFrame::getCaller() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return _ReturnAddress();
#else
  void* frame = getStackFrame();
  return getInstructionPointer(frame);
#endif
}

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

namespace {

  typedef USHORT (__stdcall *RtlCaptureStackBackTraceFunc)(ULONG FramesToSkip, ULONG FramesToCapture, PVOID* BackTrace, PULONG BackTraceHash);

  bool initialized = false;
  HMODULE handle = nullptr;
  RtlCaptureStackBackTraceFunc rtlCaptureStackBackTrace = nullptr;

  void loadNtDll()
  {
    if (!initialized) {
      initialized = true;
      handle = ::LoadLibraryExW(L"ntdll.dll", 0, 0);
      if (handle) {
        rtlCaptureStackBackTrace = (RtlCaptureStackBackTraceFunc)GetProcAddress(handle, "RtlCaptureStackBackTrace");
      }
    }
  }
}

#endif

#if 0
namaespace {
  
  bool VirtualMemory::getInfo(void*) noexcept
  {
    return true; // TAG:
  }
}
#endif

unsigned int StackFrame::getStack(const void** dest, unsigned int size, unsigned int skip, bool trim)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  USHORT count = RtlCaptureStackBackTrace(skip + 1, size, const_cast<void**>(dest), NULL);
  while ((count > 0) && (reinterpret_cast<MemorySize>(dest[count - 1]) < 0x10000)) {
    --count; // remove bad text seg pointer
  }
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
       (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX))
  int count = backtrace(const_cast<void**>(dest), size);
  if (count < 0) {
    return 0;
  }
  ++skip;
  if (skip >= count) {
    return 0;
  }
  auto write = dest;
  auto src = dest + skip;
  auto end = dest + count;
  while (src != end) { // remove bad text seg pointer
    if ((reinterpret_cast<MemorySize>(*src) >= 0x10000) /*&& !(reinterpret_cast<MemorySize>(*src) >> (12*4-1))*/) {
      *write++ = *src;
    }
    ++src;
  }
  count = static_cast<unsigned int>(write - dest);
  // we cannot detect overflow due to skip
#else
  void* frame = getStackFrame();
  // ++skip;
  while (frame && skip--) {
    frame = *reinterpret_cast<void**>(frame);
  }
  unsigned int count = 0;
  while (frame && (count < size)) {
    ++count;
    void* ip = getInstructionPointer(frame);
    void* symbol = DynamicLinker::getSymbolAddress(ip); // TAG: can we avoid this
    if (!symbol) {
      break;
    }
    // use VirtualMemory::EXECUTE
    if (reinterpret_cast<MemorySize>(ip) < 0x10000) { // TAG: what is the proper way to detect top of stack
      break;
    }
    *dest++ = ip;
    frame = *reinterpret_cast<void**>(frame);
  }
#endif
  
  if (trim && (count > 0)) {
    // trim to &Thread::entry, &main and similar roots
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) // TAG: add support for all platforms
    void* framworkModule = DynamicLinker::getBaseFrameworkImage();
    void* processModule = DynamicLinker::getProcessImage();
    while (count > 0) {
      const void* base = DynamicLinker::getImageAddress(dest[count - 1]);
      if ((base == framworkModule) || (base == processModule)) {
        break;
      }
      --count;
    }
#endif
  }
  
  return count;
}

StackFrame StackFrame::getStack(unsigned int skip, unsigned int levels, bool trim)
{
  StackFrame frames;

  if (levels == 0) {
    return frames;
  }

  ++skip;
  
  PrimitiveStackArray<const void*> buffer(256);
  unsigned int count = 0;
  while (buffer.size() < (64 * 1024)) {
    count = getStack(buffer, static_cast<unsigned int>(minimum<MemorySize>(levels, buffer.size())), skip, trim);
    if ((count == buffer.size()) && (count < levels)) { // overflow
      buffer.resize(buffer.size() * 2);
      continue;
    }
    break;
  }

  frames.frames.setSize(count);
  copy(frames.frames.getElements(), static_cast<const void**>(buffer), count);

  return frames;
}

void StackFrame::toStream(FormatOutputStream& stream, const ConstSpan<const void*>& _trace, unsigned int flags)
{
  const bool showAddress = (flags & FLAG_SHOW_ADDRESS) != 0;
  const bool useColors = (flags & FLAG_USE_COLORS) != 0;
  const unsigned int INDENT = ((flags & FLAG_INDENT) != 0) ? 2 : 0;

  ConstSpan<const void*> trace = _trace;
  if (flags & FLAG_TRIM_SYSTEM) { // TAG: remove FLAG_TRIM_SYSTEM below when ready
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) // TAG: add support for all platforms
    void* framworkModule = DynamicLinker::getBaseFrameworkImage();
    void* processModule = DynamicLinker::getProcessImage();
    for (MemoryDiff i = trace.getSize() - 1; i >= 0; --i) {
      const void* base = DynamicLinker::getImageAddress(trace[i]);
      if ((base == framworkModule) || (base == processModule)) {
        trace = ConstSpan<const void*>(_trace.begin(), i + 1);
        break;
      }
    }
#endif
  }

  int threadId = -1;
  if (auto tlc = Thread::getLocalContext()) {
    threadId = tlc->simpleId;
  }
  
  if (useColors) {
    stream << "Stack trace: THREAD=" << bold() << setForeground(ANSIEscapeSequence::BLUE)
           << Thread::getThreadName() << normal() << " [ID=" << threadId << "]" << EOL;
  } else {
    stream << "Stack trace: THREAD=" << Thread::getThreadName() << " [ID=" << threadId << "]" << EOL;
  }
  if (!trace || (trace.getSize() == 0)) {
    stream << indent(INDENT) << "NO STACK FRAMES" << EOL;
    return;
  }

  unsigned int field1 = 1;
  if (trace.getSize() >= 100) {
    field1 = 3;
  } else if (trace.getSize() >= 10) {
    field1 = 2;
  }

  unsigned int field2 = 8; // never less than 8
  if (sizeof(MemorySize) > 4) {
    MemorySize bits = 0;
    for (MemorySize i = 0; i < trace.getSize(); ++i) {
      MemorySize ip = reinterpret_cast<MemorySize>(trace[i]); // technically we should resolve symbol - but not hard done
      bits |= ip;
    }
    unsigned int digits = 0;
    while (bits) {
      bits >>= 4;
      ++digits;
    }
    field2 = maximum(digits, field2);
  }
  field2 += 2; // prefix

  String lastAddress;
  MemorySize count = 0;
  void* imageAddress = nullptr;
  String imagePath;

  for (MemorySize i = 0; i < trace.getSize(); ++i) {
    const void* ip = trace[i];
    const DynamicLinker::SymbolInfo info = DynamicLinker::getSymbolInfo(ip);

    if (flags & FLAG_SHOW_MODULE) {
      if (info.imageAddress != imageAddress) {
        imageAddress = info.imageAddress;
        imagePath = DynamicLinker::getImagePath(info.imageAddress);
        if ((flags & FLAG_FULL_PATH) == 0) {
          imagePath = FileSystem::getComponent(imagePath, FileSystem::FILENAME);
        }
      }
    }

    if (info) {
      stream << indent(INDENT) << setWidth(field1) << count << ": ";
      auto displacement = (reinterpret_cast<const uint8*>(ip) - reinterpret_cast<const uint8*>(info.address));
      if (showAddress) {
        StringOutputStream sos;
        sos << setWidth(field2) << info.address << "+" << HEX << ZEROPAD << NOPREFIX << setWidth(4) << displacement;
        const String address = sos.toString();
        if (useColors) {
          MemorySize j = 0; // find first difference for new address
          for (; j < minimum(address.getLength(), lastAddress.getLength()); ++j) {
            if (address[j] != lastAddress[j]) {
              break;
            }
          }
          lastAddress = address;
          const bool useDim = false && !WINDOWS;
          if (useDim) {
            stream << dim();
          }
          stream << setForeground(ANSIEscapeSequence::RED) << address.substring(0, j) /*<< '.'*/ << normal();
          if (!useDim) {
            stream << underscore();
          }
          stream << setForeground(ANSIEscapeSequence::RED) << address.substring(j)
                 << normal();
        } else {
          stream << address;
        }
      }
      if (info.name) {
        if (showAddress) {
          stream << " ";
        }
        String demangled = TypeInfo::demangleName(info.name.native());
        const bool stripNamespace = flags & FLAG_STRIP_NAMESPACE; // this will compact output quite a bit
        if (stripNamespace) {
          demangled.replaceAll("base::", String()); // or std:: // TAG: we should check char just before to make sure this is a namespace
          // TAG: we can add a register of namespace macro like register of test to support stripping of any namespace
        }
        if (useColors) {
          if (imagePath) {
            stream << setForeground(ANSIEscapeSequence::GREEN) << imagePath << normal() << "!";
          }
          auto index = demangled.indexOf('(');
          if (index >= 0) {
            stream << setForeground(ANSIEscapeSequence::BLUE) << bold() << demangled.substring(0, index) << normal()
                   << setForeground(ANSIEscapeSequence::BLUE) << italic() << demangled.substring(index);
          } else {
            stream << setForeground(ANSIEscapeSequence::BLUE) << bold() << demangled;
          }
          stream << normal();
        } else {
          if (imagePath) {
            stream << imagePath << "!" << demangled;
          } else {
            stream << demangled;
          }
        }

        if (flags & FLAG_TRIM_SYSTEM) {

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
          if ((demangled == MESSAGE("BaseThreadInitThunk")) || (demangled == MESSAGE("CtrlRoutine"))) {
            stream << EOL;
            break;
          }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
          if (demangled == "_pthread_start") {
            stream << EOL;
            break;
          }
#endif
          if (demangled == "main") { // TAG: add support for all platforms - better to backtrim when we get stack trace - also handle threads
            stream << EOL;
            break;
          }
        }
      }
      stream << EOL;
    } else { // no descrption
      if ((flags & FLAG_COMPACT) == 0) {
        stream << indent(INDENT) << setWidth(field1) << count << ": ";
        if (useColors) {
          stream << setForeground(ANSIEscapeSequence::RED) << setWidth(field2) << ip << normal();
        } else {
          stream << setWidth(field2) << ip;
        }
        stream << "?"; // TAG: we could still show image name
        stream << EOL;
      }
    }
    ++count;
  }
}

void StackFrame::dump(unsigned int skip, unsigned int levels)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return;
#endif

  if (levels == 0) {
    return;
  }
  const bool colors = FileDescriptor::getStandardError().isANSITerminal(); // use ferr
  
  ++skip;
  const unsigned int flags = StackFrame::FLAG_SHOW_ADDRESS | StackFrame::FLAG_SHOW_MODULE | StackFrame::FLAG_INDENT |
    (colors ? StackFrame::FLAG_USE_COLORS : 0);

  PrimitiveStackArray<const void*> buffer(256);
  MemorySize count = 0;
  while (buffer.size() < (64 * 1024)) {
    count = getStack(buffer, static_cast<unsigned int>(minimum<MemorySize>(levels, buffer.size())), skip);
    if ((count == buffer.size()) && (count < levels)) { // overflow
      buffer.resize(buffer.size() * 2);
      continue;
    }
    break;
  }

  toStream(*(useStandardOut ? &fout : &ferr), ConstSpan<const void*>(buffer, count), flags);
  ferr << FLUSH;
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const StackFrame& value)
{
  const bool colors = false; // FileDescriptor::getStandardError().isANSITerminal(); // stream may not be stderr
  StackFrame::toStream(
    stream, value.getTrace(),
    StackFrame::FLAG_DEFAULT | (colors ? StackFrame::FLAG_USE_COLORS : 0)
  );
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
