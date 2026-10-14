#!/bin/bash

sudo cp ./lidar.rules  /etc/udev/rules.d
sudo cp ./board.rules  /etc/udev/rules.d
echo -e "\nRestarting udev\n"
sudo service udev reload
sudo service udev restart
sudo udevadm control --reload && sudo udevadm trigger
echo "finish"
