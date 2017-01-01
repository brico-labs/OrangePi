# Monitorizar temperatura

Podemos ver estadísticas de nuestra OPI con:

~~~~
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
~~~~

También podemos instalar RPi-Monitor con el comando:

~~~~
sudo armbianmonitor -r
~~~~

Una vez instalado podemos visitar desde nuestro navegador la dirección
ip de nuestra OPI Zero _http://opi-adress:8888_ para ver las
estadísticas.

![Estadísticas en RPi-Monitor](src/img/rpimonitor.png)

¡Ojo! Las gráficas no se refrescan automáticamente hay que recargar la página.

Para desinstalar el RPi-Monitor basta con:

~~~~
sudo aptitude uninstall rpimonitor
~~~~


