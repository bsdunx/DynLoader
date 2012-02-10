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

#ifndef __DYNLIB_HPP__
#define __DYNLIB_HPP__

#include <platform.h>
#include <string>
#include <unordered_map>

#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
#include <windows.h>
#endif

/**
 * @namespace DynLoader
 * @brief Dynamic Module Loader
 */
namespace DynLoader
{

// FWD
class DynClass;

/**
 * @class DynLib DynLib.hpp <DynLib.hpp>
 * @brief Platform-specific dynamic library
 */
class DynLib
{
public:

	/**
	 * @brief Constructor
	 */
	DynLib();

	/**
	 * @brief Destructor
	 */
	~DynLib() throw();

	/**
	 * @brief Open library
	 * @param libName - [in] library file name
	 * @return true - loaded successfully, false otherwise
	 */
	bool Open(const PDL_CHAR * libName, bool resolveSymbols = true);

	/**
	 * @brief Get default library filename extension (platform-specific)
	 * @return default library filename extension
	 */
	static const PDL_CHAR * GetDefaultExt();

	/**
	 * @brief Check if library is opened
	 * @return true if library is opened, false otherwise
	 */
	bool Opened() const { return !!lib_; }
	
	/**
	 * @brief Get class instance
	 * @param className - [in] class name
	 * @return pointer to class instance
	 */
	DynClass & GetInstance(const PDL_CHAR * className);

	/**
	 * @brief Get last error description
	 * @return last error description
	 */
	pdl_string GetLastError() const;

	/**
	 * @brief Close library
	 * @return true if closed successfully, false otherwise
	 */
	bool Close();

private:

	/**
	 * @brief Get symbol by name
	 * @param symbolName - [in] symbol name
	 * @return pointer to symbol, 0 if not found
	 */
	PDL_SYMBOL GetSymbolByName(const PDL_CHAR * symbolName);
	
	// Forbid copy constructor and assignment operator
	DynLib(const DynLib &);
	DynLib & operator=(const DynLib &);

	// @todo this should be part of an abstraction layer
	// Library handle
#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
	HMODULE lib_;
#elif PLATFORM_POSIX
	void * lib_;
#endif

	// Library name
	pdl_string libName_;

	// Symbol map
	std::unordered_map<pdl_string, DynClass *> instances_;

}; // class DynLib

} // namespace DynLoader

#endif // __DYNLIB_HPP__

