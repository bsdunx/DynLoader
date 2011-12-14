/**
 * Copyright (c) 2007-2008, Igor Semenov
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

#include <DynClass.hpp>
#include <DynLib.hpp>
#include <DynLibManager.hpp>
#include <DynLoader.hpp>
#include <memory>

/**
 * @namespace PDL
 * @brief Portable Dynamic Loader
 */
namespace DynLoader
{

/**
 * @brief Constructor
 */
PDL_DECL DynLoader::DynLoader() : libManager_(new DynLibManager())
{
	;;
}

/**
 * @brief Destructor
 */
PDL_DECL DynLoader::~DynLoader() throw()
{
	;;
}

/**
 * @brief Get dynamic loader instance
 * @return dynamic loader instance
 */
PDL_DECL DynLoader & DynLoader::Instance()
{
	static DynLoader _dynLoader; // @ FIXME
	return _dynLoader;
}

/**
 * @brief Create class instance
 * @param libName - [in] library file name
 * @param className - [in] class name
 * @return class instance, 0 if failed
 */
PDL_DECL DynClass * DynLoader::GetDynInstance(const PDL_CHAR * libName,
                                              const PDL_CHAR * className)
{
	auto & lib = libManager_->GetLib(libName);
	return lib.GetInstance(className);
}

/**
 * @brief Reset dynamic loader
 * Unload all loaded libraries and free instances
 */
PDL_DECL void DynLoader::Reset()
{
	libManager_->Reset();
}

} // namespace DynLoader