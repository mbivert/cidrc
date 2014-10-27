# Description
Cdrc is a little tool to convert IP from/to cidr notation. It
can also list all available IPs on a given network.

The utils.c files contains various function to convert IPs
back and and forth. Internal represatation (uchar *) in inspired
from Plan9's `/sys/src/libip/parseip.c`.

# Usage

    # cc -g -W -Wall cidrc.c utils.c -o cidrc
    # ./cidrc
    ./cidrc list <network>
    ./cidrc conv <network>

Where network is either and IP address and a netmask, or a network
in CIDR notation. Eg.

    # ./cidrc conv 192.168.0.0/24
    192.168.0.0 255.255.255.0
    # ./cidrc conv `./cidrc conv 192.168.0.0/24`
    192.168.0.0/24
    # ./cidrc list 10.0.0.0/31
    10.0.0.0
    10.0.0.1

One can run tests through

    # cc -g -W -Wall tests.c utis.c -o tests && ./tests

No output is good.

# TODO
IPv6 in libcdrc.

We assume reasonable input in cdrc. Undefined results
for non-sense inpute. Ensure no segfault and the-like on
special input however.

Calculate ip/mask from a base IP and number of host.

Clean around uchar/uint32_t.
