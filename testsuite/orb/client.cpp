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

  ClientApplication() noexcept
    : Application("client") {
  }

  class DateInterface {
  public:
    
    virtual int getDate() const = 0;
  };

  class DateSkeleton;
  
  class DateStub : public OrbStub, public DateInterface {
  public:
    
    DateStub(
      ProtectedPointer<Orb> orb,
      Reference<OrbConnection> connection,
      Reference<OrbDecoder> decoder,
      Reference<OrbEncoder> encoder,
      const OrbReference& reference) noexcept
      : OrbStub(orb, connection, decoder, encoder, reference) {
      
//       DateSkeleton* result = dynamic_cast<DateSkeleton*>(
//         Cast::getPointer<OrbSkeleton*>(reference.getId())
//       );
//       bassert(result, CastException(this));
    }
    
    int getDate() const {
//       const DateInterface* self =
//         Cast::getPointer<const DateInterface*>(reference.getId());

      int methodId = getMethodId("getDate");
      bassert(
        methodId >= 0,
        OrbException("No such method.").setCause(OrbException::INVALID_METHOD)
      );
      
      // marshaling
      
      DateSkeleton* skeleton = Cast::getPointer<DateSkeleton*>(
        reference.getId()
      );
      if (!skeleton) {
        _throw NullPointer(this);
      }
      // bassert(skeleton, CastException(this));
      return skeleton->getDate();
    }
  };

  class DateSkeleton : public OrbSkeleton, public DateInterface {
  public:
    
    // association with 
    
    inline DateSkeleton()
      : OrbSkeleton("Date", 1) {
      registerMethod("getDate");
    }

    void unmarshal(int methodId) noexcept {
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

    mutable int value = 1234;
  public:
    
    DateServlet() noexcept
    {
    }

    int getDate() const noexcept
    {
      return value++;
    }
  };
  
  void run() noexcept
  {
    orb.run();
  }

  void dump() noexcept
  {
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
  
  void main() noexcept
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
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
  
  virtual ~ClientApplication() noexcept {
  }
};

STUB(ClientApplication);
