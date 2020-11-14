module ../inputs/mixedcircuit2.txt(
	input clk,
	input rst,
	input [7:0] a,
	input [15:0] b,

	output [31:0] c);

wire [31:0] cwire;

ADD#(.DATAWIDTH(32)) ADDER_0($signed({{24{a[7]}},a}),$signed({{16{b[15]}},b}),{cwire});
REG#(.DATAWIDTH(32)) REG_0(.d({cwire}),.clk(clk),.rst(rst),.q({c}));

endmodule