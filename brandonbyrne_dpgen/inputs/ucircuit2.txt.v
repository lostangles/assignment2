module ../inputs/ucircuit2.txt(
	input clk,
	input rst,
	input [7:0] a,
	input [15:0] b,

	output [31:0] c);

wire [31:0] cwire;

ADD#(.DATAWIDTH(32)) ADDER_0({23'b0,a},{15'b0,b},{cwire});
REG#(.DATAWIDTH(32)) REG_0(.d({cwire}),.clk(clk),.rst(rst),.q({c}));

endmodule