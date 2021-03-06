# what flags you want to pass to the C compiler & linker
CFLAGS = \
	-pedantic \
	-Wall \
	-Wextra \
	-Wno-pointer-sign \
	-Wno-unused-parameter \
	-std=c11 \
	-g \
	-O3 -march=native \
	-Werror -Igenerated -Isrc \
	${gmpinc}

#-fsanitize=address -lasan

define posit_preprocess
	$(AM_V_CC)$(CC) -E -P -I./src src/templates/gen_$1 -o generated/$1.0
	$(AM_V_at)$(AWK) '{gsub(/LITERAL\(\\n\)/,"\n",$$0);print$$0}' < generated/$1.0 > generated/$1.1
	$(AM_V_at)$(SED) 's/LITERAL(\([^)]*\))/\1/g' < generated/$1.1 > generated/$1
	$(AM_V_at)-rm generated/$1.0 generated/$1.1
endef

generated/.posit_dirstamp:
	mkdir generated
	mkdir generated/fuzz_outputs
	touch generated/.posit_dirstamp
generated/posit.h: src/templates/gen_posit.h src/templates/macros.h src/templates/main.h generated/.posit_dirstamp Makefile
	$(call posit_preprocess,posit.h)
generated/positgen.h: src/templates/gen_positgen.h src/templates/macros.h src/templates/main.h generated/.posit_dirstamp Makefile
	$(call posit_preprocess,positgen.h)
generated/slowimpl.h: src/templates/gen_slowimpl.h src/templates/macros.h src/templates/main.h generated/.posit_dirstamp Makefile
	$(call posit_preprocess,slowimpl.h)
generated/useslow.h: src/templates/gen_useslow.h src/templates/macros.h src/templates/main.h generated/.posit_dirstamp Makefile
	$(call posit_preprocess,useslow.h)
generated/implement_posit.h: src/templates/gen_implement_posit.h src/templates/macros.h src/templates/main.h generated/.posit_dirstamp Makefile
	$(call posit_preprocess,implement_posit.h)
generated/enumerate_implementations.h: src/templates/gen_enumerate_implementations.h src/templates/macros.h src/templates/main.h generated/.posit_dirstamp Makefile
	$(call posit_preprocess,enumerate_implementations.h)
generated/useall.h: src/templates/gen_useall.h src/templates/macros.h src/templates/main.h generated/.posit_dirstamp Makefile
	$(call posit_preprocess,useall.h)

pkgconfig_DATA = src/libposit.pc
include_HEADERS = generated/posit.h
check_PROGRAMS =
lib_LIBRARIES =

lib_LIBRARIES += libposit.a
libposit_a_SOURCES = Makefile \
	src/libposit.c
src/libposit.c: generated/posit.h \
	generated/slowimpl.h \
	generated/useslow.h \
	generated/positgen.h \
	generated/implement_posit.h

# slow impl
libposit_a_SOURCES += \
	src/slow/impl.h \
	src/slow/macros.h \
	src/slow/mpz_macros.h \
	src/slow/slowposit.c
src/slow/slowposit.c: generated/posit.h generated/implement_posit.h

# softposit impl
libposit_a_SOURCES += \
	src/softposit/impl.h \
	src/softposit/softposit.h \
	src/softposit/softposit_types.h \
	src/softposit/primitives.h \
	src/softposit/platform.h \
	src/softposit/internals.h \
	src/softposit/e_pow.c \
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
	src/softposit/ui32_to_p8.c \
	src/softposit/ui32_to_p16.c \
	src/softposit/ui32_to_p32.c \
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
src/softposit/e_pow.c: generated/posit.h generated/implement_posit.h

# cjd impl
libposit_a_SOURCES += \
	src/cjd/impl.h \
	src/cjd/impl.c
src/cjd/impl.c: generated/posit.h generated/implement_posit.h

## Add more implementations just above this line

lib_LIBRARIES += libtestutil.a
libtestutil_a_SOURCES = Makefile \
	src/test/util/allimpls.c \
	src/test/util/allimpls.h
src/test/util/allimpls.c: generated/posit.h generated/enumerate_implementations.h generated/useall.h

check_PROGRAMS += tests/universal_posit8.test
tests_universal_posit8_test_SOURCES = Makefile \
	src/test/universal/posit8.c
src/test/universal/posit8.c: generated/posit.h
tests_universal_posit8_test_LDADD = libtestutil.a libposit.a ${gmplibs} -lm

check_PROGRAMS += tests/universal_posit16.test
tests_universal_posit16_test_SOURCES = Makefile \
	src/test/universal/posit16.c
src/test/universal/posit16.c: generated/posit.h
tests_universal_posit16_test_LDADD = libtestutil.a libposit.a ${gmplibs} -lm

check_PROGRAMS += tests/universal_posit32.test
tests_universal_posit32_test_SOURCES = Makefile \
	src/test/universal/posit32.c
src/test/universal/posit32.c: generated/posit.h
tests_universal_posit32_test_LDADD = libtestutil.a libposit.a ${gmplibs} -lm

check_PROGRAMS += tests/universal_posit64.test
tests_universal_posit64_test_SOURCES = Makefile \
	src/test/universal/posit64.c
src/test/universal/posit64.c: generated/posit.h
tests_universal_posit64_test_LDADD = libtestutil.a libposit.a ${gmplibs} -lm

check_PROGRAMS += tests/math.test
tests_math_test_SOURCES = Makefile \
	src/test/math/mathtest.c \
	src/test/math/mathtest.h \
	src/test/math/mathhdr.h
src/test/math/mathtest.c: generated/posit.h
tests_math_test_LDADD = libtestutil.a libposit.a ${gmplibs}

check_PROGRAMS += tests/rounding.test
tests_rounding_test_SOURCES = Makefile \
	src/slow/mpz_macros.h \
	src/test/rounding/posit_macros_impl.h \
	src/test/rounding/roundingtest.c
src/test/rounding/roundingtest.c: generated/posit.h
tests_rounding_test_LDADD = libtestutil.a libposit.a ${gmplibs}

check_PROGRAMS += tests/perf_measure
tests_perf_measure_SOURCES = Makefile \
	src/test/perf_measure.c
src/test/perf_measure.c: generated/posit.h
tests_perf_measure_LDADD = libtestutil.a libposit.a ${gmplibs}

check_PROGRAMS += tests/fuzz
tests_fuzz_SOURCES = Makefile \
	src/test/fuzz/fuzz.c
src/test/perf_measure.c: generated/posit.h
tests_fuzz_LDADD = libtestutil.a libposit.a ${gmplibs}

check_PROGRAMS += tests/convert_toint
tests_convert_toint_SOURCES = Makefile \
	src/test/convert/toint.c
src/test/perf_measure.c: generated/posit.h
tests_convert_toint_LDADD = libtestutil.a libposit.a ${gmplibs}

check_PROGRAMS += tests/rounding/findmiddle
tests_rounding_findmiddle_SOURCES = Makefile \
	src/test/rounding/findmiddle.c
src/test/rounding/findmiddle.c: generated/posit.h
tests_rounding_findmiddle_LDADD = libtestutil.a libposit.a ${gmplibs}

TESTS= \
	tests/rounding.test \
	tests/math.test \
	tests/universal_posit16.test \
	tests/universal_posit32.test \
	tests/universal_posit64.test \
	tests/universal_posit8.test
