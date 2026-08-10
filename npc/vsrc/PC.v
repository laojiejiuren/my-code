module PC(
    input clk,
    input rst,

    input [31:0] next_pc,
    output reg [31:0] pc
);

always@(posedge clk)begin
    if(rst) begin
        pc <= 32'h00000000;
    end 
    else pc <= next_pc;
end

    function void config_base(output bit flag);
        if(rst && pc == 32'h00000000)
            flag = 1;
    endfunction
    export "DPI-C" function config_base;

endmodule
