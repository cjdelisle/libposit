# How to write an implementation

libposit is designed to make it very easy to write a new implementation of a function.
Here we will show you how to write an implementation of the cmp (compare) function.

## Step 1: decide which functions you would like to implement

After you have built libposit, open up the file `generated/posit.h` and look inside
of it to see which function or functions you would like to implement. This document only
discusses how to implement a function which *exists*, not how to add a new function to
libposit. If you would like to add a new function, please
[open an issue](https://github.com/cjdelisle/libposit/issues) so that we can begin the
conversation about whether that function is worth adding.

## Step 2: pick a name

Now you will need to name your implementation. It makes sense to use your name or the
name of the project which your code is coming from.

Once you create an implementation, you will be in charge of reviewing pull requests to
your implementation so it is easier for the maintainers if you create only one
implementation with all of the functions you wish to implement rather than creating many
separate ones.

## Step 2: create the implementation folder

You can copy the files
[src/cjd/impl.c](https://github.com/cjdelisle/libposit/tree/master/src/cjd/impl.c) and
[src/cjd/impl.h](https://github.com/cjdelisle/libposit/tree/master/src/cjd/impl.h) into
your implementation folder in order to get started.

In impl.h, you will need to change this line:

```c
#define posit_IMPLEMENTATION cjd
```

to the name of your implementation.

## Step 3: write your functions

Be careful that the function signatures match exactly the signatures defined in
`generated/posit.h`, otherwise your implementation won't compile. Also it is recommended
that you name your functions `posit<X>_<OP>_<your_impl_name>` as is done in the cjd
impl, this is the best way to avoid namespace collisions when linking the library.

**CAUTION:** libposit includes the `impl.h` file in weird ways, *DO NOT* add anything to it
except for comments and calls to the `posit<X>_<OP>_REGISTER` and `posit<X>_<OP>_REGISTER_NAMED`
macros. If you need a common header file for your implementation, create another one and include
`impl.h` in that one.

## Step 4: Register your functions

If you name your functions in the standard way (`posit<X>_<OP>_<your_impl_name>`) then you can
simply call the `posit<X>_<OP>_REGISTER` macro, if you used a different name then you must use
the `posit<X>_<OP>_REGISTER_NAMED()` macro to tell libposit which name you used. You can see
[src/cjd/impl.h](https://github.com/cjdelisle/libposit/tree/master/src/cjd/impl.h) for an example
of this.

## Step 5: Register your implementation

There are 2 places where you must register your implementation in order for it to work correctly.

1. Makefile.am
2. src/implementations.h

In Makefile.am, search for the line which says `## Add more implementations just above this line`
and add your implementation there. Your implementation should look something like this:

```make
# <your_impl_name> impl
libposit_a_SOURCES += \
	src/<your_impl>/impl.h \
	src/<your_impl>/impl.c
src/<your_impl>/impl.c: generated/posit.h generated/implement_posit.h
```

Any .c or .h files which are part of your implementation should be included in the list. The last
line is necessary in order to make sure that `make` will generate `generated/posit.h` before it
tries to compile your implementation the first part of it (`src/<your_impl>/impl.c`) can refer to
any .c file in your implementation, just not a .h file.

In src/implementations.h, all you need to do is add an #include line for your impl.h file.

```c
#include "src/<your_impl>/impl.h"
```

## Step 6: Compile!

Usually you don't need to run autogen.sh or ./configure again, it should be enough to type make.

## Step 7: Verify that your implementation is present

run `./tests/perf_measure` and it will do a performance test on every implementation that it is
aware of, under the functions which you implemented, you should see your implementation show up.

This indicates that the cjd implementation of `posit64_cmp()` has been detected:

```
// posit64_cmp
//     cjd                                        - 45 micros
//     slow                                       - 11415 micros
#define posit64_cmp_IMPL cjd
```

## Step 8: Run the tests and make sure your implementation is ok

First run `make check` to run the basic tests. If they're ok, try running `./tests/fuzz` to do
some random testing. Fuzz will run as long as you let it so don't wait for it to finish. Basically
if you see no output then things are good.

If you see this, everything looks good:
```
$ ./tests/fuzz
Seed: 1557824278
```

If you see this, there's a problem:
```
notgay:slowposit user$ ./tests/fuzz
Seed: 1557824440
function posit8_cmp: slow and [cjd] disagree
slow : ffffffff
     : 00000001
 arg0: a8
 arg1: d4
function posit8_cmp: slow and [cjd] disagree
slow : ffffffff
     : 00000001
 arg0: c7
 arg1: 7a
...
```

This is indicating that `posit8_cmp_cjd()` and `posit8_cmp_slow()` are returning different output.

## Step 8: Give it a license

Check that you've put a license header on the files in your implementation, you may use the
[SPDX license identifier](https://spdx.org/licenses/) rather than the entire text. If you are
licensing your work under *MIT OR BSD-3-Clause*, there's nothing else to do. If you choose only
one of those then you'll need to update the
[LICENSE.md](https://github.com/cjdelisle/libposit/blob/master/LICENSE.md) file as well.

**NOTE:** If you want to make an implementation with a different license, please open an issue
to discuss it first before doing work which may be deemed incompatible with the project.

## Step 9: Make a pull request!

If you're new to git, check out [setting up git](https://help.github.com/en/articles/set-up-git).

After you make a pull request, your implementation will be reviewed and fuzz tested more heavily
using the technique described in
[docs/fuzz_testing.md](https://github.com/cjdelisle/libposit/blob/master/docs/fuzz_testing.md).

If everything seems ok, your request will be merged and then after review and testing, any
functions for which your implementation is the fastest, it may become the default.

## FAQ

### Should I contribute a slow implementation ?

So you made an implementation but according to `perf_measure` it's not the fastest, should you
still make a pull request?

**YES** If you believe that your implementation is *correct* then you still help enormously by
contributing it. libposit depends on the correctness of it's mpfr-backed "slow" implementation,
but that implementation can also have bugs, and by having more implementations present, the fuzz
testing effort becomes that much more powerful. Think of it this way: more slow implementations
are helping by "watching the watchers".

### fuzz is showing disagreements but I think my implementation is correct

Please open the PR and explain what's happening, it's entirely possible that you found a bug.

### It doesn't compile

Some things to try:

1. `make clean` and then `make` again
2. `./autogen` and then the relevant `./configure` command for your platform and then `make` again
3. Check that you didn't accidently add anything other than REGISTER calls to `impl.h`
4. Check that every function you implemented has exactly the same signature as the definition in
`generated/posit.h`. If it returns an int32_t, it returns an int32_t, not an int, not an
int_fast32_t.
5. Check that the impl_name in `#define posit_IMPLEMENTATION <impl_name>` from your `impl.h`
matches the name of the folder and the name of all of the function suffixes.
6. Try removing all of the REGISTER lines and see if it works, if it does, add them back one by one
7. Push your work to a branch in git and open and issue on the project which links to it
