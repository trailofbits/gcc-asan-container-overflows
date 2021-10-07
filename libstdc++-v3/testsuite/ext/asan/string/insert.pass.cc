// Copyright (C) 2018-2021 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// { dg-options "-fsanitize=address -D_GLIBCXX_USE_CXX11_ABI=0" }
// { dg-do run }

#include <cassert>
#include <testsuite_asan_testing.h>
#include <string>

template<typename S>
void test()
{
    ASanVerify<S> verificator;

    S short_s(2, 'b');
    S long_s(1100, 'a');

    short_s.insert(1, 2, 'a');
    long_s.insert(10, 9, 'c');
    verificator.add(short_s);
    assert(verificator.add_and_verify(long_s));

#if __cplusplus >= 201103L && _GLIBCXX_USE_CXX11_ABI
    short_s.insert(short_s.cbegin() + 2, 'd');
    long_s.insert(long_s.cbegin() + 22, 'd');
#else
    short_s.insert(short_s.begin() + 2, 'd');
    long_s.insert(long_s.begin() + 22, 'd');
#endif

    verificator.add(short_s);
    assert(verificator.add_and_verify(long_s));

#if __cplusplus >=  201402L && _GLIBCXX_USE_CXX11_ABI
    short_s.insert(2, "silence", 2, 2);
    long_s.insert(2, "silenceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee", 22, 2);

    verificator.add(short_s);
    assert(verificator.add_and_verify(long_s));

    short_s.insert(2, 1000, 'c');

    assert(verificator.add_and_verify(short_s));
#endif
}

template<typename S>
void test_simple()
{
    ASanVerify<S> verificator;

    S short_s(2, 'b');
    S long_s(1100, 'a');

    short_s.insert(1, 2, 'a');
    long_s.insert(10, 9, 'c');
    verificator.add(short_s);
    assert(verificator.add_and_verify(long_s));

#if __cplusplus >= 201103L && _GLIBCXX_USE_CXX11_ABI
    short_s.insert(short_s.cbegin() + 2, 'd');
    long_s.insert(long_s.cbegin() + 22, 'd');
#else
    short_s.insert(short_s.begin() + 2, 'd');
    long_s.insert(long_s.begin() + 22, 'd');
#endif

    verificator.add(short_s);
    assert(verificator.add_and_verify(long_s));
}

int main(int, char**)
{
    {
        typedef std::string S;
        test<S>();
    }

    {
        typedef std::wstring S;
        test_simple<S>();
    }
#if __cplusplus >= 202011L
    {
        typedef std::u8string S;
        test_simple<S>();
    }
#endif
#if __cplusplus >= 201103L
    {
        typedef std::u16string S;
        test_simple<S>();
    }
    {
        typedef std::u32string S;
        test_simple<S>();
    }
#endif
}
