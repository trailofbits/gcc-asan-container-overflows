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
    ASanVerify<S> verificator;

    S c_short = get_s<S>(3, 'w'), c_long = get_s<S>(1000, 'q');
    verificator.add(c_short);
    assert(verificator.add_and_verify(c_long));

    
    S g1a = get_s<S>(3, 'a'), g1b = get_s<S>(1000, 'b');
    verificator.add(g1a);
    assert(verificator.add_and_verify(g1b));

    {
        ASanVerify<S> local_verificator;
        S la = get_s<S>(3, 'l'), lb = get_s<S>(1000, 'k'), lc = g1a, ld = g1b;
        local_verificator.add(la);
        local_verificator.add(lb);
        local_verificator.add(lc);
        local_verificator.add_and_verify(ld);
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');

        lc.push_back('c');
        assert(local_verificator.verify());
        assert(verificator.verify());

        ld.push_back('d');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = g1a;
        assert(local_verificator.verify());
        assert(verificator.verify());

        lb = g1b;
        assert(local_verificator.verify());
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = c_short;
        lb = c_long;
        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());
    }

    assert(verificator.verify());

    {
        g1a.push_back('a');
        g1b.push_back('b');
        assert(verificator.verify());

        ASanVerify<S> local_verificator;
        S la = get_s<S>(3, 'l'), lb = get_s<S>(1000, 'k'), lc = g1a, ld = g1b;
        local_verificator.add(la);
        local_verificator.add(lb);
        local_verificator.add(lc);
        local_verificator.add_and_verify(ld);
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');

        lc.push_back('c');
        assert(local_verificator.verify());
        assert(verificator.verify());

        ld.push_back('d');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = g1a;
        assert(local_verificator.verify());
        assert(verificator.verify());

        lb = g1b;
        assert(local_verificator.verify());
        assert(verificator.verify());

        g1a.push_back('a');
        g1b.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());

        g1a.push_back('a');
        g1b.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = c_short;
        lb = c_long;
        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());
    }

    assert(verificator.verify());

    S g2a = g1a, g2b = g1b;
    verificator.add(g2a);
    assert(verificator.add_and_verify(g2b));

    {
        g1a.push_back('a');
        g1b.push_back('b');
        assert(verificator.verify());

        ASanVerify<S> local_verificator;
        S la = get_s<S>(3, 'l'), lb = get_s<S>(1000, 'k'), lc = g1a, ld = g1b;
        local_verificator.add(la);
        local_verificator.add(lb);
        local_verificator.add(lc);
        local_verificator.add_and_verify(ld);
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');

        lc.push_back('c');
        assert(local_verificator.verify());
        assert(verificator.verify());

        ld.push_back('d');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = g1a;
        assert(local_verificator.verify());
        assert(verificator.verify());

        lb = g1b;
        assert(local_verificator.verify());
        assert(verificator.verify());

        g1a.push_back('a');
        g1b.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());

        g1a.push_back('a');
        g1b.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = c_short;
        lb = c_long;
        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());
    }

    assert(verificator.verify());

    S g3a = g1a, g3b = g1b;
    verificator.add(g3a);
    assert(verificator.add_and_verify(g3b));

    {
        g3a.push_back('a');
        g3b.push_back('b');
        assert(verificator.verify());

        ASanVerify<S> local_verificator;
        S la = get_s<S>(3, 'l'), lb = get_s<S>(1000, 'k'), lc = g3a, ld = g3b;
        local_verificator.add(la);
        local_verificator.add(lb);
        local_verificator.add(lc);
        local_verificator.add_and_verify(ld);
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');

        lc.push_back('c');
        assert(local_verificator.verify());
        assert(verificator.verify());

        ld.push_back('d');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = g3a;
        assert(local_verificator.verify());
        assert(verificator.verify());

        lb = g3b;
        assert(local_verificator.verify());
        assert(verificator.verify());

        g3a.push_back('a');
        g3b.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());

        g3a.push_back('a');
        g3b.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = c_short;
        lb = c_long;
        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());
    }

    assert(verificator.verify());

    S g4a = g1a, g4b = g1b;
    verificator.add(g4a);
    assert(verificator.add_and_verify(g4b));

    {
        ASanVerify<S> local_verificator;
        S la = get_s<S>(3, 'l'), lb = get_s<S>(1000, 'k'), lc = g4a, ld = g4b;
        local_verificator.add(la);
        local_verificator.add(lb);
        local_verificator.add(lc);
        local_verificator.add_and_verify(ld);
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');

        lc.push_back('c');
        assert(local_verificator.verify());
        assert(verificator.verify());

        ld.push_back('d');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = g4a;
        assert(local_verificator.verify());
        assert(verificator.verify());

        lb = g4b;
        assert(local_verificator.verify());
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = c_short;
        lb = c_long;
        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());
    }

    assert(verificator.verify());

    {
        ASanVerify<S> local_verificator;
        S la = get_s<S>(3, 'l'), lb = get_s<S>(1000, 'k'), lc = g1a, ld = g1b;
        local_verificator.add(la);
        local_verificator.add(lb);
        local_verificator.add(lc);
        local_verificator.add_and_verify(ld);
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');

        lc.push_back('c');
        assert(local_verificator.verify());
        assert(verificator.verify());

        ld.push_back('d');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = g1a;
        assert(local_verificator.verify());
        assert(verificator.verify());

        lb = g1b;
        assert(local_verificator.verify());
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = c_short;
        lb = c_long;
        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());
    }

    assert(verificator.verify());

    {
        g4a.push_back('a');
        g4b.push_back('b');
        assert(verificator.verify());

        ASanVerify<S> local_verificator;
        S la = get_s<S>(3, 'l'), lb = get_s<S>(1000, 'k'), lc = g3a, ld = g3b;
        local_verificator.add(la);
        local_verificator.add(lb);
        local_verificator.add(lc);
        local_verificator.add_and_verify(ld);
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');

        lc.push_back('c');
        assert(local_verificator.verify());
        assert(verificator.verify());

        ld.push_back('d');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = g3a;
        assert(local_verificator.verify());
        assert(verificator.verify());

        lb = g3b;
        assert(local_verificator.verify());
        assert(verificator.verify());

        g3a.push_back('a');
        g3b.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la.push_back('a');
        lb.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());

        g3a.push_back('a');
        g3b.push_back('b');
        assert(local_verificator.verify());
        assert(verificator.verify());

        la = c_short;
        lb = c_long;
        lc = c_short;
        ld = c_long;
        assert(local_verificator.verify());
        assert(verificator.verify());
    }

    assert(verificator.verify());
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
