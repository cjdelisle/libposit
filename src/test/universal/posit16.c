// posit16.c: example test of the posit API for C programs using 16-bit posits
//
// Copyright (C) 2017-2019 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

#define POSIT_NO_GENERICS // MSVC doesn't support _Generic so we'll leave it out from these tests
#include "posit.h"
#include "useall.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	const int maxNr = 1024;
	const int fullState = 256*256;
	posit16_t pa, pb, pc;
	char str[posit16_debug_SIZE];
	bool failures = false;

	// special case values
	pa = posit16_NAR;
	pb = posit16_ZERO;
	pc = posit16_add(pa, pb);
	posit16_debug(str, pc);
	printf("posit value = %s\n", str);

	pa = posit16_NAR;
	pb = posit16_ZERO;
	pc = posit16_sub(pa, pb);
	posit16_debug(str, pc);
	printf("posit value = %s\n", str);

	pa = posit16_NAR;
	pb = posit16_ZERO;
	pc = posit16_mul(pa, pb);
	posit16_debug(str, pc);
	printf("posit value = %s\n", str);

	pa = posit16_NAR;
	pb = posit16_ZERO;
	pc = posit16_div(pa, pb);
	posit16_debug(str, pc);
	printf("posit value = %s\n", str);

	// partial state space
	int fails = 0;

	for (int a = 0; a < maxNr; ++a) {
		pa = posit16_reinterpret(a);
		for (int b = 0; b < maxNr; ++b) {
			pb = posit16_reinterpret(b);
			pc = posit16_add(pa, pb);
			float da, db, dref;
			da = posit16_tof(pa);
			db = posit16_tof(pb);
			dref = da + db;
			posit16_t pref = posit16_fromf(dref);
			if (posit16_cmp(pref, pc)) {
				printf("FAIL: 16.1x%04xp + 16.1x%04xp produced 16.1x%04xp instead of 16.1x%04xp\n",
                    posit16_bits(pa), posit16_bits(pb), posit16_bits(pc), posit16_bits(pref));
				++fails;
			}
		}
	}
	if (fails) {
		printf("addition        FAIL\n");
		failures = true;
	}
	else {
		printf("addition        PASS\n");
	}

	// partial state space
	fails = 0;
	for (int a = 0; a < maxNr; ++a) {
		pa = posit16_reinterpret(a);
		for (int b = 0; b < maxNr; ++b) {
			pb = posit16_reinterpret(b);
			pc = posit16_sub(pa, pb);
			float da, db, dref;
			da = posit16_tof(pa);
			db = posit16_tof(pb);
			dref = da - db;
			posit16_t pref = posit16_fromf(dref);
			if (posit16_cmp(pref, pc)) {
				printf("FAIL: 16.1x%04xp - 16.1x%04xp produced 16.1x%04xp instead of 16.1x%04xp\n",
                    posit16_bits(pa), posit16_bits(pb), posit16_bits(pc), posit16_bits(pref));
				++fails;
			}
		}
	}
	if (fails) {
		printf("subtraction     FAIL\n");
		failures = true;
	}
	else {
		printf("subtraction     PASS\n");
	}

	// partial state space
	fails = 0;
	for (int a = 0; a < maxNr; ++a) {
		pa = posit16_reinterpret(a);
		for (int b = 0; b < maxNr; ++b) {
			pb = posit16_reinterpret(b);
			pc = posit16_mul(pa, pb);
			float da, db, dref;
			da = posit16_tof(pa);
			db = posit16_tof(pb);
			dref = da * db;
			posit16_t pref = posit16_fromf(dref);
			if (posit16_cmp(pref, pc)) {
				printf("FAIL: 16.1x%04xp * 16.1x%04xp produced 16.1x%04xp instead of 16.1x%04xp\n",
                    posit16_bits(pa), posit16_bits(pb), posit16_bits(pc), posit16_bits(pref));
				++fails;
			}
		}
	}
	if (fails) {
		printf("multiplication  FAIL\n");
		failures = true;
	}
	else {
		printf("multiplication  PASS\n");
	}

	// partial state space
	fails = 0;
	for (int a = 0; a < maxNr; ++a) {
		pa = posit16_reinterpret(a);
		for (int b = 0; b < maxNr; ++b) {
			pb = posit16_reinterpret(b);
			pc = posit16_div(pa, pb);
			float da, db, dref;
			da = posit16_tof(pa);
			db = posit16_tof(pb);
			dref = da / db;
			posit16_t pref = posit16_fromf(dref);
			if (posit16_cmp(pref, pc)) {
				printf("FAIL: 16.1x%04xp / 16.1x%04xp produced 16.1x%04xp instead of 16.1x%04xp\n",
                    posit16_bits(pa), posit16_bits(pb), posit16_bits(pc), posit16_bits(pref));
                printf("FAIL: %.20f / %.20f produced %.20f instead of %.20f\n",
                    posit16_tod(pa), posit16_tod(pb), posit16_tod(pc), posit16_tod(pref));
				++fails;
			}
		}
	}
	if (fails) {
		printf("division        FAIL\n");
		failures = true;
	}
	else {
		printf("division        PASS\n");
	}

	// full state space
	fails = 0;
	for (int a = 0; a < fullState; ++a) {   // includes negative numbers
		pa = posit16_reinterpret(a);
		pc = posit16_sqrt(pa);
		double da, dref;
		da = posit16_tod(pa);
		dref = sqrt(da);
		posit16_t pref = posit16_fromd(dref);
		if (posit16_cmp(pref, pc)) {
			printf("FAIL: sqrt(%.20f) produced %.20f instead of %.20f\n",
				posit16_tod(pa), posit16_tod(pc), posit16_tod(pref));
			++fails;
		}
	}
	if (fails) {
		printf("sqrt            FAIL\n");
		failures = true;
	}
	else {
		printf("sqrt            PASS\n");
	}

    fails = 0;
	for (int a = 0; a < fullState; ++a) {   // includes negative numbers
		pa = posit16_reinterpret(a);
		double da = posit16_tod(pa);
        pb = posit16_fromd(da);
        if (posit16_cmp(pa, pb)) {
			printf("FAIL: posit16_tod(%04x) produced %.20f and the reverse produced %04x\n",
				a, da, pb.v);
			++fails;
        }
    }

	// full state space
	fails = 0;
	for (int a = 0; a < fullState; ++a) {   // includes negative numbers
		pa = posit16_reinterpret(a);
		pc = posit16_exp(pa);
		double da, dref;
		da = posit16_tod(pa);
		dref = exp(da);
		posit16_t pref = posit16_fromd(dref);
        if (dref == 0.0) { pref = posit16_MIN; }
		if (posit16_cmp(pref, pc)) {
            printf("FAIL: exp(%.20f %.20f) produced %.20f instead of %.20f %.20f\n",
                posit16_tod(pa), da, posit16_tod(pc), posit16_tod(pref), dref);
            ++fails;
		}
	}
	if (fails) {
		printf("exp             FAIL\n");
		failures = true;
	}
	else {
		printf("exp             PASS\n");
	}

	// full state space
	fails = 0;
	for (int a = 0; a < fullState; ++a) {   // includes negative numbers
		pa = posit16_reinterpret(a);
		pc = posit16_log(pa);
		double da, dref;
		da = posit16_tod(pa);
		dref = log(da);
		posit16_t pref = posit16_fromd(dref);
		if (posit16_cmp(pref, pc)) {
			printf("FAIL: log(%f) produced %f instead of %f\n",
				da, posit16_tod(pc), posit16_tod(pref));
			++fails;
		}
	}
	if (fails) {
		printf("log             FAIL\n");
		failures = true;
	}
	else {
		printf("log             PASS\n");
	}

	return failures > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
