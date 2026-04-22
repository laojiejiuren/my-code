module RAM(
    input clk,
    input reset,

    input [7:0] wdata;
    input[1:0] R1,
    input[1:0] R2,
    input[1:0] we,

    output [7:0] data1,
    output [7:0] data2
);
    reg[7:0] mem [3:0];

    always@ (posedge clk) begin
        if(reset) begin
            mem[0] <= 8'h00;
            mem[1] <= 8'h00;
            mem[2] <= 8'h00;
            mem[3] <= 8'h00;
        end
        else begin
            mem[we] <= wdata;
        end
    end

    assign data1 = mem[R1];
    assign data2 = mem[R2];

endmodule
