#This is a script module for executable programs.
path=${BASH_SOURCE[0]}
path=${path%/*}
path=${path}/bin/
export PATH=$PATH:$path
echo "Add $path to PATH"
