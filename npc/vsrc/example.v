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
