module top(
    input clk,
    input reset,

    output [7:0] ram_mem0,
    output [7:0] ram_mem2,
    output [7:0] out0,out1,
    output reg[7:0] out
);
    reg jump,we;
    wire [2:0] select38;
    reg [3:0] switch;
    reg [7:0] jump_addr,pc_out,IS,Q1,Q2,sum,wdata;
    reg [1:0] R1,R2;
    reg [1:0] waddr;
    wire [3:0] seg0,seg1;
    
    assign R2 = (switch == 4'b0100) ? 2'b00 : IS[3:2];
    assign R1 = IS[1:0];
    assign wdata = switch[0] ? sum : {4'h0,IS[3:0]};
    assign select38 = pc_out[2:0];
    assign ram_mem0 = my_ram.mem[0];
    assign ram_mem2 = my_ram.mem[2];

    assign seg0 = out[3:0];
    assign seg1 = out[7:4];

    seg my_seg0(.b(seg0),.h(out0));
    seg my_seg1(.b(seg1),.h(out1));
    pc my_pc(.clk(clk),.reset(reset),.jump(jump),.jump_addr(jump_addr),.pc(pc_out));
    ROM my_rom(.select(select38),.out(IS));
    decoder my_de(.cin(IS[7:6]),.cout(switch));
    RAM my_ram(.clk(clk),.reset(reset),.we(we),.wdata(wdata),
                .R1(R1),.R2(R2),.waddr(waddr),.data1(Q1),.data2(Q2));
    add my_add(.A(Q1),.B(Q2),.sum(sum));

    always @(*) begin
        jump = 0;
        jump_addr = 8'h00;
        we = 0;
        out = 8'h00;
        waddr = 2'b00;
        case(switch)
        4'b0001: begin//add
            we = 1;
            waddr = IS[5:4];
        end
        4'b0010: begin//out
            out = Q1;
            jump = 1;
            jump_addr = 8'b00000111;
        end
        4'b0100: begin//li
            we = 1;
            waddr = IS[5:4];
        end
        4'b1000: begin//bner0
            if(Q1 != ram_mem0) begin
                jump = 1;
                jump_addr = {4'h0,IS[5:2]};
            end        
        end
        default:begin end
        endcase
    end

endmodule
