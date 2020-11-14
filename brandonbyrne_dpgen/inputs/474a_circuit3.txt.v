module ../inputs/474a_circuit3.txt(
	input clk,
	input rst,
	input [15:0] a,
	input [15:0] b,
	input [15:0] c,
	input [15:0] d,
	input [15:0] e,
	input [15:0] f,
	input [15:0] g,
	input [15:0] h,
	input [7:0] sa,

	output [15:0] avg);

wire [31:0] l00,l01,l02,l03,l10,l11,l2,l2div2,l2div4,l2div8;

ADD#(.DATAWIDTH(32)) ADDER_0($signed({{16{a[15]}},a}),$signed({{16{b[15]}},b}),{l00});
ADD#(.DATAWIDTH(32)) ADDER_1($signed({{16{c[15]}},c}),$signed({{16{d[15]}},d}),{l01});
ADD#(.DATAWIDTH(32)) ADDER_2($signed({{16{e[15]}},e}),$signed({{16{f[15]}},f}),{l02});
ADD#(.DATAWIDTH(32)) ADDER_3($signed({{16{g[15]}},g}),$signed({{16{h[15]}},h}),{l03});
ADD#(.DATAWIDTH(32)) ADDER_4($signed({l00}),$signed({l01}),{l10});
ADD#(.DATAWIDTH(32)) ADDER_5($signed({l02}),$signed({l03}),{l11});
ADD#(.DATAWIDTH(32)) ADDER_6($signed({l10}),$signed({l11}),{l2});
SHR#(.DATAWIDTH(32)) SHR_0({l2},{{24{sa[7]}},sa},{l2div2});
SHR#(.DATAWIDTH(32)) SHR_1({l2div2},{{24{sa[7]}},sa},{l2div4});
SHR#(.DATAWIDTH(32)) SHR_2({l2div4},{{24{sa[7]}},sa},{l2div8});
REG#(.DATAWIDTH(16)) REG_0(.d({l2div8}),.clk(clk),.rst(rst),.q({avg}));

endmodule