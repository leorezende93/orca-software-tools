/*
 * Implementation file for EXAMPLE-COUNTERS application.
 * Copyright (C) 2018-2019 Anderson Domingues, <ti.andersondomingues@gmail.com>
 * This file is part of project URSA (http://https://github.com/andersondomingues/ursa).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */
 
#include "custom-inst.h"
#include "xcustom.h"
#include "orca-hardware-counters.h"

/*
//alternativa pa implementar instrucoes customX
//https://github.com/riscv/riscv-gnu-toolchain/issues/190#issuecomment-264083212
https://github.com/chenxuhao/rocc_fib/blob/master/tests/xcustom.h
https://github.com/cliffordwolf/picorv32/blob/master/firmware/custom_ops.S  <====
// which is a more fancy way to todo this 
// Should not be inlined, because we expect arguments
// in particular registers.
__attribute__((noinline))
int mac_asm(int a, int b, int c) {
    asm __volatile__ (".word 0x02C5856B\n");
    return a;
}
taken from https://quasilyte.dev/blog/post/riscv32-custom-instruction-and-its-simulation/

ver tese como incluir um coprocessador AES
Design and programming of a coprocessor for a RISC-V architecture
by Davide Pala

*/

void custom_inst(void){ 
	uint32_t a=1,b=2,c;
	float af=1,bf=2,cf;
	register float counter asm("f1");
	char as[20], bs[20], cs[20];

	// EXT_MULT_INST(rd, rs1, rs2)
	// executes c = a * b
	EXT_MULT_INST(c,a,b);
	
	printf("\n\nTesting int multiplication: %d x %d = %d\n",a,b,c);
	if ( c == 2 ){
		printf("[[PASSED]]\n");  
	}else{
		printf("[[FAILED]]\n");
	}	

	a = -1;
	// EXT_MULT_INST(rd, rs1, rs2)
	// executes c = a * b
	EXT_MULT_INST(c,a,b);
	
	printf("\nTesting int multiplication: %d x %d = %d\n",a,b,c);
	if ( c == -2 ){
		printf("[[PASSED]]\n");  
	}else{
		printf("[[FAILED]]\n");
	}

	EXT_FMULT_INST(cf,af,bf);

	ftoa(af,as,4);
	ftoa(bf,bs,4);
	ftoa(cf,cs,4);
	printf("\nTesting float multiplication: %s x %s = %s\n",as,bs,cs);
	if ( (cf < 2.00001) && (cf > 1.9999) ){
		printf("[[PASSED]]\n");  
	}else{
		printf("[[FAILED]]\n");
	}	

	//
	// THIS TEST FAILS. FMULT DOES WORK WHEN NUMBER OS NEGATIVE !!!
	//
	bf = -2.0;
	EXT_FMULT_INST(cf,af,bf);

	ftoa(af,as,4);
	ftoa(bf,bs,4);
	ftoa(cf,cs,4);
	printf("\nTesting float multiplication: %s x %s = %s\n",as,bs,cs);
	if ( (cf < -2.00001) && (cf > -1.9999) ){
		printf("[[PASSED]]\n\n");  
	}else{
		printf("[[FAILED]]\n\n");
	}

	printf("MEM0: writes=%u, reads=%u\n", *M0_COUNTER_STORE, *M0_COUNTER_LOAD);
	printf("MEM1: writes=%u, reads=%u\n", *M1_COUNTER_STORE, *M1_COUNTER_LOAD);
	printf("MEM2: writes=%u, reads=%u\n", *M2_COUNTER_STORE, *M2_COUNTER_LOAD);
	printf("---\n");

	printf("CPU: arith=%u, logical=%u\n",   *CPU_COUNTER_ARITH, *CPU_COUNTER_LOGICAL);
	printf("CPU: shift=%u, branches=%u\n",  *CPU_COUNTER_SHIFT, *CPU_COUNTER_BRANCHES);
	printf("CPU: jumps=%u, loadstore=%u\n", *CPU_COUNTER_JUMPS, *CPU_COUNTER_LOADSTORE);
	printf("CPU: cycles=%u, stalls=%u\n", *CPU_COUNTER_CYCLES_TOTAL, *CPU_COUNTER_CYCLES_STALL);	
	printf("CPU: hosttime=%u\n", *CPU_COUNTER_HOSTTIME);
	printf("---\n");

	hf_kill(hf_selfid());
}



/*
void custom_inst(void){
	uint32_t a=1,b=2,c;

	// executes the custom instruction: ext_mult_inst rd  rs1 rs2
	// performs c = a*b
	asm volatile
	(
		"ext_mult_inst   %[z], %[x], %[y]\n"
		: [z] "=r" (c)
		: [x] "r" (a), [y] "r" (b)
	) ;
	

	if ( c == 2 ){
		printf("\n[[PASSED]]\n");  
	}else{
		printf("\n[[FAILED]]\n");
	}	

	printf("MEM0: writes=%u, reads=%u\n", *M0_COUNTER_STORE, *M0_COUNTER_LOAD);
	printf("MEM1: writes=%u, reads=%u\n", *M1_COUNTER_STORE, *M1_COUNTER_LOAD);
	printf("MEM2: writes=%u, reads=%u\n", *M2_COUNTER_STORE, *M2_COUNTER_LOAD);
	printf("---\n");

	printf("CPU: arith=%u, logical=%u\n",   *CPU_COUNTER_ARITH, *CPU_COUNTER_LOGICAL);
	printf("CPU: shift=%u, branches=%u\n",  *CPU_COUNTER_SHIFT, *CPU_COUNTER_BRANCHES);
	printf("CPU: jumps=%u, loadstore=%u\n", *CPU_COUNTER_JUMPS, *CPU_COUNTER_LOADSTORE);
	printf("CPU: cycles=%u, stalls=%u\n", *CPU_COUNTER_CYCLES_TOTAL, *CPU_COUNTER_CYCLES_STALL);	
	printf("CPU: hosttime=%u\n", *CPU_COUNTER_HOSTTIME);
	printf("---\n");

	hf_kill(hf_selfid());
}
*/



/*
//example taken from 
//https://github.com/tangyibin/goblin-core/blob/master/riscv/gc64-sim/riscv/dummy-rocc-test.c
void custom_inst(void){

	uint32_t x = 123, y = 456, z = 0;
	// load x into accumulator 2 (funct=0)
	asm volatile ("custom0 x0, %0, 2, 0" : : "r"(x));
	// read it back into z (funct=1) to verify it
	asm volatile ("custom0 %0, x0, 2, 1" : "=r"(z));
	//assert(z == x);
	if ( z == x ){
		printf("\n[[PASSED]]\n");  
	}else{
		printf("\n[[FAILED]]\n");
	}	
	// accumulate 456 into it (funct=3)
	asm volatile ("custom0 x0, %0, 2, 3" : : "r"(y));
	// verify it
	asm volatile ("custom0 %0, x0, 2, 1" : "=r"(z));
	if ( z == x+y ){
		printf("\n[[PASSED]]\n");  
	}else{
		printf("\n[[FAILED]]\n");
	}		
	//assert(z == x+y);
	// do it all again, but initialize acc2 via memory this time (funct=2)
	asm volatile ("custom0 x0, %0, 2, 2" : : "r"(&x));
	asm volatile ("custom0 x0, %0, 2, 3" : : "r"(y));
	asm volatile ("custom0 %0, x0, 2, 1" : "=r"(z));
	if ( z == x+y ){
		printf("\n[[PASSED]]\n");  
	}else{
		printf("\n[[FAILED]]\n");
	}		
	//assert(z == x+y);

	printf("MEM0: writes=%u, reads=%u\n", *M0_COUNTER_STORE, *M0_COUNTER_LOAD);
	printf("MEM1: writes=%u, reads=%u\n", *M1_COUNTER_STORE, *M1_COUNTER_LOAD);
	printf("MEM2: writes=%u, reads=%u\n", *M2_COUNTER_STORE, *M2_COUNTER_LOAD);
	printf("---\n");

	printf("CPU: arith=%u, logical=%u\n",   *CPU_COUNTER_ARITH, *CPU_COUNTER_LOGICAL);
	printf("CPU: shift=%u, branches=%u\n",  *CPU_COUNTER_SHIFT, *CPU_COUNTER_BRANCHES);
	printf("CPU: jumps=%u, loadstore=%u\n", *CPU_COUNTER_JUMPS, *CPU_COUNTER_LOADSTORE);
	printf("CPU: cycles=%u, stalls=%u\n", *CPU_COUNTER_CYCLES_TOTAL, *CPU_COUNTER_CYCLES_STALL);	
	printf("CPU: hosttime=%u\n", *CPU_COUNTER_HOSTTIME);
	printf("---\n");

	hf_kill(hf_selfid());
}
*/



/*
void custom_inst(void){

	float a,b,c;
	a = 5.0;
	b = 2.0;

	asm volatile
	(
		"@custom0   \n\t"
	) ;

	asm volatile ("custom0 x0, %0, 2, 0" : : "r"(x));

	// example taken from 
	// https://nitish2112.github.io/post/adding-instruction-riscv/
	// asm volatile
	// (
	// 	"custom0   %[z], %[x], %[y]\n\t"
	// 	: [z] "=r" (c)
	// 	: [x] "r" (a), [y] "r" (b)
	// ) ;


	if ( (c < 10.00001) && (c > 9.9999) ){
		printf("\n[[PASSED]]\n");  
	}else{
		printf("\n[[FAILED]]\n");
	}


	printf("MEM0: writes=%u, reads=%u\n", *M0_COUNTER_STORE, *M0_COUNTER_LOAD);
	printf("MEM1: writes=%u, reads=%u\n", *M1_COUNTER_STORE, *M1_COUNTER_LOAD);
	printf("MEM2: writes=%u, reads=%u\n", *M2_COUNTER_STORE, *M2_COUNTER_LOAD);
	printf("---\n");

	printf("CPU: arith=%u, logical=%u\n",   *CPU_COUNTER_ARITH, *CPU_COUNTER_LOGICAL);
	printf("CPU: shift=%u, branches=%u\n",  *CPU_COUNTER_SHIFT, *CPU_COUNTER_BRANCHES);
	printf("CPU: jumps=%u, loadstore=%u\n", *CPU_COUNTER_JUMPS, *CPU_COUNTER_LOADSTORE);
	printf("CPU: cycles=%u, stalls=%u\n", *CPU_COUNTER_CYCLES_TOTAL, *CPU_COUNTER_CYCLES_STALL);	
	printf("CPU: hosttime=%u\n", *CPU_COUNTER_HOSTTIME);
	printf("---\n");

	hf_kill(hf_selfid());
}
*/
