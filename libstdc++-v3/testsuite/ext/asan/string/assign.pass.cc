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
#include <cstring>
template<typename S>
void test1()
{
    ASanVerify<S> verificator;

    S s1a, s1b, s1c = "def", s1d = "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq";
    s1a = "abc";
    s1b = "wopqdkopqwmdpoqwmdopqwmdopqwmopfqewopmtgopewmgopwemgopweopgopwemgopwemgopwemopgwepoegmpowemgpowemgopwemgopwemgopw";

    verificator.add(s1a);
    verificator.add(s1b);
    verificator.add(s1c);
    assert(verificator.add_and_verify(s1d));

    S s2a = s1a, s2b = s1b;

    verificator.add(s2a);
    verificator.add(s2b);

    verificator.add(s1a);
    assert(verificator.add_and_verify(s1b));

    S s3a, s3b;
    s3a = s1a;
    s3b = s1b;

    verificator.add(s3a);
    verificator.add(s3b);

    verificator.add(s1a);
    assert(verificator.add_and_verify(s1b));
 
    S s4;
    s4 = s1a + s1b;

    verificator.add(s4);

    verificator.add(s1a);
    assert(verificator.add_and_verify(s1b));
#if __cplusplus >= 201103L
    S s5a, s5b;
    s5a = std::move(s1c);
    s5b = std::move(s5b);

    verificator.add(s5a);
    verificator.add(s5b);
    assert(verificator.add_and_verify(s1c));
#endif
    S s6;
    s6 = 'a';
    assert(verificator.add_and_verify(s6));
#if __cplusplus >= 201103L
    S s7a, s7b, s7c;
    s7a = { 'a', 'b'};
    s7b = { 'S', 'i', 'l', 'e', 'n', 'c', 'e', '.' };
    s7c = { 'a', 'a', 'a',  'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',  'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};

    assert(s7c.size() > 8 * 3);

    verificator.add(s7a);
    verificator.add(s7b);
    assert(verificator.add_and_verify(s7c));
#endif
}

template<typename S>
void test2()
{
    ASanVerify<S> verificator;

    const char *ptr1 = "def";
    const char *ptr2 = "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq";
    const char *ptr3 = "abc";
    const char *ptr4 = "wopqdkopqwmdpoqwmdopqwmdopqwmopfqewopmtgopewmgopwemgopweopgopwemgopwemgopwemopgwepoegmpowemgpowemgopwemgopwemgopw";

    size_t size1 = std::strlen(ptr1);
    size_t size2 = std::strlen(ptr2);
    // size_t size3 = std::strlen(ptr3);
    // size_t size4 = std::strlen(ptr4);

    S s1a, s1b, s1c(ptr3), s1d(ptr4);
    s1a.assign(ptr1, ptr1+size1);
    s1b.assign(ptr2, ptr2+size2);

    verificator.add(s1a);
    verificator.add(s1b);
    verificator.add(s1c);
    assert(verificator.add_and_verify(s1d));

    S s2a, s2b;
    s2a.assign(s1a);
    s2b.assign(s1b);

    verificator.add(s2a);
    verificator.add(s2b);

    verificator.add(s1a);
    assert(verificator.add_and_verify(s1b));

    S s3a, s3b;
    s3a.assign(s1a.data(), s1a.data() + s1a.size());
    s3b.assign(s1b.data(), s1b.data() + s1b.size());

    verificator.add(s3a);
    assert(verificator.add_and_verify(s3b));

    S s4;
    s4.assign(s1a + s1b);

    verificator.add(s4);

    verificator.add(s1a);
    assert(verificator.add_and_verify(s1b));
#if __cplusplus >= 201103L
    S s5a, s5b;
    s5a.assign(std::move(s1c));
    s5b.assign(std::move(s1d));

    verificator.add(s5a);
    verificator.add(s5b);

    verificator.add(s1c);
    assert(verificator.add_and_verify(s1d));
 #endif
    S s6a, s6b, s6c;
    s6a.assign(ptr1);
    s6b.assign(ptr2);
    s6c.assign("ce");

    verificator.add(s6a);
    verificator.add(s6b);
    assert(verificator.add_and_verify(s6c));
#if __cplusplus >= 201103L
    S s7a, s7b, s7c;
    s7a.assign({ 'a', 'b'});
    s7b.assign({ 'S', 'i', 'l', 'e', 'n', 'c', 'e', '.' });
    s7c.assign({ 'a', 'a', 'a',  'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',  'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'});

    assert(s7c.size() > 8 * 3);

    verificator.add(s7a);
    verificator.add(s7b);
    assert(verificator.add_and_verify(s7c));
#endif

    S s8a, s8b, s8c, s8d;
#if __cplusplus >= 201103L
    s8a.assign(s1a.cbegin(), s1a.cend());
    s8b.assign(s1b.cbegin(), s1b.cend());
    s8c.assign(s1a.crbegin(), s1a.crend());
    s8d.assign(s1b.crbegin(), s1b.crend());
#else
    s8a.assign(s1a.begin(), s1a.end());
    s8b.assign(s1b.begin(), s1b.end());
    s8c.assign(s1a.rbegin(), s1a.rend());
    s8d.assign(s1b.rbegin(), s1b.rend());
#endif

    verificator.add(s8a);
    verificator.add(s8b);
    verificator.add(s8c);
    verificator.add(s8d);

    verificator.add(s1a);
    assert(verificator.add_and_verify(s1b));
 
    S& s9a = s1a, s9b = s1b;
#if __cplusplus >= 201103L
    s9a.assign(s1a.cbegin() + 1, s1a.cend());
    s9b.assign(s1b.cbegin() + 4, s1b.cbegin() + 10);
#else
    s9a.assign(s1a.begin() + 1, s1a.end());
    s9b.assign(s1b.begin() + 4, s1b.begin() + 10);
#endif

    verificator.add(s9a);
    verificator.add(s9b);

    verificator.add_and_verify(s1a);
    assert(verificator.add_and_verify(s1b)); 
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
}
