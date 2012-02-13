/**
 * @mainpage libDynLoader
 *
 * @section Introduction
 *
 * libdynloader is a simple and lightweight C++ library forked from libpdl, 
 * which provides common cross-platform interface for creating and using 
 * dynamically loaded interface classes.
 *
 * Project home:@n
 * http://code.google.com/p/libdynloader @n
 *
 * Project downloads:@n
 * http://code.google.com/p/libdynloader/downloads @n
 *
 * Project repository:@n
 * http://libdynloader.googlecode.com/svn/trunk/ @n
 *
 *
 * @section Example
 *
 * Dynamically loaded class interface:
 * @code
 * class ITest : public DynLoader::DynClass
 * {
 * public:
 *	/// Class functionality
 *	virtual void DoSomething() throw() = 0;
 *	/// Declare this class as dynamically loaded
 *	DECLARE_DYN_CLASS(ITest)
 * };
 * @endcode
 *
 * Dynamically loaded class implementation:
 * @code
 * class TestClass1 : public ITest
 * {
 * public:
 *	/// Class functionality
 * 	void DoSomething() throw()
 * 	{
 * 		// Do something
 * 	}
 * };
 * @endcode
 *
 * Dynamically loaded class usage:
 * @code
 * try
 * {
 * 	DynLoader::DynLoader & dynLoader = DynLoader::DynLoader::Instance();
 * 	ITest * instance =
 * 		dynLoader.GetClassInstance<ITest>("./TestClass.so", "TestClass");
 * 	instance->DoSomething();
 * }
 * catch(DynLoader::LoaderException & ex)
 * {
 * 	fprintf(stderr, "Loader exception: %s\n", ex.what());
 * }
 * @endcode
 *
 *
 * @section Status
 *
 * The library is under development and testing now.
 *
 *
 * @author Igor Semenov <igor@progz.ru>
 * @author Adam Gregoire <bsdunx@gmail.com>
 *
 * @version 0.0.1
 *
 * @par License:
 * Copyright (c) 2007-2008, Igor Semenov @n
 * Copyright (c) 2010-2011, Adam Gregoire @n
 *
 * All rights reserved. @n
 * @n
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: @n
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer. @n
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution. @n
 *     * Neither the name of Igor Semenov nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission. @n
 * @n
 * THIS SOFTWARE IS PROVIDED BY IGOR SEMENOV ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL IGOR SEMENOV BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. @n
 * @n
 */

#ifndef __DYNLOADER_HPP__
#define __DYNLOADER_HPP__

#include <platform.h>
#include <memory>

/**
 * @namespace DynLoader
 */
namespace DynLoader
{

/* @brief Forward declarations */
class DynClass;
class DynLibManager;

/**
 * @class DynLoader DynLoader.hpp <DynLoader.hpp>
 * @brief Dynamic loader
 */
class API_EXPORT DynLoader
{

public:
	/**
	 * @brief Create class instance
	 * @param libName - [in] library file name
	 * @param className - [in] class name
	 * @return class instance, 0 if failed
	 * Class should be derived from DynamicClass
	 */
	template<typename Class>
	Class & GetClassInstance(const PDL_CHAR * libName, const PDL_CHAR * className)
	{
		return static_cast<Class &>(GetDynInstance(libName, className));
	}

	/**
	 * @brief Reset dynamic loader
	 * Unload all loaded libraries and free instances
	 */
	void Reset() const;

	/**
	 * @brief Get dynamic loader instance
	 * @return dynamic loader instance
	 */
	static DynLoader & Instance();

	/**
	 * @brief Destructor
	 */
	~DynLoader() throw();

private:

	/**
	 * @brief Constructor
	 */
	DynLoader();

	/**
	 * @brief Create class instance
	 * @param libName - [in] library file name
	 * @param className - [in] class name
	 * @return class instance, 0 if failed
	 */
	DynClass & GetDynInstance(const PDL_CHAR * libName, const PDL_CHAR * className);
	
	// Forbid copy constructor and assignment operator
	DynLoader(const DynLoader &);
	DynLoader & operator= (const DynLoader &);

	// Library manager
	std::unique_ptr<DynLibManager> libManager_;

}; // class DynLoader

} // namespace DynLoader

#endif // __DYNLOADER_HPP__
