module IFU(
    input clk,
    input rst,

    input[31:0] next_pc,
    input[32:0] pc,
    output[31:0] inst
);
    PC u_pc(.clk(clk),.rst(rst),.next_pc(next_pc),.pc(pc));

    assign inst = pmem_read[pc[31:2]];

endmodule
