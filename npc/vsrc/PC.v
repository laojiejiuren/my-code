module PC(
    input clk,
    input rst,

    input [31:0] next_pc,
    output reg [31:0] pc
);

always@(posedge clk)begin
    if(rst) begin
        pc <= 32'd0;
    end 
    else pc <= next_pc;
end

endmodule
