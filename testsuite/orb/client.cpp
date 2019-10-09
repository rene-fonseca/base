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
#include <base/rmi/Orb.h>
#include <base/rmi/LisaEncoding.h>
#include <base/rmi/LocalScheme.h>
#include <base/rmi/TCPIPProtocol.h>
#include <base/rmi/OrbObject.h>
#include <base/rmi/OrbStub.h>
#include <base/rmi/OrbSkeleton.h>
#include <base/concurrency/Thread.h>

#include <base/rmi/OrbRequest.h>

using namespace com::azure::dev::base;

class ClientApplication : public Application, public Runnable {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  Orb orb;
public:

  ClientApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("client", numberOfArguments, arguments, environment) {
  }

  class DateInterface {
  public:
    
    virtual int getDate() const throw(OrbException) = 0;
  };

  class DateSkeleton;
  
  class DateStub : public OrbStub, public DateInterface {
  public:
    
    DateStub(
      ProtectedPointer<Orb> orb,
      Reference<OrbConnection> connection,
      Reference<OrbDecoder> decoder,
      Reference<OrbEncoder> encoder,
      const OrbReference& reference) throw()
      : OrbStub(orb, connection, decoder, encoder, reference) {
      
//       DateSkeleton* result = dynamic_cast<DateSkeleton*>(
//         Cast::getPointer<OrbSkeleton*>(reference.getId())
//       );
//       bassert(result, CastException(this));
    }
    
    int getDate() const throw(OrbException) {
//       const DateInterface* self =
//         Cast::getPointer<const DateInterface*>(reference.getId());

      int methodId = getMethodId("getDate");
      bassert(
        methodId >= 0,
        bindCause(OrbException("No such method"), OrbException::INVALID_METHOD)
      );
      
      // marshaling
      
      DateSkeleton* skeleton = Cast::getPointer<DateSkeleton*>(
        reference.getId()
      );
      bassert(skeleton, NullPointer(this));
      // bassert(skeleton, CastException(this));
      return skeleton->getDate();
    }
  };

  class DateSkeleton : public OrbSkeleton, public DateInterface {
  public:
    
    // association with 
    
    inline DateSkeleton() throw(OrbException)
      : OrbSkeleton("Date", 1) {
      registerMethod("getDate");
    }

    void unmarshal(int methodId) throw() {
      switch (methodId) {
      case 0:
        {
          // TAG: unmarshal parameters
          int result = getDate();
          // return result
        }
        break;
      default:
        // return OrbException... INVALID_METHOD
        break;
      }
    }
  };

  class DateServlet : public DateSkeleton {
  private:

    mutable int value;
  public:
    
    DateServlet() throw() : value(1234) {
    }

    int getDate() const throw() {
      return value++;
    }
  };
  
  void run() throw() {
    orb.run();
  }

  void dump() throw() {
    fout << "ORB:" << EOL
         << indent(2) << "number of encoding schemes: " << orb.getNumberOfEncodings() << EOL
         << indent(2) << "encoding schemes: " << orb.getEncodings() << EOL
         << indent(2) << "number of schemes: " << orb.getNumberOfSchemes() << EOL
         << indent(2) << "schemes: " << orb.getSchemes() << EOL
         << indent(2) << "number of factories: " << orb.getNumberOfFactories() << EOL
         << indent(2) << "skeletons: " << orb.getNumberOfSkeletons() << EOL
         << indent(2) << "stubs: " << orb.getNumberOfStubs() << EOL
         << indent(2) << "connections: " << orb.getNumberOfConnections() << EOL
         << ENDL;
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    orb.registerEncoding(new LisaEncoding(), Orb::OUTGOING|Orb::INCOMING);
    orb.registerScheme(new LocalScheme());
    
    orb.registerSkeleton("Date", new DateServlet());
    
    dump();
    
    Reference<DateStub> date = orb.getObject<DateStub>("local:///Date");
    ferr << "stub.getDate(): " << date->getDate() << ENDL;
    
//     Thread thread(this);
//     thread.start();
//     thread.join();
  }

  // TAG: add terminate
  
  virtual ~ClientApplication() throw() {
  }
};

STUB(ClientApplication);
