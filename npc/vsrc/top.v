import "DPI-C" function void halt(input int code);
import "DPI-C" function int pmem_read (input int raddr);
import "DPI-C" function void pmem_write (input int waddr, input int wdata, input byte wmask);
import "DPI-C" function void set_npc_state(input int state, input bit[31:0] pc, input int halt_ret);

module top(
    input clk,
    input rst,
    
    output [31:0] data_out
);

    //IF 
    wire [31:0] next_pc;
    wire [31:0] pc;
    wire [31:0] inst_if;
    //ID
    reg [31:0] reg_wdata_wb_id;
    reg [4:0] reg_waddr_wb_id;
    reg reg_wen_wb_id;
    reg jump;
    reg reg_wen_id_ex;
    reg [2:0] alu_op_id;
    reg [4:0] reg_waddr_id_wb;
    reg [31:0] operand1_id;
    reg [31:0] operand2_id;
    
    //ID -> LS
    reg lbu_id_ls;
    reg mem_ren_id_ls;
    reg mem_wen_id_ls;
    reg [1:0] store_type_id_ls;
    reg [31:0] Q2_id_ls;

    //LS
    reg [31:0] data_mem_ls;
    //EX
    reg [31:0] alu_res_ex;

    IFU u_ifu(
        .clk(clk),.rst(rst),.next_pc(next_pc),.pc(pc),.inst(inst_if)
    );
    
    IDU u_idu(
        .clk(clk),.rst(rst),.inst(inst_if),.pc(pc),.reg_waddr_wb(reg_waddr_wb_id),.reg_wdata_wb(reg_wdata_wb_id),.reg_wen_wb(reg_wen_wb_id),
        .jump(jump),.reg_wen(reg_wen_id_ex),.alu_op(alu_op_id),.reg_waddr(reg_waddr_id_wb),.operand1(operand1_id),.operand2(operand2_id),
        .lbu(lbu_id_ls),.mem_ren(mem_ren_id_ls),.mem_wen(mem_wen_id_ls),.store_type(store_type_id_ls),.Q2(Q2_id_ls),
        
        .data(data_out)
    );

    EXU u_exu(
        .clk(clk),.rst(rst),.alu_op(alu_op_id),.operand1(operand1_id),.operand2(operand2_id),
        .alu_res(alu_res_ex)
    );

    LSU u_lsu(
        .waddr(alu_res_ex),.raddr(alu_res_ex),.wdata(Q2_id_ls),.store_type(store_type_id_ls),.lbu(lbu_id_ls),.ren(mem_ren_id_ls),.wen(mem_wen_id_ls),
        .data_mem(data_mem_ls)
    );

    WBU u_wbu(
        .reg_wen_id(reg_wen_id_ex),.jump(jump) ,.reg_waddr_id(reg_waddr_id_wb),.alu_res(alu_res_ex),.pc(pc),
        .mem_ren_id(mem_ren_id_ls),.data_mem(data_mem_ls),
        .next_pc(next_pc),.reg_wen_wb(reg_wen_wb_id),.reg_waddr_wb(reg_waddr_wb_id),.reg_wdata_wb(reg_wdata_wb_id)
    );

endmodule
