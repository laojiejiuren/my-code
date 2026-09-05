`include "opcode.v"

module IDU(
    input clk,
    input rst,

    input [31:0] pc,
    input [31:0] inst,

    input [4:0] reg_waddr_wb,
    input [31:0] reg_wdata_wb,
    input reg_wen_wb,

    //------ CSR -----
    input [31:0] csr_data,

    output reg [11:0] csr_addr,
    output reg csr_w,
    output reg csr_ren,
    output reg ecall,
    output reg mret,

    output reg reg_wen,
    output reg mem_ren, 
    output reg mem_wen,
    output reg jump, 
    output reg branch,
    output reg [3:0] alu_op,
    output reg [1:0] store_type,
    output reg [2:0] load_type,
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

    wire [31:0] imm_i = {{20{inst[31]}}, inst[31:20]};
    wire [31:0] imm_u = {inst[31:12], 12'b0};
    wire [31:0] imm_s = {{20{inst[31]}}, inst[31:25], inst[11:7]};
    wire [31:0] imm_j = {{12{inst[31]}}, inst[19:12], inst[20], inst[30:21], 1'b0};
    wire [31:0] imm_b = {{20{inst[31]}}, inst[7], inst[30:25], inst[11:8], 1'b0};
    wire [11:0] imm_C = inst[31:20];

    always@(*)begin
        operand1 = 32'b0;
        operand2 = 32'b0;
        Q2 = 32'b0;
        reg_waddr = 5'b0;
        store_type = 2'b11;
        load_type = 3'd0;
        reg_wen = 0;
        jump = 0;
        branch = 0;
        mem_ren = 0;
        mem_wen = 0;
        alu_op = 4'hA;
        csr_addr = 12'b0;
        csr_w = 0;
        csr_ren = 0;
        ecall = 0;
        mret = 0;

        case (opcode)
        `RISCV32I_I:begin //addi,sltiu,slti,srai,slli,srli,andi,xori,ori
            operand1 = rs1_rdata;
            operand2 = imm_i;
            reg_waddr = rd;
            reg_wen = 1;
            case (funct3)
            `RISCV32I_addi: alu_op = 4'h0;
            `RISCV32I_sltiu: alu_op = 4'h8;
            `RISCV32I_slti: alu_op = 4'h9;
            `RISCV32I_andi: alu_op = 4'h2;
            `RISCV32I_xori: alu_op = 4'h4;
            `RISCV32I_ori: alu_op = 4'h3;
            `RISCV32I_slli: begin
                if(funct7[5] == 0) alu_op = 4'h6;
            end
            `RISCV32I_srai: begin
                if(funct7[5] == 1) alu_op = 4'h7;//srai
                else alu_op = 4'h5; //srli
            end

            default: alu_op = 4'hA;

            endcase
        end

        `RISCV32I_R:begin
            operand1 = rs1_rdata;
            operand2 = rs2_rdata;
            reg_waddr = rd;
            reg_wen = 1;
            case(funct3)
            `RISCV32I_add: alu_op = funct7[5] ? 4'h1 : 4'h0;
            `RISCV32I_slt: alu_op = 4'h9;
            `RISCV32I_sltu: alu_op = 4'h8;
            `RISCV32I_xor: alu_op = 4'h4;
            `RISCV32I_or: alu_op = 4'h3;
            `RISCV32I_and: alu_op = 4'h2;
            `RISCV32I_sll: alu_op = 4'h6;
            `RISCV32I_srl: begin
                if(funct7[5] == 1) alu_op = 4'h7; 
                else alu_op = 4'h5; 
            end

            default: alu_op = 4'hA;
            endcase
        end

        `RISCV32I_U:begin
            operand1 = imm_u;
            operand2 = 32'b0;
            reg_waddr = rd;
            reg_wen = 1;
            alu_op = 4'h0;
        end

        `RISCV32I_auipc:begin
            operand1 = pc;
            operand2 = imm_u;
            reg_waddr = rd;
            reg_wen = 1;
            alu_op = 4'h0;
        end

         `RISCV32I_B:begin
            operand1 = pc;
            operand2 = imm_b;
            alu_op = 4'h0;
            case(funct3)
            `RISCV32I_beq: branch = (rs1_rdata == rs2_rdata);
            `RISCV32I_bne: branch = (rs1_rdata != rs2_rdata);
            `RISCV32I_bge: branch = ($signed(rs1_rdata) >= $signed(rs2_rdata));
            `RISCV32I_bgeu: branch = (rs1_rdata >= rs2_rdata);
            `RISCV32I_blt: branch = ($signed(rs1_rdata) < $signed(rs2_rdata));
            `RISCV32I_bltu: branch = (rs1_rdata < rs2_rdata);

            default: branch = 0;
            endcase
         end


        `RISCV32I_ja: begin
            case(funct3)
            `RISCV32I_jalr: begin
                operand1 = rs1_rdata;
                operand2 = imm_i;
                reg_waddr = rd;
                reg_wen = 1;
                jump = 1;
                alu_op = 4'h0;
            end

            default: begin
                operand2 = 32'b0;
                operand1 = 32'b0;
                reg_waddr = 5'b0;
                jump = 0;
                reg_wen = 0;
                alu_op = 4'hA;

            end
            endcase

        end

        `RISCV32I_jal: begin
            operand1 = pc;
            operand2 = imm_j;
            reg_waddr = rd;
            reg_wen = 1;
            jump = 1;
            alu_op = 4'h0;
        end
        
        `RISCV32I_sys:begin
            case(funct12)
            `RISCV32I_ebreak:begin
                //halt(data);
            end
            `RISCV32I_ecall:begin
                ecall = 1;
            end
            `RISCV32I_mret:begin
                mret = 1;
            end
            default:begin
            end
            endcase

            case(funct3)
            `RISCV32I_csrrs:begin
                operand1  = csr_data;
                operand2  = rs1_rdata;
                reg_waddr = rd;
                reg_wen   = 1;
                csr_ren   = 1;
                csr_addr  = imm_C;
                alu_op    = 4'h3;
                csr_w     = (rs1 != 5'b0); 
            end
            `RISCV32I_csrrw:begin
                operand1  = 32'b0;
                operand2  = rs1_rdata;
                reg_waddr = rd;
                reg_wen   = 1;
                csr_ren   = 1;
                csr_addr  = imm_C;
                alu_op    = 4'h0;
                csr_w     = (rs1 != 5'b0);
            end
            default:begin end
            endcase
        end

        `RISCV32I_load:begin
            operand1 = rs1_rdata;
            operand2 = imm_i;
            reg_waddr = rd;
            mem_ren = 1;
            reg_wen = 1;
            alu_op = 4'h0;
            case(funct3)
            `RISCV32I_lw , `RISCV32I_lb , `RISCV32I_lh , `RISCV32I_lbu , `RISCV32I_lhu :begin
                load_type = funct3;
            end

            default:begin
                load_type = 3'd0;
            end

            endcase

        end
        
        `RISCV32I_store:begin
            operand1 = rs1_rdata;
            operand2 = imm_s;
            Q2 = rs2_rdata;
            mem_wen = 1;
            alu_op = 4'h0;
            case(funct3)
            `RISCV32I_sw: store_type = 2'b00;
            `RISCV32I_sb: store_type = 2'b01;
            `RISCV32I_sh: store_type = 2'b10;

            default:begin
                store_type = 2'b11;
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
            alu_op = 4'hA;

        end
        endcase
    end

    always@(posedge clk) begin
        if(!rst && opcode == `RISCV32I_sys && funct12 == `RISCV32I_ebreak)
            halt(2, pc, data);
    end

endmodule
