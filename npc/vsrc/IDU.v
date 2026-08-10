module IDU(
    input clk,
    input rst,

    input [31:0] pc,
    input [31:0] inst,

    input [4:0] reg_waddr_wb,
    input [31:0] reg_wdata_wb,
    input reg_wen_wb,

    output reg reg_wen,
    output reg mem_ren, 
    output reg mem_wen,
    output reg lbu, 
    output reg jump, 
    output reg [2:0] alu_op,
    output reg [1:0] store_type,
    output reg [4:0] reg_waddr,
    output reg [31:0] Q2,
    output reg [31:0] operand1, 
    output reg [31:0] operand2,

    output [31:0] data 
);

    wire[6:0] opcode = inst[6:0];
    wire[4:0] rd = inst[11:7];
    wire[2:0] funct3 = inst[14:12];
    wire[4:0] rs1 = inst[19:15];
    wire[4:0] rs2 = inst[24:20];
    wire[6:0] funct7 = inst[31:25];
    wire[11:0] funct12 = inst[31:20];

    wire[31:0] rs1_rdata;
    wire[31:0] rs2_rdata;
    GPR u_gpr(.clk(clk),.reg_waddr(reg_waddr_wb),.reg_wdata(reg_wdata_wb),
            .reg_wen(reg_wen_wb),.rs1_raddr(rs1),.rs2_raddr(rs2),
            .rs1_rdata(rs1_rdata),.rs2_rdata(rs2_rdata),.data(data)
            );

    wire [31:0] imm_i = 32'($signed(inst[31:20]));
    wire [31:0] imm_u = 32'({inst[31:12],12'b0});
    wire [31:0] imm_s = 32'($signed({inst[31:25],inst[11:7]}));

    parameter [6:0] RISCV32I_I = 7'b0010011;
    parameter [6:0] RISCV32I_R = 7'b0110011;
    parameter [6:0] RISCV32I_U = 7'b0110111;
    parameter [6:0] RISCV32I_load = 7'b0000011;
    parameter [6:0] RISCV32I_store = 7'b0100011;
    parameter [6:0] RISCV32I_tiao = 7'b1100111;
    parameter [6:0] RISCV32I_sys = 7'b1110011;

    
    parameter [2:0] RISCV32I_addi = 3'b000;
    parameter [2:0] RISCV32I_jalr = 3'b000;
    parameter [2:0] RISCV32I_add = 3'b000;
    parameter [2:0] RISCV32I_lw = 3'b010;
    parameter [2:0] RISCV32I_lbu = 3'b100;
    parameter [2:0] RISCV32I_sw = 3'b010;
    parameter [2:0] RISCV32I_sb = 3'b000;
    parameter [11:0] RISCV32I_ebreak = 12'b000000000001;

    always@(*)begin
        operand1 = 32'b0;
        operand2 = 32'b0;
        Q2 = 32'b0;
        reg_waddr = 5'b0;
        store_type = 2'b11;
        reg_wen = 0;
        jump = 0;
        lbu = 0;
        mem_ren = 0;
        mem_wen = 0;
        alu_op = 3'b111;

        case (opcode)
        RISCV32I_I:begin
            case (funct3)
            RISCV32I_addi: begin
                operand2 = imm_i;
                operand1 = rs1_rdata;
                reg_waddr = rd;
                jump = 0;
                reg_wen = 1;
                alu_op = 3'b000;
            end


            default: begin
                operand2 = 32'b0;
                operand1 = 32'b0;
                reg_waddr = 5'b0;
                jump = 0;
                reg_wen = 0;
                alu_op = 3'b111;

            end
            endcase
        end

        RISCV32I_R:begin
            case(funct3)
            RISCV32I_add:begin
                operand1 = rs1_rdata;
                operand2 = rs2_rdata;
                reg_waddr = rd;
                reg_wen = 1;
                jump = 0;
                alu_op = 3'b000;
            end

            default:begin
                operand2 = 32'b0;
                operand1 = 32'b0;
                reg_waddr = 5'b0;
                jump = 0;
                reg_wen = 0;
                alu_op = 3'b111;

            end
            endcase
        end

        RISCV32I_U:begin
            operand1 = imm_u;
            operand2 = 32'b0;
            reg_waddr = rd;
            reg_wen = 1;
            jump = 0;
            alu_op = 3'b000;
        end

        RISCV32I_tiao: begin
            case(funct3)
            RISCV32I_jalr: begin
                operand1 = imm_i;
                operand2 = rs1_rdata;
                reg_waddr = rd;
                reg_wen = 1;
                jump = 1;
                alu_op = 3'b000;
            end


            default: begin
                operand2 = 32'b0;
                operand1 = 32'b0;
                reg_waddr = 5'b0;
                jump = 0;
                reg_wen = 0;
                alu_op = 3'b111;

            end
            endcase

        end
        
        RISCV32I_sys:begin
            case(funct12)
            RISCV32I_ebreak:begin
                set_npc_state(2, pc, data);
                //halt(data);
            end
            default:begin
            end
            endcase
        end

        RISCV32I_load:begin
            case(funct3)
            RISCV32I_lw:begin
                operand1 = imm_i;
                operand2 = rs1_rdata;
                reg_waddr = rd;
                store_type = 2'b00;
                jump = 0;
                mem_ren = 1;
                reg_wen = 1;
                alu_op = 3'b000;
            end
            RISCV32I_lbu:begin
                operand1 = imm_i;
                operand2 = rs1_rdata;
                reg_waddr = rd;
                store_type = 2'b00;
                jump = 0;
                lbu = 1;
                mem_ren = 1;
                reg_wen = 1;
                alu_op = 3'b000;

            end

            default:begin
                operand2 = 32'b0;
                operand1 = 32'b0;
                reg_waddr = 5'b0;
                store_type = 2'b11;
                jump = 0;
                mem_ren = 0;
                reg_wen = 0;
                alu_op = 3'b111;
            end

            endcase

        end
        
        RISCV32I_store:begin
            case(funct3)
            RISCV32I_sw:begin
                operand1 = rs1_rdata;
                operand2 = imm_s;
                store_type = 2'b00;
                Q2 = rs2_rdata;
                mem_wen = 1;
                alu_op = 3'b000;
            end
            RISCV32I_sb:begin
                operand1 = rs1_rdata;
                operand2 = imm_s;
                store_type = 2'b01;
                Q2 = rs2_rdata;
                mem_wen = 1;
                alu_op = 3'b000;
            end
            default:begin
                operand1 = 32'b0;
                operand2 = 32'b0;
                store_type = 2'b11;
                Q2 = 32'b0;
                mem_wen = 0;
                alu_op = 3'b111;
            end
            endcase

        end


        default:begin
            operand2 = 32'b0;
            operand1 = 32'b0;
            reg_waddr = 5'b0;
            jump = 0;
            mem_ren = 0;
            reg_wen = 0;
            alu_op = 3'b111;

        end
        endcase
    end


endmodule
