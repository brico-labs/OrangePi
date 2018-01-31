Empezar rápido desde linux
==========================

Para usar la Orange Pi Zero tendremos que crear imágenes arrancables en
tarjetas micro SD.

Crear una SD arrancable
-----------------------

Dependiendo de donde conectemos la tarjeta tendremos que usar diferentes
rutas. En el procedimiento descrito a continuación `${card}` será la
ruta al dispositivo de la tarjeta y ${p} la partición (si la hay).

Si la tarjeta se conecta via adaptador USB, linux la va a asociar a un
dispositivo /dev/sdx, por ejemplo en mi portátil el disco duro es
`/dev/sda` las distintas particiones serán `/dev/sda1`, `/devb/sda2`,
etc.

Si conectamos una memoria con un adaptador USB linux la podría mapear en
`/dev/sdb` por ejemplo.

Si la memoria se conecta mediante una ranura SD, linux la asociará a un
dispositivo `/dev/mmcblk0` o `/dev/mmcblk1`, etc. etc. Dependerá de la
ranura usada. Las particiones en este tipo de dispositivos tienen rutas
como por ejemplo `/dev/mmcblk0p1`.

Los datos se pueden almacenar directamente en la memoria SD o en una
partición creada en la memoria.

Resumiendo:

-   `${card}` será `/dev/sdb` o `/dev/mmcblk0`
-   *c**a**r**d*{p} será `/dev/sdb1` o `/dev/mmcblk0p1`

Antes de seguir adelante hay que estar completamente seguro del
dispositivo asociado a nuestra memoria SD para no armar ningún
estropicio.

Hay varias comprobaciones que se pueden hacer:

`dmesg |tail` nos permitirá echar un ojo a los últimos mensajes en el
log del sistema. Si acabamos de insertar la memoria veremos el
dispositivo usado.

`sudo fdisk -l` nos permite ver las particiones montadas en nuestro
linux, por ejemplo con mi SD en la ranura SD de mi portatil la salida es
(entre otras cosas, he obviado las particiones de los discos duros):

``` {bash}
Disk /dev/mmcblk0: 7.4 GiB, 7948206080 bytes, 15523840 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000
```

`cat /proc/partitions` también nos dará una lista de particiones, en mi
portátil las que interesan son:

     179        0    7761920 mmcblk0
     179        1    7757824 mmcblk0p1

Descargamos la imagen de Jessie adaptada a la *Orange Pi Zero* desde la
página <https://www.armbian.com/download/>

Descomprimimos la imagen y la grabamos en la tarjeta SD con el comando:

    sudo dd if=./Armbian_5.24_Orangepizero_Debian_jessie_3.4.113.img of=/dev/mmcblk0

Insertamos la tarjeta en la *Orange Pi* y le damos alimentación. El
primer arranque llevará alrededor de tres minutos, y tras ese tiempo aun
hará falta un minuto más para poder hacer login. Este retardo es debido
a que el sistema intentará actualizar la lista de paquetes y creará un
area de swap de emergencia en la SD, y además cambiará el tamaño de la
partición que hemos creado para ocupar todo el espacio libre en la SD.

De momento solo la he arrancado y efectivamente las particiones han
cambiado tras el arranque así que tiene buena pinta.

Volvemos a insertar la SD en la *Orange Pi* y la conectamos con un cable
ethernet al router de casa. El Armbian viene configurado por defecto
para obtener su IP desde un servidor DHCP.

Como mi cutre-router no me da información de las IP asignadas usamos
*nmap*:

    nmap -sP 192.168.0.0/24

Con eso averiguamos la IP asignada a la *Orange Pi Zero* y ya podemos
hacer login con el siguiente comando [1]:

    ssh root@192.168.0.109

¡Y ya estamos!

![Primer login en *Orange Pi*](doc/src/img/OrangePiZero_FirstLogin.png)

Lo primero es poner al dia el sistema:

    apt-get update
    apt-get upgrade

Si quieres puedes reconfigurar el *time zone*:

    dpkg-reconfigure tzdata

Conexión WIFI
-------------

Vamos a comprobar que todo va bien:

    root@orangepizero:~# iwconfig
    lo        no wireless extensions.

    tunl0     no wireless extensions.

    wlan0     IEEE 802.11bgn  ESSID:off/any
              Mode:Managed  Access Point: Not-Associated   Tx-Power=20 dBm
              Retry  long limit:7   RTS thr:off   Fragment thr:off
              Encryption key:off
              Power Management:on

    eth0      no wireless extensions.

Todo tiene buena pinta, vamos a ver si detecta WIFIs:

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

Para configurar el wifi echamos un ojo al fichero
`/etc/network/interfaces` pero en ese mismo fichero encontramos el
aviso:

    # Armbian ships with network-manager installed by default. To save you time
    # and hassles consider using 'sudo nmtui' instead of configuring Wi-Fi settings
    # manually.

Así que basta con ejecutar `sudo nmtui` y ya podemos dar de alta nuestra
wifi (yo la prefiero con IP estática).

![Configuración WIFI](doc/src/img/OrangePiZero_tmtui.png)

Ejecutamos `ifconfig` y ya vemos nuestro nuevo interface configurado:

    ifconfig

    wlan0     Link encap:Ethernet  HWaddr a4:7c:f2:9a:97:7c
              inet addr:192.168.0.120  Bcast:192.168.0.255  Mask:255.255.255.0
              inet6 addr: fe80::a67c:f2ff:fe9a:977c/64 Scope:Link
              UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
              RX packets:2 errors:0 dropped:0 overruns:0 frame:0
              TX packets:8 errors:0 dropped:0 overruns:0 carrier:0
              collisions:0 txqueuelen:1000
              RX bytes:328 (328.0 B)  TX bytes:852 (852.0 B)

Orange Pi Zero, características técnicas
========================================

La tarjeta de desarrollo Orange Pi Zero viene equipada con un procesador
Cortex A7 Allwinner H2+ quad core, con 256 o 512MB RAM, Ethernet, y
puertos USB. Disponible en Aliexpress (tienda oficial) por 6.99 dolares,
mas 3.39 dolares como gastos de envío.

Especificaciones
----------------

-   SoC – Allwinner H2(+) quad core Cortex A7 processor @ 1.2 GHz with
    Mali-400MP2 GPU @ 600 MHz
-   System Memory – 256 to 512 MB DDR3-1866 SDRAM
-   Storage – micro SD card slot
-   Connectivity – 10/100M Ethernet + 802.11 b/g/n WiFi (Allwinner XR819
    WiFi module) with u.FL antenna connector and external antenna
-   USB – 1x USB 2.0 host ports, 1x micro USB OTG port
-   Expansion headers – Unpopulated 26-pin “Raspberry Pi B+” header +
    13-pin header with headphone, 2x USB 2.0, TV out, microphone and IR
    receiver signals
-   Debugging – Unpopulated 3-pin header for serial console
-   Misc – 2x LEDs
-   Power Supply – 5V via micro USB port or optional PoE
-   Dimensions – 52 x 46 mm
-   Weight – 26 grams

Esquema de pines
----------------

Un excelente esquema de pines puede conseguirse en
[OSHLab](https://oshlab.com/orange-pi-zero-pinout/)

![Pineado Orange Pi](doc/src/img/Orange-Pi-Zero-Pinout.jpg)

El mapeado de los pines de la Orange (de los micros Allwinner en
realidad) en el kernel de Linux viene dado por la formula:

    (Position of letter in alphabet - 1) * 32 + Pin number

Para todos los pines PA de nuestra Orange Pi Zero, el número del kernel
coincide con el del pin. Pero para los pines *PG06* y *PG07* se
corresponden con los códigos *198* y *199*.

Esquemas eléctricos
-------------------

Pueden bajarse de
[aquí](http://harald.studiokubota.com/wordpress/wp-content/uploads/2016/11/Orange-Pi-Zero-Schanetics-v1_11.pdf)

Accediendo al hardware desde linea de comandos
==============================================

La memoria es más que suficiente para correr programas. El Armbian
consume únicamente 40Mb en funcionamiento.

    root@orangepizero:~# free
                 total       used       free     shared    buffers     cached
    Mem:        247068     122300     124768       4620       7908      69548
    -/+ buffers/cache:      44844     202224
    Swap:       131068          0     131068

Vamos a hacer algunas pruebas con el hardware. En Armbian, como todo
Unix que se precie, todo es un fichero.

En el directorio `/sys/class` encontraremos cosas interesantes:

    root@orangepizero:~# ls /sys/class
    backlight  cuse     graphics     i2c-dev    mdio_bus  power_supply  script   spidev      thermal     video4linux
    bdi    devfreq  hdmi     ieee80211  mem       ppp       scsi_device  spi_master  tty         vtconsole
    block      disp     hidraw   input      misc      rc        scsi_disk    sunxi_cma_test  udc
    bsg    dma      hwmon    leds       mmc_host  rfkill        scsi_host    sunxi_dump  usb_device
    cedar_dev  gpio     i2c-adapter  lirc       net       rtc       sound    sunxi_info  vc

LEDs
----

Si miramos dentro del directorio `leds` veremos que hay un directorio
que representa cada uno de los leds de la placa:

    root@orangepizero:~# cd /sys/class/leds/
    root@orangepizero:/sys/class/leds# ls
    green_led  red_led

Podemos ver, por ejemplo, a que evento está asociado cada led ejecutando
`cat green_led/trigger` (tiene el valor `default_on`) o
`cat red_led/trigger` (tiene el valor `none`).

    root@orangepizero:/sys/class/leds# cat green_led/trigger 
    none mmc0 mmc1 timer heartbeat backlight [default-on] rfkill0 phy1rx phy1tx phy1assoc phy1radio 
    root@orangepizero:/sys/class/leds# cat red_led/trigger 
    [none] mmc0 mmc1 timer heartbeat backlight default-on rfkill0 phy1rx phy1tx phy1assoc phy1radio 

O podemos encender el led rojo ejecutando `echo 1 > red_led/brightness`,
y para apagarlo ya os podéis imaginar que es
`echo 0 > red_led/brightness`.

GPIO
----

Podemos ver los GPIO disponibles ejecutando:

    root@orangepizero:~# cat /sys/kernel/debug/gpio
    GPIOs 0-383, platform/sunxi-pinctrl, sunxi-pinctrl:
     gpio-10  (?                   ) out hi
     gpio-17  (red_led             ) out lo
     gpio-202 (xradio_irq          ) in  lo
     gpio-354 (?                   ) out hi
     gpio-362 (green_led           ) out hi

Podemos activar un nuevo puerto GPIO, digamos el 15:

    root@orangepizero:~# echo 15 >/sys/class/gpio/export

Ahora veremos el puerto activo:

    root@orangepizero:~# cat /sys/kernel/debug/gpio 
    GPIOs 0-383, platform/sunxi-pinctrl, sunxi-pinctrl:
     gpio-10  (?                   ) out hi
     gpio-15  (sysfs               ) in  lo
     gpio-17  (red_led             ) out lo
     gpio-202 (xradio_irq          ) in  lo
     gpio-354 (?                   ) out hi
     gpio-362 (green_led           ) out hi

En el directorio `/sys/class/gpio/gpio15/` tendremos los interfaces
usuales para puertos gpio definidos en el kernel de linux.

Bibliotecas útiles
==================

Python
------

### Instalación de virtualenv

Para probar bibliotecas de Python instalamos:

``` {bash}
sudo aptitude install python-pip
sudo aptitude install python-virtualenv
sudo aptitude install python-dev
```

El caso es que el *virtualenv* no me funciona después de ejecutar estos
pasos. Finalmente he tenido que ejecutar, como *root*, los siguientes
comandos:

    pip install --upgrade pip
    pip install --upgrade virtualenv

Despues de eso ya he podido usar *virtualenv* sin problemas.

### orangepi\_PC\_gpio\_pyH3

Tenemos dos repos disponibles:

El repositorio original con las librerías *gpio* para H3:

    git clone https://github.com/duxingkei33/orangepi_PC_gpio_pyH3

Y un fork del repositorio original **ya adaptado** a la Orange Pi Zero
(que es el que usaremos):

    git clone https://github.com/nvl1109/orangepi_PC_gpio_pyH3

**IMPORTANTE:** Si usamos el repositorio original tenemos que revisar el
fichero `orangepi_PC_gpio_pyH3/pyA20/gpio/mapping.h`

Por ejemplo: la definicion de *STATUS\_LED* debe quedar en el *GPA17* en
lugar de *GPA15*:

    {   "STATUS_LED",  SUNXI_GPA(17),  2   },

Nos clonamos el repo. Vamos a hacer todas las pruebas desde la cuenta de
**root**.

Creamos un entorno para pruebas y lo activamos:

    virtualenv test_pyH3_zero
    source test_pyH3_zero/bin/activate

Compilamos la biblioteca:

    cd orangepi_PC_gpio_pyH3
    python setup.py install

Y ya podemos probar los ficheros de ejemplo:

    examples/blink_led.py
    examples/blink_POWER_STATUS_PL10.py

El resto de ejemplos no van a funcionar, están escritos para la
*A20-OLinuXino-MICRO*

Acceso desde C
--------------

### pyA20

La biblioteca de Python *orangepi\_PC\_gpio\_pyH3*, en realidad se basa
en bibliotecas escritas en C que tenemos disponibles dentro del repo en
el directorio *pyA20*

    cd pyA20
    ls
    gpio/  i2c/  __init__.py  spi/  utilities/

Nos interesa probar las bibliotecas en los directorios *gpio* e *i2c*,
al menos de momento. Serían *gpio\_lib* e *i2c\_lib* respectivamente.

Probamos el acceso al *gpio* desde C con un programa sencillo que nos
haga encender y apagar el led de la OPI.

``` {c}
#include <gpio_lib.h>
sunxi_gpio_init();
sunxi_gpio_set_cfgpin(SUNXI_GPA(17), SUNXI_GPIO_OUTPUT);
while(1) {
    sunxi_gpio_output(SUNXI_GPA(17), 1);
    sleep(1);
    sunxi_gpio_output(SUNXI_GPA(17), 0);
    sleep(1);
}
```

### WiringOP-Zero

Esta biblioteca imita a la *WiringPI* que se usa con *Raspberry Pi*.

Tenemos un fork que viene preparado para la Orange Pi Zero disponible
aquí:

    https://github.com/xpertsavenue/WiringOP-Zero

GPIO funciona completamente y al parecer aun no han testeado el i2c
(tiene mala pinta)

Para compilarla seguimos las instrucciones:

``` {bash}
git clone https://github.com/xpertsavenue/WiringOP-Zero
cd WiringOP-Zero
chmod +x ./build
sudo ./build
```

Podemos comprobar que todo se ha instalado correctamente:

    gpio readall
     +-----+-----+----------+------+--Orange Pi Zero--+---+------+---------+-----+--+
     | H2+ | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | H2+ |
     +-----+-----+----------+------+---+----++----+---+------+----------+-----+-----+
     |     |     |     3.3v |      |   |  1 || 2  |   |      | 5v       |     |     |
     |  12 |   8 |    SDA.0 | ALT5 | 0 |  3 || 4  |   |      | 5V       |     |     |
     |  11 |   9 |    SCL.0 | ALT5 | 0 |  5 || 6  |   |      | 0v       |     |     |
     |   6 |   7 |   GPIO.7 | ALT3 | 0 |  7 || 8  | 1 | OUT  | TxD3     | 15  | 198 |
     |     |     |       0v |      |   |  9 || 10 | 0 | ALT5 | RxD3     | 16  | 199 |
     |   1 |   0 |     RxD2 | ALT5 | 0 | 11 || 12 | 0 | ALT3 | GPIO.1   | 1   | 7   |
     |   0 |   2 |     TxD2 | ALT5 | 0 | 13 || 14 |   |      | 0v       |     |     |
     |   3 |   3 |     CTS2 | ALT3 | 0 | 15 || 16 | 0 | ALT4 | GPIO.4   | 4   | 19  |
     |     |     |     3.3v |      |   | 17 || 18 | 0 | ALT4 | GPIO.5   | 5   | 18  |
     |  15 |  12 |     MOSI | ALT5 | 0 | 19 || 20 |   |      | 0v       |     |     |
     |  16 |  13 |     MISO | ALT5 | 0 | 21 || 22 | 0 | ALT3 | RTS2     | 6   | 2   |
     |  14 |  14 |     SCLK | ALT5 | 0 | 23 || 24 | 0 | ALT5 | CE0      | 10  | 13  |
     |     |     |       0v |      |   | 25 || 26 | 0 | ALT3 | GPIO.11  | 11  | 10  |
     +-----+-----+----------+------+---+---LEDs---+---+------+----------+-----+-----+
     |  17 |  30 | STAT-LED |  OUT | 1 | 27 || 28 |   |      | PWR-LED  |     |     |
     +-----+-----+----------+------+---+-----+----+---+------+----------+-----+-----+
     | H2+ | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | H2+ |
     +-----+-----+----------+------+--Orange Pi Zero--+---+------+---------+-----+--+

Instalamos las i2c-tools `aptitude install i2c-tools`, después de eso ya
funciona el comando `gpio i2cd`.

### WiringPI-Python-OP

Una receta para compilar WiringPI-Python-OP

Edited by nopnop2002 at 2017-3-18 22:51

diyer replied at 2017-3-6 06:03 can someone explain how to map wiringPO
on zero plaese?

You can update WiringPi-Python-OP to WiringPi-Python-OP-ZERO.

1.Download \[WiringOP libary for the Orange Pi Zero\] from here.

https://github.com/xpertsavenue/WiringOP-Zero

2.Download [WiringPi-Python-OP](#wiringpi-python-op) from here.(But not
Install)

https://github.com/lanefu/WiringPi-Python-OP

3.Replace base library

cd WiringPi-Python-OP rm -R WiringPi cp -R $HOME/WiringOP-Zero ./ mv
WiringOP-Zero WiringPi

4.Build WiringPi-Python-OP-ZERO library

sudo apt-get install python-dev python-setuptools swig swig2.0 -python
wiringpi.i sudo python setup.py install cd tests sudo python test.py

WiringPi-Python-OP-ZERO have there pin. PysPin PinInLib 1(3.3V) 2(5V) 3
8 4(5V) 5 9 6(GND) 7 7 8 15 9(GND) 10 16 11 0 12 1 13 2 14(GND) 15 3 16
4 17(3.3V) 18 5 19 12 20(GND) 21 13 22 6 23 14 24 10 25(GND) 26 11

This is same as RPI TYPE A or B

Referencias
-----------

-   [Probando la Orange Pi
    Zero](http://harald.studiokubota.com/wordpress/index.php/2016/11/19/orange-pi-zero-neat/)
-   [GPIO from
    commandline](http://falsinsoft.blogspot.com.es/2012/11/access-gpio-from-linux-user-space.html)
-   [mas de lo
    mismo](http://www.emcraft.com/stm32f429discovery/controlling-gpio-from-linux-user-space)
-   [hilo GPIO en
    foro](https://forum.armbian.com/index.php/topic/3084-orange-pi-zero-python-gpio-library/)
-   [WiringPi\_OrangePi explicación
    lanefu](https://gist.github.com/lanefu/f16a67195c9fa35c466c6b50cdaeadea)
-   [El repo donde pretende unificar
    WiringPI](https://github.com/lanefu/WiringOtherPi)
-   [Otro Repo mas](https://github.com/lanefu/WiringPi-Python-OP)
-   [Otro mas, este tio no
    para](https://gist.github.com/lanefu/f16a67195c9fa35c466c6b50cdaeadea)
-   [Hilo en el foro hablando de unificiación de
    bibliotecas](https://forum.armbian.com/index.php/topic/2956-559-gpio-support-for-h2h3-boards-with-a-unified-wiringpi-library-in-a-neat-little-package/#entry20311)
-   [Otro
    hilo](https://forum.armbian.com/index.php/topic/3084-orange-pi-zero-python-gpio-library/?hl=%2Bzero+%2Bgpio+%2Blibrary)
-   [Otro hilo más con I2C
    SPI](https://forum.armbian.com/index.php/topic/3084-orange-pi-zero-python-gpio-library/)

Distribuciones disponibles para Orange Pi Zero
==============================================

Armbian oficial
---------------

En la página oficial de
[Armbian](https://www.armbian.com/orange-pi-zero/) tenemos dos opciones:

### Ubuntu Server (legacy kernel)

Es la versión estable

### Ubuntu Server (mainline kernel)

Es la versión de desarrollo.

Orange Pi oficial
-----------------

Tiene [varias
distribuciones](http://www.orangepi.org/downloadresources/), parece que
no esta puesta al dia o bien las fechas no son consistentes.

Diet Pi
-------

En [esta página](http://dietpi.com/) parece que hay una versión
ultraligera.

Monitorizar temperatura
=======================

Podemos ver estadísticas de nuestra OPI con:

    sudo armbianmonitor -m

    Stop monitoring using [ctrl]-[c]
    Time        CPU    load %cpu %sys %usr %nice %io %irq   CPU
    21:04:18: 1152MHz  0.08   0%   0%   0%   0%   0%   0%   40°C
    21:04:23:  240MHz  0.07   0%   0%   0%   0%   0%   0%   39°C
    21:04:28:  240MHz  0.15   6%   1%   0%   0%   4%   0%   40°C
    21:04:33:  240MHz  0.12   6%   1%   0%   0%   4%   0%   38°C
    21:04:38:  240MHz  0.11   2%   1%   0%   0%   0%   0%   39°C
    21:04:43:  240MHz  0.11   1%   0%   0%   0%   0%   0%   39°C
    21:04:49:  240MHz  0.10   1%   0%   0%   0%   0%   0%   38°C
    21:04:54:  240MHz  0.09   1%   0%   0%   0%   0%   0%   39°C
    21:04:59:  240MHz  0.08   2%   1%   0%   0%   0%   0%   38°C
    21:05:04:  240MHz  0.08   2%   1%   0%   0%   0%   0%   38°C
    21:05:10:  240MHz  0.07   2%   1%   0%   0%   0%   0%   40°C
    21:05:15:  240MHz  0.06   2%   1%   0%   0%   0%   0%   37°C
    21:05:20:  240MHz  0.06   2%   1%   0%   0%   0%   0%   39°C
    21:05:25:  240MHz  0.05   2%   1%   0%   0%   0%   0%   37°C
    21:05:30:  240MHz  0.05   2%   1%   0%   0%   0%   0%   39°C

También podemos instalar RPi-Monitor con el comando:

    sudo armbianmonitor -r

Una vez instalado podemos visitar desde nuestro navegador la dirección
ip de nuestra OPI Zero *http://opi-adress:8888* para ver las
estadísticas.

![Estadísticas en RPi-Monitor](doc/src/img/rpimonitor.png)

¡Ojo! Las gráficas no se refrescan automáticamente hay que recargar la
página.

Para desinstalar el RPi-Monitor basta con:

    sudo aptitude uninstall rpimonitor

Referencias
===========

-   [Página oficial](http://www.orangepi.org/)
-   [Recursos oficiales](http://www.orangepi.org/downloadresources/)
    aquí hay imágenes y los esquemáticos
-   [Tienda en
    Aliexpress](https://www.aliexpress.com/store/1553371?spm=2114.8147860.0.0.F1q43C)
-   <http://linux-sunxi.org/Bootable_SD_card>
-   <https://www.armbian.com/orange-pi-zero/>
-   <https://docs.armbian.com/User-Guide_Getting-Started/>
-   <https://docs.armbian.com/Hardware_Allwinner/>
-   [GPIO](https://linux-sunxi.org/GPIO) Una explicación de como acceder
    al gpio desde terminal
-   [Info variada](https://linux-sunxi.org/Orange_Pi_Zero) Aquí tenemos
    el esquema de pines
-   [GPIO desde el espacio de
    usuario](https://forum.armbian.com/index.php/topic/1886-gpio-access-from-user-space/)
-   [sunxi-gpio](https://forum.armbian.com/index.php/topic/1471-solved-difficulty-accessing-gpio-via-the-sunxi-gpio-export-interface/)
-   [orange pi español](http://orangepiweb.es/index.php)
-   [ArchLinux ARM on Orange
    Pi](https://www.amedeobaragiola.me/blog/2016/06/04/archlinux-arm-on-orange-pi-one/)
-   [Lakka Nightly Builds](http://mirror.lakka.tv/nightly/) Lakka is the
    official Linux distribution of RetroArch and the libretro ecosystem.
    Each game system is implemented as a libretro core, while the
    frontend RetroArch takes care of inputs and display. This clear
    separation ensures modularity and centralized configuration. Also
    nightly build for H3 is supported

META
====

Este documento está escrito en
[Markdown-Pandoc](http://pandoc.org/README.html). Pandoc es un sistema
muy sencillo de documentación que permite generar múltiples formatos de
salida.

Las fuentes del documento están en el directorio **doc/src** dentro del
árbol de directorios del proyecto.

Los formatos de salida son el fichero **README.md** en formato
*Markdown-Github* y los documentos que puedes encontrar en el directorio
**doc/out** incluyendo un fichero en formato *pdf*.

Los documentos en los distintos formatos de salida se generan
automáticamente sin mas que ejecutar:

``` {bash}
$ cd doc
$ make
```

Otras opciones que soporta el **makefile** serían:

reset  
para regenerar todos los documentos de salida.

clean  
para borrar todos los fichero de salida

pdf, latex, mediawiki, epub, odt, docx  
genera el fichero de salida en el formato especificado: pdf, latex, etc.
etc.

Ejemplos:

``` {bash}
$ make reset
$ make odt
```

Requisitos
----------

Necesitas tener instalaco **Pandoc**, hay [una pequeña
introducción](https://github.com/brico-labs/pandoc_basico) en el el
github de BricoLabs.

Licencia
========


    Attribution-ShareAlike 4.0 International

    =======================================================================

    Creative Commons Corporation ("Creative Commons") is not a law firm and
    does not provide legal services or legal advice. Distribution of
    Creative Commons public licenses does not create a lawyer-client or
    other relationship. Creative Commons makes its licenses and related
    information available on an "as-is" basis. Creative Commons gives no
    warranties regarding its licenses, any material licensed under their
    terms and conditions, or any related information. Creative Commons
    disclaims all liability for damages resulting from their use to the
    fullest extent possible.

    Using Creative Commons Public Licenses

    Creative Commons public licenses provide a standard set of terms and
    conditions that creators and other rights holders may use to share
    original works of authorship and other material subject to copyright
    and certain other rights specified in the public license below. The
    following considerations are for informational purposes only, are not
    exhaustive, and do not form part of our licenses.

         Considerations for licensors: Our public licenses are
         intended for use by those authorized to give the public
         permission to use material in ways otherwise restricted by
         copyright and certain other rights. Our licenses are
         irrevocable. Licensors should read and understand the terms
         and conditions of the license they choose before applying it.
         Licensors should also secure all rights necessary before
         applying our licenses so that the public can reuse the
         material as expected. Licensors should clearly mark any
         material not subject to the license. This includes other CC-
         licensed material, or material used under an exception or
         limitation to copyright. More considerations for licensors:
        wiki.creativecommons.org/Considerations_for_licensors

         Considerations for the public: By using one of our public
         licenses, a licensor grants the public permission to use the
         licensed material under specified terms and conditions. If
         the licensor's permission is not necessary for any reason--for
         example, because of any applicable exception or limitation to
         copyright--then that use is not regulated by the license. Our
         licenses grant only permissions under copyright and certain
         other rights that a licensor has authority to grant. Use of
         the licensed material may still be restricted for other
         reasons, including because others have copyright or other
         rights in the material. A licensor may make special requests,
         such as asking that all changes be marked or described.
         Although not required by our licenses, you are encouraged to
         respect those requests where reasonable. More_considerations
         for the public: 
        wiki.creativecommons.org/Considerations_for_licensees

    =======================================================================

    Creative Commons Attribution-ShareAlike 4.0 International Public
    License

    By exercising the Licensed Rights (defined below), You accept and agree
    to be bound by the terms and conditions of this Creative Commons
    Attribution-ShareAlike 4.0 International Public License ("Public
    License"). To the extent this Public License may be interpreted as a
    contract, You are granted the Licensed Rights in consideration of Your
    acceptance of these terms and conditions, and the Licensor grants You
    such rights in consideration of benefits the Licensor receives from
    making the Licensed Material available under these terms and
    conditions.


    Section 1 -- Definitions.

      a. Adapted Material means material subject to Copyright and Similar
         Rights that is derived from or based upon the Licensed Material
         and in which the Licensed Material is translated, altered,
         arranged, transformed, or otherwise modified in a manner requiring
         permission under the Copyright and Similar Rights held by the
         Licensor. For purposes of this Public License, where the Licensed
         Material is a musical work, performance, or sound recording,
         Adapted Material is always produced where the Licensed Material is
         synched in timed relation with a moving image.

      b. Adapter's License means the license You apply to Your Copyright
         and Similar Rights in Your contributions to Adapted Material in
         accordance with the terms and conditions of this Public License.

      c. BY-SA Compatible License means a license listed at
         creativecommons.org/compatiblelicenses, approved by Creative
         Commons as essentially the equivalent of this Public License.

      d. Copyright and Similar Rights means copyright and/or similar rights
         closely related to copyright including, without limitation,
         performance, broadcast, sound recording, and Sui Generis Database
         Rights, without regard to how the rights are labeled or
         categorized. For purposes of this Public License, the rights
         specified in Section 2(b)(1)-(2) are not Copyright and Similar
         Rights.

      e. Effective Technological Measures means those measures that, in the
         absence of proper authority, may not be circumvented under laws
         fulfilling obligations under Article 11 of the WIPO Copyright
         Treaty adopted on December 20, 1996, and/or similar international
         agreements.

      f. Exceptions and Limitations means fair use, fair dealing, and/or
         any other exception or limitation to Copyright and Similar Rights
         that applies to Your use of the Licensed Material.

      g. License Elements means the license attributes listed in the name
         of a Creative Commons Public License. The License Elements of this
         Public License are Attribution and ShareAlike.

      h. Licensed Material means the artistic or literary work, database,
         or other material to which the Licensor applied this Public
         License.

      i. Licensed Rights means the rights granted to You subject to the
         terms and conditions of this Public License, which are limited to
         all Copyright and Similar Rights that apply to Your use of the
         Licensed Material and that the Licensor has authority to license.

      j. Licensor means the individual(s) or entity(ies) granting rights
         under this Public License.

      k. Share means to provide material to the public by any means or
         process that requires permission under the Licensed Rights, such
         as reproduction, public display, public performance, distribution,
         dissemination, communication, or importation, and to make material
         available to the public including in ways that members of the
         public may access the material from a place and at a time
         individually chosen by them.

      l. Sui Generis Database Rights means rights other than copyright
         resulting from Directive 96/9/EC of the European Parliament and of
         the Council of 11 March 1996 on the legal protection of databases,
         as amended and/or succeeded, as well as other essentially
         equivalent rights anywhere in the world.

      m. You means the individual or entity exercising the Licensed Rights
         under this Public License. Your has a corresponding meaning.


    Section 2 -- Scope.

      a. License grant.

           1. Subject to the terms and conditions of this Public License,
              the Licensor hereby grants You a worldwide, royalty-free,
              non-sublicensable, non-exclusive, irrevocable license to
              exercise the Licensed Rights in the Licensed Material to:

                a. reproduce and Share the Licensed Material, in whole or
                   in part; and

                b. produce, reproduce, and Share Adapted Material.

           2. Exceptions and Limitations. For the avoidance of doubt, where
              Exceptions and Limitations apply to Your use, this Public
              License does not apply, and You do not need to comply with
              its terms and conditions.

           3. Term. The term of this Public License is specified in Section
              6(a).

           4. Media and formats; technical modifications allowed. The
              Licensor authorizes You to exercise the Licensed Rights in
              all media and formats whether now known or hereafter created,
              and to make technical modifications necessary to do so. The
              Licensor waives and/or agrees not to assert any right or
              authority to forbid You from making technical modifications
              necessary to exercise the Licensed Rights, including
              technical modifications necessary to circumvent Effective
              Technological Measures. For purposes of this Public License,
              simply making modifications authorized by this Section 2(a)
              (4) never produces Adapted Material.

           5. Downstream recipients.

                a. Offer from the Licensor -- Licensed Material. Every
                   recipient of the Licensed Material automatically
                   receives an offer from the Licensor to exercise the
                   Licensed Rights under the terms and conditions of this
                   Public License.

                b. Additional offer from the Licensor -- Adapted Material.
                   Every recipient of Adapted Material from You
                   automatically receives an offer from the Licensor to
                   exercise the Licensed Rights in the Adapted Material
                   under the conditions of the Adapter's License You apply.

                c. No downstream restrictions. You may not offer or impose
                   any additional or different terms or conditions on, or
                   apply any Effective Technological Measures to, the
                   Licensed Material if doing so restricts exercise of the
                   Licensed Rights by any recipient of the Licensed
                   Material.

           6. No endorsement. Nothing in this Public License constitutes or
              may be construed as permission to assert or imply that You
              are, or that Your use of the Licensed Material is, connected
              with, or sponsored, endorsed, or granted official status by,
              the Licensor or others designated to receive attribution as
              provided in Section 3(a)(1)(A)(i).

      b. Other rights.

           1. Moral rights, such as the right of integrity, are not
              licensed under this Public License, nor are publicity,
              privacy, and/or other similar personality rights; however, to
              the extent possible, the Licensor waives and/or agrees not to
              assert any such rights held by the Licensor to the limited
              extent necessary to allow You to exercise the Licensed
              Rights, but not otherwise.

           2. Patent and trademark rights are not licensed under this
              Public License.

           3. To the extent possible, the Licensor waives any right to
              collect royalties from You for the exercise of the Licensed
              Rights, whether directly or through a collecting society
              under any voluntary or waivable statutory or compulsory
              licensing scheme. In all other cases the Licensor expressly
              reserves any right to collect such royalties.


    Section 3 -- License Conditions.

    Your exercise of the Licensed Rights is expressly made subject to the
    following conditions.

      a. Attribution.

           1. If You Share the Licensed Material (including in modified
              form), You must:

                a. retain the following if it is supplied by the Licensor
                   with the Licensed Material:

                     i. identification of the creator(s) of the Licensed
                        Material and any others designated to receive
                        attribution, in any reasonable manner requested by
                        the Licensor (including by pseudonym if
                        designated);

                    ii. a copyright notice;

                   iii. a notice that refers to this Public License;

                    iv. a notice that refers to the disclaimer of
                        warranties;

                     v. a URI or hyperlink to the Licensed Material to the
                        extent reasonably practicable;

                b. indicate if You modified the Licensed Material and
                   retain an indication of any previous modifications; and

                c. indicate the Licensed Material is licensed under this
                   Public License, and include the text of, or the URI or
                   hyperlink to, this Public License.

           2. You may satisfy the conditions in Section 3(a)(1) in any
              reasonable manner based on the medium, means, and context in
              which You Share the Licensed Material. For example, it may be
              reasonable to satisfy the conditions by providing a URI or
              hyperlink to a resource that includes the required
              information.

           3. If requested by the Licensor, You must remove any of the
              information required by Section 3(a)(1)(A) to the extent
              reasonably practicable.

      b. ShareAlike.

         In addition to the conditions in Section 3(a), if You Share
         Adapted Material You produce, the following conditions also apply.

           1. The Adapter's License You apply must be a Creative Commons
              license with the same License Elements, this version or
              later, or a BY-SA Compatible License.

           2. You must include the text of, or the URI or hyperlink to, the
              Adapter's License You apply. You may satisfy this condition
              in any reasonable manner based on the medium, means, and
              context in which You Share Adapted Material.

           3. You may not offer or impose any additional or different terms
              or conditions on, or apply any Effective Technological
              Measures to, Adapted Material that restrict exercise of the
              rights granted under the Adapter's License You apply.


    Section 4 -- Sui Generis Database Rights.

    Where the Licensed Rights include Sui Generis Database Rights that
    apply to Your use of the Licensed Material:

      a. for the avoidance of doubt, Section 2(a)(1) grants You the right
         to extract, reuse, reproduce, and Share all or a substantial
         portion of the contents of the database;

      b. if You include all or a substantial portion of the database
         contents in a database in which You have Sui Generis Database
         Rights, then the database in which You have Sui Generis Database
         Rights (but not its individual contents) is Adapted Material,

         including for purposes of Section 3(b); and
      c. You must comply with the conditions in Section 3(a) if You Share
         all or a substantial portion of the contents of the database.

    For the avoidance of doubt, this Section 4 supplements and does not
    replace Your obligations under this Public License where the Licensed
    Rights include other Copyright and Similar Rights.


    Section 5 -- Disclaimer of Warranties and Limitation of Liability.

      a. UNLESS OTHERWISE SEPARATELY UNDERTAKEN BY THE LICENSOR, TO THE
         EXTENT POSSIBLE, THE LICENSOR OFFERS THE LICENSED MATERIAL AS-IS
         AND AS-AVAILABLE, AND MAKES NO REPRESENTATIONS OR WARRANTIES OF
         ANY KIND CONCERNING THE LICENSED MATERIAL, WHETHER EXPRESS,
         IMPLIED, STATUTORY, OR OTHER. THIS INCLUDES, WITHOUT LIMITATION,
         WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR
         PURPOSE, NON-INFRINGEMENT, ABSENCE OF LATENT OR OTHER DEFECTS,
         ACCURACY, OR THE PRESENCE OR ABSENCE OF ERRORS, WHETHER OR NOT
         KNOWN OR DISCOVERABLE. WHERE DISCLAIMERS OF WARRANTIES ARE NOT
         ALLOWED IN FULL OR IN PART, THIS DISCLAIMER MAY NOT APPLY TO YOU.

      b. TO THE EXTENT POSSIBLE, IN NO EVENT WILL THE LICENSOR BE LIABLE
         TO YOU ON ANY LEGAL THEORY (INCLUDING, WITHOUT LIMITATION,
         NEGLIGENCE) OR OTHERWISE FOR ANY DIRECT, SPECIAL, INDIRECT,
         INCIDENTAL, CONSEQUENTIAL, PUNITIVE, EXEMPLARY, OR OTHER LOSSES,
         COSTS, EXPENSES, OR DAMAGES ARISING OUT OF THIS PUBLIC LICENSE OR
         USE OF THE LICENSED MATERIAL, EVEN IF THE LICENSOR HAS BEEN
         ADVISED OF THE POSSIBILITY OF SUCH LOSSES, COSTS, EXPENSES, OR
         DAMAGES. WHERE A LIMITATION OF LIABILITY IS NOT ALLOWED IN FULL OR
         IN PART, THIS LIMITATION MAY NOT APPLY TO YOU.

      c. The disclaimer of warranties and limitation of liability provided
         above shall be interpreted in a manner that, to the extent
         possible, most closely approximates an absolute disclaimer and
         waiver of all liability.


    Section 6 -- Term and Termination.

      a. This Public License applies for the term of the Copyright and
         Similar Rights licensed here. However, if You fail to comply with
         this Public License, then Your rights under this Public License
         terminate automatically.

      b. Where Your right to use the Licensed Material has terminated under
         Section 6(a), it reinstates:

           1. automatically as of the date the violation is cured, provided
              it is cured within 30 days of Your discovery of the
              violation; or

           2. upon express reinstatement by the Licensor.

         For the avoidance of doubt, this Section 6(b) does not affect any
         right the Licensor may have to seek remedies for Your violations
         of this Public License.

      c. For the avoidance of doubt, the Licensor may also offer the
         Licensed Material under separate terms or conditions or stop
         distributing the Licensed Material at any time; however, doing so
         will not terminate this Public License.

      d. Sections 1, 5, 6, 7, and 8 survive termination of this Public
         License.


    Section 7 -- Other Terms and Conditions.

      a. The Licensor shall not be bound by any additional or different
         terms or conditions communicated by You unless expressly agreed.

      b. Any arrangements, understandings, or agreements regarding the
         Licensed Material not stated herein are separate from and
         independent of the terms and conditions of this Public License.


    Section 8 -- Interpretation.

      a. For the avoidance of doubt, this Public License does not, and
         shall not be interpreted to, reduce, limit, restrict, or impose
         conditions on any use of the Licensed Material that could lawfully
         be made without permission under this Public License.

      b. To the extent possible, if any provision of this Public License is
         deemed unenforceable, it shall be automatically reformed to the
         minimum extent necessary to make it enforceable. If the provision
         cannot be reformed, it shall be severed from this Public License
         without affecting the enforceability of the remaining terms and
         conditions.

      c. No term or condition of this Public License will be waived and no
         failure to comply consented to unless expressly agreed to by the
         Licensor.

      d. Nothing in this Public License constitutes or may be interpreted
         as a limitation upon, or waiver of, any privileges and immunities
         that apply to the Licensor or You, including from the legal
         processes of any jurisdiction or authority.


    =======================================================================

    Creative Commons is not a party to its public
    licenses. Notwithstanding, Creative Commons may elect to apply one of
    its public licenses to material it publishes and in those instances
    will be considered the “Licensor.” The text of the Creative Commons
    public licenses is dedicated to the public domain under the CC0 Public
    Domain Dedication. Except for the limited purpose of indicating that
    material is shared under a Creative Commons public license or as
    otherwise permitted by the Creative Commons policies published at
    creativecommons.org/policies, Creative Commons does not authorize the
    use of the trademark "Creative Commons" or any other trademark or logo
    of Creative Commons without its prior written consent including,
    without limitation, in connection with any unauthorized modifications
    to any of its public licenses or any other arrangements,
    understandings, or agreements concerning use of licensed material. For
    the avoidance of doubt, this paragraph does not form part of the
    public licenses.

    Creative Commons may be contacted at creativecommons.org.

[1] La password por defecto de Armbian es **1234**, nos pedirá cambiarla
en el primer login.
