PIDS=`ps | grep -v proc.sh | grep -v ps | grep -v sh | grep -v PID | awk '{print $1}'`
ps
for PID in $PIDS
do
	P_NAME=`ps | grep $PID | grep -v grep | awk '{print $5}'`
	echo "$PID: $P_NAME"
	mem_lists=$(cat /proc/$PID/smaps | grep Private_ | awk '{print $2}')

#	for mem_item in $mem_lists
#	do
#        let tmp_cnt+=1
#        let mem_item+=1
#        mem_sum=$(expr $mem_sum + $mem_item)
#		let mem_sum+=$mem_item
#	done
#		mem_sum=$(expr $mem_sum - $tmp_cnt)
#	echo "    Private_*:"$mem_sum 

	for contxt in Size Private_  Shared_ Rss Pss 
	do
		mem_lists=`cat /proc/$PID/smaps | grep $contxt | awk '{print $2}'`

		mem_sum=0
		tmp_cnt=0

		for mem_item in $mem_lists
		do
			let mem_sum+=$mem_item
		done
		
		echo "    "$contxt":"$mem_sum 
		
	done
	
done