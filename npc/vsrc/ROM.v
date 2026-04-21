module ROM(
    input [2:0]select,
    output [7:0] out
);
    wire [7:0] mem[7:0];
    assign mem[0] = 8'h8a;
    assign mem[1] = 8'h90;
    assign mem[2] = 8'ha0;
    assign mem[3] = 8'hb1;
    assign mem[4] = 8'h17;
    assign mem[5] = 8'h29;
    assign mem[6] = 8'hd1;
    assign mem[7] = 8'hdf;

    always @(*) begin
        case(select)
        4'b000:out = mem[0];
        4'b001:out = mem[1];
        4'b010:out = mem[2];
        4'b011:out = mem[3];
        4'b100:out = mem[4];
        4'b101:out = mem[5];
        4'b110:out = mem[6];
        4'b111:out = mem[7];
        endcase
    end

endmodule