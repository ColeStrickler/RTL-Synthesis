class TestModule extends Module {val io = IO(new Bundle {
	val input0 =	Input(UInt(32.W))
	val input1 =	Input(UInt(32.W))
	val input2 =	Input(UInt(32.W))
	val input3 =	Input(UInt(32.W))
	val input4 =	Input(UInt(32.W))
	val output =	Output(UInt(32.W))
})
	val wire0 = Wire(UInt(32.W))
	val reg0 = Reg(UInt(32.W))
	val wire1 = Wire(UInt(32.W))
	val reg1 = Reg(UInt(32.W))
	val wire2 = Wire(UInt(32.W))
	val reg2 = Reg(UInt(32.W))
	val wire3 = Wire(UInt(32.W))
	wire0 := io.input3 * io.input2
	reg0 := wire0
	wire1 := io.input4 * reg0
	reg1 := wire1
	wire2 := reg1 * io.input1
	reg2 := wire2
	wire3 := reg2 - io.input0
	io.output := wire3
}
