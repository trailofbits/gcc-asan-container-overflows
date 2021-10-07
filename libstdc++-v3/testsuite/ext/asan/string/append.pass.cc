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
void test1()
{
    ASanVerify<S> verificator;

    S empty_s;
    S short_s(3, 'b');
    S long_s(1100, 'a');

    empty_s.append(empty_s);
    short_s.append(empty_s);
    long_s.append(empty_s);

    verificator.add_and_verify(empty_s);
    verificator.add_and_verify(short_s);
    assert(verificator.add_and_verify(long_s));

    empty_s.append("ab");
    short_s.append(short_s);
    long_s.append(short_s);

    verificator.add_and_verify(empty_s);
    verificator.add_and_verify(short_s);
    assert(verificator.add_and_verify(long_s));

    S &not_empty_s = empty_s;
    short_s.append(empty_s);
    long_s.append(long_s);

    verificator.add_and_verify(not_empty_s);
    verificator.add_and_verify(short_s);
    assert(verificator.add_and_verify(long_s));


}

template<typename S>
void test2()
{
    ASanVerify<S> verificator;

    S empty_s;
    S short_s(3, 'b');
    S long_s(1100, 'a');

    empty_s += empty_s;
    short_s += empty_s;
    long_s += empty_s;

    verificator.add_and_verify(empty_s);
    verificator.add_and_verify(short_s);
    assert(verificator.add_and_verify(long_s));

    empty_s += "ab";
    short_s += short_s;
    long_s += short_s;

    verificator.add_and_verify(empty_s);
    verificator.add_and_verify(short_s);
    assert(verificator.add_and_verify(long_s));

    S &not_empty_s = empty_s;
    short_s += not_empty_s;
    long_s += long_s;

    verificator.add_and_verify(not_empty_s);
    verificator.add_and_verify(short_s);
    assert(verificator.add_and_verify(long_s));
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
