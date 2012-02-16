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

#if defined _WIN32 || defined __CYGWIN__
# define API_HELPER_IMPORT __declspec(dllimport)
# define API_HELPER_EXPORT __declspec(dllexport)
# define API_HELPER_LOCAL
#else
# if __GNUC__ >= 4
#  define API_HELPER_IMPORT __attribute__ ((visibility ("default")))
#  define API_HELPER_EXPORT __attribute__ ((visibility ("default")))
#  define API_HELPER_LOCAL  __attribute__ ((visibility ("hidden")))
# else
#  define API_HELPER_IMPORT
#  define API_HELPER_EXPORT
#  define API_HELPER_LOCAL
# endif
#endif

#ifdef SHARED
# ifdef EXPORTS
#  define API_EXPORT API_HELPER_EXPORT
# else
#  define API_EXPORT API_HELPER_IMPORT
# endif
# define API_LOCAL API_HELPER_LOCAL
#else // !SHARED
# define API_EXPORT
# define API_LOCAL
#endif // SHARED

#if defined(_WIN32)
/**
 * @brief Windows platform
 */
#  undef _WIN32_WINNT
#  define _WIN32_WINNT 0x0501
#  define NTDDI_VERSION NTDDI_WINXP

#ifndef NOGDICAPMASKS
#define NOGDICAPMASKS // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#endif
#ifndef NOMENUS
#define NOMENUS           // MF_*
#endif
#ifndef NOICONS
#define NOICONS           // IDI_*
#endif
#ifndef NOKEYSTATES
#define NOKEYSTATES       // MK_*
#endif
#ifndef NOSYSCOMMANDS
#define NOSYSCOMMANDS     // SC_*
#endif
#ifndef NORASTEROPS
#define NORASTEROPS       // Binary and Tertiary raster ops
#endif
#ifndef OEMRESOURCE
#define OEMRESOURCE       // OEM Resource values
#endif
#ifndef NOATOM
#define NOATOM            // Atom Manager routines
#endif
#ifndef NOCLIPBOARD
#define NOCLIPBOARD       // Clipboard routines
#endif
#ifndef NOCOLOR
#define NOCOLOR           // Screen colors
#endif
#ifndef NOCTLMGR
#define NOCTLMGR          // Control and Dialog routines
#endif
#ifndef NODRAWTEXT
#define NODRAWTEXT        // DrawText() and DT_*
#endif
#ifndef NOGDI
#define NOGDI             // All GDI defines and routines
#endif
#ifndef NOKERNEL
#define NOKERNEL          // All KERNEL defines and routines
#endif
#ifndef NONLS
#define NONLS             // All NLS defines and routines
#endif
#ifndef NOMB
#define NOMB              // MB_* and MessageBox()
#endif
#ifndef NOMEMMGR
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#endif
#ifndef NOMETAFILE
#define NOMETAFILE        // typedef METAFILEPICT
#endif
#ifndef NOMINMAX
#define NOMINMAX          // Macros min(a,b) and max(a,b)
#endif
#ifndef NOOPENFILE
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#endif
#ifndef NOSCROLL
#define NOSCROLL          // SB_* and scrolling routines
#endif
#ifndef NOSERVICE
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#endif
#ifndef NOSOUND
#define NOSOUND           // Sound driver routines
#endif
#ifndef NOTEXTMETRIC
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#endif
#ifndef NOWINOFFSETS
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#endif
#ifndef NOCOMM
#define NOCOMM            // COMM driver routines
#endif
#ifndef NOKANJI
#define NOKANJI           // Kanji support stuff.
#endif
#ifndef NOHELP
#define NOHELP            // Help engine interface.
#endif
#ifndef NOPROFILER
#define NOPROFILER        // Profiler interface.
#endif
#ifndef NODEFERWINDOWPOS
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#endif
#ifndef NOMCX
#define NOMCX             // Modem Configuration ExtensionsA
#endif
#ifndef NOVIRTUALKEYCODES
#define NOVIRTUALKEYCODES // VK_*
#endif
#ifndef NOWINMESSAGES
#define NOWINMESSAGES // WM_*, EM_*, LB_*, CB_*
#endif
#ifndef NOWINSTYLES
#define NOWINSTYLES // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#endif
#ifndef NOSYSMETRICS
#define NOSYSMETRICS // SM_*
#endif
#ifndef NORASTEROPS
#define NORASTEROPS // Binary and Tertiary raster ops
#endif
#ifndef NOSHOWWINDOW
#define NOSHOWWINDOW // SW_*
#endif
#ifndef NOUSER
#define NOUSER // All USER defines and routines
#endif
#ifndef NOMB
#define NOMB // MB_* and MessageBox()
#endif
#ifndef NOMEMMGR
#define NOMEMMGR // GMEM_*, LMEM_*, GHND, LHND, associated routines
#endif
#ifndef NOMETAFILE
#define NOMETAFILE // typedef METAFILEPICT
#endif

#  if defined(_MSC_VER)
#    define PLATFORM_WIN32_VC 1
#    define PLATFORM 1
#    if defined(WANT_CRT_DEBUG)
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

// character types
#  define DYN_CHAR char

#  define DYN_SYMBOL void *
#  define DYN_HANDLE HMODULE

#elif defined(unix) || defined(__unix__)
/**
 * @brief POSIX platform
 */

#  define PLATFORM_POSIX 1
#  define PLATFORM 1

// Char types
#  define DYN_CHAR char

#  define DYN_SYMBOL void *
#  define DYN_HANDLE void *

#  define INVALID_HANDLE_VALUE NULL // To keep things more consistent in init lists

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
	typedef std::basic_string<DYN_CHAR> dyn_string;
}

#endif // __PLATFORM_H__
