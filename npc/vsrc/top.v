module top(
    input clk,
    input rst,
    input [31:9] pc,
    input [31:0] inst
);
    //IF 
    wire [31:0] next_pc;

    //ID
    wire [31:0] reg_wdata_wb_id;
    wire [4:0] reg_wdaddr_wb_id;
    wire reg_wen_wb_id
    
    wire reg_wen_id_ex;
    wire [2:0] alu_op_id;
    wire [4:0] reg_waddr_id_wb;
    wire [31:0] opeeran1_id;
    wire [31:0] operand2_id;

    //EX
    wire [31:0] alu_res_ex;

    /*
    IFU u_ifu(
        .clk(clk),.rst(rst),.next_pc(next_pc),.pc(pc),.inst(inst)
    );
    */
    IDU u_idu(
        .clk(clk),.rst(rst),.inst(inst),.reg_waddr_wb(reg_wdaddr_wb_id),.reg_wdata_wb(reg_wdata_wb_id),.reg_wen_wb(reg_wen_wb_id),
        .reg_wen(reg_wen_id_ex),.alu_op(alu_op_id),.reg_waddr(reg_waddr_id_wb),.opeeran1(opeeran1_id),.opeeran2(operand2_id)
    );

    EXU u_exu(
        .clk(clk),.rst(rst),.alu_op(alu_op_id),.opeeran1(opeeran1_id),.opeeran2(operand2_id),
        .alu_res(alu_res_ex)
    );

    WBU u_wbu(
        .clk(clk),.rst(rst),.reg_wen_id(reg_wen_id_ex),.reg_waddr_id(reg_waddr_id_wb),.pc(pc),
        .next_pc(next_pc),.reg_wen_wb(reg_wen_wb_id),.reg_waddr_wb(reg_wdaddr_wb_id),.reg_wdata_wb(reg_wdata_wb_id)
    );

endmodule
