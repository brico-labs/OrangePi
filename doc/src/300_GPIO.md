# Accediendo al hardware desde linea de comandos

La memoria es más que suficiente para correr programas. El Armbian
consume únicamente 40Mb en funcionamiento.

~~~~
root@orangepizero:~# free
             total       used       free     shared    buffers     cached
Mem:        247068     122300     124768       4620       7908      69548
-/+ buffers/cache:      44844     202224
Swap:       131068          0     131068
~~~~

Vamos a hacer algunas pruebas con el hardware. En Armbian, como todo
Unix que se precie, todo es un fichero.

En el directorio `/sys/class` encontraremos cosas interesantes:

~~~~
root@orangepizero:~# ls /sys/class
backlight  cuse     graphics	 i2c-dev    mdio_bus  power_supply  script	 spidev		 thermal     video4linux
bdi	   devfreq  hdmi	 ieee80211  mem       ppp	    scsi_device  spi_master	 tty	     vtconsole
block	   disp     hidraw	 input	    misc      rc	    scsi_disk	 sunxi_cma_test  udc
bsg	   dma	    hwmon	 leds	    mmc_host  rfkill	    scsi_host	 sunxi_dump	 usb_device
cedar_dev  gpio     i2c-adapter  lirc	    net       rtc	    sound	 sunxi_info	 vc
~~~~

## LEDs

Si miramos dentro del directorio `leds` veremos que hay un directorio
que representa cada uno de los leds de la placa:

~~~~
root@orangepizero:~# cd /sys/class/leds/
root@orangepizero:/sys/class/leds# ls
green_led  red_led
~~~~

Podemos ver, por ejemplo, a que evento está asociado cada led ejecutando
`cat green_led/trigger` (tiene el valor `default_on`) o `cat
red_led/trigger` (tiene el valor `none`).

~~~~
root@orangepizero:/sys/class/leds# cat green_led/trigger 
none mmc0 mmc1 timer heartbeat backlight [default-on] rfkill0 phy1rx phy1tx phy1assoc phy1radio 
root@orangepizero:/sys/class/leds# cat red_led/trigger 
[none] mmc0 mmc1 timer heartbeat backlight default-on rfkill0 phy1rx phy1tx phy1assoc phy1radio 
~~~~

O podemos encender el led rojo ejecutando `echo 1 >
red_led/brightness`, y para apagarlo ya os podéis imaginar que es
`echo 0 > red_led/brightness`.

## GPIO

Podemos ver los GPIO disponibles ejecutando:

~~~~
root@orangepizero:~# cat /sys/kernel/debug/gpio
GPIOs 0-383, platform/sunxi-pinctrl, sunxi-pinctrl:
 gpio-10  (?                   ) out hi
 gpio-17  (red_led             ) out lo
 gpio-202 (xradio_irq          ) in  lo
 gpio-354 (?                   ) out hi
 gpio-362 (green_led           ) out hi
~~~~

Podemos activar un nuevo puerto GPIO, digamos el 15:

~~~~
root@orangepizero:~# echo 15 >/sys/class/gpio/export
~~~~

Ahora veremos el puerto activo:

~~~~
root@orangepizero:~# cat /sys/kernel/debug/gpio 
GPIOs 0-383, platform/sunxi-pinctrl, sunxi-pinctrl:
 gpio-10  (?                   ) out hi
 gpio-15  (sysfs               ) in  lo
 gpio-17  (red_led             ) out lo
 gpio-202 (xradio_irq          ) in  lo
 gpio-354 (?                   ) out hi
 gpio-362 (green_led           ) out hi
~~~~

En el directorio `/sys/class/gpio/gpio15/` tendremos los interfaces
usuales para puertos gpio definidos en el kernel de linux.

# Bibliotecas útiles

## Python

### Instalación de virtualenv 

Para probar bibliotecas de Python instalamos:

~~~~{bash}
sudo aptitude install python-pip
sudo aptitude install python-virtualenv
sudo aptitude install python-dev
~~~~

El caso es que el virtualenv no me funciona después de ejecutar estos
pasos. Finalmente he tenido que ejecutar, como _root_, los siguientes
comandos:

~~~~
pip install --upgrade pip
pip install --upgrade virtualenv
~~~~

Despues de eso ya he podido usar _virtualenv_ sin problemas.

### orangepi_PC_gpio_pyH3

Nos clonamos el repo. Voy a hacer todas las pruebas desde la cuenta de
__root__.

~~~~
git clone https://github.com/duxingkei33/orangepi_PC_gpio_pyH3
~~~~

Hay que cambiar el fichero `orangepi_PC_gpio_pyH3/pyA20/gpio/mapping.h`

La definicion de *STATUS_LED* debe quedar en el _GPA17_ en lugar de _GPA15_:

~~~~
{   "STATUS_LED",  SUNXI_GPA(17),  2   },
~~~~

Creamos un entorno para pruebas y lo activamos:

~~~~
virtualenv test_A
source test_A/bin/activate
~~~~

Compilamos la biblioteca:

~~~~
cd orangepi_PC_gpio_pyH3
python setup.py install
~~~~

Y ya podemos probar los ficheros de ejemplo.

### Acceso desde C

La biblioteca de Python *orangepi_PC_gpio_pyH3*, en realidad se basa
en bibliotecas escritas en C que tenemos disponibles dentro del repo
en el directorio _pyA20_

~~~~
cd pyA20
ls
gpio/  i2c/  __init__.py  spi/  utilities/
~~~~

Nos interesa probar las bibliotecas en los directorios _gpio_ e _i2c_,
al menos de momento. Serían *gpio_lib* e *i2c_lib* respectivamente.

Probamos el acceso al _gpio_ desde C con un programa sencillo que nos
haga encender y apagar el led de la OPI.

~~~~{c}
sunxi_gpio_init();
sunxi_gpio_set_cfgpin(SUNXI_GPA(17), SUNXI_GPIO_OUTPUT);
while(1) {
    sunxi_gpio_output(SUNXI_GPA(17), 1);
    sleep(1);
    sunxi_gpio_output(SUNXI_GPA(17), 0);
    sleep(1);
}
~~~~


## Referencias

* [Probando la Orange Pi Zero](http://harald.studiokubota.com/wordpress/index.php/2016/11/19/orange-pi-zero-neat/)
* [GPIO from commandline](http://falsinsoft.blogspot.com.es/2012/11/access-gpio-from-linux-user-space.html)
* [mas de lo mismo](http://www.emcraft.com/stm32f429discovery/controlling-gpio-from-linux-user-space)
* [hilo GPIO en foro](https://forum.armbian.com/index.php/topic/3084-orange-pi-zero-python-gpio-library/)
* [WiringPi_OrangePi explicación lanefu](https://gist.github.com/lanefu/f16a67195c9fa35c466c6b50cdaeadea)
* [El repo donde pretende unificar WiringPI](https://github.com/lanefu/WiringOtherPi)
* [Otro Repo mas](https://github.com/lanefu/WiringPi-Python-OP)
* [Otro mas, este tio no para](https://gist.github.com/lanefu/f16a67195c9fa35c466c6b50cdaeadea)
* [Hilo en el foro hablando de unificiación de bibliotecas](https://forum.armbian.com/index.php/topic/2956-559-gpio-support-for-h2h3-boards-with-a-unified-wiringpi-library-in-a-neat-little-package/#entry20311)
* [Otro hilo](https://forum.armbian.com/index.php/topic/3084-orange-pi-zero-python-gpio-library/?hl=%2Bzero+%2Bgpio+%2Blibrary)
* [Otro hilo más con I2C SPI](https://forum.armbian.com/index.php/topic/3084-orange-pi-zero-python-gpio-library/)
