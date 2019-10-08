/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/Primitives.h>
#include <base/mem/Allocator.h>
#include <base/io/File.h>
#include <base/io/async/AsynchronousStream.h>

using namespace dk::sdu::mip::base;

class AsyncTransfer : public AsynchronousReadEventListener,
                      public AsynchronousWriteEventListener {
private:

  bool stop;
  bool eof;
  unsigned int bufferSize;
  unsigned int bytesRead;
public:

  void asynchronousCompletion(
    const AsynchronousReadCompletion& completion) throw() {
    bytesRead = completion.getBytesRead();
    if (completion.successful()) {
      fout << "Read completed:" << EOL
           << "  bytesRead=" << completion.getBytesRead() << EOL
           << "  offset=" << completion.getOffset() << ENDL;
    } else {
      if (completion.eof()) {
        fout << "Read operation reached end of file." << ENDL;
        eof = true;
      } else if (completion.wasAborted()) {
        fout << "Read operation was aborted." << ENDL;
      } else {
        ferr << "Read operation failed." << ENDL;
        stop = true;
      }
    }
  }
  
  void asynchronousCompletion(
    const AsynchronousWriteCompletion& completion) throw() {
    if (completion.successful()) {
      fout << "Write completed:" << EOL
           << "  bytesWritten=" << completion.getBytesWritten() << EOL
           << "  offset=" << completion.getOffset() << ENDL;
    } else {
      if (completion.wasAborted()) {
        fout << "Write operation was aborted." << ENDL;
      } else {
        fout << "Write operation failed." << ENDL;
        stop = true;
      }
    }
  }
  
  AsyncTransfer(const String& path) throw()
    : stop(false),
      eof(false),
      bufferSize(4096 * 64 * 8),
      bytesRead(0) {
    fout << "Testing asynchronous reading from file..." << ENDL;
    try {
      File file(path, File::READ, 0);
      Allocator<uint8> buffer(bufferSize);
      unsigned long long offset = 0;
      while (!eof && !stop) {
        try {
          file.read(buffer.getElements(), buffer.getSize(), offset, this);
        } catch (IOException&) {
          fout << "Unable to read from file." << ENDL;
          file.asyncCancel();
          Application::getApplication()->setExitCode(Application::EXIT_CODE_ERROR);
          return;
        }
        offset += buffer.getSize();
        unsigned int counter = 0;
        if (eof) break;
        while (true) {
          if (AsynchronousStream::asyncTest()) break;
          ++counter;
        }
        fout << "Read operation completed with: counter=" << counter << ENDL;
      }
    } catch (IOException&) {
      ferr << "Unable to open file." << ENDL;
      Application::getApplication()->setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
  }

  AsyncTransfer(const String& sourcePath, const String& destPath) throw()
    : stop(false),
      eof(false),
      bufferSize(4096 * 64 * 8),
      bytesRead(0) {
    fout << "Testing asynchronous reading from and writing to file..." << ENDL;

    File sourceFile;
    File destFile;
    try {
      sourceFile = File(sourcePath, File::READ, File::ASYNCHRONOUS);
    } catch (IOException&) {
      ferr << "Unable to open source file." << ENDL;
      Application::getApplication()->setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
    try {
      destFile = File(
        destPath,
        File::WRITE,
        File::CREATE | File::EXCLUSIVE | File::ASYNCHRONOUS
      );
    } catch (IOException&) {
      ferr << "Unable to create destination file (make sure it does not exist)." << ENDL;
      Application::getApplication()->setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }

    Allocator<uint8> buffer(bufferSize);
    unsigned long long offset = 0;
    while (!eof && !stop) {
      AsynchronousReadOperation readOperation;
      AsynchronousWriteOperation writeOperation;
      try {
        readOperation = sourceFile.read(
          buffer.getElements(),
          buffer.getSize(),
          offset,
          this
        );
      } catch (IOException&) {
        ferr << "Unable to read from file." << ENDL;
        stop = true;
        Application::getApplication()->setExitCode(
          Application::EXIT_CODE_ERROR
        );
        return;
      }
      unsigned int counter = 0;
      while (!eof && !stop) {
        if (AsynchronousStream::asyncTest()) break;
        ++counter;
      }
      fout << "Read operation completed with: counter=" << counter << ENDL;
      try {
        writeOperation = destFile.write(buffer.getElements(), bytesRead, offset, this);
      } catch (IOException&) {
        ferr << "Unable to write to file." << ENDL;
        stop = true;
        Application::getApplication()->setExitCode(Application::EXIT_CODE_ERROR);
        return;
      }
      counter = 0;
      while (!stop) {
        if (AsynchronousStream::asyncTest()) break;
        ++counter;
      }
      fout << "Write operation completed with: counter=" << counter << ENDL;
      offset += buffer.getSize();
    }
    if (stop) {
      sourceFile.asyncCancel();
      destFile.asyncCancel();
      AsynchronousStream::asyncTest(); // wait forever!
    }
  }
};

class FileApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  FileApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw() 
    : Application("File", numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
    
    switch (arguments.getSize()) {
    case 0: // use default
      setExitCode(Application::EXIT_CODE_ERROR);
      ferr << "Usage: [file] or [source file] [output file]" << ENDL;
      return;
    case 1:
      {
        String sourcePath = arguments[0]; // file path
        AsyncTransfer asyncTransfer(sourcePath);
      }
      break;
    case 2:
      {
        String sourcePath = arguments[0]; // file path
        String destPath = arguments[1]; // file path
        AsyncTransfer(sourcePath, destPath);
      }
      break;
    default:
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
  }
};

STUB(FileApplication);
