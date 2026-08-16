
//------- 指令类型 opcode -------
`define RISCV32I_I      7'b0010011
`define RISCV32I_R      7'b0110011
`define RISCV32I_U      7'b0110111
`define RISCV32I_B      7'b1100011

`define RISCV32I_auipc  7'b0010111
`define RISCV32I_load   7'b0000011
`define RISCV32I_store  7'b0100011
`define RISCV32I_ja     7'b1100111
`define RISCV32I_jal    7'b1101111
`define RISCV32I_sys    7'b1110011

//------- I型指令 funct3 -------
`define RISCV32I_addi   3'b000
`define RISCV32I_sltiu  3'b011
`define RISCV32I_slti   3'b010
`define RISCV32I_srai   3'b101
`define RISCV32I_slli   3'b001
`define RISCV32I_srli   3'b101
`define RISCV32I_andi   3'b111
`define RISCV32I_xori   3'b100
`define RISCV32I_ori    3'b110

//------- R型指令 funct3 -------
`define RISCV32I_add    3'b000
`define RISCV32I_sub    3'b000
`define RISCV32I_slt    3'b010
`define RISCV32I_sltu   3'b011
`define RISCV32I_xor    3'b100
`define RISCV32I_or     3'b110
`define RISCV32I_and    3'b111
`define RISCV32I_sll    3'b001
`define RISCV32I_srl    3'b101
`define RISCV32I_sra    3'b101

//------- load型指令 funct3 -------
`define RISCV32I_lw     3'b010
`define RISCV32I_lbu    3'b100
`define RISCV32I_lb     3'b000
`define RISCV32I_lh     3'b001
`define RISCV32I_lhu    3'b101

//------- store型指令 funct3 -------
`define RISCV32I_sw     3'b010
`define RISCV32I_sh     3'b001
`define RISCV32I_sb     3'b000

//------- jalr指令 funct3 -------
`define RISCV32I_jalr   3'b000

//------- B型指令 funct3 -------
`define RISCV32I_beq    3'b000
`define RISCV32I_bne    3'b001
`define RISCV32I_bge    3'b101
`define RISCV32I_bgeu   3'b111
`define RISCV32I_blt    3'b100
`define RISCV32I_bltu   3'b110

`define RISCV32I_ebreak 12'b000000000001
