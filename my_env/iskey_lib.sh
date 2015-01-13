#This is a script module for libraries.
path=${BASH_SOURCE[0]}
path=${path%/*}
path=${path}/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$path
echo "Add $path to LD_LIBRARY_PATH"
