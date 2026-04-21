module decoder(
    input [1:0] cin,
    output [3:0] cout
);
    always @(*) begin
        case(cin)
        2'b00:cout = 4'b0001;
        2'b01:cout = 4'b0010;
        2'b10:cout = 4'b0100;
        2'b11:cout = 4'b1000;
        default:cout = 4'h0;
        endcase
    end

endmodule