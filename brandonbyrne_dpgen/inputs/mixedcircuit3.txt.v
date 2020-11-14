module ../inputs/mixedcircuit3.txt(
	input clk,
	input rst,
	input [31:0] a,
	input [15:0] b,

	output [7:0] c);

wire [7:0] cwire;

ADD#(.DATAWIDTH(8)) ADDER_0($signed({a}),$signed({b}),{cwire});
REG#(.DATAWIDTH(8)) REG_0(.d({cwire}),.clk(clk),.rst(rst),.q({c}));

endmodule