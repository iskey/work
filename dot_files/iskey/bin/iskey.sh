#This is a script module for executable programs.
alias updatedb="updatedb -U ~/ -o ~/iskey/iskey.db -l 0"
alias locate="locate -d ~/iskey/iskey.db"
alias iskey="tmux attach -t iskey"

path=${BASH_SOURCE[0]}
path=${path%/*}
path=${path}/
export PATH=$PATH:$path
echo "Add $path to PATH"

