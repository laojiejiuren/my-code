module ALU(
    input[31:0] A,
    input[31:0] B,
    input[3:0] alu_op,

    output reg[31:0] result
);
    wire[4:0] offset = B[4:0];

    always@(*) begin
        case(alu_op)
            4'h0: result = A + B;                //ADD、ADDI
            4'h1: result = A - B;                //SUB
            4'h2: result = A & B;                //AND、ANDI
            4'h3: result = A | B;                //OR、ORI
            4'h4: result = A ^ B;                //XOR、XORI
            4'h5: result = A >> offset;          //srl、srli
            4'h6: result = A << offset;          //sll、slli
            4'h7: result = $signed(A) >> offset; //srai、sra
            4'h8: result = (A < B) ? 1 : 0;      //sltui、sltu  
            4'h9: result = ($signed(A) < $signed(B) ? 1 : 0); //slt、slti
            default: result = 32'h0;
        endcase
    end

endmodule
