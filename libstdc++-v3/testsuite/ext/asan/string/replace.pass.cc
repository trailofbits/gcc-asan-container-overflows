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
void test1()
{
    S short_s, long_s, x, one;

    one.push_back('1');
    for(size_t i = 0; i < 1000; ++i)
    {
        long_s.push_back('a' + (i % 3));
        x.push_back('x');
        if(i < 5)
            short_s.push_back('a' + (i%3));
    }

    short_s.replace(1, 2, one.data());
    long_s.replace(20, 20, one.data());
    assert(is_contiguous_container_asan_correct(short_s));
    assert(is_contiguous_container_asan_correct(long_s));

    short_s.replace(short_s.begin(), short_s.begin() + 1, 2, 'A');
    long_s.replace(long_s.begin(), long_s.begin() + 1, 2, 'A');

    assert(is_contiguous_container_asan_correct(short_s));
    assert(is_contiguous_container_asan_correct(long_s));

    short_s.replace(1, 2, x.data());
    long_s.replace(20, 2, x.data());

    assert(is_contiguous_container_asan_correct(short_s));
    assert(is_contiguous_container_asan_correct(long_s));
#if __cplusplus >= 201103L
    S short_s2(3, 'a'), long_s2(300, 'b');
    
    short_s2.replace(short_s2.begin() + 1, short_s2.begin() + 3, {'x'});
    long_s2.replace(long_s2.begin() + 1, long_s2.begin() + 3, {'x'});

    assert(is_contiguous_container_asan_correct(short_s));
    assert(is_contiguous_container_asan_correct(long_s));

    short_s2.push_back('c');

    short_s2.replace(short_s2.begin() + 1, short_s2.begin() + 3, {'x', 'y', 'z', 'c'});
    long_s2.replace(long_s2.begin() + 1, long_s2.begin() + 3, {'x', 'y', 'z', 'c'});

    assert(is_contiguous_container_asan_correct(short_s));
    assert(is_contiguous_container_asan_correct(long_s));
#endif
}


template<typename S>
void test2()
{
    S short_s(3, 'a'), long_s(3000, 'b');

    S short_substr = short_s.substr(1, 2);

    assert(is_contiguous_container_asan_correct(short_substr));

    S long_substr1 = long_s.substr(10, 2);
    S long_substr2 = long_s.substr(10, 200);

    assert(is_contiguous_container_asan_correct(long_substr1));
    assert(is_contiguous_container_asan_correct(long_substr2));

    S short_empty = short_s.substr(2, 0);
    S long_empty = long_s.substr(2, 0);

    assert(is_contiguous_container_asan_correct(short_empty));
    assert(is_contiguous_container_asan_correct(long_empty));
}

template<typename S>
void test()
{
    test1<S>();
    test2<S>();
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

    return 0;
}
