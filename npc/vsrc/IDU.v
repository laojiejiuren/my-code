module IDU(
    input clk,
    input rst,

    input [31:0] inst,

    input [4:0] reg_waddr_wb,
    input [31:0] reg_wdata_wb,
    input reg_wen_wb,

    output reg reg_wen,
    output reg [2:0] alu_op,
    output reg [4:0] reg_waddr,
    output reg [31:0] operand1, 
    output reg [31:0] operand2
);

    wire[6:0] opcode = inst[6:0];
    wire[4:0] rd = inst[11:7];
    wire[2:0] funct3 = inst[14:12];
    wire[4:0] rs1 = inst[19:15];
    wire[4:0] rs2 = inst[24:20];
    wire[6:0] funct7 = inst[31:25];

    wire[31:0] rs1_rdata;
    wire[31:0] rs2_rdata;
    GPR u_gpr(.clk(clk),.reg_waddr(reg_waddr_wb),.reg_wdata(reg_wdata_wb),
            .reg_wen(reg_wen_wb),.rs1_raddr(inst[19:15]),.rs2_raddr(inst[24:20]),
            .rs1_rdata(rs1_rdata),.rs2_rdata(rs1_rdata)
            );

    wire [31:0] imm_i = 32'($signed(inst[31:20]));
    wire [31:0] imm_u = 32'($unsigned(inst[31:12]));
    wire [31:0] imm_s = 32'($signed({inst[31:25],inst[11:7]}));

    parameter [6:0] RISCV32I_I = 7'b0010011;
    parameter [2:0] RISCV32I_addi = 3'b000;

    always@(*)begin
        operand1 = 32'b0;
        operand2 = 32'b0;
        reg_wen = 1'b0;

        case (opcode)
        RISCV32I_I:begin
            case (funct3)
            RISCV32I_addi: begin
                operand2 = imm_i;
                operand1 = rs1_rdata;
                reg_waddr = rd;
                reg_wen = 1;
                alu_op = 3'b000;
            end


            default: begin
                operand2 = 32'b0;
                operand1 = 32'b0;
                reg_waddr = 5'b0;
                reg_wen = 0;
                alu_op = 3'b000;

            end
            endcase
        end

        default:begin
            operand2 = 32'b0;
            operand1 = 32'b0;
            reg_waddr = 5'b0;
            reg_wen = 0;
            alu_op = 3'b000;

        end
        endcase
    end


endmodule
