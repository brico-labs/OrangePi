#!/bin/bash

cd /etc/network/if-up.d/
echo 'cd to'
pwd

echo 'creating script'
echo '#!/bin/sh

# filename: tun-up

if [ "$IFACE" = wlan0 ]; then
  echo 1 > /sys/class/leds/red_led/brightness
fi' > led

echo 'changing permissions'
chmod 777 led

echo 'done'


