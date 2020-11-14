module ../inputs/474a_circuit2.txt(
	input clk,
	input rst,
	input [31:0] a,
	input [31:0] b,
	input [31:0] c,

	output [31:0] z,
	output [31:0] x);

wire [31:0] d,e,f,g,h;
wire [0:0] dLTe,dEQe;
wire [31:0] zwire,xwire;

ADD#(.DATAWIDTH(32)) ADDER_0($signed({a}),$signed({b}),{d});
ADD#(.DATAWIDTH(32)) ADDER_1($signed({a}),$signed({c}),{e});
SUB#(.DATAWIDTH(32)) SUB_0($signed({a}),$signed({b}),{f});
COMP#(.DATAWIDTH(32)) COMP_0(.a($signed({d})),.b($signed({e})),.gt(),.lt(),.eq(d));
COMP#(.DATAWIDTH(1)) COMP_1(.a($signed({d})),.b($signed({e})),.gt(),.lt(dLTe),.eq());
MUX2x1#(.DATAWIDTH(32)) MUX_0(.a({d}),.b({e}),.sel(dLTe),.d({g}));
MUX2x1#(.DATAWIDTH(32)) MUX_1(.a({g}),.b({f}),.sel(dEQe),.d({h}));
SHL#(.DATAWIDTH(32)) SHL_0({g},{{31{dLTe[0]}},dLTe},{xwire});
SHR#(.DATAWIDTH(32)) SHR_0({h},{{31{dEQe[0]}},dEQe},{zwire});
REG#(.DATAWIDTH(32)) REG_0(.d({xwire}),.clk(clk),.rst(rst),.q({x}));
REG#(.DATAWIDTH(32)) REG_1(.d({zwire}),.clk(clk),.rst(rst),.q({z}));

endmodule