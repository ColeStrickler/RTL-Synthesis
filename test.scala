class TestModule extends Module {val io = IO(new Bundle {
	val input0 =	Input(UInt(32.W))
	val input1 =	Input(UInt(32.W))
	val input2 =	Input(UInt(32.W))
	val output =	Output(UInt(32.W))
})
	val wire0 = Wire(UInt(32.W))
	val reg0 = Reg(UInt(32.W))
	val wire1 = Wire(UInt(32.W))
	wire0 := io.input1 + io.input0
	reg0 := wire0
	wire1 := io.input2 + reg0
	io.output := wire1
}
