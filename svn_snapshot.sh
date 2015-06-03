#/bin/sh

OUT=~/t.sh

echo "### V3.02.20 svn snapshot scripts ###" >$OUT

for svn_dir in $(find ./ -name '*.svn' -type d -print)
#for svn_dir in $(svn status | grep ^Perf | cut -d\' -f 2)
do
	svn_dir=${svn_dir%/*}
	cd $svn_dir
	REV=$(svn info . | sed -n '/Last Changed Rev/p' | awk '{print $4}')
	echo "cd $svn_dir"
	echo "echo Reverting: $svn_dir" >>$OUT
	echo "cd $svn_dir" >>$OUT
	echo "svn update . -r $REV >/dev/null" >>$OUT
	echo "svn revert . -R" >>$OUT
	echo "cd ->/dev/null" >> $OUT
	echo "" >>$OUT
	echo "###############">> $OUT
	echo "" >>$OUT
	cd - > /dev/null
done
