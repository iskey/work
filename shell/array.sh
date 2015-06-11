#/bin/sh

AA=(a b c d e f)
for i in ${AA[@]};
do
	echo $i
done

echo ${AA[@]}
echo ${AA[2]}
echo ${AA[3]}

BB=($(ls))

for file in ${BB[@]}
do
	echo $file
done

echo ${AA[*]}

#Get the depth of the array
echo ${#AA[@]}
echo ${#BB[@]}
