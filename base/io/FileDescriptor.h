/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_FILE_DESCRIPTOR_H
#define _DK_SDU_MIP_BASE_IO_FILE_DESCRIPTOR_H

/**
  File descriptor.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class FileDescriptor {
protected:

  int handle;
public:

  FileDescriptor(int handle);

  unsigned int read(char* buffer, unsigned int count);

  ~FileDescriptor();
};

#endif
