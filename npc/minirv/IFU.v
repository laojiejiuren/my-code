module IFU(
    input clk,
    input rst,

    input[31:0] next_pc,
    output[31:0] pc,
    output[31:0] inst
);
    PC u_pc(.clk(clk),.rst(rst),.next_pc(next_pc),.pc(pc));
    assign inst = pmem_read(pc, 32'b0, 32'd4);

    function void pc_get(output bit[31:0] pc_);
        pc_ = pc;
    endfunction
    export "DPI-C" function pc_get;

    function void inst_get(output bit[31:0] inst_ );
        inst_ = inst;
    endfunction
    export "DPI-C" function inst_get;

endmodule
