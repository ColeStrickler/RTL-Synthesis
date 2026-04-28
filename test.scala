class TestModule extends Module {val io = IO(new Bundle {
	val input0 =	Input(UInt(32.W))
	val input1 =	Input(UInt(32.W))
	val output =	Output(UInt(32.W))
})
	val wire0 = Wire(UInt(32.W))
	wire0 := io.input1 * io.input0
	io.output := wire0
}
