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

#if defined(WIN32) || defined(_WIN32)
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
#      pragma warning(disable : 4985) // Should be in cmake prolly
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

/// Char types
#  include <tchar.h>

#  define PDL_CHAR char
#  define PDL_WCHAR wchar_t
#  define PDL_TCHAR TCHAR

#  define PDL_SYMBOL void *

// hardcode for now on windows
#  ifdef PDL_USE_THREADS
#    if PDL_USE_THREADS == 0
#      error "Non threaded builds are unsupported on Windows"
#    endif
#  endif

#  define PDL_THREADED 1

typedef CRITICAL_SECTION pdl_mutex;
typedef HANDLE pdl_thread_handle;
typedef LPTHREAD_START_ROUTINE pdl_thread_routine;
typedef DWORD pdl_thread_status;

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

#  ifdef PDL_USE_THREADS
#    define PDL_THREADED 1
#  else
#    define PDL_THREADED 0
#  endif

#include <pthread.h>

typedef pthread_mutex_t pdl_mutex;
typedef pthread_t pdl_thread_handle;
typedef void * (* pdl_thread_routine)(void *);
typedef void * pdl_thread_status;

#define INVALID_HANDLE_VALUE NULL // To keep things more consistent in init lists

#endif

#ifndef PLATFORM
/**
 * @brief Unknown platform. Cannot build
 */
#  error "Unknown platform detected."
#endif

#if defined(VC_DBG_BREAK)
#  error "VC_DBG_BREAK is defined elsewhere."
#endif
#if defined(_DEBUG)
#  if PLATFORM_WIN32_VC
#    define VC_DBG_BREAK DebugBreak();
#  endif
#endif
#if !defined(VC_DBG_BREAK)
#  define VC_DBG_BREAK
#endif

#include <string>

namespace DynLoader
{
	typedef std::basic_string<PDL_CHAR> pdl_string;
}

#endif // __PLATFORM_H__

