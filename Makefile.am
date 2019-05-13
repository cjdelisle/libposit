# what flags you want to pass to the C compiler & linker
CFLAGS = \
    -pedantic \
    -Wall \
    -Wextra \
    -Wno-pointer-sign \
    -Wno-unused-parameter \
    -std=c11 \
    -g \
    -O0 -march=native \
    -Werror -Igenerated -Isrc \
    ${mpfr_CFLAGS}

#-fsanitize=address -lasan

define posit_preprocess
    $(AM_V_CC)$(CC) -E -P -I./src src/templates/gen_$1 -o generated/$1.0
    $(AM_V_at)$(AWK) '{gsub(/LITERAL\(\\n\)/,"\n",$$0);print$$0}' < generated/$1.0 > generated/$1.1
    $(AM_V_at)$(SED) 's/LITERAL(\([^)]*\))/\1/g' < generated/$1.1 > generated/$1
    $(AM_V_at)-rm generated/$1.0 generated/$1.1
endef

generated:
	mkdir generated
generated/posit.h: src/templates/gen_posit.h src/templates/macros.h src/templates/main.h generated Makefile
	$(call posit_preprocess,posit.h)
generated/positgen.h: src/templates/gen_positgen.h src/templates/macros.h src/templates/main.h generated Makefile
	$(call posit_preprocess,positgen.h)
generated/slowimpl.h: src/templates/gen_slowimpl.h src/templates/macros.h src/templates/main.h generated Makefile
	$(call posit_preprocess,slowimpl.h)
generated/useslow.h: src/templates/gen_useslow.h src/templates/macros.h src/templates/main.h generated Makefile
	$(call posit_preprocess,useslow.h)
generated/implement_posit.h: src/templates/gen_implement_posit.h src/templates/macros.h src/templates/main.h generated Makefile
	$(call posit_preprocess,implement_posit.h)
generated/enumerate_implementations.h: src/templates/gen_enumerate_implementations.h src/templates/macros.h src/templates/main.h generated Makefile
	$(call posit_preprocess,enumerate_implementations.h)
generated/useall.h: src/templates/gen_useall.h src/templates/macros.h src/templates/main.h generated Makefile
	$(call posit_preprocess,useall.h)

bin_PROGRAMS =
lib_LIBRARIES =

lib_LIBRARIES += libposit.a
libposit_a_SOURCES = Makefile \
    src/slow/macros.h \
    src/slow/mpz_macros.h \
    src/slow/slowposit.c \
    src/libposit.c \
    src/softposit/c_convertPosit8ToDec.c \
    src/softposit/c_convertPosit16ToDec.c \
    src/softposit/c_convertPosit32ToDec.c \
    src/softposit/c_convertDecToPosit8.c \
    src/softposit/c_convertDecToPosit16.c \
    src/softposit/c_convertDecToPosit32.c \
    src/softposit/p8_add.c \
    src/softposit/p16_add.c \
    src/softposit/p32_add.c \
    src/softposit/p8_sub.c \
    src/softposit/p16_sub.c \
    src/softposit/p32_sub.c \
    src/softposit/s_addMagsP8.c \
    src/softposit/s_addMagsP16.c \
    src/softposit/s_addMagsP32.c \
    src/softposit/s_subMagsP8.c \
    src/softposit/s_subMagsP16.c \
    src/softposit/s_subMagsP32.c \
    src/softposit/p8_mul.c \
    src/softposit/p16_mul.c \
    src/softposit/p32_mul.c \
    src/softposit/p8_div.c \
    src/softposit/p16_div.c \
    src/softposit/p32_div.c \
    src/softposit/p8_sqrt.c \
    src/softposit/p16_sqrt.c \
    src/softposit/p32_sqrt.c \
    src/softposit/s_approxRecipSqrt_1Ks.c \
    src/softposit/i32_to_p8.c \
    src/softposit/i32_to_p16.c \
    src/softposit/i32_to_p32.c \
    src/softposit/ui32_to_p8.c \
    src/softposit/ui32_to_p16.c \
    src/softposit/ui32_to_p32.c \
    src/softposit/i64_to_p8.c \
    src/softposit/i64_to_p16.c \
    src/softposit/i64_to_p32.c \
    src/softposit/ui64_to_p8.c \
    src/softposit/ui64_to_p16.c \
    src/softposit/ui64_to_p32.c \
    src/softposit/p8_to_i32.c \
    src/softposit/p16_to_i32.c \
    src/softposit/p32_to_i32.c \
    src/softposit/p8_to_ui32.c \
    src/softposit/p16_to_ui32.c \
    src/softposit/p32_to_ui32.c \
    src/softposit/p8_to_i64.c \
    src/softposit/p16_to_i64.c \
    src/softposit/p32_to_i64.c \
    src/softposit/p8_to_ui64.c \
    src/softposit/p16_to_ui64.c \
    src/softposit/p32_to_ui64.c \
    src/softposit/p8_to_p16.c \
    src/softposit/p8_to_p32.c \
    src/softposit/p16_to_p8.c \
    src/softposit/p16_to_p32.c \
    src/softposit/p32_to_p8.c \
    src/softposit/p32_to_p16.c
src/libposit.c: generated/posit.h \
    generated/slowimpl.h \
    generated/useslow.h \
    generated/positgen.h \
    generated/implement_posit.h
src/slow/slowposit.c: generated/posit.h

lib_LIBRARIES += libtestutil.a
libtestutil_a_SOURCES = Makefile \
    src/test/util/allimpls.c \
    src/test/util/allimpls.h
src/test/util/allimpls.c: generated/posit.h generated/enumerate_implementations.h generated/useall.h

bin_PROGRAMS += tests/universal_posit8.test
tests_universal_posit8_test_SOURCES = Makefile \
    src/test/universal/posit8.c
src/test/universal/posit8.c: generated/posit.h
tests_universal_posit8_test_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

bin_PROGRAMS += tests/universal_posit16.test
tests_universal_posit16_test_SOURCES = Makefile \
    src/test/universal/posit16.c
src/test/universal/posit16.c: generated/posit.h
tests_universal_posit16_test_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

bin_PROGRAMS += tests/universal_posit32.test
tests_universal_posit32_test_SOURCES = Makefile \
    src/test/universal/posit32.c
src/test/universal/posit32.c: generated/posit.h
tests_universal_posit32_test_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

bin_PROGRAMS += tests/universal_posit64.test
tests_universal_posit64_test_SOURCES = Makefile \
    src/test/universal/posit64.c
src/test/universal/posit64.c: generated/posit.h
tests_universal_posit64_test_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

bin_PROGRAMS += tests/math.test
tests_math_test_SOURCES = Makefile \
    src/test/math/mathtest.c \
    src/test/math/mathtest.h \
    src/test/math/mathhdr.h
src/test/math/mathtest.c: generated/posit.h
tests_math_test_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

bin_PROGRAMS += tests/rounding.test
tests_rounding_test_SOURCES = Makefile \
    src/slow/mpz_macros.h \
    src/test/rounding/posit_macros_impl.h \
    src/test/rounding/roundingtest.c
src/test/rounding/roundingtest.c: generated/posit.h
tests_rounding_test_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

bin_PROGRAMS += tests/perf_measure
tests_perf_measure_SOURCES = Makefile \
    src/test/perf_measure.c
src/test/perf_measure.c: generated/posit.h
tests_perf_measure_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

bin_PROGRAMS += tests/fuzz
tests_fuzz_SOURCES = Makefile \
    src/test/fuzz/fuzz.c
src/test/perf_measure.c: generated/posit.h
tests_fuzz_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

bin_PROGRAMS += tests/convert_toint
tests_convert_toint_SOURCES = Makefile \
    src/test/convert/toint.c
src/test/perf_measure.c: generated/posit.h
tests_convert_toint_LDADD = libposit.a libtestutil.a ${mpfr_LIBS}

TESTS= \
    tests/rounding.test \
    tests/math.test \
    tests/universal_posit16.test \
    tests/universal_posit32.test \
    tests/universal_posit64.test \
    tests/universal_posit8.test
