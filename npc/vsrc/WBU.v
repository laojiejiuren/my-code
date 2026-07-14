module WBU(
    input clk,
    input rst,

    input reg_wen_id,
    input jump,
    input [4:0] reg_waddr_id,
    input [31:0] alu_res,
    input [31:0] pc,
    
    output [31:0] next_pc,
    output reg_wen_wb,
    output [4:0] reg_waddr_wb,
    output [31:0] reg_wdata_wb
);

    assign next_pc = jump ? alu_res : pc + 4;
    assign reg_wen_wb = reg_wen_id;
    assign reg_waddr_wb = reg_waddr_id;
    assign reg_wdata_wb = jump ? pc + 4 : alu_res;

endmodule
//jalr,alu相加的地址给pc，PC+4给寄存器
