#DEPENDENCIES

hcidump must be installed
sudo apt-get install bluez-hcidump

ssh must be set up for the push to work in pushToGit() (main.c)
https://help.github.com/articles/connecting-to-github-with-ssh/

set the git push url to use that ssh key
git remote set-url origin git+ssh://git@github.com/username/repo.git

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

######WEB
index.html redirects to correct page

Added blank page just to get AJAX working

JS file that loads file from server added
