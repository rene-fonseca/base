/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/AdvancedEncryptionStandard.h>
#include <base/Functor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace AdvancedEncryptionStandardImpl {
  
  /** Substitution lookup table. */
  const uint8 S[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
  };
  
  /** Inverse substitution lookup table. */
  const uint8 INVERSE_S[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
    0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
    0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
    0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
    0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
    0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
    0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
    0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
    0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
    0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
    0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
    0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
  };
  
  /** Lookup table for multiplication in GF(2^8) by 0x02. */
  const uint8 DOT_2[256] = {
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, // 0x00
    0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, // 0x08
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, // 0x10
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, // 0x18
    0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, // 0x20
    0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, // 0x28
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, // 0x30
    0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e, // 0x38
    0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, // 0x40
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, // 0x48
    0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, // 0x50
    0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, // 0x58
    0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, // 0x60
    0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde, // 0x68
    0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, // 0x70
    0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, // 0x78
    0x1b, 0x19, 0x1f, 0x1d, 0x13, 0x11, 0x17, 0x15, // 0x80
    0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05, // 0x88
    0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31, 0x37, 0x35, // 0x90
    0x2b, 0x29, 0x2f, 0x2d, 0x23, 0x21, 0x27, 0x25, // 0x98
    0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55, // 0xa0
    0x4b, 0x49, 0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45, // 0xa8
    0x7b, 0x79, 0x7f, 0x7d, 0x73, 0x71, 0x77, 0x75, // 0xb0
    0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61, 0x67, 0x65, // 0xb8
    0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95, // 0xc0
    0x8b, 0x89, 0x8f, 0x8d, 0x83, 0x81, 0x87, 0x85, // 0xc8
    0xbb, 0xb9, 0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5, // 0xd0
    0xab, 0xa9, 0xaf, 0xad, 0xa3, 0xa1, 0xa7, 0xa5, // 0xd8
    0xdb, 0xd9, 0xdf, 0xdd, 0xd3, 0xd1, 0xd7, 0xd5, // 0xe0
    0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5, // 0xe8
    0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5, // 0xf0
    0xeb, 0xe9, 0xef, 0xed, 0xe3, 0xe1, 0xe7, 0xe5 // 0xf8
  };

  /** Lookup table for multiplication in GF(2^8) by 0x03. */
  const uint8 DOT_3[256] = {
    0x00, 0x03, 0x06, 0x05, 0x0c, 0x0f, 0x0a, 0x09, // 0x00
    0x18, 0x1b, 0x1e, 0x1d, 0x14, 0x17, 0x12, 0x11, // 0x08
    0x30, 0x33, 0x36, 0x35, 0x3c, 0x3f, 0x3a, 0x39, // 0x10
    0x28, 0x2b, 0x2e, 0x2d, 0x24, 0x27, 0x22, 0x21, // 0x18
    0x60, 0x63, 0x66, 0x65, 0x6c, 0x6f, 0x6a, 0x69, // 0x20
    0x78, 0x7b, 0x7e, 0x7d, 0x74, 0x77, 0x72, 0x71, // 0x28
    0x50, 0x53, 0x56, 0x55, 0x5c, 0x5f, 0x5a, 0x59, // 0x30
    0x48, 0x4b, 0x4e, 0x4d, 0x44, 0x47, 0x42, 0x41, // 0x38
    0xc0, 0xc3, 0xc6, 0xc5, 0xcc, 0xcf, 0xca, 0xc9, // 0x40
    0xd8, 0xdb, 0xde, 0xdd, 0xd4, 0xd7, 0xd2, 0xd1, // 0x48
    0xf0, 0xf3, 0xf6, 0xf5, 0xfc, 0xff, 0xfa, 0xf9, // 0x50
    0xe8, 0xeb, 0xee, 0xed, 0xe4, 0xe7, 0xe2, 0xe1, // 0x58
    0xa0, 0xa3, 0xa6, 0xa5, 0xac, 0xaf, 0xaa, 0xa9, // 0x60
    0xb8, 0xbb, 0xbe, 0xbd, 0xb4, 0xb7, 0xb2, 0xb1, // 0x68
    0x90, 0x93, 0x96, 0x95, 0x9c, 0x9f, 0x9a, 0x99, // 0x70
    0x88, 0x8b, 0x8e, 0x8d, 0x84, 0x87, 0x82, 0x81, // 0x78
    0x9b, 0x98, 0x9d, 0x9e, 0x97, 0x94, 0x91, 0x92, // 0x80
    0x83, 0x80, 0x85, 0x86, 0x8f, 0x8c, 0x89, 0x8a, // 0x88
    0xab, 0xa8, 0xad, 0xae, 0xa7, 0xa4, 0xa1, 0xa2, // 0x90
    0xb3, 0xb0, 0xb5, 0xb6, 0xbf, 0xbc, 0xb9, 0xba, // 0x98
    0xfb, 0xf8, 0xfd, 0xfe, 0xf7, 0xf4, 0xf1, 0xf2, // 0xa0
    0xe3, 0xe0, 0xe5, 0xe6, 0xef, 0xec, 0xe9, 0xea, // 0xa8
    0xcb, 0xc8, 0xcd, 0xce, 0xc7, 0xc4, 0xc1, 0xc2, // 0xb0
    0xd3, 0xd0, 0xd5, 0xd6, 0xdf, 0xdc, 0xd9, 0xda, // 0xb8
    0x5b, 0x58, 0x5d, 0x5e, 0x57, 0x54, 0x51, 0x52, // 0xc0
    0x43, 0x40, 0x45, 0x46, 0x4f, 0x4c, 0x49, 0x4a, // 0xc8
    0x6b, 0x68, 0x6d, 0x6e, 0x67, 0x64, 0x61, 0x62, // 0xd0
    0x73, 0x70, 0x75, 0x76, 0x7f, 0x7c, 0x79, 0x7a, // 0xd8
    0x3b, 0x38, 0x3d, 0x3e, 0x37, 0x34, 0x31, 0x32, // 0xe0
    0x23, 0x20, 0x25, 0x26, 0x2f, 0x2c, 0x29, 0x2a, // 0xe8
    0x0b, 0x08, 0x0d, 0x0e, 0x07, 0x04, 0x01, 0x02, // 0xf0
    0x13, 0x10, 0x15, 0x16, 0x1f, 0x1c, 0x19, 0x1a // 0xf8
  };

  /** Lookup table for multiplication in GF(2^8) by 0x09. */
  const uint8 DOT_9[256] = {
    0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f, // 0x00
    0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77, // 0x08
    0x90, 0x99, 0x82, 0x8b, 0xb4, 0xbd, 0xa6, 0xaf, // 0x10
    0xd8, 0xd1, 0xca, 0xc3, 0xfc, 0xf5, 0xee, 0xe7, // 0x18
    0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04, // 0x20
    0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c, // 0x28
    0xab, 0xa2, 0xb9, 0xb0, 0x8f, 0x86, 0x9d, 0x94, // 0x30
    0xe3, 0xea, 0xf1, 0xf8, 0xc7, 0xce, 0xd5, 0xdc, // 0x38
    0x76, 0x7f, 0x64, 0x6d, 0x52, 0x5b, 0x40, 0x49, // 0x40
    0x3e, 0x37, 0x2c, 0x25, 0x1a, 0x13, 0x08, 0x01, // 0x48
    0xe6, 0xef, 0xf4, 0xfd, 0xc2, 0xcb, 0xd0, 0xd9, // 0x50
    0xae, 0xa7, 0xbc, 0xb5, 0x8a, 0x83, 0x98, 0x91, // 0x58
    0x4d, 0x44, 0x5f, 0x56, 0x69, 0x60, 0x7b, 0x72, // 0x60
    0x05, 0x0c, 0x17, 0x1e, 0x21, 0x28, 0x33, 0x3a, // 0x68
    0xdd, 0xd4, 0xcf, 0xc6, 0xf9, 0xf0, 0xeb, 0xe2, // 0x70
    0x95, 0x9c, 0x87, 0x8e, 0xb1, 0xb8, 0xa3, 0xaa, // 0x78
    0xec, 0xe5, 0xfe, 0xf7, 0xc8, 0xc1, 0xda, 0xd3, // 0x80
    0xa4, 0xad, 0xb6, 0xbf, 0x80, 0x89, 0x92, 0x9b, // 0x88
    0x7c, 0x75, 0x6e, 0x67, 0x58, 0x51, 0x4a, 0x43, // 0x90
    0x34, 0x3d, 0x26, 0x2f, 0x10, 0x19, 0x02, 0x0b, // 0x98
    0xd7, 0xde, 0xc5, 0xcc, 0xf3, 0xfa, 0xe1, 0xe8, // 0xa0
    0x9f, 0x96, 0x8d, 0x84, 0xbb, 0xb2, 0xa9, 0xa0, // 0xa8
    0x47, 0x4e, 0x55, 0x5c, 0x63, 0x6a, 0x71, 0x78, // 0xb0
    0x0f, 0x06, 0x1d, 0x14, 0x2b, 0x22, 0x39, 0x30, // 0xb8
    0x9a, 0x93, 0x88, 0x81, 0xbe, 0xb7, 0xac, 0xa5, // 0xc0
    0xd2, 0xdb, 0xc0, 0xc9, 0xf6, 0xff, 0xe4, 0xed, // 0xc8
    0x0a, 0x03, 0x18, 0x11, 0x2e, 0x27, 0x3c, 0x35, // 0xd0
    0x42, 0x4b, 0x50, 0x59, 0x66, 0x6f, 0x74, 0x7d, // 0xd8
    0xa1, 0xa8, 0xb3, 0xba, 0x85, 0x8c, 0x97, 0x9e, // 0xe0
    0xe9, 0xe0, 0xfb, 0xf2, 0xcd, 0xc4, 0xdf, 0xd6, // 0xe8
    0x31, 0x38, 0x23, 0x2a, 0x15, 0x1c, 0x07, 0x0e, // 0xf0
    0x79, 0x70, 0x6b, 0x62, 0x5d, 0x54, 0x4f, 0x46 // 0xf8
  };
  
  /** Lookup table for multiplication in GF(2^8) by 0x0b. */
  const uint8 DOT_B[256] = {
    0x00, 0x0b, 0x16, 0x1d, 0x2c, 0x27, 0x3a, 0x31, // 0x00
    0x58, 0x53, 0x4e, 0x45, 0x74, 0x7f, 0x62, 0x69, // 0x08
    0xb0, 0xbb, 0xa6, 0xad, 0x9c, 0x97, 0x8a, 0x81, // 0x10
    0xe8, 0xe3, 0xfe, 0xf5, 0xc4, 0xcf, 0xd2, 0xd9, // 0x18
    0x7b, 0x70, 0x6d, 0x66, 0x57, 0x5c, 0x41, 0x4a, // 0x20
    0x23, 0x28, 0x35, 0x3e, 0x0f, 0x04, 0x19, 0x12, // 0x28
    0xcb, 0xc0, 0xdd, 0xd6, 0xe7, 0xec, 0xf1, 0xfa, // 0x30
    0x93, 0x98, 0x85, 0x8e, 0xbf, 0xb4, 0xa9, 0xa2, // 0x38
    0xf6, 0xfd, 0xe0, 0xeb, 0xda, 0xd1, 0xcc, 0xc7, // 0x40
    0xae, 0xa5, 0xb8, 0xb3, 0x82, 0x89, 0x94, 0x9f, // 0x48
    0x46, 0x4d, 0x50, 0x5b, 0x6a, 0x61, 0x7c, 0x77, // 0x50
    0x1e, 0x15, 0x08, 0x03, 0x32, 0x39, 0x24, 0x2f, // 0x58
    0x8d, 0x86, 0x9b, 0x90, 0xa1, 0xaa, 0xb7, 0xbc, // 0x60
    0xd5, 0xde, 0xc3, 0xc8, 0xf9, 0xf2, 0xef, 0xe4, // 0x68
    0x3d, 0x36, 0x2b, 0x20, 0x11, 0x1a, 0x07, 0x0c, // 0x70
    0x65, 0x6e, 0x73, 0x78, 0x49, 0x42, 0x5f, 0x54, // 0x78
    0xf7, 0xfc, 0xe1, 0xea, 0xdb, 0xd0, 0xcd, 0xc6, // 0x80
    0xaf, 0xa4, 0xb9, 0xb2, 0x83, 0x88, 0x95, 0x9e, // 0x88
    0x47, 0x4c, 0x51, 0x5a, 0x6b, 0x60, 0x7d, 0x76, // 0x90
    0x1f, 0x14, 0x09, 0x02, 0x33, 0x38, 0x25, 0x2e, // 0x98
    0x8c, 0x87, 0x9a, 0x91, 0xa0, 0xab, 0xb6, 0xbd, // 0xa0
    0xd4, 0xdf, 0xc2, 0xc9, 0xf8, 0xf3, 0xee, 0xe5, // 0xa8
    0x3c, 0x37, 0x2a, 0x21, 0x10, 0x1b, 0x06, 0x0d, // 0xb0
    0x64, 0x6f, 0x72, 0x79, 0x48, 0x43, 0x5e, 0x55, // 0xb8
    0x01, 0x0a, 0x17, 0x1c, 0x2d, 0x26, 0x3b, 0x30, // 0xc0
    0x59, 0x52, 0x4f, 0x44, 0x75, 0x7e, 0x63, 0x68, // 0xc8
    0xb1, 0xba, 0xa7, 0xac, 0x9d, 0x96, 0x8b, 0x80, // 0xd0
    0xe9, 0xe2, 0xff, 0xf4, 0xc5, 0xce, 0xd3, 0xd8, // 0xd8
    0x7a, 0x71, 0x6c, 0x67, 0x56, 0x5d, 0x40, 0x4b, // 0xe0
    0x22, 0x29, 0x34, 0x3f, 0x0e, 0x05, 0x18, 0x13, // 0xe8
    0xca, 0xc1, 0xdc, 0xd7, 0xe6, 0xed, 0xf0, 0xfb, // 0xf0
    0x92, 0x99, 0x84, 0x8f, 0xbe, 0xb5, 0xa8, 0xa3 // 0xf8
  };
  
  /** Lookup table for multiplication in GF(2^8) by 0x0d. */
  const uint8 DOT_D[256] = {
    0x00, 0x0d, 0x1a, 0x17, 0x34, 0x39, 0x2e, 0x23, // 0x00
    0x68, 0x65, 0x72, 0x7f, 0x5c, 0x51, 0x46, 0x4b, // 0x08
    0xd0, 0xdd, 0xca, 0xc7, 0xe4, 0xe9, 0xfe, 0xf3, // 0x10
    0xb8, 0xb5, 0xa2, 0xaf, 0x8c, 0x81, 0x96, 0x9b, // 0x18
    0xbb, 0xb6, 0xa1, 0xac, 0x8f, 0x82, 0x95, 0x98, // 0x20
    0xd3, 0xde, 0xc9, 0xc4, 0xe7, 0xea, 0xfd, 0xf0, // 0x28
    0x6b, 0x66, 0x71, 0x7c, 0x5f, 0x52, 0x45, 0x48, // 0x30
    0x03, 0x0e, 0x19, 0x14, 0x37, 0x3a, 0x2d, 0x20, // 0x38
    0x6d, 0x60, 0x77, 0x7a, 0x59, 0x54, 0x43, 0x4e, // 0x40
    0x05, 0x08, 0x1f, 0x12, 0x31, 0x3c, 0x2b, 0x26, // 0x48
    0xbd, 0xb0, 0xa7, 0xaa, 0x89, 0x84, 0x93, 0x9e, // 0x50
    0xd5, 0xd8, 0xcf, 0xc2, 0xe1, 0xec, 0xfb, 0xf6, // 0x58
    0xd6, 0xdb, 0xcc, 0xc1, 0xe2, 0xef, 0xf8, 0xf5, // 0x60
    0xbe, 0xb3, 0xa4, 0xa9, 0x8a, 0x87, 0x90, 0x9d, // 0x68
    0x06, 0x0b, 0x1c, 0x11, 0x32, 0x3f, 0x28, 0x25, // 0x70
    0x6e, 0x63, 0x74, 0x79, 0x5a, 0x57, 0x40, 0x4d, // 0x78
    0xda, 0xd7, 0xc0, 0xcd, 0xee, 0xe3, 0xf4, 0xf9, // 0x80
    0xb2, 0xbf, 0xa8, 0xa5, 0x86, 0x8b, 0x9c, 0x91, // 0x88
    0x0a, 0x07, 0x10, 0x1d, 0x3e, 0x33, 0x24, 0x29, // 0x90
    0x62, 0x6f, 0x78, 0x75, 0x56, 0x5b, 0x4c, 0x41, // 0x98
    0x61, 0x6c, 0x7b, 0x76, 0x55, 0x58, 0x4f, 0x42, // 0xa0
    0x09, 0x04, 0x13, 0x1e, 0x3d, 0x30, 0x27, 0x2a, // 0xa8
    0xb1, 0xbc, 0xab, 0xa6, 0x85, 0x88, 0x9f, 0x92, // 0xb0
    0xd9, 0xd4, 0xc3, 0xce, 0xed, 0xe0, 0xf7, 0xfa, // 0xb8
    0xb7, 0xba, 0xad, 0xa0, 0x83, 0x8e, 0x99, 0x94, // 0xc0
    0xdf, 0xd2, 0xc5, 0xc8, 0xeb, 0xe6, 0xf1, 0xfc, // 0xc8
    0x67, 0x6a, 0x7d, 0x70, 0x53, 0x5e, 0x49, 0x44, // 0xd0
    0x0f, 0x02, 0x15, 0x18, 0x3b, 0x36, 0x21, 0x2c, // 0xd8
    0x0c, 0x01, 0x16, 0x1b, 0x38, 0x35, 0x22, 0x2f, // 0xe0
    0x64, 0x69, 0x7e, 0x73, 0x50, 0x5d, 0x4a, 0x47, // 0xe8
    0xdc, 0xd1, 0xc6, 0xcb, 0xe8, 0xe5, 0xf2, 0xff, // 0xf0
    0xb4, 0xb9, 0xae, 0xa3, 0x80, 0x8d, 0x9a, 0x97 // 0xf8
  };
  
  /** Lookup table for multiplication in GF(2^8) by 0x0e. */
  const uint8 DOT_E[256] = {
    0x00, 0x0e, 0x1c, 0x12, 0x38, 0x36, 0x24, 0x2a, // 0x00
    0x70, 0x7e, 0x6c, 0x62, 0x48, 0x46, 0x54, 0x5a, // 0x08
    0xe0, 0xee, 0xfc, 0xf2, 0xd8, 0xd6, 0xc4, 0xca, // 0x10
    0x90, 0x9e, 0x8c, 0x82, 0xa8, 0xa6, 0xb4, 0xba, // 0x18
    0xdb, 0xd5, 0xc7, 0xc9, 0xe3, 0xed, 0xff, 0xf1, // 0x20
    0xab, 0xa5, 0xb7, 0xb9, 0x93, 0x9d, 0x8f, 0x81, // 0x28
    0x3b, 0x35, 0x27, 0x29, 0x03, 0x0d, 0x1f, 0x11, // 0x30
    0x4b, 0x45, 0x57, 0x59, 0x73, 0x7d, 0x6f, 0x61, // 0x38
    0xad, 0xa3, 0xb1, 0xbf, 0x95, 0x9b, 0x89, 0x87, // 0x40
    0xdd, 0xd3, 0xc1, 0xcf, 0xe5, 0xeb, 0xf9, 0xf7, // 0x48
    0x4d, 0x43, 0x51, 0x5f, 0x75, 0x7b, 0x69, 0x67, // 0x50
    0x3d, 0x33, 0x21, 0x2f, 0x05, 0x0b, 0x19, 0x17, // 0x58
    0x76, 0x78, 0x6a, 0x64, 0x4e, 0x40, 0x52, 0x5c, // 0x60
    0x06, 0x08, 0x1a, 0x14, 0x3e, 0x30, 0x22, 0x2c, // 0x68
    0x96, 0x98, 0x8a, 0x84, 0xae, 0xa0, 0xb2, 0xbc, // 0x70
    0xe6, 0xe8, 0xfa, 0xf4, 0xde, 0xd0, 0xc2, 0xcc, // 0x78
    0x41, 0x4f, 0x5d, 0x53, 0x79, 0x77, 0x65, 0x6b, // 0x80
    0x31, 0x3f, 0x2d, 0x23, 0x09, 0x07, 0x15, 0x1b, // 0x88
    0xa1, 0xaf, 0xbd, 0xb3, 0x99, 0x97, 0x85, 0x8b, // 0x90
    0xd1, 0xdf, 0xcd, 0xc3, 0xe9, 0xe7, 0xf5, 0xfb, // 0x98
    0x9a, 0x94, 0x86, 0x88, 0xa2, 0xac, 0xbe, 0xb0, // 0xa0
    0xea, 0xe4, 0xf6, 0xf8, 0xd2, 0xdc, 0xce, 0xc0, // 0xa8
    0x7a, 0x74, 0x66, 0x68, 0x42, 0x4c, 0x5e, 0x50, // 0xb0
    0x0a, 0x04, 0x16, 0x18, 0x32, 0x3c, 0x2e, 0x20, // 0xb8
    0xec, 0xe2, 0xf0, 0xfe, 0xd4, 0xda, 0xc8, 0xc6, // 0xc0
    0x9c, 0x92, 0x80, 0x8e, 0xa4, 0xaa, 0xb8, 0xb6, // 0xc8
    0x0c, 0x02, 0x10, 0x1e, 0x34, 0x3a, 0x28, 0x26, // 0xd0
    0x7c, 0x72, 0x60, 0x6e, 0x44, 0x4a, 0x58, 0x56, // 0xd8
    0x37, 0x39, 0x2b, 0x25, 0x0f, 0x01, 0x13, 0x1d, // 0xe0
    0x47, 0x49, 0x5b, 0x55, 0x7f, 0x71, 0x63, 0x6d, // 0xe8
    0xd7, 0xd9, 0xcb, 0xc5, 0xef, 0xe1, 0xf3, 0xfd, // 0xf0
    0xa7, 0xa9, 0xbb, 0xb5, 0x9f, 0x91, 0x83, 0x8d // 0xf8
  };
  
  typedef AdvancedEncryptionStandard Aes;
  
  class Cipher128 : public Aes::Cipher128Traits {
  public:

    static inline void fillSchedule(uint8* schedule, const uint8* key) throw() {
      // first value is not used - just offset by +1
      static const uint8 ROUND_CONSTANT[1 + (Nb * (Nr+1) - 1)/Nk] = {
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
      };
      
      for (unsigned int i = 0; i < Nk; ++i) {
        schedule[Nb * (Nr+1) * 0 + i] = key[4 * i + 0];
        schedule[Nb * (Nr+1) * 1 + i] = key[4 * i + 1];
        schedule[Nb * (Nr+1) * 2 + i] = key[4 * i + 2];
        schedule[Nb * (Nr+1) * 3 + i] = key[4 * i + 3];
      }
      
      for (unsigned int i = Nk; i < (Nb * (Nr+1)); ++i) {
        if ((i % Nk) == 0) {
          schedule[Nb * (Nr+1) * 0 + i] = schedule[Nb * (Nr+1) * 0 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 1 + i - 1]] ^ ROUND_CONSTANT[i/Nk]);
          schedule[Nb * (Nr+1) * 1 + i] = schedule[Nb * (Nr+1) * 1 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 2 + i - 1]] ^ 0x00);
          schedule[Nb * (Nr+1) * 2 + i] = schedule[Nb * (Nr+1) * 2 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 3 + i - 1]] ^ 0x00);
          schedule[Nb * (Nr+1) * 3 + i] = schedule[Nb * (Nr+1) * 3 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 0 + i - 1]] ^ 0x00);
        } else {
          schedule[Nb * (Nr+1) * 0 + i] = schedule[Nb * (Nr+1) * 0 + i - Nk] ^
            schedule[Nb * (Nr+1) * 0 + i - 1];
          schedule[Nb * (Nr+1) * 1 + i] = schedule[Nb * (Nr+1) * 1 + i - Nk] ^
            schedule[Nb * (Nr+1) * 1 + i - 1];
          schedule[Nb * (Nr+1) * 2 + i] = schedule[Nb * (Nr+1) * 2 + i - Nk] ^
            schedule[Nb * (Nr+1) * 2 + i - 1];
          schedule[Nb * (Nr+1) * 3 + i] = schedule[Nb * (Nr+1) * 3 + i - Nk] ^
            schedule[Nb * (Nr+1) * 3 + i - 1];
        }
      }
    }
    
    static inline void substitute(uint8 (&state)[4][Nb]) throw() {
      state[0][0] = S[state[0][0]];
      state[0][1] = S[state[0][1]];
      state[0][2] = S[state[0][2]];
      state[0][3] = S[state[0][3]];
      
      state[1][0] = S[state[1][0]];
      state[1][1] = S[state[1][1]];
      state[1][2] = S[state[1][2]];
      state[1][3] = S[state[1][3]];
      
      state[2][0] = S[state[2][0]];
      state[2][1] = S[state[2][1]];
      state[2][2] = S[state[2][2]];
      state[2][3] = S[state[2][3]];
      
      state[3][0] = S[state[3][0]];
      state[3][1] = S[state[3][1]];
      state[3][2] = S[state[3][2]];
      state[3][3] = S[state[3][3]];
    }
    
    static inline void shiftRows(uint8 (&state)[4][Nb]) throw() {
      {
        uint8 temp0 = state[1][0];
        state[1][0] = state[1][1];
        state[1][1] = state[1][2];
        state[1][2] = state[1][3];
        state[1][3] = temp0;
      }
      
      {
        uint8 temp0 = state[2][0];
        uint8 temp1 = state[2][1];
        state[2][0] = state[2][2];
        state[2][1] = state[2][3];
        state[2][2] = temp0;
        state[2][3] = temp1;
      }
      
      {
        uint8 temp0 = state[3][3];
        state[3][3] = state[3][2];
        state[3][2] = state[3][1];
        state[3][1] = state[3][0];
        state[3][0] = temp0;
      }
    }
    
    static inline void mixColumns(uint8 (&s)[4][Nb]) throw() {
      uint8 t[4][Nb];
      
      t[0][0] = DOT_2[s[0][0]] ^ DOT_3[s[1][0]] ^ s[2][0] ^ s[3][0];
      t[1][0] = s[0][0] ^ DOT_2[s[1][0]] ^ DOT_3[s[2][0]] ^ s[3][0];
      t[2][0] = s[0][0] ^ s[1][0] ^ DOT_2[s[2][0]] ^ DOT_3[s[3][0]];
      t[3][0] = DOT_3[s[0][0]] ^ s[1][0] ^ s[2][0] ^ DOT_2[s[3][0]];
      
      t[0][1] = DOT_2[s[0][1]] ^ DOT_3[s[1][1]] ^ s[2][1] ^ s[3][1];
      t[1][1] = s[0][1] ^ DOT_2[s[1][1]] ^ DOT_3[s[2][1]] ^ s[3][1];
      t[2][1] = s[0][1] ^ s[1][1] ^ DOT_2[s[2][1]] ^ DOT_3[s[3][1]];
      t[3][1] = DOT_3[s[0][1]] ^ s[1][1] ^ s[2][1] ^ DOT_2[s[3][1]];
      
      t[0][2] = DOT_2[s[0][2]] ^ DOT_3[s[1][2]] ^ s[2][2] ^ s[3][2];
      t[1][2] = s[0][2] ^ DOT_2[s[1][2]] ^ DOT_3[s[2][2]] ^ s[3][2];
      t[2][2] = s[0][2] ^ s[1][2] ^ DOT_2[s[2][2]] ^ DOT_3[s[3][2]];
      t[3][2] = DOT_3[s[0][2]] ^ s[1][2] ^ s[2][2] ^ DOT_2[s[3][2]];
      
      t[0][3] = DOT_2[s[0][3]] ^ DOT_3[s[1][3]] ^ s[2][3] ^ s[3][3];
      t[1][3] = s[0][3] ^ DOT_2[s[1][3]] ^ DOT_3[s[2][3]] ^ s[3][3];
      t[2][3] = s[0][3] ^ s[1][3] ^ DOT_2[s[2][3]] ^ DOT_3[s[3][3]];
      t[3][3] = DOT_3[s[0][3]] ^ s[1][3] ^ s[2][3] ^ DOT_2[s[3][3]];

      s[0][0] = t[0][0];
      s[1][0] = t[1][0];
      s[2][0] = t[2][0];
      s[3][0] = t[3][0];

      s[0][1] = t[0][1];
      s[1][1] = t[1][1];
      s[2][1] = t[2][1];
      s[3][1] = t[3][1];

      s[0][2] = t[0][2];
      s[1][2] = t[1][2];
      s[2][2] = t[2][2];
      s[3][2] = t[3][2];

      s[0][3] = t[0][3];
      s[1][3] = t[1][3];
      s[2][3] = t[2][3];
      s[3][3] = t[3][3];
    }
    
    static inline void addRoundKey(
      uint8 (&state)[4][Nb],
      unsigned int round,
      const uint8* schedule) throw() {
      for (unsigned int row = 0; row < 4; row++) {
        for (unsigned int column = 0; column < Nb; column++) {
          state[row][column] ^=
            schedule[(Nb * (Nr + 1) * row) + round * Nb + column];
        }
      }
    }
  };
  
  
  
  class InverseCipher128 : public Aes::Cipher128Traits {
  public:
    
    static inline void substitute(uint8 (&state)[4][Nb]) throw() {
      state[0][0] = INVERSE_S[state[0][0]];
      state[0][1] = INVERSE_S[state[0][1]];
      state[0][2] = INVERSE_S[state[0][2]];
      state[0][3] = INVERSE_S[state[0][3]];
      
      state[1][0] = INVERSE_S[state[1][0]];
      state[1][1] = INVERSE_S[state[1][1]];
      state[1][2] = INVERSE_S[state[1][2]];
      state[1][3] = INVERSE_S[state[1][3]];
      
      state[2][0] = INVERSE_S[state[2][0]];
      state[2][1] = INVERSE_S[state[2][1]];
      state[2][2] = INVERSE_S[state[2][2]];
      state[2][3] = INVERSE_S[state[2][3]];
    
      state[3][0] = INVERSE_S[state[3][0]];
      state[3][1] = INVERSE_S[state[3][1]];
      state[3][2] = INVERSE_S[state[3][2]];
      state[3][3] = INVERSE_S[state[3][3]];
    }
    
    static inline void shiftRows(uint8 (&state)[4][Nb]) throw() {
      {
        uint8 temp0 = state[1][3];
        state[1][3] = state[1][2];
        state[1][2] = state[1][1];
        state[1][1] = state[1][0];
        state[1][0] = temp0;
      }
      
      {
        uint8 temp0 = state[2][0];
        uint8 temp1 = state[2][1];
        state[2][0] = state[2][2];
        state[2][1] = state[2][3];
        state[2][2] = temp0;
        state[2][3] = temp1;
      }
      
      {
        uint8 temp0 = state[3][0];
        state[3][0] = state[3][1];
        state[3][1] = state[3][2];
        state[3][2] = state[3][3];
        state[3][3] = temp0;
      }
    }
    
    static inline void mixColumns(uint8 (&s)[4][Nb]) throw() {
      uint8 t[4][Nb];
      
      t[0][0] = DOT_E[s[0][0]] ^ DOT_B[s[1][0]] ^ DOT_D[s[2][0]] ^ DOT_9[s[3][0]];
      t[1][0] = DOT_9[s[0][0]] ^ DOT_E[s[1][0]] ^ DOT_B[s[2][0]] ^ DOT_D[s[3][0]];
      t[2][0] = DOT_D[s[0][0]] ^ DOT_9[s[1][0]] ^ DOT_E[s[2][0]] ^ DOT_B[s[3][0]];
      t[3][0] = DOT_B[s[0][0]] ^ DOT_D[s[1][0]] ^ DOT_9[s[2][0]] ^ DOT_E[s[3][0]];

      t[0][1] = DOT_E[s[0][1]] ^ DOT_B[s[1][1]] ^ DOT_D[s[2][1]] ^ DOT_9[s[3][1]];
      t[1][1] = DOT_9[s[0][1]] ^ DOT_E[s[1][1]] ^ DOT_B[s[2][1]] ^ DOT_D[s[3][1]];
      t[2][1] = DOT_D[s[0][1]] ^ DOT_9[s[1][1]] ^ DOT_E[s[2][1]] ^ DOT_B[s[3][1]];
      t[3][1] = DOT_B[s[0][1]] ^ DOT_D[s[1][1]] ^ DOT_9[s[2][1]] ^ DOT_E[s[3][1]];

      t[0][2] = DOT_E[s[0][2]] ^ DOT_B[s[1][2]] ^ DOT_D[s[2][2]] ^ DOT_9[s[3][2]];
      t[1][2] = DOT_9[s[0][2]] ^ DOT_E[s[1][2]] ^ DOT_B[s[2][2]] ^ DOT_D[s[3][2]];
      t[2][2] = DOT_D[s[0][2]] ^ DOT_9[s[1][2]] ^ DOT_E[s[2][2]] ^ DOT_B[s[3][2]];
      t[3][2] = DOT_B[s[0][2]] ^ DOT_D[s[1][2]] ^ DOT_9[s[2][2]] ^ DOT_E[s[3][2]];

      t[0][3] = DOT_E[s[0][3]] ^ DOT_B[s[1][3]] ^ DOT_D[s[2][3]] ^ DOT_9[s[3][3]];
      t[1][3] = DOT_9[s[0][3]] ^ DOT_E[s[1][3]] ^ DOT_B[s[2][3]] ^ DOT_D[s[3][3]];
      t[2][3] = DOT_D[s[0][3]] ^ DOT_9[s[1][3]] ^ DOT_E[s[2][3]] ^ DOT_B[s[3][3]];
      t[3][3] = DOT_B[s[0][3]] ^ DOT_D[s[1][3]] ^ DOT_9[s[2][3]] ^ DOT_E[s[3][3]];
      
      s[0][0] = t[0][0];
      s[1][0] = t[1][0];
      s[2][0] = t[2][0];
      s[3][0] = t[3][0];

      s[0][1] = t[0][1];
      s[1][1] = t[1][1];
      s[2][1] = t[2][1];
      s[3][1] = t[3][1];

      s[0][2] = t[0][2];
      s[1][2] = t[1][2];
      s[2][2] = t[2][2];
      s[3][2] = t[3][2];

      s[0][3] = t[0][3];
      s[1][3] = t[1][3];
      s[2][3] = t[2][3];
      s[3][3] = t[3][3];
    }
    
    static inline void addRoundKey(
      uint8 (&state)[4][Nb],
      unsigned int round,
      const uint8* schedule) throw() {
      for (unsigned int row = 0; row < 4; row++) {
        for (unsigned int column = 0; column < Nb; column++) {
          state[row][column] ^=
            schedule[Nb * (Nr + 1) * row + round * Nb + column];
        }
      }
    }
  };



  class Cipher192 : public Aes::Cipher192Traits {
  public:
    
    static inline void fillSchedule(uint8* schedule, const uint8* key) throw() {
      // first value is not used - just offset by +1
      static const uint8 ROUND_CONSTANT[1 + (Nb * (Nr+1) - 1)/Nk] = {
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
      };
      
      for (unsigned int i = 0; i < Nk; ++i) {
        schedule[Nb * (Nr+1) * 0 + i] = key[4 * i + 0];
        schedule[Nb * (Nr+1) * 1 + i] = key[4 * i + 1];
        schedule[Nb * (Nr+1) * 2 + i] = key[4 * i + 2];
        schedule[Nb * (Nr+1) * 3 + i] = key[4 * i + 3];
      }
      
      for (unsigned int i = Nk; i < (Nb * (Nr+1)); ++i) {
        if ((i % Nk) == 0) {
          schedule[Nb * (Nr+1) * 0 + i] = schedule[Nb * (Nr+1) * 0 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 1 + i - 1]] ^ ROUND_CONSTANT[i/Nk]);
          schedule[Nb * (Nr+1) * 1 + i] = schedule[Nb * (Nr+1) * 1 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 2 + i - 1]] ^ 0x00);
          schedule[Nb * (Nr+1) * 2 + i] = schedule[Nb * (Nr+1) * 2 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 3 + i - 1]] ^ 0x00);
          schedule[Nb * (Nr+1) * 3 + i] = schedule[Nb * (Nr+1) * 3 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 0 + i - 1]] ^ 0x00);
        } else {
          schedule[Nb * (Nr+1) * 0 + i] = schedule[Nb * (Nr+1) * 0 + i - Nk] ^
            schedule[Nb * (Nr+1) * 0 + i - 1];
          schedule[Nb * (Nr+1) * 1 + i] = schedule[Nb * (Nr+1) * 1 + i - Nk] ^
            schedule[Nb * (Nr+1) * 1 + i - 1];
          schedule[Nb * (Nr+1) * 2 + i] = schedule[Nb * (Nr+1) * 2 + i - Nk] ^
            schedule[Nb * (Nr+1) * 2 + i - 1];
          schedule[Nb * (Nr+1) * 3 + i] = schedule[Nb * (Nr+1) * 3 + i - Nk] ^
            schedule[Nb * (Nr+1) * 3 + i - 1];
        }
      }
    }
    
    static inline void substitute(uint8 (&state)[4][Nb]) throw() {
      state[0][0] = S[state[0][0]];
      state[0][1] = S[state[0][1]];
      state[0][2] = S[state[0][2]];
      state[0][3] = S[state[0][3]];
      
      state[1][0] = S[state[1][0]];
      state[1][1] = S[state[1][1]];
      state[1][2] = S[state[1][2]];
      state[1][3] = S[state[1][3]];
      
      state[2][0] = S[state[2][0]];
      state[2][1] = S[state[2][1]];
      state[2][2] = S[state[2][2]];
      state[2][3] = S[state[2][3]];
      
      state[3][0] = S[state[3][0]];
      state[3][1] = S[state[3][1]];
      state[3][2] = S[state[3][2]];
      state[3][3] = S[state[3][3]];
    }
    
    static inline void shiftRows(uint8 (&state)[4][Nb]) throw() {
      {
        uint8 temp0 = state[1][0];
        state[1][0] = state[1][1];
        state[1][1] = state[1][2];
        state[1][2] = state[1][3];
        state[1][3] = temp0;
      }
      
      {
        uint8 temp0 = state[2][0];
        uint8 temp1 = state[2][1];
        state[2][0] = state[2][2];
        state[2][1] = state[2][3];
        state[2][2] = temp0;
        state[2][3] = temp1;
      }
      
      {
        uint8 temp0 = state[3][3];
        state[3][3] = state[3][2];
        state[3][2] = state[3][1];
        state[3][1] = state[3][0];
        state[3][0] = temp0;
      }
    }
    
    static inline void mixColumns(uint8 (&s)[4][Nb]) throw() {
      uint8 t[4][Nb];
      
      t[0][0] = DOT_2[s[0][0]] ^ DOT_3[s[1][0]] ^ s[2][0] ^ s[3][0];
      t[1][0] = s[0][0] ^ DOT_2[s[1][0]] ^ DOT_3[s[2][0]] ^ s[3][0];
      t[2][0] = s[0][0] ^ s[1][0] ^ DOT_2[s[2][0]] ^ DOT_3[s[3][0]];
      t[3][0] = DOT_3[s[0][0]] ^ s[1][0] ^ s[2][0] ^ DOT_2[s[3][0]];
      
      t[0][1] = DOT_2[s[0][1]] ^ DOT_3[s[1][1]] ^ s[2][1] ^ s[3][1];
      t[1][1] = s[0][1] ^ DOT_2[s[1][1]] ^ DOT_3[s[2][1]] ^ s[3][1];
      t[2][1] = s[0][1] ^ s[1][1] ^ DOT_2[s[2][1]] ^ DOT_3[s[3][1]];
      t[3][1] = DOT_3[s[0][1]] ^ s[1][1] ^ s[2][1] ^ DOT_2[s[3][1]];
      
      t[0][2] = DOT_2[s[0][2]] ^ DOT_3[s[1][2]] ^ s[2][2] ^ s[3][2];
      t[1][2] = s[0][2] ^ DOT_2[s[1][2]] ^ DOT_3[s[2][2]] ^ s[3][2];
      t[2][2] = s[0][2] ^ s[1][2] ^ DOT_2[s[2][2]] ^ DOT_3[s[3][2]];
      t[3][2] = DOT_3[s[0][2]] ^ s[1][2] ^ s[2][2] ^ DOT_2[s[3][2]];
      
      t[0][3] = DOT_2[s[0][3]] ^ DOT_3[s[1][3]] ^ s[2][3] ^ s[3][3];
      t[1][3] = s[0][3] ^ DOT_2[s[1][3]] ^ DOT_3[s[2][3]] ^ s[3][3];
      t[2][3] = s[0][3] ^ s[1][3] ^ DOT_2[s[2][3]] ^ DOT_3[s[3][3]];
      t[3][3] = DOT_3[s[0][3]] ^ s[1][3] ^ s[2][3] ^ DOT_2[s[3][3]];

      s[0][0] = t[0][0];
      s[1][0] = t[1][0];
      s[2][0] = t[2][0];
      s[3][0] = t[3][0];

      s[0][1] = t[0][1];
      s[1][1] = t[1][1];
      s[2][1] = t[2][1];
      s[3][1] = t[3][1];

      s[0][2] = t[0][2];
      s[1][2] = t[1][2];
      s[2][2] = t[2][2];
      s[3][2] = t[3][2];

      s[0][3] = t[0][3];
      s[1][3] = t[1][3];
      s[2][3] = t[2][3];
      s[3][3] = t[3][3];
    }
    
    static inline void addRoundKey(
      uint8 (&state)[4][Nb],
      unsigned int round,
      const uint8* schedule) throw() {
      for (unsigned int row = 0; row < 4; row++) {
        for (unsigned int column = 0; column < Nb; column++) {
          state[row][column] ^=
            schedule[(Nb * (Nr + 1) * row) + round * Nb + column];
        }
      }
    }
  };

  class InverseCipher192 : public Aes::Cipher192Traits {
  public:
    
    static inline void substitute(uint8 (&state)[4][Nb]) throw() {
      state[0][0] = INVERSE_S[state[0][0]];
      state[0][1] = INVERSE_S[state[0][1]];
      state[0][2] = INVERSE_S[state[0][2]];
      state[0][3] = INVERSE_S[state[0][3]];
      
      state[1][0] = INVERSE_S[state[1][0]];
      state[1][1] = INVERSE_S[state[1][1]];
      state[1][2] = INVERSE_S[state[1][2]];
      state[1][3] = INVERSE_S[state[1][3]];
      
      state[2][0] = INVERSE_S[state[2][0]];
      state[2][1] = INVERSE_S[state[2][1]];
      state[2][2] = INVERSE_S[state[2][2]];
      state[2][3] = INVERSE_S[state[2][3]];
    
      state[3][0] = INVERSE_S[state[3][0]];
      state[3][1] = INVERSE_S[state[3][1]];
      state[3][2] = INVERSE_S[state[3][2]];
      state[3][3] = INVERSE_S[state[3][3]];
    }
    
    static inline void shiftRows(uint8 (&state)[4][Nb]) throw() {
      {
        uint8 temp0 = state[1][3];
        state[1][3] = state[1][2];
        state[1][2] = state[1][1];
        state[1][1] = state[1][0];
        state[1][0] = temp0;
      }
      
      {
        uint8 temp0 = state[2][0];
        uint8 temp1 = state[2][1];
        state[2][0] = state[2][2];
        state[2][1] = state[2][3];
        state[2][2] = temp0;
        state[2][3] = temp1;
      }
      
      {
        uint8 temp0 = state[3][0];
        state[3][0] = state[3][1];
        state[3][1] = state[3][2];
        state[3][2] = state[3][3];
        state[3][3] = temp0;
      }
    }
    
    static inline void mixColumns(uint8 (&s)[4][Nb]) throw() {
      uint8 t[4][Nb];
      
      t[0][0] = DOT_E[s[0][0]] ^ DOT_B[s[1][0]] ^ DOT_D[s[2][0]] ^ DOT_9[s[3][0]];
      t[1][0] = DOT_9[s[0][0]] ^ DOT_E[s[1][0]] ^ DOT_B[s[2][0]] ^ DOT_D[s[3][0]];
      t[2][0] = DOT_D[s[0][0]] ^ DOT_9[s[1][0]] ^ DOT_E[s[2][0]] ^ DOT_B[s[3][0]];
      t[3][0] = DOT_B[s[0][0]] ^ DOT_D[s[1][0]] ^ DOT_9[s[2][0]] ^ DOT_E[s[3][0]];

      t[0][1] = DOT_E[s[0][1]] ^ DOT_B[s[1][1]] ^ DOT_D[s[2][1]] ^ DOT_9[s[3][1]];
      t[1][1] = DOT_9[s[0][1]] ^ DOT_E[s[1][1]] ^ DOT_B[s[2][1]] ^ DOT_D[s[3][1]];
      t[2][1] = DOT_D[s[0][1]] ^ DOT_9[s[1][1]] ^ DOT_E[s[2][1]] ^ DOT_B[s[3][1]];
      t[3][1] = DOT_B[s[0][1]] ^ DOT_D[s[1][1]] ^ DOT_9[s[2][1]] ^ DOT_E[s[3][1]];

      t[0][2] = DOT_E[s[0][2]] ^ DOT_B[s[1][2]] ^ DOT_D[s[2][2]] ^ DOT_9[s[3][2]];
      t[1][2] = DOT_9[s[0][2]] ^ DOT_E[s[1][2]] ^ DOT_B[s[2][2]] ^ DOT_D[s[3][2]];
      t[2][2] = DOT_D[s[0][2]] ^ DOT_9[s[1][2]] ^ DOT_E[s[2][2]] ^ DOT_B[s[3][2]];
      t[3][2] = DOT_B[s[0][2]] ^ DOT_D[s[1][2]] ^ DOT_9[s[2][2]] ^ DOT_E[s[3][2]];

      t[0][3] = DOT_E[s[0][3]] ^ DOT_B[s[1][3]] ^ DOT_D[s[2][3]] ^ DOT_9[s[3][3]];
      t[1][3] = DOT_9[s[0][3]] ^ DOT_E[s[1][3]] ^ DOT_B[s[2][3]] ^ DOT_D[s[3][3]];
      t[2][3] = DOT_D[s[0][3]] ^ DOT_9[s[1][3]] ^ DOT_E[s[2][3]] ^ DOT_B[s[3][3]];
      t[3][3] = DOT_B[s[0][3]] ^ DOT_D[s[1][3]] ^ DOT_9[s[2][3]] ^ DOT_E[s[3][3]];
      
      s[0][0] = t[0][0];
      s[1][0] = t[1][0];
      s[2][0] = t[2][0];
      s[3][0] = t[3][0];

      s[0][1] = t[0][1];
      s[1][1] = t[1][1];
      s[2][1] = t[2][1];
      s[3][1] = t[3][1];

      s[0][2] = t[0][2];
      s[1][2] = t[1][2];
      s[2][2] = t[2][2];
      s[3][2] = t[3][2];

      s[0][3] = t[0][3];
      s[1][3] = t[1][3];
      s[2][3] = t[2][3];
      s[3][3] = t[3][3];
    }
    
    static inline void addRoundKey(
      uint8 (&state)[4][Nb],
      unsigned int round,
      const uint8* schedule) throw() {
      for (unsigned int row = 0; row < 4; row++) {
        for (unsigned int column = 0; column < Nb; column++) {
          state[row][column] ^=
            schedule[Nb * (Nr + 1) * row + round * Nb + column];
        }
      }
    }
  };
  
  class Cipher256 : public Aes::Cipher256Traits {
  public:
    
    static inline void fillSchedule(uint8* schedule, const uint8* key) throw() {
      // first value is not used - just offset by +1
      static const uint8 ROUND_CONSTANT[1 + (Nb * (Nr+1) - 1)/Nk] = {
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40
      };
      
      for (unsigned int i = 0; i < Nk; ++i) {
        schedule[Nb * (Nr+1) * 0 + i] = key[4 * i + 0];
        schedule[Nb * (Nr+1) * 1 + i] = key[4 * i + 1];
        schedule[Nb * (Nr+1) * 2 + i] = key[4 * i + 2];
        schedule[Nb * (Nr+1) * 3 + i] = key[4 * i + 3];
      }
      
      for (unsigned int i = Nk; i < (Nb * (Nr+1)); ++i) {
        if ((i % Nk) == 0) {
          schedule[Nb * (Nr+1) * 0 + i] = schedule[Nb * (Nr+1) * 0 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 1 + i - 1]] ^ ROUND_CONSTANT[i/Nk]);
          schedule[Nb * (Nr+1) * 1 + i] = schedule[Nb * (Nr+1) * 1 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 2 + i - 1]] ^ 0x00);
          schedule[Nb * (Nr+1) * 2 + i] = schedule[Nb * (Nr+1) * 2 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 3 + i - 1]] ^ 0x00);
          schedule[Nb * (Nr+1) * 3 + i] = schedule[Nb * (Nr+1) * 3 + i - Nk] ^
            (S[schedule[Nb * (Nr+1) * 0 + i - 1]] ^ 0x00);
        } else if ((i % Nk) == 4) {
          schedule[Nb * (Nr+1) * 0 + i] = schedule[Nb * (Nr+1) * 0 + i - Nk] ^
            S[schedule[Nb * (Nr+1) * 0 + i - 1]];
          schedule[Nb * (Nr+1) * 1 + i] = schedule[Nb * (Nr+1) * 1 + i - Nk] ^
            S[schedule[Nb * (Nr+1) * 1 + i - 1]];
          schedule[Nb * (Nr+1) * 2 + i] = schedule[Nb * (Nr+1) * 2 + i - Nk] ^
            S[schedule[Nb * (Nr+1) * 2 + i - 1]];
          schedule[Nb * (Nr+1) * 3 + i] = schedule[Nb * (Nr+1) * 3 + i - Nk] ^
            S[schedule[Nb * (Nr+1) * 3 + i - 1]];
        } else {
          schedule[Nb * (Nr+1) * 0 + i] = schedule[Nb * (Nr+1) * 0 + i - Nk] ^
            schedule[Nb * (Nr+1) * 0 + i - 1];
          schedule[Nb * (Nr+1) * 1 + i] = schedule[Nb * (Nr+1) * 1 + i - Nk] ^
            schedule[Nb * (Nr+1) * 1 + i - 1];
          schedule[Nb * (Nr+1) * 2 + i] = schedule[Nb * (Nr+1) * 2 + i - Nk] ^
            schedule[Nb * (Nr+1) * 2 + i - 1];
          schedule[Nb * (Nr+1) * 3 + i] = schedule[Nb * (Nr+1) * 3 + i - Nk] ^
            schedule[Nb * (Nr+1) * 3 + i - 1];
        }
      }
    }
    
    static inline void substitute(uint8 (&state)[4][Nb]) throw() {
      state[0][0] = S[state[0][0]];
      state[0][1] = S[state[0][1]];
      state[0][2] = S[state[0][2]];
      state[0][3] = S[state[0][3]];
      
      state[1][0] = S[state[1][0]];
      state[1][1] = S[state[1][1]];
      state[1][2] = S[state[1][2]];
      state[1][3] = S[state[1][3]];
      
      state[2][0] = S[state[2][0]];
      state[2][1] = S[state[2][1]];
      state[2][2] = S[state[2][2]];
      state[2][3] = S[state[2][3]];
      
      state[3][0] = S[state[3][0]];
      state[3][1] = S[state[3][1]];
      state[3][2] = S[state[3][2]];
      state[3][3] = S[state[3][3]];
    }
    
    static inline void shiftRows(uint8 (&state)[4][Nb]) throw() {
      {
        uint8 temp0 = state[1][0];
        state[1][0] = state[1][1];
        state[1][1] = state[1][2];
        state[1][2] = state[1][3];
        state[1][3] = temp0;
      }
      
      {
        uint8 temp0 = state[2][0];
        uint8 temp1 = state[2][1];
        state[2][0] = state[2][2];
        state[2][1] = state[2][3];
        state[2][2] = temp0;
        state[2][3] = temp1;
      }
      
      {
        uint8 temp0 = state[3][3];
        state[3][3] = state[3][2];
        state[3][2] = state[3][1];
        state[3][1] = state[3][0];
        state[3][0] = temp0;
      }
    }
    
    static inline void mixColumns(uint8 (&s)[4][Nb]) throw() {
      uint8 t[4][Nb];
      
      t[0][0] = DOT_2[s[0][0]] ^ DOT_3[s[1][0]] ^ s[2][0] ^ s[3][0];
      t[1][0] = s[0][0] ^ DOT_2[s[1][0]] ^ DOT_3[s[2][0]] ^ s[3][0];
      t[2][0] = s[0][0] ^ s[1][0] ^ DOT_2[s[2][0]] ^ DOT_3[s[3][0]];
      t[3][0] = DOT_3[s[0][0]] ^ s[1][0] ^ s[2][0] ^ DOT_2[s[3][0]];
      
      t[0][1] = DOT_2[s[0][1]] ^ DOT_3[s[1][1]] ^ s[2][1] ^ s[3][1];
      t[1][1] = s[0][1] ^ DOT_2[s[1][1]] ^ DOT_3[s[2][1]] ^ s[3][1];
      t[2][1] = s[0][1] ^ s[1][1] ^ DOT_2[s[2][1]] ^ DOT_3[s[3][1]];
      t[3][1] = DOT_3[s[0][1]] ^ s[1][1] ^ s[2][1] ^ DOT_2[s[3][1]];
      
      t[0][2] = DOT_2[s[0][2]] ^ DOT_3[s[1][2]] ^ s[2][2] ^ s[3][2];
      t[1][2] = s[0][2] ^ DOT_2[s[1][2]] ^ DOT_3[s[2][2]] ^ s[3][2];
      t[2][2] = s[0][2] ^ s[1][2] ^ DOT_2[s[2][2]] ^ DOT_3[s[3][2]];
      t[3][2] = DOT_3[s[0][2]] ^ s[1][2] ^ s[2][2] ^ DOT_2[s[3][2]];
      
      t[0][3] = DOT_2[s[0][3]] ^ DOT_3[s[1][3]] ^ s[2][3] ^ s[3][3];
      t[1][3] = s[0][3] ^ DOT_2[s[1][3]] ^ DOT_3[s[2][3]] ^ s[3][3];
      t[2][3] = s[0][3] ^ s[1][3] ^ DOT_2[s[2][3]] ^ DOT_3[s[3][3]];
      t[3][3] = DOT_3[s[0][3]] ^ s[1][3] ^ s[2][3] ^ DOT_2[s[3][3]];

      s[0][0] = t[0][0];
      s[1][0] = t[1][0];
      s[2][0] = t[2][0];
      s[3][0] = t[3][0];

      s[0][1] = t[0][1];
      s[1][1] = t[1][1];
      s[2][1] = t[2][1];
      s[3][1] = t[3][1];

      s[0][2] = t[0][2];
      s[1][2] = t[1][2];
      s[2][2] = t[2][2];
      s[3][2] = t[3][2];

      s[0][3] = t[0][3];
      s[1][3] = t[1][3];
      s[2][3] = t[2][3];
      s[3][3] = t[3][3];
    }
    
    static inline void addRoundKey(
      uint8 (&state)[4][Nb],
      unsigned int round,
      const uint8* schedule) throw() {
      for (unsigned int row = 0; row < 4; row++) {
        for (unsigned int column = 0; column < Nb; column++) {
          state[row][column] ^=
            schedule[(Nb * (Nr + 1) * row) + round * Nb + column];
        }
      }
    }
  };
  
  class InverseCipher256 : public Aes::Cipher256Traits {
  public:
    
    static inline void substitute(uint8 (&state)[4][Nb]) throw() {
      state[0][0] = INVERSE_S[state[0][0]];
      state[0][1] = INVERSE_S[state[0][1]];
      state[0][2] = INVERSE_S[state[0][2]];
      state[0][3] = INVERSE_S[state[0][3]];
      
      state[1][0] = INVERSE_S[state[1][0]];
      state[1][1] = INVERSE_S[state[1][1]];
      state[1][2] = INVERSE_S[state[1][2]];
      state[1][3] = INVERSE_S[state[1][3]];
      
      state[2][0] = INVERSE_S[state[2][0]];
      state[2][1] = INVERSE_S[state[2][1]];
      state[2][2] = INVERSE_S[state[2][2]];
      state[2][3] = INVERSE_S[state[2][3]];
    
      state[3][0] = INVERSE_S[state[3][0]];
      state[3][1] = INVERSE_S[state[3][1]];
      state[3][2] = INVERSE_S[state[3][2]];
      state[3][3] = INVERSE_S[state[3][3]];
    }
    
    static inline void shiftRows(uint8 (&state)[4][Nb]) throw() {
      {
        uint8 temp0 = state[1][3];
        state[1][3] = state[1][2];
        state[1][2] = state[1][1];
        state[1][1] = state[1][0];
        state[1][0] = temp0;
      }
      
      {
        uint8 temp0 = state[2][0];
        uint8 temp1 = state[2][1];
        state[2][0] = state[2][2];
        state[2][1] = state[2][3];
        state[2][2] = temp0;
        state[2][3] = temp1;
      }
      
      {
        uint8 temp0 = state[3][0];
        state[3][0] = state[3][1];
        state[3][1] = state[3][2];
        state[3][2] = state[3][3];
        state[3][3] = temp0;
      }
    }
    
    static inline void mixColumns(uint8 (&s)[4][Nb]) throw() {
      uint8 t[4][Nb];
      
      t[0][0] = DOT_E[s[0][0]] ^ DOT_B[s[1][0]] ^ DOT_D[s[2][0]] ^ DOT_9[s[3][0]];
      t[1][0] = DOT_9[s[0][0]] ^ DOT_E[s[1][0]] ^ DOT_B[s[2][0]] ^ DOT_D[s[3][0]];
      t[2][0] = DOT_D[s[0][0]] ^ DOT_9[s[1][0]] ^ DOT_E[s[2][0]] ^ DOT_B[s[3][0]];
      t[3][0] = DOT_B[s[0][0]] ^ DOT_D[s[1][0]] ^ DOT_9[s[2][0]] ^ DOT_E[s[3][0]];

      t[0][1] = DOT_E[s[0][1]] ^ DOT_B[s[1][1]] ^ DOT_D[s[2][1]] ^ DOT_9[s[3][1]];
      t[1][1] = DOT_9[s[0][1]] ^ DOT_E[s[1][1]] ^ DOT_B[s[2][1]] ^ DOT_D[s[3][1]];
      t[2][1] = DOT_D[s[0][1]] ^ DOT_9[s[1][1]] ^ DOT_E[s[2][1]] ^ DOT_B[s[3][1]];
      t[3][1] = DOT_B[s[0][1]] ^ DOT_D[s[1][1]] ^ DOT_9[s[2][1]] ^ DOT_E[s[3][1]];

      t[0][2] = DOT_E[s[0][2]] ^ DOT_B[s[1][2]] ^ DOT_D[s[2][2]] ^ DOT_9[s[3][2]];
      t[1][2] = DOT_9[s[0][2]] ^ DOT_E[s[1][2]] ^ DOT_B[s[2][2]] ^ DOT_D[s[3][2]];
      t[2][2] = DOT_D[s[0][2]] ^ DOT_9[s[1][2]] ^ DOT_E[s[2][2]] ^ DOT_B[s[3][2]];
      t[3][2] = DOT_B[s[0][2]] ^ DOT_D[s[1][2]] ^ DOT_9[s[2][2]] ^ DOT_E[s[3][2]];

      t[0][3] = DOT_E[s[0][3]] ^ DOT_B[s[1][3]] ^ DOT_D[s[2][3]] ^ DOT_9[s[3][3]];
      t[1][3] = DOT_9[s[0][3]] ^ DOT_E[s[1][3]] ^ DOT_B[s[2][3]] ^ DOT_D[s[3][3]];
      t[2][3] = DOT_D[s[0][3]] ^ DOT_9[s[1][3]] ^ DOT_E[s[2][3]] ^ DOT_B[s[3][3]];
      t[3][3] = DOT_B[s[0][3]] ^ DOT_D[s[1][3]] ^ DOT_9[s[2][3]] ^ DOT_E[s[3][3]];
      
      s[0][0] = t[0][0];
      s[1][0] = t[1][0];
      s[2][0] = t[2][0];
      s[3][0] = t[3][0];

      s[0][1] = t[0][1];
      s[1][1] = t[1][1];
      s[2][1] = t[2][1];
      s[3][1] = t[3][1];

      s[0][2] = t[0][2];
      s[1][2] = t[1][2];
      s[2][2] = t[2][2];
      s[3][2] = t[3][2];

      s[0][3] = t[0][3];
      s[1][3] = t[1][3];
      s[2][3] = t[2][3];
      s[3][3] = t[3][3];
    }
    
    static inline void addRoundKey(
      uint8 (&state)[4][Nb],
      unsigned int round,
      const uint8* schedule) throw() {
      for (unsigned int row = 0; row < 4; row++) {
        for (unsigned int column = 0; column < Nb; column++) {
          state[row][column] ^=
            schedule[Nb * (Nr + 1) * row + round * Nb + column];
        }
      }
    }
  };
  
} // end of namespace - AdvancedEncryptionStandardImpl


void AdvancedEncryptionStandard::Cipher128::operator()(
  uint8* dest, const uint8* src, const uint8* schedule) throw() {
  uint8 state[4][Nb];
  
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nb; c++) {
      state[r][c] = src[c * 4 + r];
    }
  }
  
  AdvancedEncryptionStandardImpl::Cipher128::addRoundKey(
    state,
    0,
    schedule
  );
  for (unsigned int round = 1; round <= (Nr - 1); ++round) {   
    AdvancedEncryptionStandardImpl::Cipher128::substitute(state);
    AdvancedEncryptionStandardImpl::Cipher128::shiftRows(state);
    AdvancedEncryptionStandardImpl::Cipher128::mixColumns(state);
    AdvancedEncryptionStandardImpl::Cipher128::addRoundKey(
      state,
      round,
      schedule
    );
  }
  AdvancedEncryptionStandardImpl::Cipher128::substitute(state);
  AdvancedEncryptionStandardImpl::Cipher128::shiftRows(state);
  AdvancedEncryptionStandardImpl::Cipher128::addRoundKey(
    state,
    Nr,
    schedule
  );
  
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nb; c++) {
      dest[4 * c + r] = state[r][c];
    }
  }
}

void AdvancedEncryptionStandard::InverseCipher128::operator()(
  uint8* dest, const uint8* src, const uint8* schedule) throw() {
  uint8 state[4][Nb];
  
  for (unsigned int r = 0; r < 4; r++) { // copy to state
    for (unsigned int c = 0; c < Nb; c++) {
      state[r][c] = src[c * 4 + r];
    }
  }
  
  AdvancedEncryptionStandardImpl::InverseCipher128::addRoundKey(
    state,
    Nr,
    schedule
  );
  for (unsigned int round = (Nr - 1); round >= 1; --round) {
    AdvancedEncryptionStandardImpl::InverseCipher128::shiftRows(state);
    AdvancedEncryptionStandardImpl::InverseCipher128::substitute(state);
    AdvancedEncryptionStandardImpl::InverseCipher128::addRoundKey(
      state,
      round,
      schedule
    );
    AdvancedEncryptionStandardImpl::InverseCipher128::mixColumns(state);
  }
  AdvancedEncryptionStandardImpl::InverseCipher128::shiftRows(state);
  AdvancedEncryptionStandardImpl::InverseCipher128::substitute(state);
  AdvancedEncryptionStandardImpl::InverseCipher128::addRoundKey(
    state,
    0,
    schedule
  );
  
  for (unsigned int r = 0; r < 4; r++) { // copy to dest
    for (unsigned int c = 0; c < Nb; c++) {
      dest[4 * c + r] = state[r][c];
    }
  }
}

void AdvancedEncryptionStandard::Cipher192::operator()(
  uint8* dest, const uint8* src, const uint8* schedule) throw() {
  uint8 state[4][Nb];
  
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nb; c++) {
      state[r][c] = src[c * 4 + r];
    }
  }
  
  AdvancedEncryptionStandardImpl::Cipher192::addRoundKey(
    state,
    0,
    schedule
  );
  for (unsigned int round = 1; round <= (Nr - 1); ++round) {   
    AdvancedEncryptionStandardImpl::Cipher192::substitute(state);
    AdvancedEncryptionStandardImpl::Cipher192::shiftRows(state);
    AdvancedEncryptionStandardImpl::Cipher192::mixColumns(state);
    AdvancedEncryptionStandardImpl::Cipher192::addRoundKey(
      state,
      round,
      schedule
    );
  }
  AdvancedEncryptionStandardImpl::Cipher192::substitute(state);
  AdvancedEncryptionStandardImpl::Cipher192::shiftRows(state);
  AdvancedEncryptionStandardImpl::Cipher192::addRoundKey(
    state,
    Nr,
    schedule
  );
  
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nb; c++) {
      dest[4 * c + r] = state[r][c];
    }
  }
}

void AdvancedEncryptionStandard::InverseCipher192::operator()(
  uint8* dest, const uint8* src, const uint8* schedule) throw() {
  uint8 state[4][Nb];
  
  for (unsigned int r = 0; r < 4; r++) { // copy to state
    for (unsigned int c = 0; c < Nb; c++) {
      state[r][c] = src[c * 4 + r];
    }
  }
  
  AdvancedEncryptionStandardImpl::InverseCipher192::addRoundKey(
    state,
    Nr,
    schedule
  );
  for (unsigned int round = (Nr - 1); round >= 1; --round) {
    AdvancedEncryptionStandardImpl::InverseCipher192::shiftRows(state);
    AdvancedEncryptionStandardImpl::InverseCipher192::substitute(state);
    AdvancedEncryptionStandardImpl::InverseCipher192::addRoundKey(
      state,
      round,
      schedule
    );
    AdvancedEncryptionStandardImpl::InverseCipher192::mixColumns(state);
  }
  AdvancedEncryptionStandardImpl::InverseCipher192::shiftRows(state);
  AdvancedEncryptionStandardImpl::InverseCipher192::substitute(state);
  AdvancedEncryptionStandardImpl::InverseCipher192::addRoundKey(
    state,
    0,
    schedule
  );
  
  for (unsigned int r = 0; r < 4; r++) { // copy to dest
    for (unsigned int c = 0; c < Nb; c++) {
      dest[4 * c + r] = state[r][c];
    }
  }
}

void AdvancedEncryptionStandard::Cipher256::operator()(
  uint8* dest, const uint8* src, const uint8* schedule) throw() {
  uint8 state[4][Nb];
  
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nb; c++) {
      state[r][c] = src[c * 4 + r];
    }
  }
  
  AdvancedEncryptionStandardImpl::Cipher256::addRoundKey(
    state,
    0,
    schedule
  );
  for (unsigned int round = 1; round <= (Nr - 1); ++round) {   
    AdvancedEncryptionStandardImpl::Cipher256::substitute(state);
    AdvancedEncryptionStandardImpl::Cipher256::shiftRows(state);
    AdvancedEncryptionStandardImpl::Cipher256::mixColumns(state);
    AdvancedEncryptionStandardImpl::Cipher256::addRoundKey(
      state,
      round,
      schedule
    );
  }
  AdvancedEncryptionStandardImpl::Cipher256::substitute(state);
  AdvancedEncryptionStandardImpl::Cipher256::shiftRows(state);
  AdvancedEncryptionStandardImpl::Cipher256::addRoundKey(
    state,
    Nr,
    schedule
  );
  
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nb; c++) {
      dest[4 * c + r] = state[r][c];
    }
  }
}

void AdvancedEncryptionStandard::InverseCipher256::operator()(
  uint8* dest, const uint8* src, const uint8* schedule) throw() {
  uint8 state[4][Nb];
  
  for (unsigned int r = 0; r < 4; r++) { // copy to state
    for (unsigned int c = 0; c < Nb; c++) {
      state[r][c] = src[c * 4 + r];
    }
  }
  
  AdvancedEncryptionStandardImpl::InverseCipher256::addRoundKey(
    state,
    Nr,
    schedule
  );
  for (unsigned int round = (Nr - 1); round >= 1; --round) {
    AdvancedEncryptionStandardImpl::InverseCipher256::shiftRows(state);
    AdvancedEncryptionStandardImpl::InverseCipher256::substitute(state);
    AdvancedEncryptionStandardImpl::InverseCipher256::addRoundKey(
      state,
      round,
      schedule
    );
    AdvancedEncryptionStandardImpl::InverseCipher256::mixColumns(state);
  }
  AdvancedEncryptionStandardImpl::InverseCipher256::shiftRows(state);
  AdvancedEncryptionStandardImpl::InverseCipher256::substitute(state);
  AdvancedEncryptionStandardImpl::InverseCipher256::addRoundKey(
    state,
    0,
    schedule
  );
  
  for (unsigned int r = 0; r < 4; r++) { // copy to dest
    for (unsigned int c = 0; c < Nb; c++) {
      dest[4 * c + r] = state[r][c];
    }
  }
}

AdvancedEncryptionStandard::AdvancedEncryptionStandard(
  const uint8* key, Cipher _cipher, bool _inverse) throw()
  : cipher(_cipher),
    inverse(_inverse),
    bytesInBuffer(0) {
  switch (cipher) {
  case CIPHER_128:
    AdvancedEncryptionStandardImpl::Cipher128::fillSchedule(schedule, key);
    break;
  case CIPHER_192:
    AdvancedEncryptionStandardImpl::Cipher192::fillSchedule(schedule, key);
    break;
  case CIPHER_256:
    AdvancedEncryptionStandardImpl::Cipher256::fillSchedule(schedule, key);
    break;
  }
}

unsigned int AdvancedEncryptionStandard::push(
  uint8* dest, const uint8* src, unsigned int size) throw() {
  switch (cipher) {
  case CIPHER_128:
    if (inverse) {
      return cipherPush<InverseCipher128>(dest, src, size);
    } else {
      return cipherPush<Cipher128>(dest, src, size);
    }
  case CIPHER_192:
    if (inverse) {
      return cipherPush<InverseCipher192>(dest, src, size);
    } else {
      return cipherPush<Cipher192>(dest, src, size);
    }
  case CIPHER_256:
    if (inverse) {
      return cipherPush<InverseCipher256>(dest, src, size);
    } else {
      return cipherPush<Cipher256>(dest, src, size);
    }
  default:
    return 0;
  }
}

unsigned int AdvancedEncryptionStandard::pushEnd(
  uint8* dest, unsigned int size) throw() {
  switch (cipher) {
  case CIPHER_128:
    if (inverse) {
      return cipherPushEnd<InverseCipher128>(dest, size);
    } else {
      return cipherPushEnd<Cipher128>(dest, size);
    }
  case CIPHER_192:
    if (inverse) {
      return cipherPushEnd<InverseCipher192>(dest, size);
    } else {
      return cipherPushEnd<Cipher192>(dest, size);
    }
  case CIPHER_256:
    if (inverse) {
      return cipherPushEnd<InverseCipher256>(dest, size);
    } else {
      return cipherPushEnd<Cipher256>(dest, size);
    }
  default:
    return 0;
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
