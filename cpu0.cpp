/**
*
*	CPU0 Simulator In C++
*
*	Author	aaaddress1@gmail.com aka adr
*	Date	2016/10/28
*
**/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "cpu0_op.h"

void simu(context *currContext)
{
	/* fetch code to cpu0 syntax */
	uint8_t op, ra, rb, rc, by;
	uint32_t ir, cx5, cx12, cx16, cx24;
	ir = currContext->ir;
	op = (ir & 0xFF000000) >> 4*6;
	ra = (ir & 0x00F00000) >> 4*5;
	rb = (ir & 0x000F0000) >> 4*4;
	rc = (ir & 0x0000F000) >> 4*3;

	/* Status Word */
	uint32_t sw;
	uint32_t n, z, c, v;
	sw = currContext->r[12];
	n = (sw & 0xF0000000) >> (28 + 3);
	z = (sw & 0x70000000) >> (28 + 2);
	c = (sw & 0x30000000) >> (28 + 1);
	c = (sw & 0x10000000) >> (28 + 0);

	cx5 = ir & 0x0000001F;
	cx12 = ((int32_t)ir << 4*5) >> 4*5;
	cx16 = ((int32_t)ir << 4*4) >> 4*4;
	cx24 = ((int32_t)ir << 4*2) >> 4*2;

	/* simulator opcode case each operation */
	switch (op)
	{
		/* init jump table for branch */
		case op_ret:
			printf("%08x ==> ret\n", ir);
			currContext->r[15] = currContext->r[14];
			break;
		case op_call:
			printf("%08x ==> call %d\n", ir, cx24);
			currContext->r[14] = currContext->r[15];
			currContext->r[15] += cx24;
			break;
		case op_swi5:
			printf("%08x ==> swi %d\n", ir, cx24);
			currContext->r[14] = currContext->r[15];
			currContext->r[15] = cx24;
			break;
		case op_jmp:
			printf("%08x ==> jmp %d\n", ir, cx24);
			currContext->r[15] += cx24;
			break;
		case op_jge:
			printf("%08x ==> jge %d\n", ir, cx24);
			if (!n|z) currContext->r[15] += cx24;
			break;
		case op_jle:
			printf("%08x ==> jle %d\n", ir, cx24);
			if (n|z) currContext->r[15] += cx24;
			break;
		case op_jgt:
			printf("%08x ==> jgt %d\n", ir, cx24);
			if (!n) currContext->r[15] += cx24;
			break;
		case op_jlt:
			printf("%08x ==> jlt %d\n", ir, cx24);
			if (n) currContext->r[15] += cx24;
			break;
		case op_jne:
			printf("%08x ==> jne %d\n", ir, cx24);
			if (!z) currContext->r[15] += cx24;
			break;
		case op_jeq5:
			printf("%08x ==> jeq %d\n", ir, cx24);
			break;

		/* init jump table for syntax L - load & store operation */
		case op_ldi:
			printf("%08x ==> ldi r%d, r%d%+d\n", ir, ra, rb, cx16);
			currContext->r[ra] = (uint16_t)(currContext->r[rb] + (size_t)cx16);
			break;
		case op_ldb:
			printf("%08x ==> ldb r%d, [r%d%+d]\n", ir, ra, rb, cx16);
			currContext->r[ra] = *(uint8_t *)(currContext->memoryBase + currContext->r[rb] + (size_t)cx16);
			break;
		case op_stb:
			printf("%08x ==> stb r%d, [r%d%+d]\n", ir, ra, rb, cx16);
			*(uint8_t *)(currContext->memoryBase + currContext->r[rb] + (size_t)cx16) = currContext->r[ra];
			break;
		case op_st:
			printf("%08x ==> st r%d, [r%d%+d]\n", ir, ra, rb, cx16);
			*(uint16_t *)(currContext->memoryBase + currContext->r[rb] + (size_t)cx16) = currContext->r[ra];
			break;
		case op_ld2:
			printf("%08x ==> ld r%d, [r%d+%d]\n", ir, ra, rb, cx16);
			currContext->r[ra] = *(uint16_t *)(currContext->memoryBase + currContext->r[rb] + (size_t)cx16);
			break;

		/* init jump table for syntax A - stack operation */
		case op_popb:
			printf("%08x ==> popb r%d\n", ir, ra);
			currContext->r[ra] = *(uint8_t*)(currContext->memoryBase+currContext->r[13]);
			currContext->r[13] += sizeof(uint16_t);
			break;
		case op_pushb:
			printf("%08x ==> pushb r%d\n", ir, ra);
			currContext->r[13] -= sizeof(uint16_t);
			*(uint8_t*)(currContext->memoryBase+currContext->r[13]) = (uint8_t)currContext->r[ra];
			break;
		case op_pop:
			printf("%08x ==> pop r%d\n", ir, ra);
			currContext->r[ra] = *(uint16_t*)(currContext->memoryBase+currContext->r[13]);
			currContext->r[13] += sizeof(uint16_t);
			break;
		case op_push:
			printf("%08x ==> push r%d\n", ir, ra);
			currContext->r[13] -= sizeof(uint16_t);
			*(uint16_t*)(currContext->memoryBase+currContext->r[13]) = currContext->r[ra];
			break;

		/* init jump table for syntax A - generally operation */
		case op_shl:
			printf("%08x ==> shl r%d, r%d, %d\n", ir, ra, rb, cx5);
			by = cx5;
			currContext->r[ra] = currContext->r[rb] << by;
			break;
		case op_shr:
			printf("%08x ==> shr r%d, r%d, %d\n", ir, ra, rb, cx5);
			by = cx5;
			currContext->r[ra] = currContext->r[rb] >> by;
			break;
		case op_ror:
			printf("%08x ==> ror r%d, r%d, %d\n", ir, ra, rb, cx5);
			by = cx5 % sizeof(uint16_t); 
			currContext->r[ra] = (currContext->r[rb] << by)|(currContext->r[rb] >> (sizeof(uint16_t)-by));
			break;
		case op_rol4:
			printf("%08x ==> rol r%d, r%d, %d\n", ir, ra, rb, cx5);
			by = cx5 % sizeof(uint16_t); 
			currContext->r[ra] = (currContext->r[rb] >> by)|(currContext->r[rb] << (sizeof(uint16_t)-by));
			break;
		case op_and:
			printf("%08x ==> and r%d, r%d, r%d\n", ir, ra, rb, rc);
			currContext->r[ra] = currContext->r[rb] & currContext->r[rc];
			break;
		case op_or:
			printf("%08x ==> or r%d, r%d, r%d\n", ir, ra, rb, rc);
			currContext->r[ra] = currContext->r[rb] | currContext->r[rc];
			break;
		case op_xor:
			printf("%08x ==> xor r%d, r%d, r%d\n", ir, ra, rb, rc);
			currContext->r[ra] = currContext->r[rb] ^ currContext->r[rc];
			break;
		case op_div:
			printf("%08x ==> div r%d, r%d, r%d\n", ir, ra, rb, rc);
			currContext->r[ra] = currContext->r[rb] / currContext->r[rc];
			break;
		case op_mul:
			printf("%08x ==> mul r%d, r%d, r%d\n", ir, ra, rb, rc);
			currContext->r[ra] = currContext->r[rb] * currContext->r[rc];
			break;
		case op_mov:
			printf("%08x ==> mov r%d, r%d\n", ir, ra, rb);
			currContext->r[ra] = currContext->r[rb];
			break;
		case op_cmp3:
			printf("%08x ==> cmp r%d, r%d\n", ir, ra, rb);
			n = (((int32_t)currContext->r[ra] - (int32_t)currContext->r[rb]) < 0) & 0x01;
			z = (!(currContext->r[ra] - currContext->r[rb])) & 0x01;
			c = false;
			v = false;
			currContext->r[12] = (n << 31 | z << 30 | c << 29 | v << 28);
			break;

		/* init jump table for syntax A - load & store operation */
		case op_sbr:
			printf("%08x ==> sbr r%d, [r%d+r%d]\n", ir, ra, rb, rc);
			*(uint8_t *)(currContext->memoryBase + currContext->r[rb] + currContext->r[rc]) = currContext->r[ra];
			break;
		case op_lbr:
			printf("%08x ==> lbr r%d, [r%d+r%d]\n", ir, ra, rb, rc);
			currContext->r[ra] = *(uint8_t *)(currContext->memoryBase + currContext->r[rb] + currContext->r[rc]);
			break;
		case op_str:
			printf("%08x ==> str r%d, [r%d+r%d]\n", ir, ra, rb, rc);
			*(uint16_t *)(currContext->memoryBase + currContext->r[rb] + currContext->r[rc]) = currContext->r[ra];
			break;
		case op_ldr:
			printf("%08x ==> ldr r%d, [r%d+r%d]\n", ir, ra, rb, rc);
			currContext->r[ra] = *(uint16_t *)(currContext->memoryBase + currContext->r[rb] + currContext->r[rc]);
			break;
		case op_add:
			currContext->r[ra] = currContext->r[rb] + currContext->r[rc];
			printf("%08x ==> add r%d, r%d, r%d\n", ir, ra, rb, rc);
			break;
		case op_sub:
			currContext->r[ra] = currContext->r[rb] - currContext->r[rc];
			printf("%08x ==> sub r%d, r%d, r%d\n", ir, ra, rb, rc);
			break;
		default:
			puts("unimplemented or invalid opcode. (opcode out of bounds)");
			break;
	}


}


bool loadExeFile(char *filePath,context *currContext) 
{
	FILE *fin;
	fin = fopen(filePath, "r");
	if (!fin) {
		puts("Reading Executable File Fail.");
		return false;
	}

	fseek(fin, 0, SEEK_END);
	currContext->exeFileLen = ftell(fin);
	rewind(fin);

	if (currContext->exeFileLen % 4) {
		puts("CPU0 Executable File Padding Error!");
		fclose(fin);
		return false;
	}

	currContext->memoryBase = (uint8_t *)calloc(0xFFFFFFFF, sizeof(uint8_t));
	currContext->exeEntry = 0x400000;
	currContext->r[15] = currContext->exeEntry;//update program counter

	/* allow memory for sp */
	currContext->startOfStack = (uint8_t *)((size_t)currContext->memoryBase + 0x000000);
	currContext->endOfStack = (uint8_t *)((size_t)currContext->memoryBase + currContext->exeEntry);
	currContext->r[13] = currContext->exeEntry;

	fread
	(
		(uint8_t *)((size_t)currContext->memoryBase + currContext->r[15]), 
		currContext->exeFileLen, 
		sizeof(uint8_t), 
		fin
	); 
	fclose(fin);
	return true;
}


int main(void) 
{
	context *thContext = (context *)calloc(sizeof(context), sizeof(uint8_t));
	char filePath[] = "bin";

	if (!loadExeFile(filePath, thContext)) {
		puts("There're some problems during reading Executable file.");
		free(thContext);
		exit(1);
	}

	printf("Address Opcode ==> Disassembly\n");
	while (thContext->r[15] < (thContext->exeEntry+thContext->exeFileLen)) {
		printf("0x%08x ", thContext->r[15]);
		thContext->ir = *(uint32_t *)((size_t)thContext->memoryBase + thContext->r[15]);//fetch code
		thContext->r[15] += sizeof(uint32_t);//update program counter
		simu(thContext);
	}

	puts("\n==========\t\tRegisters\t\t==========");
	for (int i = 0; i < 16; ++i)
		printf("r%d = %x\n", i, thContext->r[i]);

	puts("==========\t\tStack\t\t==========");
	for (size_t i = 0, currStack; i < 5; ++i) {
		currStack = (size_t)thContext->memoryBase + thContext->r[13] + sizeof(uint16_t)*i ;
		if (currStack >= (size_t)thContext->endOfStack || currStack <= (size_t)thContext->startOfStack)
			printf("[sp + %lu] = *Dirty Space*\n", sizeof(uint16_t)*i);
		else
			printf("[sp + %lu] = %x\n", i, *(uint16_t *)currStack);
	}
	return 0;
}
