#/bin/sh

### exec this scripts in root directory.
### the snapshot of the work copy will be saved to $OUT.

OUT=$(pwd)/snapshot.sh
PATCH_DIR=$(pwd)/patches
EAT=$(pwd)/eat
patch_idx=0

echo "### V3.02.20 svn snapshot scripts ###" >$OUT
echo "PATCH_DIR=\$\(pwd\)" >>$OUT

#Prepare eat program for svn diff.
touch $EAT
chmod a+x $EAT

mkdir -p $PATCH_DIR

#Find svn repositories.
svn_repos=$(find ./ -name '*.svn' -type d -print)
#svn_repos=$(svn status | grep ^Perf | cut -d\' -f 2)
for svn_repo in $svn_repos
do
	svn_dir=${svn_repo%/*}
	#svn_dir=$svn_repo
	cd $svn_dir
	#Get svn local copy's revision.
	REV=$(svn info . | sed -n '/Revision:/p' | awk '{print $2}')
	echo "cd $svn_dir"
	echo "echo Reverting: $svn_dir" >>$OUT
	echo "cd $svn_dir" >>$OUT
	echo "svn cleanup" >>$OUT
	echo "svn revert . -R" >>$OUT
	echo "svn update . -r $REV" >>$OUT
	echo "svn revert . -R" >>$OUT
	
	#Get svn local copy's changed files.
	M_LIST=$(svn diff -r BASE:COMMITTED --diff-cmd $EAT | sed -n '/Index:/p' | awk '{print $2}')
	for svn_file in $M_LIST
	do
		echo "  Changed File: "$svn_file
		#Get changed file's revision.
		file_rev=$(svn info $svn_file | sed -n '/Last Changed Rev:/p' | awk '{print $4}')
		echo "#----------" >>$OUT
		echo "svn revert $svn_file" >>$OUT		
		echo "svn update -r $file_rev $svn_file" >>$OUT
		echo "svn revert $svn_file" >>$OUT
	done
	
	#Get temporary patches
	PATCH_FILES=$(svn diff . --diff-cmd $EAT | sed -n '/Index:/p' | awk '{print $2}')
	if [ ""x != "$PATCH_FILES"x ];then
		let patch_idx+=1
		echo "#==========" >>$OUT
		svn diff . >$PATCH_DIR/$patch_idx.patch
		echo "patch -p0 <\$PATCH_DIR/$patch_idx.patch" >>$OUT
	fi
	
	echo "cd ->/dev/null" >> $OUT
	echo "" >>$OUT
	echo "###############">> $OUT
	echo "" >>$OUT
	cd - > /dev/null
done
