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
  getaddrinfo(NULL, NULL, &hint, &ai);
  freeaddrinfo(NULL);
  inet_pton(AF_INET6, NULL, NULL);
  inet_ntop(AF_INET6, NULL, NULL, 0);
}
