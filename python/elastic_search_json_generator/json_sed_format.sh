sed -i -e "s/, \"/, /g" $1
sed -i -e "s/\":/:/g" $1
sed -i -e "s/{\"/{/g" $1
sed -i -e "s#\"#\\\\\"#g" $1
