# C++ skeleton for Bison

# Copyright (C) 2002-2015, 2018 Free Software Foundation, Inc.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# b4_stack_file
# -------------
# Name of the file containing the stack class, if we want this file.
b4_percent_define_ifdef([[api.stack.file]],
[b4_percent_define_check_values([[[[api.stack.file]],
                                  [[none]]]])],
[b4_defines_if([b4_percent_define_ifdef([[api.location.file]],
                                        [],
                                        [m4_define([b4_stack_file], [stack.hh])])])])


# b4_stack_define
# ---------------
m4_define([b4_stack_define],
[[  /// A stack with random access from its top.
  template <class T, class S = std::vector<T> >
  class stack
  {
  public:
    // Hide our reversed order.
    typedef typename S::reverse_iterator iterator;
    typedef typename S::const_reverse_iterator const_iterator;
    typedef typename S::size_type size_type;

    stack ()
    {
      seq_.reserve (200);
    }

    stack (size_type n)
      : seq_ (n)
    {}

    /// Random access.
    ///
    /// Index 0 returns the topmost element.
    T&
    operator[] (size_type i)
    {
      return seq_[seq_.size () - 1 - i];
    }

    /// Random access.
    ///
    /// Index 0 returns the topmost element.
    const T&
    operator[] (size_type i) const
    {
      return seq_[seq_.size () - 1 - i];
    }

    /// Steal the contents of \a t.
    ///
    /// Close to move-semantics.
    void
    push (YY_MOVE_REF (T) t)
    {
      seq_.push_back (T ());
      operator[](0).move (t);
    }

    void
    pop (size_type n = 1)
    {
      for (; n; --n)
        seq_.pop_back ();
    }

    void
    clear ()
    {
      seq_.clear ();
    }

    size_type
    size () const
    {
      return seq_.size ();
    }

    const_iterator
    begin () const
    {
      return seq_.rbegin ();
    }

    const_iterator
    end () const
    {
      return seq_.rend ();
    }

  private:
    stack (const stack&);
    stack& operator= (const stack&);
    /// The wrapped container.
    S seq_;
  };

  /// Present a slice of the top of a stack.
  template <class T, class S = stack<T> >
  class slice
  {
  public:
    typedef typename S::size_type size_type;
    slice (const S& stack, size_type range)
      : stack_ (stack)
      , range_ (range)
    {}

    const T&
    operator[] (size_type i) const
    {
      return stack_[range_ - i];
    }

  private:
    const S& stack_;
    size_type range_;
  };
]])


m4_ifdef([b4_stack_file],
[b4_output_begin([b4_dir_prefix], [b4_stack_file])[
// Starting with Bison 3.2, this file is useless: the structure it
// used to define is now defined with the parser itself.
//
// To get rid of this file:
// 1. add '%define api.stack.file none'
//     or '%define api.location.file none'
//     or '%define api.location.file "my-loc.hh"' to your grammar file
// 2. add 'require "3.2"' to your grammar file
// 3. remove references to this file from your build system.
]b4_output_end[
]])
