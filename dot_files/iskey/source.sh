#This script will call scrpits in sub dirctories.

for Dir in ~/iskey/execs/*
do
		source $Dir/iskey.sh
done

source ~/iskey/bin/iskey.sh

 # Add git auto complete
if [ -f ~/.git-completion.bash ]; then
	. ~/.git-completion.bash
fi
