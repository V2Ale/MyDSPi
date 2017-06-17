

#Copy the binary file into /usr/bin/

install -D -g root -o root $PWD/MyDSPi /usr/local/bin/MyDSPi


#Register MyDSPi boot script

install -D -g root -o root $PWD/service/MyDSPi_boot   /etc/init.d;
chmod 755 /etc/init.d/MyDSPi_boot;

update-rc.d MyDSPi_boot defaults;
/etc/init.d/MyDSPi_boot start;

#Copy the default file

install -D -g root -o root $PWD/config/MyDSPi.conf /etc/default/MyDSPi.conf

#uninstall
#rm /etc/init.d/MyDSPi_start.sh
#update-rc.d -f MyDSPi_start.sh remove