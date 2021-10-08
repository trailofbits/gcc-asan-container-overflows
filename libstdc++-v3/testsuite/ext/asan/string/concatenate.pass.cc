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

// { dg-options "-fsanitize=address" }
// { dg-do run }

#include <cassert>
#include <testsuite_asan_testing.h>
#include <string>

template<typename S>
void test() {
    S empty_s;
    S short_s;
    short_s.push_back('a');
    short_s.push_back('s');
    short_s.push_back('d');
    S long_s(1100, 'a');

    assert(is_contiguous_container_asan_correct(empty_s + empty_s));
    assert(is_contiguous_container_asan_correct(empty_s + short_s));
    assert(is_contiguous_container_asan_correct(empty_s + long_s));

    assert(is_contiguous_container_asan_correct(short_s + empty_s));
    assert(is_contiguous_container_asan_correct(long_s  + empty_s));

    assert(is_contiguous_container_asan_correct(short_s + short_s));
    assert(is_contiguous_container_asan_correct(long_s  + long_s));

    assert(is_contiguous_container_asan_correct(short_s + long_s));
    assert(is_contiguous_container_asan_correct(long_s  + long_s));
}

int main(int, char**)
{
    {
        typedef std::string S;
        test<S>();
    }
    {
        typedef std::wstring S;
        test<S>();
    }
#if __cplusplus >= 202011L
    {
        typedef std::u8string S;
        test<S>();
    }
#endif
#if __cplusplus >= 201103L
    {
        typedef std::u16string S;
        test<S>();
    }
    {
        typedef std::u32string S;
        test<S>();
    }
#endif
}
