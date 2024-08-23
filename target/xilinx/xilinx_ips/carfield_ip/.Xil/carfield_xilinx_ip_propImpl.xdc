set_property SRC_FILE_INFO {cfile:/usr/scratch2/badile40/msc24h6/carfield/target/xilinx/xilinx_ips/carfield_ip/carfield_ip.srcs/constrs_1/imports/constraints/carfield_xilinx_ip.xdc rfile:../carfield_ip.srcs/constrs_1/imports/constraints/carfield_xilinx_ip.xdc id:1} [current_design]
set_property SRC_FILE_INFO {cfile:/usr/scratch2/badile40/msc24h6/carfield/target/xilinx/xilinx_ips/carfield_ip/carfield_ip.srcs/constrs_1/imports/constraints/carfield.xdc rfile:../carfield_ip.srcs/constrs_1/imports/constraints/carfield.xdc id:2 order:LATE scoped_inst:i_carfield_xilinx/i_carfield} [current_design]
set_property src_info {type:XDC file:1 line:57 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath -from [get_pins i_carfield_xilinx/i_hyper_cdc_dst/i_cdc_fifo_gray_*/*reg*/C] -to [get_pins i_carfield_xilinx/i_carfield/i_cheshire_wrap/i_cheshire_ext_llc_cdc_src/i_cdc_fifo_gray_*/i_spill_register/spill_register_flushable_i/*reg*/D] 20
set_property src_info {type:XDC file:1 line:61 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath -from [get_pins i_carfield_xilinx/i_carfield/i_cheshire_wrap/i_cheshire_ext_llc_cdc_src/i_cdc_fifo_gray_*/*reg*/C] -to [get_pins i_carfield_xilinx/i_hyper_cdc_dst/i_cdc_fifo_gray_*/i_spill_register/spill_register_flushable_i/*reg*/D] 20
set_property src_info {type:XDC file:1 line:65 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath -from [get_pins i_carfield_xilinx/i_hyper_cdc_dst/i_cdc_fifo_gray_*/*reg*/C] -to [get_pins i_carfield_xilinx/i_carfield/i_cheshire_wrap/i_cheshire_ext_llc_cdc_src/i_cdc_fifo_gray_*/*i_sync/*reg*/D] 20
set_property src_info {type:XDC file:1 line:69 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath -from [get_pins i_carfield_xilinx/i_carfield/i_cheshire_wrap/i_cheshire_ext_llc_cdc_src/i_cdc_fifo_gray_*/*reg*/C] -to [get_pins i_carfield_xilinx/i_hyper_cdc_dst/i_cdc_fifo_gray_*/*i_sync/*reg*/D] 20
current_instance i_carfield_xilinx/i_carfield
set_property src_info {type:SCOPED_XDC file:2 line:23 export:INPUT save:INPUT read:READ} [current_design]
set_case_analysis 1 [get_pins {i_carfield_reg_top/u_*_clk_en/q_reg[0]/Q}]
set_property src_info {type:SCOPED_XDC file:2 line:41 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -hold -through [get_pins -filter {DIRECTION==OUT} -of_objects [get_cells -hier -filter {REF_NAME == rstgen || ORIG_REF_NAME == rstgen}]]
set_property src_info {type:SCOPED_XDC file:2 line:42 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -setup -hold -from [get_pins -of_objects [get_cells -hier -filter {NAME=~*i_carfield_reg_top/u_*_rst/*}] -filter {IS_CLOCK}] -to [get_clocks *domain_clk]
set_property src_info {type:SCOPED_XDC file:2 line:48 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath -from [get_pins i_host_rstgen/i_rstgen_bypass/synch_regs_q_reg[3]/C] -through [get_pins -of_object [get_cells -hier -filter {REF_NAME==clk_mux_glitch_free || ORIG_REF_NAME==clk_mux_glitch_free}] -filter { NAME =~*async* }] 20
set_property src_info {type:SCOPED_XDC file:2 line:52 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -hold -to [get_pins -of_objects [get_cells -hier -filter {NAME =~ "*i_carfield_rstgen/*/i_rstgen_bypass/synch*"}] -filter {REF_PIN_NAME == CLR}]
set_property src_info {type:SCOPED_XDC file:2 line:59 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath_only -through [get_cells i_carfield_reg_top] -from [get_clocks -filter {NAME !~ "*clk_50*"}] -to [get_clocks *clk_50*] 20
set_property src_info {type:SCOPED_XDC file:2 line:60 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath_only -through [get_cells i_carfield_reg_top] -from [get_clocks *clk_50*] -to [get_clocks -filter {NAME !~ "*clk_50*"}] 20
set_property src_info {type:SCOPED_XDC file:2 line:73 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -hold -through [get_nets -filter {NAME=~"*async*"} -of_objects [get_cells -hier -filter {REF_NAME =~ cdc_2phase_src* || ORIG_REF_NAME =~ cdc_2phase_src*}]]
set_property src_info {type:SCOPED_XDC file:2 line:77 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -hold -through [get_nets -filter {NAME=~"*async*"} -of_objects [get_cells -hier -filter {REF_NAME == cdc_4phase_src || ORIG_REF_NAME == cdc_4phase_src}]]
set_property src_info {type:SCOPED_XDC file:2 line:82 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -hold -through [get_pins -of_objects [get_cells -hier -filter {ORIG_REF_NAME=="sync" || REF_NAME=="sync"}] -filter {REF_PIN_NAME=~serial_i}]
set_property src_info {type:SCOPED_XDC file:2 line:88 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -hold -through [get_pins -of_objects [get_cells -hier -filter {ORIG_REF_NAME=="pulp_sync" || REF_NAME=="pulp_sync"}] -filter {REF_PIN_NAME=~serial_i}]
set_property src_info {type:SCOPED_XDC file:2 line:94 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -hold -through [get_pins -of_objects [get_cells -hier -filter {ORIG_REF_NAME=="prim_ot_flop_2sync" || REF_NAME=="prim_ot_flop_2sync"}] -filter {REF_PIN_NAME=~d_i*}]
