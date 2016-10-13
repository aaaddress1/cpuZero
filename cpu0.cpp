/**
*
*	CPU0 Simulator In C++
*
*	Author	aaaddress1@gmail.com aka adr
*	Date	2016/10/23
*
**/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "cpu0_op.h"

void simu(uint32_t ir)
{
	/* fetch code to cpu0 syntax */
	uint8_t op, ra, rb, rc;
	uint32_t cx;
	op = (ir & 0xff000000) >> 4*6;
	ra = (ir & 0x00F00000) >> 4*5;
	rb = (ir & 0x000F0000) >> 4*4;
	rc = (ir & 0x0000F000) >> 4*3;
	cx = (ir & 0x00000fff);

	/* deal with negative numbers */
	if (cx >> 11)
		cx = cx | 0xffffff00;

	/* simulator jump table to case each operation */
	static const void* jmpTable[0x34] = {};
	static bool inited = false;
	if (!inited) {

		/* init jump table for syntax A - load & store operation */
		jmpTable[ op_add ] = &&simu_op_add;
		jmpTable[ op_sub ] = &&simu_op_sub;
		jmpTable[ op_ldr ] = &&simu_op_ldr;
		jmpTable[ op_str ] = &&simu_op_str;
		jmpTable[ op_lbr ] = &&simu_op_lbr;
		jmpTable[ op_sbr ] = &&simu_op_sbr;

		/* init jump table for syntax A - generally operation */
		jmpTable[ op_cmp3 ]= &&simu_op_cmp3;
		jmpTable[ op_mov ] = &&simu_op_mov;
		jmpTable[ op_mul ] = &&simu_op_mul;
		jmpTable[ op_div ] = &&simu_op_div;
		jmpTable[ op_and ] = &&simu_op_and;
		jmpTable[ op_or  ] = &&simu_op_or;
		jmpTable[ op_xor ] = &&simu_op_xor;
		jmpTable[ op_rol4 ]= &&simu_op_rol4;
		jmpTable[ op_ror ] = &&simu_op_ror;
		jmpTable[ op_shl ] = &&simu_op_shl;
		jmpTable[ op_shr ] = &&simu_op_shr;

		/* init jump table for syntax A - stack operation */
		jmpTable[ op_push] = &&simu_op_push;
		jmpTable[ op_pop ] = &&simu_op_pop;
		jmpTable[ op_pushb]= &&simu_op_pushb;
		jmpTable[ op_popb] = &&simu_op_popb;

		/* init jump table for syntax L - load & store operation */
		jmpTable[ op_ld2 ] = &&simu_op_ld2;
		jmpTable[ op_st ]  = &&simu_op_st;
		jmpTable[ op_ldb ] = &&simu_op_ldb;
		jmpTable[ op_stb ] = &&simu_op_stb;
		jmpTable[ op_ldi ] = &&simu_op_ldi;

		/* init jump table for branch */
		jmpTable[ op_jeq5]= &&simu_op_jeq5;
		jmpTable[ op_jne] = &&simu_op_jne;
		jmpTable[ op_jlt] = &&simu_op_jlt;
		jmpTable[ op_jgt] = &&simu_op_jgt;
		jmpTable[ op_jle] = &&simu_op_jle;
		jmpTable[ op_jge] = &&simu_op_jge;
		jmpTable[ op_jmp] = &&simu_op_jmp;
		jmpTable[ op_swi5]= &&simu_op_swi5;
		jmpTable[ op_call]= &&simu_op_call;
		jmpTable[ op_ret] = &&simu_op_ret;
	}
	if ((size_t)jmpTable[op])
		goto *jmpTable[op];

	puts("unimplemented or invalid opcode");
	return;
simu_op_ret:
	printf("%08x ==> ret\n", ir);
	return;
simu_op_call:
	printf("%08x ==> call %d\n", ir, cx);
	return;
simu_op_swi5:
	printf("%08x ==> swi %d\n", ir, cx);
	return;
simu_op_jmp:
	printf("%08x ==> jmp %d\n", ir, cx);
	return;
simu_op_jge:
	printf("%08x ==> jge %d\n", ir, cx);
	return;
simu_op_jle:
	printf("%08x ==> jle %d\n", ir, cx);
	return;
simu_op_jgt:
	printf("%08x ==> jgt %d\n", ir, cx);
	return;
simu_op_jlt:
	printf("%08x ==> jlt %d\n", ir, cx);
	return;
simu_op_jne:
	printf("%08x ==> jne %d\n", ir, cx);
	return;
simu_op_jeq5:
	printf("%08x ==> jeq %d\n", ir, cx);
	return;
simu_op_ldi:
	printf("%08x ==> ldi r%d, r%d%d\n", ir, ra, rb, cx);
	return;
simu_op_ldb:
	printf("%08x ==> ldb r%d, [r%d%d]\n", ir, ra, rb, cx);
	return;
simu_op_stb:
	printf("%08x ==> stb r%d, [r%d%d]\n", ir, ra, rb, cx);
	return;
simu_op_st:
	printf("%08x ==> st r%d, [r%d%d]\n", ir, ra, rb, cx);
	return;
simu_op_ld2:
	printf("%08x ==> ld r%d, [r%d%d]\n", ir, ra, rb, cx);
	return;
simu_op_popb:
	printf("%08x ==> popb r%d\n", ir, ra);
	return;
simu_op_pushb:
	printf("%08x ==> pushb r%d\n", ir, ra);
	return;
simu_op_pop:
	printf("%08x ==> pop r%d\n", ir, ra);
	return;
simu_op_push:
	printf("%08x ==> push r%d\n", ir, ra);
	return;
simu_op_shl:
	printf("%08x ==> shl r%d, r%d, %d\n", ir, ra, rb, cx);
	return;
simu_op_shr:
	printf("%08x ==> shr r%d, r%d, %d\n", ir, ra, rb, cx);
	return;
simu_op_ror:
	printf("%08x ==> ror r%d, r%d, %d\n", ir, ra, rb, cx);
	return;
simu_op_rol4:
	printf("%08x ==> rol r%d, r%d, %d\n", ir, ra, rb, cx);
	return;
simu_op_and:
	printf("%08x ==> and r%d, r%d, r%d\n", ir, ra, rb, rc);
	return;
simu_op_or:
	printf("%08x ==> or r%d, r%d, r%d\n", ir, ra, rb, rc);
	return;
simu_op_xor:
	printf("%08x ==> xor r%d, r%d, r%d\n", ir, ra, rb, rc);
	return;
simu_op_div:
	printf("%08x ==> div r%d, r%d, r%d\n", ir, ra, rb, rc);
	return;
simu_op_mul:
	printf("%08x ==> mul r%d, r%d, r%d\n", ir, ra, rb, rc);
	return;
simu_op_mov:
	printf("%08x ==> mov r%d, r%d\n", ir, ra, rb);
	return;
simu_op_cmp3:
	printf("%08x ==> cmp r%d, r%d\n", ir, ra, rb);
	return;
simu_op_sbr:
	printf("%08x ==> sbr r%d, [r%d+r%d]\n", ir, ra, rb, rc);
	return;
simu_op_lbr:
	printf("%08x ==> lbr r%d, [r%d+r%d]\n", ir, ra, rb, rc);
	return;
simu_op_str:
	printf("%08x ==> str r%d, [r%d+r%d]\n", ir, ra, rb, rc);
	return;
simu_op_ldr:
	printf("%08x ==> ldr r%d, [r%d+r%d]\n", ir, ra, rb, rc);
	return;
simu_op_add:
	printf("%08x ==> add r%d, r%d, r%d\n", ir, ra, rb, rc);
	return;
simu_op_sub:
	printf("%08x ==> sub r%d, r%d, r%d\n", ir, ra, rb, rc);
	return;
}


int main(void)
{
	simu(0x13BA1000);
	simu(0x14BA1000);
	simu(0x00120fff);
	simu(0x08120fb0);
	simu(0x220000b0);
	return 0;
}
