module ../inputs/474a_circuit4.txt(
	input clk,
	input rst,
	input [63:0] a,
	input [63:0] b,
	input [63:0] c,

	output [31:0] z,
	output [31:0] x);

wire [63:0] d,e,f,g,h;
wire [0:0] dLTe,dEQe;
wire [63:0] xrin,zrin;


ADD#(.DATAWIDTH(64)) ADDER_0($signed({a}),$signed({b}),{d});
ADD#(.DATAWIDTH(64)) ADDER_1($signed({a}),$signed({c}),{e});
SUB#(.DATAWIDTH(64)) SUB_0($signed({a}),$signed({b}),{f});
COMP#(.DATAWIDTH(64)) COMP_0(.a($signed({d})),.b($signed({e})),.gt(),.lt(),.eq(d));
COMP#(.DATAWIDTH(1)) COMP_1(.a($signed({d})),.b($signed({e})),.gt(),.lt(dLTe),.eq());
MUX2x1#(.DATAWIDTH(64)) MUX_0(.a({d}),.b({e}),.sel(dLTe),.d({g}));
MUX2x1#(.DATAWIDTH(64)) MUX_1(.a({g}),.b({f}),.sel(dEQe),.d({h}));
REG#(.DATAWIDTH(64)) REG_0(.d({g}),.clk(clk),.rst(rst),.q({greg}));
REG#(.DATAWIDTH(64)) REG_1(.d({h}),.clk(clk),.rst(rst),.q({hreg}));
SHL#(.DATAWIDTH(64)) SHL_0({hreg},{{63{dLTe[0]}},dLTe},{xrin});
SHR#(.DATAWIDTH(64)) SHR_0({greg},{{63{dEQe[0]}},dEQe},{zrin});
REG#(.DATAWIDTH(32)) REG_2(.d({xrin}),.clk(clk),.rst(rst),.q({x}));
REG#(.DATAWIDTH(32)) REG_3(.d({zrin}),.clk(clk),.rst(rst),.q({z}));

endmodule