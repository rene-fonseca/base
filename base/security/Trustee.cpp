/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/security/Trustee.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/NotImplemented.h>

// TAG: need static method to set id space
// TAG: space is used for mapping id's into short/local integral id's
// TAG: void Trustee::setDomain(const Trustee& trustee) throw();
// TAG: Trustee Trustee::getDomain() throw();
// TAG: at init call Trustee::setDomain(User::getCurrentUser());

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  include <aclapi.h>
#  include <aclapi.h>
#  include <lm.h>

#  if (!defined(SID_MAX_SUB_AUTHORITIES))
#    define SID_MAX_SUB_AUTHORITIES 15
#  endif
#  if (!defined(SECURITY_MAX_SID_SIZE))
#    define SECURITY_MAX_SID_SIZE (sizeof(SID) - sizeof(DWORD) + (SID_MAX_SUB_AUTHORITIES * sizeof(DWORD)))
#  endif
#else // unix
#  include <sys/types.h>
#  include <grp.h>
#  include <pwd.h>
#  include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Trustee::Trustee() throw()
  : type(UNSPECIFIED),
    integralId(PrimitiveTraits<unsigned long>::MAXIMUM) {
}

Trustee::Trustee(User user) throw() : type(USER) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  integralId = 0;
  if (!user.isValid()) {
    type = UNSPECIFIED;
    return;
  }
  id = user.id;
#else // unix
  integralId = user.getIntegralId();
#endif // flavor
}

Trustee::Trustee(Group group) throw() : type(GROUP) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  integralId = 0;
  if (!group.isValid()) {
    type = UNSPECIFIED;
    return;
  }
  id = group.id;
#else // unix
  integralId = group.getIntegralId();
#endif // flavor
}

Trustee::Trustee(TrusteeType type, const void* _id) throw(OutOfDomain) {
  if (_id == 0) {
    type = Trustee::UNSPECIFIED;
    integralId = PrimitiveTraits<unsigned long>::MAXIMUM;
    return;
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::IsValidSid((PSID)_id) != 0, OutOfDomain("Invalid trustee", this));
  unsigned int size = ::GetLengthSid((PSID)_id);
  integralId = 0;
  id = new ReferenceCountedAllocator<uint8>(size);
  copy(id->getElements(), Cast::pointer<const uint8*>(_id), size);
#else // unix
  assert(
    (type == Trustee::USER) ||
    (type == Trustee::GROUP) ||
    (type == Trustee::EVERYONE),
    OutOfDomain("Invalid trustee", this)
  );
//   assert(
//     ((type == Trustee::USER) ||
//      (type == Trustee::GROUP) ||
//      (type == Trustee::EVERYONE)) &&
//     ((unsigned long)_id <= PrimitiveTraits<uid_t>::MAXIMUM),
//     OutOfDomain("Invalid trustee", this)
//   );
  integralId = (unsigned long)_id;
#endif // flavor
}

Trustee::Trustee(const Trustee& copy) throw()
  : type(copy.type), integralId(copy.integralId), id(copy.id) {
}

Trustee& Trustee::operator=(const Trustee& eq) throw() {
  type = eq.type;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  id = eq.id; // dont care about integralId
#else // unix
  integralId = eq.integralId; // dont care about id
#endif // flavor
  return *this;
}

bool Trustee::operator==(const Trustee& eq) const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!((id.isValid()) && (eq.id.isValid()))) {
    return !id.isValid() && !eq.id.isValid();
  }
  return (id->getSize() == eq.id->getSize()) &&
    (compare(id->getElements(), eq.id->getElements(), id->getSize()) == 0);
#else // unix
  // id attribute is dont-care
  if (Constraint<sizeof(uid_t) == sizeof(gid_t)>::UNSPECIFIED);
  return Cast::extract<uid_t>(integralId) ==
    Cast::extract<uid_t>(eq.integralId);
#endif
}

Trustee::Trustee(const String& name) throw(TrusteeException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SID_NAME_USE sidType;
  uint8 sid[SECURITY_MAX_SID_SIZE];
  DWORD size = sizeof(sid);
  assert(::LookupAccountName(0,
                             name.getElements(),
                             &sid,
                             &size,
                             0,
                             0,
                             &sidType) != 0,
         TrusteeException("Unable to lookup name", this)
  );
  assert(sidType != SidTypeInvalid, TrusteeException("Not a trustee", this));
  unsigned int sidSize = ::GetLengthSid((PSID)sid);
  integralId = getMaximum(integralId);
  id = new ReferenceCountedAllocator<uint8>(sidSize);
  copy(id->getElements(), Cast::pointer<const uint8*>(sid), sidSize);
#else // unix
  id = 0;
  //long sysconf(_SC_GETPW_R_SIZE_MAX);
  //long sysconf(_SC_GETGR_R_SIZE_MAX);
  char buffer[4096];
  struct passwd pw;
  struct passwd* entry;
  int result = ::getpwnam_r(
    name.getElements(),
    &pw,
    buffer,
    sizeof(buffer),
    &entry
  );
  if (result == 0) {
    type = Trustee::USER;
    integralId = entry->pw_uid;
  } else {
  #if defined(_DK_SDU_MIP__BASE__HAVE_GETGRNAM_R)
    struct group grp;
    struct group* entry;
    int result = ::getgrnam_r(
      name.getElements(),
      &grp,
      buffer,
      sizeof(buffer),
      &entry
    );
  #else
    #warning Trustee::Trustee(const String& name) uses non-reentrant getgrnam
    struct group* entry = ::getgrnam(name.getElements());
    int result = entry ? 0 : -1;
  #endif
    if (result == 0) {
      type = Trustee::GROUP;
      integralId = entry->gr_gid;
    } else {
      if (name == "other") {
        type = Trustee::EVERYONE;
        integralId = 0;
      } else {
        throw TrusteeException("Not a trustee", this);
      }
    }
  }
#endif // flavor
}

bool Trustee::isInitialized() const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return id.isValid();
#else // unix
  return integralId == PrimitiveTraits<unsigned long>::MAXIMUM;
#endif // flavor
}

Trustee::TrusteeType Trustee::getType() const throw(TrusteeException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(id.isValid(), TrusteeException("Invalid", this));
  if (type == UNSPECIFIED) {
    DWORD nameSize = 0;
    DWORD domainNameSize = 0;
    SID_NAME_USE sidType = SidTypeInvalid;
    assert(::LookupAccountSid(0,
                              (PSID)id->getElements(), // must be valid
                              0,
                              &nameSize,
                              0,
                              &domainNameSize,
                              &sidType) == ERROR_INSUFFICIENT_BUFFER,
           TrusteeException("Unable to lookup name", this)
    );
    type = ((sidType == SidTypeGroup) || (sidType == SidTypeWellKnownGroup)) ? Trustee::GROUP : Trustee::USER;
  }
  return type;
#else // unix
  ASSERT((type == Trustee::USER) || (type == Trustee::GROUP) || (type == Trustee::EVERYONE));
  return type;
#endif // flavor
}

bool Trustee::isMemberOf(const Trustee& trustee) const throw(TrusteeException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!id.isValid()) {
    return false;
  }
  if (type == UNSPECIFIED) {
    DWORD nameSize = 0;
    DWORD domainNameSize = 0;
    SID_NAME_USE sidType = SidTypeInvalid;
    ::LookupAccountSid(0, (PSID)id->getElements(), // must be valid
                       0, &nameSize,
                       0, &domainNameSize,
                       &sidType); // does function set sidType even when it fails
    // expected error: ERROR_INSUFFICIENT_BUFFER
    ASSERT(sidType != SidTypeInvalid);
    type = ((sidType == SidTypeGroup) || (sidType == SidTypeWellKnownGroup)) ? Trustee::GROUP : Trustee::USER;
  }
  if (type != Trustee::GROUP) {
    return false;
  }
#else // unix
  ASSERT((type == Trustee::USER) || (type == Trustee::GROUP) || (type == Trustee::EVERYONE));
  if (type == Trustee::EVERYONE) {
    return true;
  }
#endif // flavor
  if (type == Trustee::GROUP) {
    // check if member
    return true; // TAG: implement me
    #warning bool Trustee::isMemberOf(const Trustee& trustee) const throw(TrusteeException) not implemented
  }
  return false;
}

// TAG: select full name domain/user with option: LOCAL prefix?, BUILTIN prefix (no)?
String Trustee::getName() const throw(TrusteeException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!id.isValid()) {
    return Literal("<unknown>");
  }
  SID_NAME_USE sidType;
  char name[UNLEN+1];
  DWORD nameSize = sizeof(name);
  char domainName[DNLEN+1];
  DWORD domainNameSize = sizeof(domainName);
  assert(::LookupAccountSid(0, (PSID)id->getElements(), // must be valid
                            name, &nameSize,
                            domainName, &domainNameSize,
                            &sidType) != 0,
         TrusteeException("Unable to lookup name", this)
  );
  if (domainName[0] != 0) {
    return String(domainName) + MESSAGE("\\") + String(name);
  } else {
    return String(name); // TAG: does nameSize hold length of name
  }
#else // unix

  switch (type) {
  case Trustee::GROUP:
    {
#if defined(_DK_SDU_MIP__BASE__HAVE_GETGRNAM_R)
      //long sysconf(_SC_GETGR_R_SIZE_MAX);
      Allocator<char>* buffer = Thread::getLocalStorage();
      struct group grp;
      struct group* entry;
      int result = ::getgrgid_r(
        (gid_t)integralId,
        &grp,
        buffer->getElements(),
        buffer->getSize(),
        &entry
      );
      assert(result == 0, TrusteeException(this));
      return String(entry->gr_name);
#else
      //long sysconf(_SC_GETGR_R_SIZE_MAX);
      struct group* entry = ::getgrgid((gid_t)integralId);
      assert(entry != 0, TrusteeException(this));
      return String(entry->gr_name);
#endif // HAVE_GETGRNAM_R
    }
  case Trustee::USER:
    {
      Allocator<char>* buffer = Thread::getLocalStorage();
      struct passwd pw;
      struct passwd* entry;
      int result = ::getpwuid_r(
        (uid_t)integralId,
        &pw,
        buffer->getElements(),
        buffer->getSize(),
        &entry
      );
      assert(result == 0, TrusteeException("Unable to lookup name", this));
      return String(entry->pw_name);
    }
  case Trustee::EVERYONE:
  default: // UNSPECIFIED and CLASS are not possible
    return Literal("other");
  }
#endif // flavor
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Trustee& value) throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!value.id.isValid()) {
    return stream << "<unknown>";
  }
  StringOutputStream s;
  PSID sid = (PSID)value.id->getElements(); // must be valid
  
  // write prefix and revision number
  s << "S-" << ((SID*)sid)->Revision << '-';
  
  // write identifier authority
  PSID_IDENTIFIER_AUTHORITY identifier = ::GetSidIdentifierAuthority(sid);
  if ((identifier->Value[0] == 0) && (identifier->Value[1] == 0)) {
    s << DEC << static_cast<unsigned int>(identifier->Value[5]) +
      (static_cast<unsigned int>(identifier->Value[4]) << 8) +
      (static_cast<unsigned int>(identifier->Value[3]) << 16) +
      (static_cast<unsigned int>(identifier->Value[2]) << 24);
  } else {
    s << HEX << ZEROPAD << PREFIX << static_cast<unsigned int>(identifier->Value[0])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[1])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[2])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[3])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[4])
      << HEX << ZEROPAD << NOPREFIX << static_cast<unsigned int>(identifier->Value[5]);
  }
  
  // write subauthorities
  const unsigned int numberOfSubAuthorities = *::GetSidSubAuthorityCount(sid);
  for (unsigned int i = 0; i < numberOfSubAuthorities; i++) {
    s << '-' << *::GetSidSubAuthority(sid, i);
  }
  s << FLUSH;
  return stream << s.getString();
#else
  unsigned long id = value.integralId;
  if (id == PrimitiveTraits<unsigned long>::MAXIMUM) {
    return stream << "<unknown>";
  }
  StringOutputStream s;
  switch (value.getType()) {
  case Trustee::GROUP:
    s << 'G' << '-' << id << FLUSH;
    break;
  case Trustee::USER:
    s << 'U' << '-' << id << FLUSH;
    break;
  case Trustee::EVERYONE:
  default: // UNSPECIFIED and CLASS not possible
    s << 'G' << '-' << "other";
    break;
  }
  return stream << s.getString();
#endif
}

unsigned long Hash<Trustee>::operator()(const Trustee& value) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!value.id.isValid()) {
    return 0;
  }
  const uint8* src = value.id->getElements();
  const uint8* end = src + value.id->getSize();
  unsigned long result = 5381; // TAG: fixme
  while (src < end) {
    result = /*33*/ 31 /*65599*/ * result + static_cast<unsigned char>(*src++);
  }
  return result;
#else // unix
  return value.getIntegralId();
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
