# what flags you want to pass to the C compiler & linker
CFLAGS = \
    --pedantic \
    -Wall \
    -Wextra \
    -Wno-pointer-sign \
    -Wno-unused-parameter \
    -std=c11 \
    -g \
    -O3 -march=native -Werror

#-fsanitize=address -lasan

bin_PROGRAMS =
lib_LIBRARIES =

lib_LIBRARIES += libslowposit.a
libslowposit_a_SOURCES = Makefile \
    include/slowposit/posit_macros.h \
    include/slowposit/posit_types.h \
    include/slowposit/posit.h \
    src/posit_macros_impl.h \
    src/slowposit.c

libslowposit_a_CFLAGS = -Iinclude

bin_PROGRAMS += tests/roundingtest
bin_roundingtest_SOURCES = Makefile \
    src/optest.c \
    src/Hash.c                  src/Hash.h \
    src/RandGen.c               src/RandGen.h \
    src/RandHash_interpreted.c
bin_roundingtest_CFLAGS = -Iinclude
#bin_roundingtest_LDADD =
