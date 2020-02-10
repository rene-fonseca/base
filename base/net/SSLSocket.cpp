/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/SSLSocket.h>
#include <base/ResourceHandle.h>
#include <base/Module.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
//#  include <winsock2.h>
//#  include <mstcpip.h>
//#  include <ws2tcpip.h>
//#  undef interface
#endif

#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
#  include <openssl/bio.h>
#  include <openssl/ssl.h>
#  include <openssl/err.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
MODULE_REGISTER_EXPLICIT(_COM_AZURE_DEV__BASE__THIS_MODULE, "org.openssl", "OpenSSL", "", "https://www.openssl.org/");
#endif

// https ://docs.microsoft.com/en-us/windows/win32/winsock/winsock-secure-socket-extensions

namespace {

#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
  class InitializeSSL {
  public:
    
    InitializeSSL()
    {
      SSL_load_error_strings();
      SSL_library_init();
      OpenSSL_add_all_algorithms();
    }

    ~InitializeSSL()
    {
      ERR_free_strings();
      EVP_cleanup();
    }
  };
#endif
}

class SSLHandle : public ResourceHandle {
public:
  
#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
  SSL_CTX* sslctx = nullptr;
  SSL* cSSL = nullptr;
#endif

  inline bool isValid() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
    return cSSL;
#else
    return false;
#endif
  }

  void close()
  {
#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
    if (cSSL) {
      SSL_shutdown(cSSL);
      SSL_free(cSSL);
    }
#endif
  }
  
  ~SSLHandle()
  {
    close();
  }
};

namespace {

  // Fwpuclnt.lib
  // Fwpuclnt.dll

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  int queryTcpSocketSecurity(SOCKET sock)
  {
    ULONG infoLength = 0;
    int result = WSAQuerySocketSecurity(sock, NULL, NULL, NULL, &infoLength, NULL, NULL);
    SOCKET_SECURITY_QUERY_INFO* queryInfo = NULL;
    result = WSAQuerySocketSecurity(sock, NULL, NULL, queryInfo, &infoLength, NULL, NULL);
    if (result) {
    }
  }
#endif
}

SSLSocket::SSLSocket()
{
}

void SSLSocket::createSecure(StreamSocket _socket, const String& pem)
{
  if (handle) {
    _throw Exception();
  }

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SOCKET sock = socket.getHandle();
  SOCKET_SECURITY_SETTINGS* securitySettings = NULL;
  if (WSASetSocketSecurity(sock, securitySettings, 0 /*length*/, NULL, NULL) != 0) {
    int errorCode = WSAGetLastError();
    _throw NetworkException("Failed to convert socket to secure socket.");
  }

  if (WSAConnect(sock, serverAddr, serverAddrLen, NULL, NULL, NULL, NULL) != 0) {
    int errorCode = WSAGetLastError();
    _throw NetworkException("Failed to convert socket to secure socket.");
  }
#endif

#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
    static InitializeSSL initialize;
#endif

  Reference<SSLHandle> handle = new SSLHandle();
  
#if 0 && defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
  handle->sslctx = SSL_CTX_new(SSLv23_server_method());
  SSL_CTX_set_options(handle->sslctx, SSL_OP_SINGLE_DH_USE);
  int use_cert = SSL_CTX_use_certificate_file(handle->sslctx, pem.native(), SSL_FILETYPE_PEM);
  int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, pem.native(), SSL_FILETYPE_PEM);
  cSSL = SSL_new(handle->sslctx);
  SSL_set_fd(cSSL, _socket->getHandle()->getInternalHandle<xxx>()->getHandle());

  auto ssl_err = SSL_accept(cSSL);
  if (ssl_err <= 0) {
  }
#endif

  this->handle = handle;
}

void SSLSocket::connect(const InetAddress& address, unsigned short port)
{
  Reference<SSLHandle> handle = this->handle.castChecked<SSLHandle>();
  if (handle) {
    _throw NetworkException("SSL socket already connected.", this);
  }

  // Profiler::WaitTask profile("SSLSocket::connect()", socket);

#if 0 && defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
  SocketAddress sa(address, port, socket.getDomain());
  if (::SSL_connect(handle->cSSL, sa.getValue(), sa.getSize())) {
    switch (SSL_get_error()) {
    case ECONNREFUSED:
      _throw AccessDenied(this);
    case ETIMEDOUT:
      _throw TimedOut(this);
    default:
      _throw NetworkException("Unable to connect to SSL socket.", this);
    }
  }
#else
  socket.connect(address, port);
#endif
}

unsigned int SSLSocket::read(
  uint8* buffer,
  unsigned int bytesToRead,
  bool nonblocking)
{
  Reference<SSLHandle> handle = this->handle.castChecked<SSLHandle>();
  if (!handle) {
    _throw NetworkException("Invalid handle.", this);
  }

  // TAG: add profiling
#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
  int status = SSL_read(handle->cSSL, buffer, bytesToRead);
  if (status < 0) {
    _throw NetworkException("Failed to read from SSL socket.", this);
  }
  return status;
#else
  return socket.read(buffer, bytesToRead, nonblocking);
#endif
}

unsigned int SSLSocket::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  bool nonblocking)
{
  Reference<SSLHandle> handle = this->handle.castChecked<SSLHandle>();
  if (!handle) {
    _throw NetworkException("Invalid handle.", this);
  }
  
  // TAG: add profiling
#if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
  int status = SSL_write(handle->cSSL, buffer, bytesToWrite);
  if (status < 0) {
    _throw NetworkException("Failed to write to SSL socket.", this);
  }
  return status;
#else
  return socket.write(buffer, bytesToWrite, nonblocking);
#endif
}

SSLSocket::~SSLSocket()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
