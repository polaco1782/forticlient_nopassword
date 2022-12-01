
# Fortinet CLI injector

What is this?

It's a dynamic library which you can sideload with fortinet_cli application to overcome some limitations.
One of them is the lack to automate task with it, for example reconnect VPN when it drops,
or provide a file with password for it.




## Deployment

To deploy this project, clone and enter directory. Run make to compile the shared library

```bash
  make
```


## Usage/Examples

We're going to load the library using LD_PRELOAD to inject it into forclient process.

Replace XXX with your server address

Replace YYY with your VPN username

Edit the passwd.txt file and add your password into it. This file will be read instead of asking for it.


```shell
LD_PRELOAD=./hijack.so /opt/forticlient-sslvpn/64bit/forticlientsslvpn_cli --server XXX --vpnuser YYY
```

It won't ask for password, or any other input, it will just connect to the VPN, if the remote certificate is not encrypted with a password.

## Tech Info

The library intercepts some libc calls, changes are not permanent, and means no harm to the application

intercepts calls:

fgets()
https://pubs.opengroup.org/onlinepubs/009696699/functions/fgets.html

puts()
https://pubs.opengroup.org/onlinepubs/009696699/functions/puts.html

getpass()
https://www.gnu.org/software/libc/manual/html_node/getpass.html