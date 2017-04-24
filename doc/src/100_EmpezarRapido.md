# Empezar rápido desde linux

Para usar la Orange Pi Zero tendremos que crear imágenes arrancables
en tarjetas micro SD.

## Crear una SD arrancable

Dependiendo de donde conectemos la tarjeta tendremos que usar
diferentes rutas.  En el procedimiento descrito a continuación
`${card}` será la ruta al dispositivo de la tarjeta y ${p} la
partición (si la hay).

Si la tarjeta se conecta via adaptador USB, linux la va a asociar a un
dispositivo /dev/sdx, por ejemplo en mi portátil el disco duro es
`/dev/sda` las distintas particiones serán `/dev/sda1`, `/devb/sda2`,
etc.

Si conectamos una memoria con un adaptador USB linux la podría mapear
en `/dev/sdb` por ejemplo.

Si la memoria se conecta mediante una ranura SD, linux la asociará a
un dispositivo `/dev/mmcblk0` o `/dev/mmcblk1`, etc. etc. Dependerá de
la ranura usada. Las particiones en este tipo de dispositivos tienen
rutas como por ejemplo `/dev/mmcblk0p1`.

Los datos se pueden almacenar directamente en la memoria SD o en una
partición creada en la memoria.

Resumiendo:

* `${card}` será `/dev/sdb` o `/dev/mmcblk0`
* ${card}${p} será `/dev/sdb1` o `/dev/mmcblk0p1`

Antes de seguir adelante hay que estar completamente seguro del
dispositivo asociado a nuestra memoria SD para no armar ningún
estropicio.

Hay varias comprobaciones que se pueden hacer:

`dmesg |tail` nos permitirá echar un ojo a los últimos mensajes en el
log del sistema. Si acabamos de insertar la memoria veremos el
dispositivo usado.

`sudo fdisk -l` nos permite ver las particiones montadas en nuestro
linux, por ejemplo con mi SD en la ranura SD de mi portatil la salida
es (entre otras cosas, he obviado las particiones de los discos
duros):

~~~~{bash}
Disk /dev/mmcblk0: 7.4 GiB, 7948206080 bytes, 15523840 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000
~~~~

`cat /proc/partitions` también nos dará una lista de particiones, en
mi portátil las que interesan son:

~~~~
 179        0    7761920 mmcblk0
 179        1    7757824 mmcblk0p1
~~~~

Descargamos la imagen de Jessie adaptada a la _Orange Pi Zero_ desde
la página <https://www.armbian.com/download/>

Descomprimimos la imagen y la grabamos en la tarjeta SD con el comando:

~~~~
sudo dd if=./Armbian_5.24_Orangepizero_Debian_jessie_3.4.113.img of=/dev/mmcblk0
~~~~

Insertamos la tarjeta en la _Orange Pi_ y le damos alimentación. El
primer arranque llevará alrededor de tres minutos, y tras ese tiempo
aun hará falta un minuto más para poder hacer login. Este retardo es
debido a que el sistema intentará actualizar la lista de paquetes y
creará un area de swap de emergencia en la SD, y además cambiará el
tamaño de la partición que hemos creado para ocupar todo el espacio
libre en la SD.

De momento solo la he arrancado y efectivamente las particiones han
cambiado tras el arranque así que tiene buena pinta.

Volvemos a insertar la SD en la _Orange Pi_ y la conectamos con un
cable ethernet al router de casa. El Armbian viene configurado por
defecto para obtener su IP desde un servidor DHCP.

Como mi cutre-router no me da información de las IP asignadas usamos
_nmap_:

~~~~
nmap -sP 192.168.0.0 /24
~~~~

Con eso averiguamos la IP asignada a la _Orange Pi Zero_ y ya podemos
hacer login con el siguiente comando ^[La password por defecto de
Armbian es __1234__, nos pedirá cambiarla en el primer login.]:

~~~~
ssh root@192.168.0.109
~~~~

¡Y ya estamos!

![Primer login en _Orange Pi_](src/img/OrangePiZero_FirstLogin.png)


Lo primero es poner al dia el sistema:

~~~~
apt-get update
apt-get upgrade
~~~~

Si quieres puedes reconfigurar el _time zone_:

~~~~
dpgk-reconfigura tzdata
~~~~

## Conexión WIFI

Vamos a comprobar que todo va bien:

~~~~
root@orangepizero:~# iwconfig
lo        no wireless extensions.

tunl0     no wireless extensions.

wlan0     IEEE 802.11bgn  ESSID:off/any
          Mode:Managed  Access Point: Not-Associated   Tx-Power=20 dBm
          Retry  long limit:7   RTS thr:off   Fragment thr:off
          Encryption key:off
          Power Management:on

eth0      no wireless extensions.
~~~~

Todo tiene buena pinta, vamos a ver si detecta WIFIs:

~~~~
root@orangepizero:~# iwlist wlan0  scan |grep ESSID
                    ESSID:"wificlientesR"
                    ESSID:"casa_de_verano"
                    ESSID:"MOVISTAR_BEEF"
                    ESSID:"wificlientesR"
                    ESSID:"R-wlan90"
                    ESSID:"MOVISTAR_BAAF"
                    ESSID:"ababab"
                    ESSID:"WLAN 77"
                    ESSID:"castillo"
                    ESSID:"unaWifi"
                    ESSID:""
                    ESSID:"mikasa"
~~~~

Para configurar el wifi echamos un ojo al fichero
`/etc/network/interfaces` pero en ese mismo fichero encontramos el
aviso:

~~~~
# Armbian ships with network-manager installed by default. To save you time
# and hassles consider using 'sudo nmtui' instead of configuring Wi-Fi settings
# manually.
~~~~

Así que basta con ejecutar `sudo nmtui` y ya podemos dar de alta
nuestra wifi (yo la prefiero con IP estática).

![Configuración WIFI](src/img/OrangePiZero_tmtui.png)

Ejecutamos `ifconfig` y ya vemos nuestro nuevo interface configurado:

~~~~
ifconfig

wlan0     Link encap:Ethernet  HWaddr a4:7c:f2:9a:97:7c
          inet addr:192.168.0.120  Bcast:192.168.0.255  Mask:255.255.255.0
          inet6 addr: fe80::a67c:f2ff:fe9a:977c/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:2 errors:0 dropped:0 overruns:0 frame:0
          TX packets:8 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:328 (328.0 B)  TX bytes:852 (852.0 B)
~~~~



