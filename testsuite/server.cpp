/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/net/ServerSocket.h>
#include <base/net/InetService.h>

void test() {
  fout << "Testing Client Socket...\n";

  InetAddress address("127.0.0.1");
  ServerSocket socket();

  fout.flush();
}

int main() {
  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << "\n";
  } catch(...) {
    ferr << "Unknown exception\n";
  }
}
