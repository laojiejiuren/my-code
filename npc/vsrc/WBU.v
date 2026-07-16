module WBU(
    input reg_wen_id,
    input jump,
    input mem_ren_id,
    input [4:0] reg_waddr_id,
    input [31:0] data_mem,
    input [31:0] alu_res,
    input [31:0] pc,
    
    output [31:0] next_pc,
    output reg_wen_wb,
    output [4:0] reg_waddr_wb,
    output reg [31:0] reg_wdata_wb
);

    assign next_pc = jump ? (alu_res & ~32'h1): pc + 4;
    assign reg_wen_wb = reg_wen_id;
    assign reg_waddr_wb = reg_waddr_id;

    always@(*)begin
        if(jump)
            reg_wdata_wb = pc + 4;
        else if(mem_ren_id)
            reg_wdata_wb = data_mem;
        else reg_wdata_wb = alu_res;

    end

endmodule
//jalr,alu相加的地址给pc，PC+4给寄存器
