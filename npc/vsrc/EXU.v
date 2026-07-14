module EXU(
    input clk,
    input rst,

    input[2:0] alu_op,
    input[31:0] opeeran1,
    input[31:0] operand2,


    output[31:0] alu_res
);

    always@(*) begin
        case(alu_op)
        3'b000:alu_res = opeeran1 + opeeran2;


        default:alu_res = 32'b0;
        endcase

    end


endmodule
