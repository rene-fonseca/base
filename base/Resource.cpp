/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Resource.h>
#include <base/ResourceHandle.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const String& Resource::getDescription() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->description;
}

void Resource::setDescription(const String& description)
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  handle->description = description;
}

unsigned int Resource::getResourceId() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->resourceId;
}

unsigned int Resource::getCreatedById() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->createdById;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
