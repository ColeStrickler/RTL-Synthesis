class TestModule extends Module {val io = IO(new Bundle {
	val input0 =	Input(UInt(32.W))
	val input1 =	Input(UInt(32.W))
	val input2 =	Input(UInt(32.W))
	val input3 =	Input(UInt(32.W))
	val input4 =	Input(UInt(32.W))
	val input5 =	Input(UInt(32.W))
	val output =	Output(UInt(32.W))
})
	val wire0 = Wire(UInt(32.W))
	val wire1 = Wire(UInt(32.W))
	val wire2 = Wire(UInt(32.W))
	val wire3 = Wire(UInt(32.W))
	val wire4 = Wire(UInt(32.W))
	wire2 := io.input4 + io.input3
	wire3 := io.input5 * wire2
	wire0 := io.input1 + io.input0
	wire1 := io.input2 * wire0
	wire4 := wire3 + wire1
	io.output := wire4
}
