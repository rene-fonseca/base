/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ServerSocket.h"

ServerSocket::ServerSocket() throw(IOException) :
  Socket() {
}

ServerSocket::ServerSocket(const InetAddress& addr, unsigned short port) throw(IOException) :
  Socket() {
}

ServerSocket::~ServerSocket() {
}
