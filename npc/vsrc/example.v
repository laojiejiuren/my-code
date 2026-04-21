module top(
    input clk,
    input reset,


    output reg[7:0] out
);
    reg [7:0] tmp;
    
    always @(posedge clk) begin
        if(reset) begin
            out <= 8'h00;
        end
        else begin
            
        end
    end

endmodule
