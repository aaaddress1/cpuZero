
/* CPU0 Syntax A => Load & Store Operation */
#define op_ldr	0x04
#define op_str	0x05
#define op_lbr	0x06
#define op_sbr	0x07

/* CPU0 Syntax L => Load & Store Operation */
#define op_ld2	0x00
#define op_st	0x01
#define op_ldb	0x02
#define op_stb	0x03
#define op_ldi	0x08

/* CPU0 Syntax A => Generally Operation */
#define op_cmp3	0x10
#define op_mov	0x12
#define op_add	0x13
#define op_sub	0x14
#define op_mul	0x15
#define op_div	0x16
#define op_and	0x18
#define op_or	0x19
#define op_xor	0x1a
#define op_rol4	0x1c
#define op_ror	0x1d
#define op_shl	0x1e
#define op_shr	0x1f

/* CPU0 Syntax A => Stack Operation */
#define op_push	0x30
#define op_pop	0x31
#define op_pushb 0x32
#define op_popb	0x33

/* CPU0 Syntax J => branch */
#define op_jeq5	0x20
#define op_jne	0x21
#define op_jlt	0x22
#define	op_jgt	0x23 
#define	op_jle	0x24
#define op_jge	0x25
#define op_jmp	0x26
#define op_swi5	0x2a
#define	op_call	0x2b 
#define op_ret	0x2c 