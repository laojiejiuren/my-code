module WBU(
    input reg_wen_id,
    input jump,
    input branch,
    input mem_ren_id,
    input csr_ren_id,
    input ecall_id,
    input mret_id,

    input [4:0] reg_waddr_id,
    input [31:0] data_mem,
    input [31:0] alu_res,
    input [31:0] pc,
    input [31:0] csr_data,
    input [31:0] csr_mtvec,
    input [31:0] csr_mepc,

    output [31:0] next_pc,
    output reg_wen_wb,
    output [4:0] reg_waddr_wb,
    output reg [31:0] reg_wdata_wb
);

    assign next_pc = mret_id ? csr_mepc : ecall_id ? csr_mtvec : branch ? alu_res : (jump ? (alu_res & ~32'h1): pc + 4);

    assign reg_wen_wb = reg_wen_id;
    assign reg_waddr_wb = reg_waddr_id;

    always@(*)begin
        if(jump)
            reg_wdata_wb = pc + 4;
        else if(mem_ren_id)
            reg_wdata_wb = data_mem;
        else if(csr_ren_id)
            reg_wdata_wb = csr_data;
        else reg_wdata_wb = alu_res;

    end

    function void snpc_get(output bit [31:0] npc_);
        npc_ = next_pc;
    endfunction
    export "DPI-C" function snpc_get;

endmodule
//jalr,alu相加的地址给pc，PC+4给寄存器
