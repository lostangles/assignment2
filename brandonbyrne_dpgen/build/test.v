module test(
	input clk,
	input rst,
	input [7:0] a,
	input [7:0] b,

	output [7:0] c);

wire [7:0] cwire;

ADD#(.DATAWIDTH(8)) ADDER_0({a},{b},{cwire});
REG#(.DATAWIDTH(8)) REG_0(.d({cwire}),.clk(clk),.rst(rst),.q({c}));

endmodule