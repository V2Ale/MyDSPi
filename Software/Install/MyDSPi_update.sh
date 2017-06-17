

#Copy the binary file into /usr/bin/

install -D -g root -o root $PWD/MyDSPi /usr/local/bin/MyDSPi
#cp $PWD/MyDSPi_start.sh /usr/local/bin/MyDSPi_start.sh

#Copy the default file

#cp -n $PWD/config/MyDSPi.default /etc/default/MyDSPi.default
# mkdir /etc/default/MyDSPi/
cp -n $PWD/config/MyDSPi.conf /etc/default/MyDSPi.conf

