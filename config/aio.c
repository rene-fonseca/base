/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  Compiles if asynchronous io is supported.
*/

#define _XOPEN_SOURCE 600
#define _LARGEFILE64_SOURCE

#include <aio.h>
#include <string.h>

static const char MESSAGE[] = "Hello, World\n";
static char readBuffer[16];
static char writeBuffer[sizeof(MESSAGE)];

void completion(union sigval value) {
}

void compilation() {
  int result;
  ssize_t size;
  struct aiocb64 aio;
  const struct aiocb64* suspendOprs[1];
  struct aiocb64* oprs[1];
  struct timespec timeout;
  
  memset(&aio, 0, sizeof(aio));
  aio.aio_fildes = 1; // stdout
  aio.aio_offset = 0;
  aio.aio_buf = &readBuffer;
  aio.aio_nbytes = sizeof(readBuffer);
  aio.aio_reqprio = 0;
  aio.aio_sigevent.sigev_notify = SIGEV_THREAD;
  aio.aio_sigevent.sigev_value.sival_ptr = &aio;
  aio.aio_sigevent.sigev_notify_function = completion;
  
  result = aio_error64(&aio);
  result = aio_read64(&aio);
  size = aio_return64(&aio);
  
  timeout.tv_sec = 0;
  timeout.tv_nsec = 1;
  suspendOprs[0] = &aio;
  result = aio_suspend64(suspendOprs, sizeof(oprs), &timeout);
  
  result = aio_write64(&aio);
  
  result = aio_fsync64(O_DSYNC, &aio); // or O_SYNC
  
  memcpy(writeBuffer, MESSAGE, sizeof(MESSAGE));
  aio.aio_fildes = 1; // stdout
  aio.aio_offset = 0;
  aio.aio_buf = writeBuffer;
  aio.aio_nbytes = sizeof(writeBuffer);
  aio.aio_reqprio = 0;
  aio.aio_sigevent.sigev_notify = SIGEV_THREAD;
  aio.aio_sigevent.sigev_value.sival_ptr = &aio;
  aio.aio_sigevent.sigev_notify_function = completion;
  aio.aio_lio_opcode = LIO_WRITE; // LIO_WRITE, LIO_READ, LIO_NOP
  oprs[0] = &aio;
  result = lio_listio64(LIO_NOWAIT, // LIO_NOWAIT, LIO_WAIT
			oprs,
			sizeof(oprs),
			0); // struct sigevent *restrict
  
  result = aio_cancel64(aio.aio_fildes, &aio);
}
