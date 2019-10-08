/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  Compiles and runs if the interface extensions for IP version 6 are supported
  by the platform.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
  struct in6_addr ia;
  struct sockaddr_in6 sa;
  struct addrinfo hint;
  struct addrinfo* ai;
  char buffer[INET6_ADDRSTRLEN];
  int handle;

  getaddrinfo("localhost", 0, 0, &ai);
  freeaddrinfo(ai);
  inet_pton(AF_INET6, "::", &ia);
  inet_ntop(AF_INET6, &ia, buffer, sizeof(buffer));
  handle = socket(PF_INET6, SOCK_STREAM, 0);
  if (handle == -1) {
    return 1;
  }
  return 0;
}
