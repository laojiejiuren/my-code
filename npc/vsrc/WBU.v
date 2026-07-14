module WBU(
    input clk,
    input rst,

    input reg_wen_id,
    input [4:0] reg_waddr_id,
    input [31:0] reg_wdata,
    input [31:0] pc,

    output [31:0] next_pc,
    output reg_wen_wb,
    output [4:0] reg_waddr_wb,
    output [31:0] reg_wdata_wb
);

    assign next_pc = pc + 4;
    assign reg_wen_wb = reg_wen_id;
    assign reg_waddr_wb = reg_waddr_id;
    assign reg_wdata_wb = reg_wdata;

endmodule
