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


## Referencias

* [Probando la Orange Pi Zero](http://harald.studiokubota.com/wordpress/index.php/2016/11/19/orange-pi-zero-neat/)
* [GPIO from commandline](http://falsinsoft.blogspot.com.es/2012/11/access-gpio-from-linux-user-space.html)
* [mas de lo mismo](http://www.emcraft.com/stm32f429discovery/controlling-gpio-from-linux-user-space)

