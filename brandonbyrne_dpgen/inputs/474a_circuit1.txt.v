module ../inputs/474a_circuit1.txt(
	input clk,
	input rst,
	input [7:0] a,
	input [7:0] b,
	input [7:0] c,

	output [7:0] z,
	output [15:0] x);

wire [7:0] d,e;
wire [15:0] f,g;
wire [15:0] xwire;

ADD#(.DATAWIDTH(8)) ADDER_0($signed({a}),$signed({b}),{d});
ADD#(.DATAWIDTH(8)) ADDER_1($signed({a}),$signed({c}),{e});
COMP#(.DATAWIDTH(16)) COMP_0(.a($signed({{8{d[7]}},d})),.b($signed({{8{e[7]}},e})),.gt(g),.lt(),.eq());
MUX2x1#(.DATAWIDTH(8)) MUX_0(.a({d}),.b({e}),.sel(g),.d({z}));
MUL#(.DATAWIDTH(16)) MUL_0($signed({{8{a[7]}},a}),$signed({{8{c[7]}},c}),{f});
SUB#(.DATAWIDTH(16)) SUB_0($signed({f}),$signed({{8{d[7]}},d}),{xwire});
REG#(.DATAWIDTH(16)) REG_0(.d({xwire}),.clk(clk),.rst(rst),.q({x}));

endmodule