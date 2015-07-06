#usage get_macro.sh file.c
Macro=`awk '/#if\w?def.*/{print $2}' $1 | sort -u`
for T_M in $Macro
do
	echo "#ifdef" $T_M
	echo "#pragma message(\"$T_M defined, set value to 1.\")"
	echo "#else"
	echo "#pragma message(\"$T_M is not defined, set value to 0.\")"
	echo "#endif"
done
