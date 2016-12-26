#!/bin/bash

cd $HOME

echo 'Move to'
pwd

echo 'Cloning repository'
git clone https://github.com/duxingkei33/orangepi_PC_gpio_pyH3.git

cd orangepi_PC_gpio_pyH3
echo 'Move to'
pwd

echo 'Installing pyH3 library'
sudo python setup.py install

echo 'Moving examples'
mkdir ../python_gpio_examples
cp ./examples/* ../python_gpio_examples

cd ..
echo 'Move to'
pwd

echo 'Removing installation remains'
sudo rm -rf orangepi_PC_gpio_pyH3

echo 'done'
