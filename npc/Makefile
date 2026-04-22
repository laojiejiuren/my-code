all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self"
	# 1. 用Verilator编译Our.v和sim_main.cpp
	verilator --cc --exe --build -j 0 -Wall --trace-fst main.cpp example.v
	# 2. 打印提示信息
	@echo "=== 编译成功 ==="
	# 3. 运行仿真程序
	./obj_dir/Vour
	@echo "=== 仿真执行完成 ==="
	# 4. 打开GTKWave查看波形
	gtkwave wave.fst
