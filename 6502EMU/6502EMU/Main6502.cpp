#include "Main6502.h"

using BYTE = unsigned char;
using WORD = unsigned short;

using u32 = unsigned int;

struct MEM {
	static constexpr u32 MAX_MEM = 1024 * 64;
	BYTE DATA[MAX_MEM];

	void INIT() {
		for (u32 i = 0; i < MAX_MEM; i++) {
			DATA[i] = 0;
		}
	}

	/* READ ONE BYTE */
	BYTE operator[](u32 ADDRESS) const {

		/* ASSERT HERE ADDRESS IS < MAX_MEM */
		return DATA[ADDRESS];
	}
};

struct CPU {

	WORD PC;			// Program Counter (Register)
	WORD SP;			// Stack Pointer (Register)

	BYTE A, X, Y;		// Default Registers (A, X, Y)
	
	BYTE C : 1;			// Status Flag Carry		(SED = Set Carry Flag, CLC = Clear Carry Flag)
	BYTE Z : 1;			// Status Flag Zero			(Will be set if the previous instruction was equal to zero, if accumulator is zero)
	BYTE I : 1;			// Status Flag IRQ Disable	(SEI = Set Interrupt Disable, CLI = Clear Interrupt Disable)
	BYTE D : 1;			// Status Flag Decimal Mode	(SED = Set Decimal Flag "Dont ask", CLD = Clear Decimal Flag)
	BYTE B : 1;			// Status Flag Break		(BReaK instruction)
	BYTE V : 1;			// Status Flag Overflow		(64 + 64 => -128)
	BYTE N : 1;			// Status Flag Negative		(bit 7 = 1)

	void RESET( MEM& memory ) {
		PC = 0xFFFC;
		SP = 0x0100;
		C = Z = I = D = B = V = N = 0;
		A = X = Y = 0;
		memory.INIT();
	}

	BYTE FETCHBYTE(u32& CYCLES, MEM& memory) {
		BYTE DATA = memory[PC];
		PC++;
		CYCLES--;
		return DATA;
	}

	// OPCODES
	static constexpr BYTE
		INS_LDA_IM = 0xA9;

	void EXEC( u32 CYCLES, MEM& memory ) {
		while (CYCLES > 0) {
			BYTE INST = FETCHBYTE( CYCLES, memory );
			switch ( INST ) {
			case INS_LDA_IM: {
				BYTE VALUE = 
					FETCHBYTE(CYCLES, memory);
				A = VALUE;
				Z = (A == 0);
				N = (A & 0b10000000) > 0;
			} break;
			default: {
				printf("INSTRUCTION NOT HANDLED: %d", INST);
			} break;
			}
		}
	}
};

int main() {
	MEM mem;
	CPU cpu;
	cpu.RESET( mem );
	cpu.EXEC( 2, mem );
	return 0;
}