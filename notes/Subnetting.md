The process of logically partitioning a network address space into a set of smaller networks. This is commonly utilized for either network segmentation or the efficient allocation of ip addresses within a large enough organization.

Every ip address contains two parts, those being the routing prefix and host identifier. The trailing slash is known as CIDR notation, it specifies the quantity of bits allocated for the routing prefix. Say for example, 192.168.0.0/24, the first three octets "192.168.0" equivalent to 24 bits would be the routing prefix. The last octet being reserved for the host identifier.

Networks are organized into different classes, each named from A-E, although class D and E are not typically used.

Class A Network: Everything before the first period indicates the network, and everything after it specifies the device within that network. Using 203.0.113.112 as an example, the network is indicated by "203" and the device by "0.113.112."

Class B Network: Everything before the second period indicates the network. Again using 203.0.113.112 as an example, "203.0" indicates the network and "113.112" indicates the device within that network.

Class C Network: For Class C networks, everything before the third period indicates the network. Using the same example, "203.0.113" indicates the Class C network, and "112" indicates the device.

Network and broadcast addresses are reserved and not usable, DHCP will know not to assign such addresses when dynamically configuring the routing table.

A subnet mask is simply a bitwise mask utilized to calculate the routing prefix and the host identifier. To calculate the routing prefix, simply apply the mask to the ip with a bitwise and (ip & mask). To calculate the host identifier, we do the same but with the bitwise complement of the mask (ip & ~mask).

We can partition a network into smaller subnets by allocating more bits from the host identifier to the routing prefix. 

```
network: 10.1.1.0/24

partition subnet for 40 hosts between 3 coffee shops. take 2 bits from host identifier, partitions into 4 subnets each with 64 addresses disregarding network/broadcast 62 usable per subnet.

bitmask: 11111111.11111111.11111111.11000000

ipv4: 10.1.1.0/26
mask: 255.255.255.192
ranges:
  10.1.1.0/26: 10.1.1.0   - 10.1.1.63
  10.1.1.64/26: 10.1.1.64  - 10.1.1.127
  10.1.1.128/26: 10.1.1.128 - 10.1.1.191
  10.1.1.192/26: 10.1.1.192 - 10.1.1.255
```

The private ipv4 ranges are 10.0.0.0/8 (single class A), 172.16.0.0/12 (16 contiguous class B) and 192.168.0.0/16 (256 contiguous class C).
