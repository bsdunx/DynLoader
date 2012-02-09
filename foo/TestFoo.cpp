/**
* Copyright (c) 2010, Adam Gregoire
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are 
* met:
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of Adam Gregoire nor the names of its contributors 
*     may be used to endorse or promote products derived from this 
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY ADAM GREGOIRE ``AS IS'' AND ANY EXPRESS OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL ADAM GREGOIRE BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
*/

#include <platform.h>
#include <DynLoader.hpp>
#include <LoaderException.hpp>

#include "TestFoo.hpp"

#include "FooInterface.hpp"

#include <iostream>

#include <cstdio>
#include <cstring> // memcpy
#include <cstdlib> // malloc

#if PLATFORM_POSIX
#include <dlfcn.h>
#endif

#include "../tests/UnitTest.hpp"

int TestFoo(void)
{
	try
	{

		auto & dynLoader = DynLoader::DynLoader::Instance();

		dynLoader.Reset();
		UNIT_TEST(true);

#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
		auto Foo_I = dynLoader.GetClassInstance<IFoo>("libfoo_module.dll", "Foo");
		auto Bar_I = dynLoader.GetClassInstance<IBar>("libfoo_module.dll", "Bar");
#elif PLATFORM_POSIX
		auto Foo_I = dynLoader.GetClassInstance<IFoo>("./libfoo_module.so", "Foo");
		auto Bar_I = dynLoader.GetClassInstance<IBar>("./libfoo_module.so", "Bar");
#endif
		UNIT_TEST(true);

		for(int i = 0, j = 10; i <= 10; i++, j--)
		{
			fprintf(stderr, "Foo Fib(%d) = %d\n", i, Foo_I->Fib(i));
			fprintf(stderr, "Bar Fib(%d) = %d\n", j, Bar_I->Fib(j));
		}

		dynLoader.Reset();
		UNIT_TEST(true);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "OK: LoaderException caught: %s\n", ex.what());
		UNIT_TEST(false);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}

	return 0;
}

int TestFooLib(void)
{ 
	try
	{
		auto &dynLoader = DynLoader::DynLoader::Instance();

		dynLoader.Reset();

#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
		IFoo * Foo_I =
			dynLoader.GetClassInstance<IFoo>("libfoo_module.dll", "Foo");
		IBar * Bar_I  = 
			dynLoader.GetClassInstance<IBar>("libfoo_module.dll", "Bar");

#elif PLATFORM_POSIX
		IFoo * Foo_I =
			dynLoader.GetClassInstance<IFoo>("./libfoo_module.so", "Foo");
		IBar * Bar_I =
			dynLoader.GetClassInstance<IBar>("./libfoo_module.so", "Bar");
#endif

		for(int i = 0, j = 10; i <= 10; i++, j--)
		{
			fprintf(stderr, "Foo Fib(%d) = %d\n", i, Foo_I->Fib(i));
			fprintf(stderr, "Bar Fib(%d) = %d\n", j, Bar_I->Fib(j));
		}

#if PLATFORM_WIN32_VC || PLATFORM_WIN32_MINGW
		MessageBox(NULL, "Completed, click OK to reset DynLoader.", "Reset", NULL);
#endif

		dynLoader.Reset();

		UNIT_TEST(true);
	}
	catch(DynLoader::LoaderException & ex)
	{
		fprintf(stderr, "OK: LoaderException caught: %s\n", ex.what());
		UNIT_TEST(false);
	}
	catch(...)
	{
		UNIT_TEST(false);
	}

	return 0;
}

