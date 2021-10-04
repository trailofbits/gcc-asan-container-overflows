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

#if __cplusplus < 201103L

template<typename T>
T emove(T &obj) {
    T copy(obj);

    return copy;
}

#endif

template<typename S>
void test_basic() {
    // Empty string
    S s1;
    assert(is_contiguous_container_asan_correct(s1));

    //
    S s2a = "a", s2b = "bb", s2c = "ccc";
    assert(is_contiguous_container_asan_correct(s2a));
    assert(is_contiguous_container_asan_correct(s2b));
    assert(is_contiguous_container_asan_correct(s2c));

    S s3a = "1234567890123456789012";
    assert(s3a.size() == 22);
    S s3b = "1234567890";
    assert(s3b.size() == 10);

    assert(is_contiguous_container_asan_correct(s3a));
    assert(is_contiguous_container_asan_correct(s3b));


    S s4 = "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqwwwwwwwwwwwwwwwwwwwweeeeeeeeeeeeeeeee";

    assert(s4.size() > 8 * 4);
    assert(is_contiguous_container_asan_correct(s4));

    S s5a(1, 'a'), s5b(3, 'c'), s5c(1000, 'q');

    assert(is_contiguous_container_asan_correct(s5a));
    assert(is_contiguous_container_asan_correct(s5b));
    assert(is_contiguous_container_asan_correct(s5c));

    S s6a(s5c.data()+3, s5c.data()+5), s6b(s4.data(), s4.data()+22), s6c(s5c.data(), s5c.data()+999);

    assert(is_contiguous_container_asan_correct(s6a));
    assert(is_contiguous_container_asan_correct(s6b));
    assert(is_contiguous_container_asan_correct(s6c));

    // That test case was failing with old ABI,
    // while all above were passing.
    S s7a(11100, 'b');

    assert(is_contiguous_container_asan_correct(s7a));
}

template<typename S>
S get_s(size_t n, char c = 'a')
{
    S s;
    while(s.size() < n)
        s.push_back(c);

    return s;
}

template<typename S>
void test()
{
    S short_s1a(get_s<S>(1)), short_s1b = get_s<S>(1, 'b');
    S long_s1a(get_s<S>(91, 'c')), long_s1b = get_s<S>(91);

    assert(is_contiguous_container_asan_correct(short_s1a));
    assert(is_contiguous_container_asan_correct(short_s1b));
    assert(is_contiguous_container_asan_correct(long_s1a));
    assert(is_contiguous_container_asan_correct(long_s1b));
#if __cplusplus >= 201103L
    S short_s2a(std::move(short_s1a)), short_s2b = std::move(short_s1b);
    S long_s2a(std::move(long_s1a)), long_s2b = std::move(long_s1b);
#else
    S short_s2a(emove(short_s1a)), short_s2b = emove(short_s1b);
    S long_s2a(emove(long_s1a)), long_s2b = emove(long_s1b);
#endif
    assert(is_contiguous_container_asan_correct(short_s2a));
    assert(is_contiguous_container_asan_correct(short_s2b));
    assert(is_contiguous_container_asan_correct(long_s2a));
    assert(is_contiguous_container_asan_correct(long_s2b));

    assert(is_contiguous_container_asan_correct(short_s1a));
    assert(is_contiguous_container_asan_correct(short_s1b));
    assert(is_contiguous_container_asan_correct(long_s1a));
    assert(is_contiguous_container_asan_correct(long_s1b));

    S short_s3a(short_s2a), short_s3b = short_s2b;
    S long_s3a(long_s2a), long_s3b = long_s2b;

    assert(is_contiguous_container_asan_correct(short_s3a));
    assert(is_contiguous_container_asan_correct(short_s3b));
    assert(is_contiguous_container_asan_correct(long_s3a));
    assert(is_contiguous_container_asan_correct(long_s3b));

    assert(is_contiguous_container_asan_correct(short_s2a));
    assert(is_contiguous_container_asan_correct(short_s2b));
    assert(is_contiguous_container_asan_correct(long_s2a));
    assert(is_contiguous_container_asan_correct(long_s2b));

    typedef typename S::value_type value_type;
    value_type a = value_type('a');

    S short_s4(1, a);
    S long_s4(91, a);

    assert(is_contiguous_container_asan_correct(short_s4));
    assert(is_contiguous_container_asan_correct(long_s4));

    S short_s5(long_s4.begin(), long_s4.begin() + 1);
    S long_s5(long_s4.rbegin(), long_s4.rbegin() + 81);

    assert(is_contiguous_container_asan_correct(short_s5));
    assert(is_contiguous_container_asan_correct(long_s5));
    assert(is_contiguous_container_asan_correct(long_s4));

#if __cplusplus >= 201103L
    S short_s6({short_s5[0]});
    S long_s6({
        long_s5[0], long_s5[1], long_s5[2], long_s5[3], long_s5[4], long_s5[5],
        long_s5[6], long_s5[7], long_s5[8], long_s5[9], long_s5[0], long_s5[1],
        long_s5[2], long_s5[3], long_s5[4], long_s5[5], long_s5[6], long_s5[7],
        long_s5[1], long_s5[2], long_s5[3], long_s5[4], long_s5[5], long_s5[6],
    });

    assert(is_contiguous_container_asan_correct(short_s6));
    assert(is_contiguous_container_asan_correct(long_s6));
    assert(is_contiguous_container_asan_correct(short_s5));
    assert(is_contiguous_container_asan_correct(long_s5));
#endif
}

int main(int, char**)
{
    {
        typedef std::string S;
        test_basic<S>();
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
