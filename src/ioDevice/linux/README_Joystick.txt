
Using a joystick under linux may require you to install
the kernel "joydev" driver.

To see if it is already installed issue the following 
command:

prompt> lsmod

If it is not listed issue the command:

prompt> insmod joydev

Edit and add to /etc/rc.d/rc.local file if you want
installation each time system is booted.
 
