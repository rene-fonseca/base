/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/ApplicationSynchronize.h>
#include <base/Application.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ApplicationSynchronize::ApplicationSynchronize()
{
  auto app = Application::getApplication();
  if (app) { // ok during initialization of main thread
    app->getLock().exclusiveLock();
  }
}

ApplicationSynchronize::~ApplicationSynchronize()
{
  auto app = Application::getApplication();
  if (app) {
    app->getLock().releaseLock();
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
