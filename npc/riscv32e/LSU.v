`include "opcode.vh"
import opcode::*;

module LSU(
    input [31:0] waddr,
    input [31:0] raddr,
    input [31:0] wdata,
    input [1:0] store_type,
    input [2:0] load_type,
    input ren,
    input wen,

    output reg [31:0] data_mem 
);
    reg[31:0] data;
    wire[1:0] select = waddr[1:0];
    reg[7:0] wmask;
    reg[31:0] wdata_tmp;
    reg[31:0] rdata_tmp;

    //sw,sb指令是将rs2的值的低n位写入内存,相当于要做选择的是Q2的字节
    always @(*) begin
        wdata_tmp = wdata;
        case(store_type)
        2'b00:begin wmask = 8'h0f; end //sw
        2'b01:begin //sb
            case(select)
            2'b00:wmask = 8'h01;
            2'b01:begin wmask = 8'h02; wdata_tmp = wdata << 8; end
            2'b10:begin wmask = 8'h04; wdata_tmp = wdata << 16; end
            2'b11:begin wmask = 8'h08; wdata_tmp = wdata << 24; end
            endcase
        end

        default:wmask = 8'h0f;
        endcase
    end
    
    //lw,lbu指令是将imm_i + Q1 内存地址的值的第n个字节存入rd
    //那么要做选择字节的应该是内存数据
    always@(*) begin
        rdata_tmp = 32'b0;
        data_mem = 32'b0;
        if(ren) begin
            rdata_tmp = pmem_read(raddr, {31'b0,ren},
                ((load_type == RISCV32I_lb) || (load_type == RISCV32I_lbu)) ? 32'd1 :
                ((load_type == RISCV32I_lh) || (load_type == RISCV32I_lhu)) ? 32'd2 : 32'd4);
            case(load_type)
            RISCV32I_lw: data_mem = rdata_tmp;
            RISCV32I_lb: begin
                rdata_tmp = (rdata_tmp >> (raddr[1:0] * 8)) & 32'hff;
                data_mem = {{24{rdata_tmp[7]}}, rdata_tmp[7:0]};
            end
            RISCV32I_lbu: data_mem = (rdata_tmp >> (raddr[1:0] * 8)) & 32'hff;
            RISCV32I_lh:begin
                rdata_tmp = (rdata_tmp >> (raddr[1] * 16)) & 32'hffff;
                data_mem = {{16{rdata_tmp[15]}}, rdata_tmp[15:0]};
            end
            RISCV32I_lhu: data_mem = (rdata_tmp >> (raddr[1] * 16)) & 32'hffff;
            default: data_mem = 32'b0;
            endcase
        end
    end

    always@(*) begin
        if(wen) begin
            pmem_write(waddr,wdata_tmp,wmask);
        end
    end

endmodule
