module pc(
    input clk,
    input reset,
    input jump,
    input [7:0]jump_addr,
    output reg[7:0] pc
);

    always @(posedge clk) begin
        if(reset) 
            pc <= 8'h00;
        else if(jump)
            pc <= jump_addr;
        else
            pc <= pc + 1;
    end

endmodule
