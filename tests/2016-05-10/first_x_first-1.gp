Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 74.39      4.07     4.07   531441     0.00     0.00  naive_multiplication(char*, char*, char*, unsigned int)
 15.76      4.93     0.86  1062880     0.00     0.00  big_number_summation(char*, char*, char*, unsigned int, unsigned int)
  5.50      5.23     0.30   265720     0.00     0.00  big_number_subtraction(char*, char*, char*, unsigned int, unsigned int)
  4.58      5.48     0.25        1     0.25     5.48  _karatsuba(char*, char*, char*, char*, unsigned int)
  0.00      5.48     0.00        3     0.00     0.00  fprint_big_number(_IO_FILE*, char*, unsigned int)
  0.00      5.48     0.00        2     0.00     0.00  fget_big_number(char const*, char*, unsigned int)
  0.00      5.48     0.00        2     0.00     0.00  get_file_length(char const*)
  0.00      5.48     0.00        1     0.00     5.48  karatsuba(char*, char*, char*, unsigned int)

 %         the percentage of the total running time of the
time       program used by this function.

cumulative a running sum of the number of seconds accounted
 seconds   for by this function and those listed above it.

 self      the number of seconds accounted for by this
seconds    function alone.  This is the major sort for this
           listing.

calls      the number of times this function was invoked, if
           this function is profiled, else blank.
 
 self      the average number of milliseconds spent in this
ms/call    function per call, if this function is profiled,
	   else blank.

 total     the average number of milliseconds spent in this
ms/call    function and its descendents per call, if this 
	   function is profiled, else blank.

name       the name of the function.  This is the minor sort
           for this listing. The index shows the location of
	   the function in the gprof listing. If the index is
	   in parenthesis it shows where it would appear in
	   the gprof listing if it were to be printed.

Copyright (C) 2012 Free Software Foundation, Inc.

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

		     Call graph (explanation follows)


granularity: each sample hit covers 2 byte(s) for 0.18% of 5.48 seconds

index % time    self  children    called     name
                                                 <spontaneous>
[1]    100.0    0.00    5.48                 main [1]
                0.00    5.48       1/1           karatsuba(char*, char*, char*, unsigned int) [3]
                0.00    0.00       3/3           fprint_big_number(_IO_FILE*, char*, unsigned int) [13]
                0.00    0.00       2/2           get_file_length(char const*) [15]
                0.00    0.00       2/2           fget_big_number(char const*, char*, unsigned int) [14]
-----------------------------------------------
                              797160             _karatsuba(char*, char*, char*, char*, unsigned int) [2]
                0.25    5.23       1/1           karatsuba(char*, char*, char*, unsigned int) [3]
[2]    100.0    0.25    5.23       1+797160  _karatsuba(char*, char*, char*, char*, unsigned int) [2]
                4.07    0.00  531441/531441      naive_multiplication(char*, char*, char*, unsigned int) [4]
                0.86    0.00 1062880/1062880     big_number_summation(char*, char*, char*, unsigned int, unsigned int) [5]
                0.30    0.00  265720/265720      big_number_subtraction(char*, char*, char*, unsigned int, unsigned int) [6]
                              797160             _karatsuba(char*, char*, char*, char*, unsigned int) [2]
-----------------------------------------------
                0.00    5.48       1/1           main [1]
[3]    100.0    0.00    5.48       1         karatsuba(char*, char*, char*, unsigned int) [3]
                0.25    5.23       1/1           _karatsuba(char*, char*, char*, char*, unsigned int) [2]
-----------------------------------------------
                4.07    0.00  531441/531441      _karatsuba(char*, char*, char*, char*, unsigned int) [2]
[4]     74.2    4.07    0.00  531441         naive_multiplication(char*, char*, char*, unsigned int) [4]
-----------------------------------------------
                0.86    0.00 1062880/1062880     _karatsuba(char*, char*, char*, char*, unsigned int) [2]
[5]     15.7    0.86    0.00 1062880         big_number_summation(char*, char*, char*, unsigned int, unsigned int) [5]
-----------------------------------------------
                0.30    0.00  265720/265720      _karatsuba(char*, char*, char*, char*, unsigned int) [2]
[6]      5.5    0.30    0.00  265720         big_number_subtraction(char*, char*, char*, unsigned int, unsigned int) [6]
-----------------------------------------------
                0.00    0.00       3/3           main [1]
[13]     0.0    0.00    0.00       3         fprint_big_number(_IO_FILE*, char*, unsigned int) [13]
-----------------------------------------------
                0.00    0.00       2/2           main [1]
[14]     0.0    0.00    0.00       2         fget_big_number(char const*, char*, unsigned int) [14]
-----------------------------------------------
                0.00    0.00       2/2           main [1]
[15]     0.0    0.00    0.00       2         get_file_length(char const*) [15]
-----------------------------------------------

 This table describes the call tree of the program, and was sorted by
 the total amount of time spent in each function and its children.

 Each entry in this table consists of several lines.  The line with the
 index number at the left hand margin lists the current function.
 The lines above it list the functions that called this function,
 and the lines below it list the functions this one called.
 This line lists:
     index	A unique number given to each element of the table.
		Index numbers are sorted numerically.
		The index number is printed next to every function name so
		it is easier to look up where the function is in the table.

     % time	This is the percentage of the `total' time that was spent
		in this function and its children.  Note that due to
		different viewpoints, functions excluded by options, etc,
		these numbers will NOT add up to 100%.

     self	This is the total amount of time spent in this function.

     children	This is the total amount of time propagated into this
		function by its children.

     called	This is the number of times the function was called.
		If the function called itself recursively, the number
		only includes non-recursive calls, and is followed by
		a `+' and the number of recursive calls.

     name	The name of the current function.  The index number is
		printed after it.  If the function is a member of a
		cycle, the cycle number is printed between the
		function's name and the index number.


 For the function's parents, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the function into this parent.

     children	This is the amount of time that was propagated from
		the function's children into this parent.

     called	This is the number of times this parent called the
		function `/' the total number of times the function
		was called.  Recursive calls to the function are not
		included in the number after the `/'.

     name	This is the name of the parent.  The parent's index
		number is printed after it.  If the parent is a
		member of a cycle, the cycle number is printed between
		the name and the index number.

 If the parents of the function cannot be determined, the word
 `<spontaneous>' is printed in the `name' field, and all the other
 fields are blank.

 For the function's children, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the child into the function.

     children	This is the amount of time that was propagated from the
		child's children to the function.

     called	This is the number of times the function called
		this child `/' the total number of times the child
		was called.  Recursive calls by the child are not
		listed in the number after the `/'.

     name	This is the name of the child.  The child's index
		number is printed after it.  If the child is a
		member of a cycle, the cycle number is printed
		between the name and the index number.

 If there are any cycles (circles) in the call graph, there is an
 entry for the cycle-as-a-whole.  This entry shows who called the
 cycle (as parents) and the members of the cycle (as children.)
 The `+' recursive calls entry shows the number of function calls that
 were internal to the cycle, and the calls entry for each member shows,
 for that member, how many times it was called from other members of
 the cycle.

Copyright (C) 2012 Free Software Foundation, Inc.

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

Index by function name

   [2] _karatsuba(char*, char*, char*, char*, unsigned int) [13] fprint_big_number(_IO_FILE*, char*, unsigned int) [6] big_number_subtraction(char*, char*, char*, unsigned int, unsigned int)
  [14] fget_big_number(char const*, char*, unsigned int) [5] big_number_summation(char*, char*, char*, unsigned int, unsigned int) [3] karatsuba(char*, char*, char*, unsigned int)
  [15] get_file_length(char const*) [4] naive_multiplication(char*, char*, char*, unsigned int)
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 73.97      4.04     4.04   531441     0.00     0.00  naive_multiplication(char*, char*, char*, unsigned int)
 16.15      4.92     0.88  1062880     0.00     0.00  big_number_summation(char*, char*, char*, unsigned int, unsigned int)
  5.32      5.21     0.29   265720     0.00     0.00  big_number_subtraction(char*, char*, char*, unsigned int, unsigned int)
  4.59      5.46     0.25        1     0.25     5.46  _karatsuba(char*, char*, char*, char*, unsigned int)
  0.18      5.47     0.01        3     0.00     0.00  fprint_big_number(_IO_FILE*, char*, unsigned int)
  0.00      5.47     0.00        2     0.00     0.00  fget_big_number(char const*, char*, unsigned int)
  0.00      5.47     0.00        2     0.00     0.00  get_file_length(char const*)
  0.00      5.47     0.00        1     0.00     5.46  karatsuba(char*, char*, char*, unsigned int)

 %         the percentage of the total running time of the
time       program used by this function.

cumulative a running sum of the number of seconds accounted
 seconds   for by this function and those listed above it.

 self      the number of seconds accounted for by this
seconds    function alone.  This is the major sort for this
           listing.

calls      the number of times this function was invoked, if
           this function is profiled, else blank.
 
 self      the average number of milliseconds spent in this
ms/call    function per call, if this function is profiled,
	   else blank.

 total     the average number of milliseconds spent in this
ms/call    function and its descendents per call, if this 
	   function is profiled, else blank.

name       the name of the function.  This is the minor sort
           for this listing. The index shows the location of
	   the function in the gprof listing. If the index is
	   in parenthesis it shows where it would appear in
	   the gprof listing if it were to be printed.

Copyright (C) 2012 Free Software Foundation, Inc.

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

		     Call graph (explanation follows)


granularity: each sample hit covers 2 byte(s) for 0.18% of 5.47 seconds

index % time    self  children    called     name
                                                 <spontaneous>
[1]    100.0    0.00    5.47                 main [1]
                0.00    5.46       1/1           karatsuba(char*, char*, char*, unsigned int) [3]
                0.01    0.00       3/3           fprint_big_number(_IO_FILE*, char*, unsigned int) [7]
                0.00    0.00       2/2           get_file_length(char const*) [15]
                0.00    0.00       2/2           fget_big_number(char const*, char*, unsigned int) [14]
-----------------------------------------------
                              797160             _karatsuba(char*, char*, char*, char*, unsigned int) [2]
                0.25    5.21       1/1           karatsuba(char*, char*, char*, unsigned int) [3]
[2]     99.8    0.25    5.21       1+797160  _karatsuba(char*, char*, char*, char*, unsigned int) [2]
                4.04    0.00  531441/531441      naive_multiplication(char*, char*, char*, unsigned int) [4]
                0.88    0.00 1062880/1062880     big_number_summation(char*, char*, char*, unsigned int, unsigned int) [5]
                0.29    0.00  265720/265720      big_number_subtraction(char*, char*, char*, unsigned int, unsigned int) [6]
                              797160             _karatsuba(char*, char*, char*, char*, unsigned int) [2]
-----------------------------------------------
                0.00    5.46       1/1           main [1]
[3]     99.8    0.00    5.46       1         karatsuba(char*, char*, char*, unsigned int) [3]
                0.25    5.21       1/1           _karatsuba(char*, char*, char*, char*, unsigned int) [2]
-----------------------------------------------
                4.04    0.00  531441/531441      _karatsuba(char*, char*, char*, char*, unsigned int) [2]
[4]     73.8    4.04    0.00  531441         naive_multiplication(char*, char*, char*, unsigned int) [4]
-----------------------------------------------
                0.88    0.00 1062880/1062880     _karatsuba(char*, char*, char*, char*, unsigned int) [2]
[5]     16.1    0.88    0.00 1062880         big_number_summation(char*, char*, char*, unsigned int, unsigned int) [5]
-----------------------------------------------
                0.29    0.00  265720/265720      _karatsuba(char*, char*, char*, char*, unsigned int) [2]
[6]      5.3    0.29    0.00  265720         big_number_subtraction(char*, char*, char*, unsigned int, unsigned int) [6]
-----------------------------------------------
                0.01    0.00       3/3           main [1]
[7]      0.2    0.01    0.00       3         fprint_big_number(_IO_FILE*, char*, unsigned int) [7]
-----------------------------------------------
                0.00    0.00       2/2           main [1]
[14]     0.0    0.00    0.00       2         fget_big_number(char const*, char*, unsigned int) [14]
-----------------------------------------------
                0.00    0.00       2/2           main [1]
[15]     0.0    0.00    0.00       2         get_file_length(char const*) [15]
-----------------------------------------------

 This table describes the call tree of the program, and was sorted by
 the total amount of time spent in each function and its children.

 Each entry in this table consists of several lines.  The line with the
 index number at the left hand margin lists the current function.
 The lines above it list the functions that called this function,
 and the lines below it list the functions this one called.
 This line lists:
     index	A unique number given to each element of the table.
		Index numbers are sorted numerically.
		The index number is printed next to every function name so
		it is easier to look up where the function is in the table.

     % time	This is the percentage of the `total' time that was spent
		in this function and its children.  Note that due to
		different viewpoints, functions excluded by options, etc,
		these numbers will NOT add up to 100%.

     self	This is the total amount of time spent in this function.

     children	This is the total amount of time propagated into this
		function by its children.

     called	This is the number of times the function was called.
		If the function called itself recursively, the number
		only includes non-recursive calls, and is followed by
		a `+' and the number of recursive calls.

     name	The name of the current function.  The index number is
		printed after it.  If the function is a member of a
		cycle, the cycle number is printed between the
		function's name and the index number.


 For the function's parents, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the function into this parent.

     children	This is the amount of time that was propagated from
		the function's children into this parent.

     called	This is the number of times this parent called the
		function `/' the total number of times the function
		was called.  Recursive calls to the function are not
		included in the number after the `/'.

     name	This is the name of the parent.  The parent's index
		number is printed after it.  If the parent is a
		member of a cycle, the cycle number is printed between
		the name and the index number.

 If the parents of the function cannot be determined, the word
 `<spontaneous>' is printed in the `name' field, and all the other
 fields are blank.

 For the function's children, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the child into the function.

     children	This is the amount of time that was propagated from the
		child's children to the function.

     called	This is the number of times the function called
		this child `/' the total number of times the child
		was called.  Recursive calls by the child are not
		listed in the number after the `/'.

     name	This is the name of the child.  The child's index
		number is printed after it.  If the child is a
		member of a cycle, the cycle number is printed
		between the name and the index number.

 If there are any cycles (circles) in the call graph, there is an
 entry for the cycle-as-a-whole.  This entry shows who called the
 cycle (as parents) and the members of the cycle (as children.)
 The `+' recursive calls entry shows the number of function calls that
 were internal to the cycle, and the calls entry for each member shows,
 for that member, how many times it was called from other members of
 the cycle.

Copyright (C) 2012 Free Software Foundation, Inc.

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

Index by function name

   [2] _karatsuba(char*, char*, char*, char*, unsigned int) [7] fprint_big_number(_IO_FILE*, char*, unsigned int) [6] big_number_subtraction(char*, char*, char*, unsigned int, unsigned int)
  [14] fget_big_number(char const*, char*, unsigned int) [5] big_number_summation(char*, char*, char*, unsigned int, unsigned int) [3] karatsuba(char*, char*, char*, unsigned int)
  [15] get_file_length(char const*) [4] naive_multiplication(char*, char*, char*, unsigned int)
