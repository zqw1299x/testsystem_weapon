connect -url tcp:127.0.0.1:3121
source D:/WorkFile/206Proj_20230105/1553_rt_v0331_0089_night_ok/prj/td_check_v0.1/td_check_v0.1.sdk/bd_top_wrapper_hw_platform_0/ps7_init.tcl
targets -set -filter {jtag_cable_name =~ "Digilent JTAG-SMT2 210251A08870" && level==0} -index 1
fpga -file D:/WorkFile/206Proj_20230105/1553_rt_v0331_0089_night_ok/prj/td_check_v0.1/td_check_v0.1.runs/impl_1/bd_top_wrapper.bit
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-SMT2 210251A08870"} -index 0
loadhw -hw D:/WorkFile/206Proj_20230105/1553_rt_v0331_0089_night_ok/prj/td_check_v0.1/td_check_v0.1.sdk/bd_top_wrapper_hw_platform_0/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-SMT2 210251A08870"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-SMT2 210251A08870"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-SMT2 210251A08870"} -index 0
dow D:/WorkFile/206Proj_20230105/1553_rt_v0331_0089_night_ok/prj/td_check_v0.1/td_check_v0.1.sdk/td_check/Debug/td_check.elf
configparams force-mem-access 0
bpadd -addr &main
