module EXU(
    input clk,
    input rst,

    input[3:0] alu_op,
    input[31:0] operand1,
    input[31:0] operand2,


    output reg [31:0] alu_res
);
    ALU my_ALU(.A(operand1), .B(operand2), .alu_op(alu_op), .result(alu_res));
    

endmodule
