module IFU(
    input clk,
    input rst,

    input[31:0] next_pc,
    output[31:0] pc
);
    PC u_pc(.clk(clk),.rst(rst),.next_pc(next_pc),.pc(pc));

endmodule
