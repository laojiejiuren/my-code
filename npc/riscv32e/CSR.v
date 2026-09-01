module CSR(
    input clk,
    input rst,

    input [31:0] pc,
    input [31:0] cause,
    input [31:0] data_csr,

    input [11:0] csr_addr,
    input csr_ecall,
    input csr_r,
    input csr_w,

    output [31:0] csr_out
);

    logic [31:0] mstatus,mtvepc,mepc,mcause;
    logic [63:0] mcycle;
    
    always @(posedge clk) begin
        if(rst) begin
            mcycle <= 64'b0;
            mstatus <= 32'h1800;
            mtvepc <= 32'b0;
            mepc <= 32'b0;
            mcause <= 32'b0;
        end
        else begin
            if(csr_ecall)begin
                mstatus <= 32'h1800;
                mcause <= cause;
                mepc <= pc;
                csr_out <= mtvepc;
            end
            else if(csr_w)begin
                case(csr_addr)

                endcase
            end
        end

    end

endmodule
