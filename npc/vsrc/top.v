module top(
    input clk,
    input rst,
    input [31:0] inst,
    output [31:0] pc
);
    //IF 
    wire [31:0] next_pc;

    //ID
    reg [31:0] reg_wdata_wb_id;
    reg [4:0] reg_wdaddr_wb_id;
    reg reg_wen_wb_id;
    reg jump;
    reg reg_wen_id_ex;
    reg [2:0] alu_op_id;
    reg [4:0] reg_waddr_id_wb;
    reg [31:0] operand1_id;
    reg [31:0] operand2_id;

    //EX
    reg [31:0] alu_res_ex;

    IFU u_ifu(
        .clk(clk),.rst(rst),.next_pc(next_pc),.pc(pc)
    );
    
    IDU u_idu(
        .clk(clk),.rst(rst),.inst(inst),.reg_waddr_wb(reg_wdaddr_wb_id),.reg_wdata_wb(reg_wdata_wb_id),.reg_wen_wb(reg_wen_wb_id),
        .jump(jump),.reg_wen(reg_wen_id_ex),.alu_op(alu_op_id),.reg_waddr(reg_waddr_id_wb),.operand1(operand1_id),.operand2(operand2_id)
    );

    EXU u_exu(
        .clk(clk),.rst(rst),.alu_op(alu_op_id),.operand1(operand1_id),.operand2(operand2_id),
        .alu_res(alu_res_ex)
    );

    WBU u_wbu(
        .clk(clk),.rst(rst),.reg_wen_id(reg_wen_id_ex),.jump(jump) ,.reg_waddr_id(reg_waddr_id_wb),.alu_res(alu_res_ex),.pc(pc),
        .next_pc(next_pc),.reg_wen_wb(reg_wen_wb_id),.reg_waddr_wb(reg_wdaddr_wb_id),.reg_wdata_wb(reg_wdata_wb_id)
    );

endmodule
