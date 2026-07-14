module top(
    input clk,
    input rst,



);
    IFU u_ifu(
        .clk(clk),.rst(rst),.next_pc(next_pc),.pc(pc),.inst(inst)
    );

    IDU u_idu(
        .clk(clk),.rst(rst),.inst(inst),.reg_waddr_wb(),.reg_wdata_wb(),.reg_wen_wb(),
        .reg_wen(),.alu_op(),.reg_waddr(),.opeeran1(),.opeeran2
    );

    EXU u_exu(
        .clk(clk),.rst(rst),.alu_op(),.opeeran1,.opeeran2(),
        .alu_res()
    );

    WBU u_wbu(
        .clk(clk),.rst(rst),.reg_wen_id(),.reg_waddr_id(),.pc(),
        .next_pc(),.reg_wen_id(),.reg_waddr_wb,.reg_wdata_wb()
    );

endmodule
