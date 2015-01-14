#This script will call scrpits in sub dirctories.

for Fir in /iskey/execs/*
do
	source $Dir/iskey.sh
done

alias updatedb="updatedb -U ~/ -o ~/iskey/iskey.db -l 0"
alias locate="locate -d ~/iskey/iskey.db"
