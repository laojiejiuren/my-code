module GPR(
    input clk,

    //写入
    input [4:0] reg_waddr,
    input [31:0] reg_wdata,
    input reg_wen,

    //读取
    input [4:0] rs1_raddr,
    input [4:0] rs2_raddr,
    output reg [31:0] rs1_rdata,
    output reg [31:0] rs2_rdata
);
    reg [31:0] gpr[32];

    //要读取数据时
    always@(*) begin
        if(rs1_raddr == 0)
            rs1_rdata = 0;
        else rs1_rdata = gpr[rs1_raddr];
    end
    always@(*) begin
        if(rs2_raddr == 0)
            rs2_rdata = 0;
        else rs2_rdata = gpr[rs2_raddr];
    end
    
    //要写入数据时
    always@(posedge clk) begin
        if(reg_waddr != 0 && reg_wen)
            gpr[reg_waddr] <= reg_wdata;
    end

endmodule
