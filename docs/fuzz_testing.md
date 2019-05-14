# Fuzz testing

Fuzz testing is testing of software with random inputs. In the case of libposit, fuzzing
is used to verify that for a given random input, all implementations of a function return
the same result.

If you invoke `./tests/fuzz`, it will begin *naive fuzzing*, this is just making up random
values and testing them. It's quick and effective at catching obvious bugs but it is not
nearly as powerful as fuzzing with
[AFL](https://en.wikipedia.org/wiki/American_fuzzy_lop_(fuzzer)).

The AFL fuzzer uses a genetic algorithm to *guide* the generation of inputs toward those
which exercise the widest variety of different code-paths. While it is not exactly AI,
it has an uncanny ability to discover valid inputs which cause code to behave in unexpected
ways.

However, fuzz testing requires significant processor resources, so if you would like to
contribute without writing code, you can do so by joining the fuzz testing effort.

## Building an AFL-instrumented version of libposit

First you'll need to install AFL, this is not the place to describe how you should install
AFL on your machine, you can learn the ropes with AFL from
[Hunting For Bugs With AFL 101 - A PRIMER](https://research.aurainfosec.io/hunting-for-bugs-101/).

Once you have AFL on your machine, you can re-run `./configure` with `CC=afl-gcc` or
(if you built `afl-clang-fast`) then `CC=afl-clang-fast`.

What it looks like on a mac:

```
$ CC=~/wrk/afl-2.52b/afl-clang PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ ./configure
$ make
```

## Fuzzing
To start fuzzing, start up AFL with the paths to the fuzz inputs and fuzz outputs:

```
afl-fuzz -i ./src/test/fuzz_inputs/ -o ./generated/fuzz_outputs/ -- ./tests/fuzz --afl
```

You should see the afl-fuzz screen come up and show you the stats.

## Exploring crashes
When crashes are detected, AFL will output files to `./generated/fuzz_outputs/crashes/`.
You can reproduce each crash by executing
`./tests/fuzz --afl < ./generated/fuzz_outputs/crashes/<file>`.

You can run all of them at once with the following command:

```
ls ./generated/fuzz_outputs/crashes/ | while read x; do ./tests/fuzz --afl < ./generated/fuzz_outputs/crashes/$x; done
```

