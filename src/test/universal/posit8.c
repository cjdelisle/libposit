// posit8.c: example test of the posit API for C programs using 8-bit posits
//
// Copyright (C) 2017-2019 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

#define POSIT_NO_GENERICS // MSVC doesn't support _Generic so we'll leave it out from these tests
#include "posit.h"
#include "useall.h"
#include <math.h> // sqrt()
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	posit8_t pa, pb, pc;
	char str[posit8_debug_SIZE];
	bool failures = false;

	// special case values
	pa = posit8_NAR;
	pb = posit8_ZERO;
	pc = posit8_add_all(pa, pb);
	posit8_debug(str, pc);
	printf("posit value = %s\n", str);
	printf("posit value = 8.0x%02xp\n", posit8_bits(pc));

	pa = posit8_NAR;
	pb = posit8_ZERO;
	pc = posit8_sub_all(pa, pb);
	posit8_debug(str, pc);
	printf("posit value = %s\n", str);
	printf("posit value = 8.0x%02xp\n", posit8_bits(pc));

	pa = posit8_NAR;
	pb = posit8_ZERO;
	pc = posit8_mul_all(pa, pb);
	posit8_debug(str, pc);
	printf("posit value = %s\n", str);
	printf("posit value = 8.0x%02xp\n", posit8_bits(pc));

	pa = posit8_NAR;
	pb = posit8_ZERO;
	pc = posit8_div_all(pa, pb);
	posit8_debug(str, pc);
	printf("posit value = %s\n", str);
	printf("posit value = 8.0x%02xp\n", posit8_bits(pc));


	// full state space
	int fails = 0;
	for (int a = 0; a < 256; ++a) {
		pa = posit8_reinterpret(a);
		for (int b = 0; b < 256; ++b) {
			pb = posit8_reinterpret(b);
			pc = posit8_add(pa, pb);
			float da, db, dref;
			da = posit8_tof(pa);
			db = posit8_tof(pb);
			dref = da + db;
			posit8_t pref = posit8_fromf(dref);
			if (posit8_bits(pref) != posit8_bits(pc)) {
				printf("FAIL: 8.0x%02xp + 8.0x%02xp produced 8.0x%02xp instead of 8.0x%02xp\n",
                    posit8_bits(pa), posit8_bits(pb), posit8_bits(pc), posit8_bits(pref));
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

	// full state space
	fails = 0;
	for (int a = 0; a < 256; ++a) {
		pa = posit8_reinterpret(a);
		for (int b = 0; b < 256; ++b) {
			pb = posit8_reinterpret(b);
			pc = posit8_sub(pa, pb);
			float da, db, dref;
			da = posit8_tof(pa);
			db = posit8_tof(pb);
			dref = da - db;
			posit8_t pref = posit8_fromf(dref);
			if (posit8_bits(pref) != posit8_bits(pc)) {
				printf("FAIL: 8.0x%02xp - 8.0x%02xp produced 8.0x%02xp instead of 8.0x%02xp\n",
                    posit8_bits(pa), posit8_bits(pb), posit8_bits(pc), posit8_bits(pref));
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

	// full state space
	fails = 0;
	for (int a = 0; a < 256; ++a) {
		pa = posit8_reinterpret(a);
		for (int b = 0; b < 256; ++b) {
			pb = posit8_reinterpret(b);
			pc = posit8_mul(pa, pb);
			float da, db, dref;
			da = posit8_tof(pa);
			db = posit8_tof(pb);
			dref = da * db;
			posit8_t pref = posit8_fromf(dref);
			if (posit8_bits(pref) != posit8_bits(pc)) {
				printf("FAIL: 8.0x%02xp * 8.0x%02xp produced 8.0x%02xp instead of 8.0x%02xp\n",
                    posit8_bits(pa), posit8_bits(pb), posit8_bits(pc), posit8_bits(pref));
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

	// full state space
	fails = 0;
	for (int a = 0; a < 256; ++a) {
		pa = posit8_reinterpret(a);
		for (int b = 0; b < 256; ++b) {
			pb = posit8_reinterpret(b);
			pc = posit8_div(pa, pb);
			float da, db, dref;
			da = posit8_tof(pa);
			db = posit8_tof(pb);
			dref = da / db;
			posit8_t pref = posit8_fromf(dref);
			if (posit8_bits(pref) != posit8_bits(pc)) {
				printf("FAIL: 8.0x%02xp / 8.0x%02xp produced 8.0x%02xp instead of 8.0x%02xp\n",
                    posit8_bits(pa), posit8_bits(pb), posit8_bits(pc), posit8_bits(pref));
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
	for (int a = 0; a < 256; ++a) {   // includes negative numbers
		pa = posit8_reinterpret(a);
		pc = posit8_sqrt(pa);
		double da, dref;
		da = posit8_tod(pa);
		dref = sqrt(da);
		posit8_t pref = posit8_fromd(dref);
		if (posit8_bits(pref) != posit8_bits(pc)) {
			printf("FAIL: sqrt(8.0x%02xp) produced 8.0x%02xp instead of 8.0x%02xp\n",
	    posit8_bits(pa), posit8_bits(pc), posit8_bits(pref));
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

	// full state space
	fails = 0;
	for (int a = 0; a < 256; ++a) {   // includes negative numbers
		pa = posit8_reinterpret(a);
		pc = posit8_exp(pa);
		double da, dref;
		da = posit8_tod(pa);
		dref = exp(da);
		posit8_t pref = posit8_fromd(dref);
		if (posit8_bits(pref) != posit8_bits(pc)) {
			printf("FAIL: exp(8.0x%02xp) produced 8.0x%02xp instead of 8.0x%02xp\n",
				posit8_bits(pa), posit8_bits(pc), posit8_bits(pref));
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
	for (int a = 0; a < 256; ++a) {   // includes negative numbers
		pa = posit8_reinterpret(a);
		pc = posit8_log(pa);
		double da, dref;
		da = posit8_tod(pa);
		dref = log(da);
		posit8_t pref = posit8_fromd(dref);
		if (posit8_bits(pref) != posit8_bits(pc)) {
			printf("FAIL: log(8.0x%02xp) produced 8.0x%02xp instead of 8.0x%02xp\n",
				posit8_bits(pa), posit8_bits(pc), posit8_bits(pref));
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
