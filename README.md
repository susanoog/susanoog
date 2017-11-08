
# Susanow

[![Build Status](https://travis-ci.org/susanow/susanow.svg?branch=master)](https://travis-ci.org/susanow/susanow)

Thread based high Performance Networking Platform on DPDK.
Please see https://susanow.github.io


## Usage

```
$ export SSN_SDK=/path/to/susanow_src
$ git clone http://github.com/susanow/susanow.git $SSN_SDK
$ cd $SSN_SDK
$ ./setup.sh
```

Template Makefile

```
SSN_SDK = /path/to/susanow
include $(SSN_SDK)/mk/ssn.vars.mk

# If you want to use REST-API to control susanow,
# you must link libboost_system and libboost_thread
# for REST-API-Framework 'crow'
CXXFLAGS = -std=c++11 $(SSN_CXXFLAGS)
LDFLAGS  = $(SSN_LDFLAGS) -lboost_system -lboost_thread

all:
	g++ $(CXXFLAGS) main.cc $(LDFLAGS)
```

## Depend Software

 - dpdk-16.11 (susanow/dpdk.git)
 - L-thread slankdev (susanow/lthread.git)
 - Libslankdev (slankdev/libslankdev.git)
 -

Depend Relationship (B<-A: A depends on B)
```
pure-c++11 <- libslankdev

dpdk        <- libdpdk_cpp
libslankdev <- libdpdk_cpp
libslankdev <- libvty

libslankdev      <- libsusanow
libdpdk_cpp      <- libsusanow
liblthread_dpdk  <- libsusanow
libvty           <- libsusanow
```


## Licence and Author

MIT Licence, see LICENCE file

Hiroki SHIROKURA

 - E-mail slank.dev@gmail.com
 - Twitter @slankdev
 - Github  slankdev
 - Facebook hiroki.shirokura


