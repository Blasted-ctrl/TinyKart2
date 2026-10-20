#!/bin/bash
echo -e "\n--Importing udev rules--\n"
sudo cp ./lidar.rules  /etc/udev/rules.d
sudo cp ./board.rules  /etc/udev/rules.d
echo -e "\n--Restarting udev--\n"
sudo service udev reload
sudo service udev restart
sudo udevadm control --reload && sudo udevadm trigger
echo -e "\n--udev reloaded--\n"
echo -e "\n === Serial Setup Complete ===\n If you continue to run into errors with USB devices, try replugging devices, or rebooting the system."
