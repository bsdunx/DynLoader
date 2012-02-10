/**
 * Copyright (c) 2007-2008, Igor Semenov
 * Copyright (c) 2010, Adam Gregoire
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Igor Semenov nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY IGOR SEMENOV ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL IGOR SEMENOV BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// @todo Push detection of platform and toolchain to cmake
#undef PLATFORM

#if defined(_WIN32)
/**
 * @brief Windows platform
 */
#if PDL_NTDDI_VER || PDL_WINNT_VER
  #error "Do not define PDL_NTDDI_VER or PDL_WINNT_VER use PDL_WIN_VER"
#endif

#undef _WIN32_WINNT

#define PDL_WIN2K 0x5000
#define PDL_WINXP 0x5001
//#define PDL_VISTA 3
//#define PDL_WIN7 4

#if PDL_WINVER == PDL_WINXP
  #define _WIN32_WINNT 0x0500
  #define NTDDI_VERSION NTDDI_WIN2K
#endif

#ifndef PDL_WINVER
  //#warning "Defaulting to Win2k build to override please define PDL_WINVER before including platform.h"
  #define PDL_DEF_WINVER PDL_WIN2K
  #define _WIN32_WINNT 0x0500
  #define NTDDI_VERSION NTDDI_WIN2K
#endif

#define WIN32_MEAN_AND_LEAN
#  include <Windows.h>
#  if defined(_MSC_VER)
#    define PLATFORM_WIN32_VC 1
#    define PLATFORM 1
#    if defined(_DEBUG)
#      define _CRTDBG_MAP_ALLOC
#      define _CRTDBG_MAP_ALLOC_NEW
#      include <stdlib.h>
#      include <crtdbg.h>
#      undef THIS_FILE
       static char THIS_FILE[] = __FILE__; 
#      define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#    endif
#  elif defined(__GNUC__)
#    if defined(__CYGWIN__)
#      define PLATFORM_WIN32_CYGWIN 1
#      define PLATFORM 1
#    endif // __CYGWIN__
#    if defined(__MINGW32__)
#      define PLATFORM_WIN32_MINGW 1
#      define PLATFORM 1
#    endif // __MINGW32__
#  endif // __GNUC__

#  define PDL_DECL_EXPORT __declspec(dllexport)

#  ifdef PDL_SHARED
#    ifdef PDL_EXPORT
#      define PDL_DECL PDL_DECL_EXPORT
#    else
#      define PDL_DECL __declspec(dllimport)
#    endif
#  else
#    define PDL_DECL
#  endif

// character types
#  include <tchar.h>

#  define PDL_CHAR char
#  define PDL_WCHAR wchar_t
#  define PDL_TCHAR TCHAR

#  define PDL_SYMBOL void *

#elif defined(unix) || defined(__unix__)
/**
 * @brief POSIX platform
 */

#  define PLATFORM_POSIX 1
#  define PLATFORM 1

#  define PDL_DECL_EXPORT
#  define PDL_DECL

/// Char types
#  define PDL_CHAR char

#  define PDL_SYMBOL void *

#define INVALID_HANDLE_VALUE NULL // To keep things more consistent in init lists

#endif

#ifndef PLATFORM
/**
 * @brief Unknown platform. Cannot build
 */
#  error "Unknown platform detected."
#endif

#include <string>

namespace DynLoader
{
	typedef std::basic_string<PDL_CHAR> pdl_string;
}

#endif // __PLATFORM_H__

