#DEPENDENCIES
hcidump must be installed
sudo apt-get install bluez-hcidump

firebase must be installed for python
sudo pip install requests
sudo pip install python-firebase
######

#scan.sh
Implemented script that starts scanning for low power bluetooth devices

#dump.sh
Implemented script that shows some data about the low power bluetooth device and saves the data to a file in HOME/dump.txt

#git_push.sh
Bash script added that will eventually automatically push the data to the server for a website to grab

#main.c
Added c file that reads the data and prints each line
added function to that writes to a file
added function that pushes the changes to github
now gets the rssi value from all entries that have an rssi value
run python file to update database

######WEB
~/index.html redirects to correct page

blank page that loads the database from firebase

JS file that loads file from server firebase in real-time
