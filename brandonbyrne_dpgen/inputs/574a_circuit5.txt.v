module ../inputs/574a_circuit5.txt(
	input clk,
	input rst,
	input [63:0] a,
	input [63:0] b,
	input [63:0] c,
	input [63:0] d,
	input [63:0] zero,

	output [63:0] z);

wire [63:0] e,f,g,zwire;
wire [0:0] gEQz;

DIV#(.DATAWIDTH(64)) DIV_0($signed({a}),$signed({b}),{e});
DIV#(.DATAWIDTH(64)) DIV_1($signed({c}),$signed({d}),{f});
MOD#(.DATAWIDTH(64)) MOD_0($signed({a}),$signed({b}),{g});
COMP#(.DATAWIDTH(64)) COMP_0(.a($signed({g})),.b($signed({zero})),.gt(),.lt(),.eq(g));
MUX2x1#(.DATAWIDTH(64)) MUX_0(.a({e}),.b({f}),.sel(gEQz),.d({zwire}));
REG#(.DATAWIDTH(64)) REG_0(.d({zwire}),.clk(clk),.rst(rst),.q({z}));

endmodule