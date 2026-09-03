`include "opcode.v"

module CSR(
    input clk,
    input rst,

    //------- ecall ------
    input [31:0] pc,

    //csrrs
    input [31:0] data_csr,//在ALU中经过计算后的数据

    input [11:0] csr_addr,
    input csr_ecall,
    input csr_mret,
    input csr_w,

    output reg [31:0] csr_mtvec,
    output reg [31:0] csr_mepc,
    output reg [31:0] csr_data
);

    logic [31:0] mstatus,mtvec,mepc,mcause,mvendorid,marchid;
    logic [31:0] mcycle,mcycleh;
    
    always @(*) begin//读取CSR的数据
        case (csr_addr)
        `CSR_mstatus:csr_data = mstatus;
        `CSR_mtvec:  csr_data = mtvec;
        `CSR_mepc:   csr_data = mepc;
        `CSR_mcause: csr_data = mcause;
        `CSR_mcycle: csr_data = mcycle;
        `CSR_mcycleh:csr_data = mcycleh;
        `CSR_mvendorid: csr_data = 32'h79737978;
        `CSR_marchid:   csr_data = 32'h018D7E6E;
        default:     csr_data = 32'b0;
        endcase
    end

    always @(posedge clk) begin
        if(rst)begin
            mcycle  <= 32'b0;
            mcycleh <= 32'b0;
        end
        else if(csr_w && csr_addr == `CSR_mcycle)
            mcycle <= data_csr;
        else if(csr_w && csr_addr == `CSR_mcycleh)
            mcycleh <= data_csr;
        else {mcycleh,mcycle} <= {mcycleh,mcycle} + 1;
    end

    always @(posedge clk) begin
        if(rst) begin
            mstatus <= 32'h1800;
            mtvec <= 32'b0;
            mepc <= 32'b0;
            mcause <= 32'b0;
        end
        else begin
            if(csr_ecall)begin
                mstatus <= 32'h1800;
                mcause <= 32'h11;
                mepc <= pc;
                csr_mtvec <= mtvec;
            end
            else if(csr_mret)begin
                csr_mepc <= mepc;
            end
            else if(csr_w)begin//将数据写进CSR
                case(csr_addr)
                `CSR_mstatus:   mstatus <= data_csr;
                `CSR_mtvec:     mtvec   <= data_csr;
                `CSR_mepc:      mepc    <= data_csr;
                `CSR_mcause:    mcause  <= data_csr;
                default: begin end
                endcase
            end
        end

    end

    function void get_csr(input int flag, output bit[31:0] csr_);
        if(flag == 1)
            csr_ = mcycle;
        else csr_ = mcycleh;
    endfunction
    export "DPI-C" function get_csr;

endmodule
