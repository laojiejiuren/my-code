module top(
    input clk,
    input reset,


    output reg[7:0] out
);
    wire jump;
    wire [2:0] select38;
    wire [3:0] switch;
    wire [7:0] jump_addr,pc_out,IS,Q1,Q2,sum,wdata;
    wire [1:0] R1,R2;
    
    assign R2 = (switch == 4'b0100) ? 2'b00 : IS[3:2];
    assign wdata = switch[0] ? sum : {4'h0,IS[3:0]};
    assign select38 = pc_out[2:0];

    pc my_pc(.clk(clk),.reset(reset),.jump(jump),.jump_addr(jump_addr),.pc(pc_out));
    ROM my_rom(.select(select38),.out(IS));
    decoder my_de(.cin(IS[7:6]),.cout(switch));
    RAM my_ram(.clk(clk),.reset(reset),.wdata(wdata),
                .R1(R1),.R2(R2),.we(IS[5:4]),.data1(Q1),.data2(Q2));
    add my_add(.A(Q1),.B(Q2),.sum(sum));

    always @(*) begin
        jump = 0;
        jump_addr = 8'h00;
        out = 8'h00;
        case(switch)
        4'b0001: begin//add
            
        end
        4'b0010: begin//out
            out = Q1;
        end
        4'b0100: begin//li

        end
        4'b1000: begin//bner0
            if(Q1!=Q2) begin
                jump = 1;
                jump_addr = IS[5:2];
            end        
        end
        endcase
    end

endmodule

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
        3'b000:out = mem[0];
        3'b001:out = mem[1];
        3'b010:out = mem[2];
        3'b011:out = mem[3];
        3'b100:out = mem[4];
        3'b101:out = mem[5];
        3'b110:out = mem[6];
        3'b111:out = mem[7];
        endcase
    end

endmodule
module add(
    input [7:0] A,
    input [7:0] B,

    output [7:0] sum
);
assign sum = A + b;
endmodule
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
