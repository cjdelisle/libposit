#include "posit.h"
#include "test/util/allimpls.h"

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char** argv) {
    union {
        posit8_t p8[2];
        posit16_t p16[2];
        posit32_t p32[2];
        posit64_t p64[2];
        uint32_t ui;
        int32_t si;
        uint64_t ul;
        int64_t sl;
        uint16_t shorts[8];
        float f;
        double d;
        long double ld;
    } buf;

    int afl = 0;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "--afl")) {
            afl = 1;
        }
    }
    #ifdef __AFL_INIT
        __AFL_INIT();
    #endif
    if (afl) {
        read(STDIN_FILENO, &buf, sizeof buf);
    } else {
        int seed = time(NULL);
        srand(seed);
        printf("Seed: %d\n", seed);
    }

    do {
        if (!afl) {
            for (int i = 0; i < (int)sizeof buf / 2; i++) { buf.shorts[i] = rand(); }
        }

        #define OP2(__op__) \
            posit__GLUE3(posit8_,__op__,_all)(buf.p8[0], buf.p8[1]); \
            posit__GLUE3(posit16_,__op__,_all)(buf.p16[0], buf.p16[1]); \
            posit__GLUE3(posit32_,__op__,_all)(buf.p32[0], buf.p32[1]); \
            posit__GLUE3(posit64_,__op__,_all)(buf.p64[0], buf.p64[1]);
        OP2(add)
        OP2(sub)
        OP2(mul)
        OP2(div)
        OP2(cmp)
        OP2(equals)
        OP2(cmpabs)
        OP2(add_exact)
        OP2(sub_exact)

        #define OP1(__op__) \
            posit__GLUE3(posit8_,__op__,_all)(buf.p8[0]); \
            posit__GLUE3(posit16_,__op__,_all)(buf.p16[0]); \
            posit__GLUE3(posit32_,__op__,_all)(buf.p32[0]); \
            posit__GLUE3(posit64_,__op__,_all)(buf.p64[0]);
        OP1(sqrt)
        OP1(abs)
        OP1(log)
        OP1(exp)
        OP1(toui)
        OP1(tosi)
        OP1(toul)
        OP1(tof)
        OP1(tod)
        OP1(told)
        OP1(iexp)
        OP1(fract)

        posit8_fromp16_all(buf.p16[0]);
        posit8_fromp32_all(buf.p32[0]);
        posit8_fromp64_all(buf.p64[0]);

        posit16_fromp8_all(buf.p8[0]);
        posit16_fromp32_all(buf.p32[0]);
        posit16_fromp64_all(buf.p64[0]);

        posit32_fromp16_all(buf.p16[0]);
        posit32_fromp8_all(buf.p8[0]);
        posit32_fromp64_all(buf.p64[0]);

        posit64_fromp16_all(buf.p16[0]);
        posit64_fromp32_all(buf.p32[0]);
        posit64_fromp8_all(buf.p8[0]);

        #define FROM(__name__) \
            posit__GLUE3(posit8_from,__name__,_all)(buf.__name__); \
            posit__GLUE3(posit16_from,__name__,_all)(buf.__name__); \
            posit__GLUE3(posit32_from,__name__,_all)(buf.__name__); \
            posit__GLUE3(posit64_from,__name__,_all)(buf.__name__);
        FROM(ld)
        FROM(d)
        FROM(f)
        FROM(ui)
        FROM(si)
        FROM(ul)
        FROM(sl)

    } while (!afl);
}
