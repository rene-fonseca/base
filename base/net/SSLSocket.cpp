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

SSLSocket::SSLSocket()
{
}

#if 0
void SSLSocket::createSSL(StreamSocket _socket, const String& pem)
{
  if (handle) {
    _throw Exception();
  }

  #if defined(_COM_AZURE_DEV__BASE__USE_OPENSSL)
    static InitializeSSL initialize;
  #endif

  Reference<SSLHandle> handle = new SSLHandle();
  
  handle->sslctx = SSL_CTX_new(SSLv23_server_method());
  SSL_CTX_set_options(handle->sslctx, SSL_OP_SINGLE_DH_USE);
  int use_cert = SSL_CTX_use_certificate_file(handle->sslctx, pem.native(), SSL_FILETYPE_PEM);
  int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, pem.native(), SSL_FILETYPE_PEM);
  cSSL = SSL_new(handle->sslctx);
  SSL_set_fd(cSSL, _socket->getHandle()->getInternalHandle<xxx>()->getHandle());

  auto ssl_err = SSL_accept(cSSL);
  if (ssl_err <= 0) {
    
  }
}
#endif

// SSL_read(cSSL, buffer, size);
// SSL_write(cSSL, buffer, size);

SSLSocket::~SSLSocket()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
