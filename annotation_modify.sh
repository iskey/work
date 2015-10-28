GIT_FILES="code/pm/cp/dev/source/cp_boardctl.c 
           code/pm/cp/dev/include/cp_boardctl.h
		   code/pm/cp/rm/rm_agent/source/rm_agent_app.c
		   code/pm/bsp/source/boards/MS/MSXP/ksource/brd_sfp.c
		   code/pm/bsp/source/boards/MS/MSXP/ksource/brd_intr.c
		   code/pm/bsp/source/boards/MS/MSXP/include/brd_sfp.h
		   code/pm/bsp/source/boards/MS/MSXP/include/brd_intr.h
		   code/pm/cp/adm/vmm/common/source/vmm_common_func.c
		   code/pm/bsp/source/boards/MS/MSXP/ksource/brd_cpld.c
		   code/pm/bsp/source/boards/MS/MSXP/include/brd_cpld.h
			"
TOTAL_LINE=$(awk -F @  '/XYC.ISSUE.BSP.DUAL_MNG_ETH\[#0918:*/{print $1}' ${GIT_FILES} | wc -l)
echo $TOTAL_LINE
LINE_CUR=1
for file in $GIT_FILES
do
	LOCAL_LINE=$(awk -F @  '/XYC.ISSUE.BSP.DUAL_MNG_ETH\[#0918:*/{print $1}' ${file} | wc -l)
	awk -F @  '/XYC.ISSUE.BSP.DUAL_MNG_ETH\[#0918:*/{$1="/*XYC.ISSUE.BSP.DUAL_MNG_ETH[#0918:"line_sum"-"tt++"]@"}1' tt=$LINE_CUR line_sum=$TOTAL_LINE $file LINE_CNT=$tt > ~/t.c
	let LINE_CUR+=${LOCAL_LINE}
	mv ~/t.c $file
done
