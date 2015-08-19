GIT_FILES="code/pm/cp/dev/source/cp_boardctl.c 
		   code/pm/cp/rm/rm_agent/source/rm_agent_app.c
		   code/pm/bsp/source/boards/MS/MSXP/ksource/brd_sfp.c
		   code/pm/bsp/source/boards/MS/MSXP/ksource/brd_intr.c
		   code/pm/bsp/source/boards/MS/MSXP/include/brd_sfp.h
		   code/pm/bsp/source/boards/MS/MSXP/include/brd_intr.h
		   code/pm/cp/adm/vmm/common/source/vmm_common_func.c
		   code/pm/bsp/source/boards/MS/MSXP/ksource/brd_cpld.c
		   code/pm/bsp/source/boards/MS/MSXP/include/brd_cpld.h
			"
TOTAL_LINE=$(awk -F @  '/XYC.ISSUE.BSP.DUAL_MNG_ETH\[#0918:\d*\]*/{print $1}' ${GIT_FILES} | wc -l)
echo $TOTAL_LINE
for file in $GIT_FILES
do
	awk -F @  '/\[#0918:\d*\]*/{$1="/*XYC.ISSUE.BSP.DUAL_MNG_ETH[#0918:"line_sum"-"tt++"]@"}1' tt=1 line_sum=$TOTAL_LINE $file > ~/t.c
	mv ~/t.c $file
done

