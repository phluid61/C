# hostlookup

Dump some low-level information about DNS names and/or IP addresses.

### Example

```
$ ./hostlookup library.qut.edu.au 127.0.0.1 2404:6800:4006:0800::2003
***
*** library.qut.edu.au
***

gethostbyname()
  hostname: library.qut.edu.au
getaddrinfo()
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 2 [AF_INET]
  ai_socktype = 1 [SOCK_STREAM]
  ai_protocol = 6 [SOL_TCP]
  ai_canonname = "library.qut.edu.au"
  ai_addr = {
    sin_family = 2 [AF_INET]
    sin_port = 0
    sin_addr = {
      s_addr = 0x11C4B583 (131.181.196.17)
    }
  }
  getnameinfo(ai_addr)
    hostname: www.library.qut.edu.au
  gethostbyaddr(ai_addr->sin_addr)
    hostname: www.library.qut.edu.au
>
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 2 [AF_INET]
  ai_socktype = 2 [SOCK_DGRAM]
  ai_protocol = 17 [SOL_UDP]
  ai_canonname = NULL
  ai_addr = {
    sin_family = 2 [AF_INET]
    sin_port = 0
    sin_addr = {
      s_addr = 0x11C4B583 (131.181.196.17)
    }
  }
  getnameinfo(ai_addr)
    hostname: www.library.qut.edu.au
  gethostbyaddr(ai_addr->sin_addr)
    hostname: www.library.qut.edu.au
>
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 2 [AF_INET]
  ai_socktype = 3 [SOCK_RAW]
  ai_protocol = 0 [SOL_IP]
  ai_canonname = NULL
  ai_addr = {
    sin_family = 2 [AF_INET]
    sin_port = 0
    sin_addr = {
      s_addr = 0x11C4B583 (131.181.196.17)
    }
  }
  getnameinfo(ai_addr)
    hostname: www.library.qut.edu.au
  gethostbyaddr(ai_addr->sin_addr)
    hostname: www.library.qut.edu.au
.

***
*** 127.0.0.1
***

gethostbyname()
  hostname: 127.0.0.1
getaddrinfo()
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 2 [AF_INET]
  ai_socktype = 1 [SOCK_STREAM]
  ai_protocol = 6 [SOL_TCP]
  ai_canonname = "127.0.0.1"
  ai_addr = {
    sin_family = 2 [AF_INET]
    sin_port = 0
    sin_addr = {
      s_addr = 0x0100007F (127.0.0.1)
    }
  }
  getnameinfo(ai_addr)
    hostname: localhost
  gethostbyaddr(ai_addr->sin_addr)
    hostname: localhost
>
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 2 [AF_INET]
  ai_socktype = 2 [SOCK_DGRAM]
  ai_protocol = 17 [SOL_UDP]
  ai_canonname = NULL
  ai_addr = {
    sin_family = 2 [AF_INET]
    sin_port = 0
    sin_addr = {
      s_addr = 0x0100007F (127.0.0.1)
    }
  }
  getnameinfo(ai_addr)
    hostname: localhost
  gethostbyaddr(ai_addr->sin_addr)
    hostname: localhost
>
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 2 [AF_INET]
  ai_socktype = 3 [SOCK_RAW]
  ai_protocol = 0 [SOL_IP]
  ai_canonname = NULL
  ai_addr = {
    sin_family = 2 [AF_INET]
    sin_port = 0
    sin_addr = {
      s_addr = 0x0100007F (127.0.0.1)
    }
  }
  getnameinfo(ai_addr)
    hostname: localhost
  gethostbyaddr(ai_addr->sin_addr)
    hostname: localhost
.

***
*** 2404:6800:4006:0800::2003
***

error in gethostbyname: HOST_NOT_FOUND
getaddrinfo()
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 10 [AF_INET6]
  ai_socktype = 1 [SOCK_STREAM]
  ai_protocol = 6 [SOL_TCP]
  ai_canonname = "2404:6800:4006:0800::2003"
  ai_addr = {
    sin6_family = 10 [AF_INET6]
    sin6_port = 0
    sin6_flowinfo = 0
    sin6_addr = {
      s6_addr = [2404:6800:4006:0800::2003]
    }
    sin6_scope_id = 0
  }
  getnameinfo(ai_addr)
    hostname: syd09s01-in-x03.1e100.net
>
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 10 [AF_INET6]
  ai_socktype = 2 [SOCK_DGRAM]
  ai_protocol = 17 [SOL_UDP]
  ai_canonname = NULL
  ai_addr = {
    sin6_family = 10 [AF_INET6]
    sin6_port = 0
    sin6_flowinfo = 0
    sin6_addr = {
      s6_addr = [2404:6800:4006:0800::2003]
    }
    sin6_scope_id = 0
  }
  getnameinfo(ai_addr)
    hostname: syd09s01-in-x03.1e100.net
>
  ai_flags = 0xA [AI_CANONNAME|AI_V4MAPPED]
  ai_family = 10 [AF_INET6]
  ai_socktype = 3 [SOCK_RAW]
  ai_protocol = 0 [SOL_IP]
  ai_canonname = NULL
  ai_addr = {
    sin6_family = 10 [AF_INET6]
    sin6_port = 0
    sin6_flowinfo = 0
    sin6_addr = {
      s6_addr = [2404:6800:4006:0800::2003]
    }
    sin6_scope_id = 0
  }
  getnameinfo(ai_addr)
    hostname: syd09s01-in-x03.1e100.net
.

```
