module LSU(
    input [31:0] waddr,
    input [31:0] raddr,
    input [31:0] wdata,
    input [1:0] store_type,
    input lbu,
    input ren,
    input wen,

    output reg [31:0] data_mem 
);
    reg[31:0] data;
    wire[1:0] select = waddr[1:0];
    reg[7:0] wmask;
    reg[31:0] wdata_tmp;

    always @(*) begin
        wdata_tmp = wdata;
        case(store_type)
        2'b00:begin
            wmask = 8'h0f;
        end

        2'b01:begin
            case(select)
            2'b00:begin
                wmask = 8'h01;
            end
            2'b01:begin
                wmask = 8'h02;
                wdata_tmp = wdata << 8;
            end
            2'b10:begin
                wmask = 8'h04;
                wdata_tmp = wdata << 16;
            end
            2'b11:begin
                wmask = 8'h08;
                wdata_tmp = wdata << 24;
            end
            endcase
        end

        default:wmask = 8'h0f;
        endcase
    end

    always@(*) begin
        if(ren) begin
            data_mem = pmem_read(raddr);
            if(lbu) begin
                if(raddr[1:0] == 2'b00)
                    data_mem = (data_mem & 32'h000000ff);
                else if(raddr[1:0] == 2'b01)
                    data_mem = (data_mem & 32'h0000ff00) >> 8;
                else if(raddr[1:0] == 2'b10)
                    data_mem = (data_mem & 32'h00ff0000) >> 16;
                else data_mem = (data_mem & 32'hff000000) >> 24;
            end
        end
        else begin
            data_mem = 32'b0;
        end
    end

    always@(*) begin
        if(wen) begin
            pmem_write(waddr,wdata_tmp,wmask);
        end
    end

endmodule
