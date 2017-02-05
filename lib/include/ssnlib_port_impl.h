

/*-
 * MIT License
 *
 * Copyright (c) 2017 Susanoo G
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file ssnlib_port_impl.h
 * @brief include classes that provide port-infomation
 * @author slankdev
 */


#pragma once
#include <ssnlib_dpdk.h>



namespace ssnlib {



/*
 * This class has dynamically infomations.
 */
class port_conf {
public:
    const size_t id;
    rte_eth_conf raw;
    port_conf(size_t i) : id(i)
    {
        memset(&raw, 0x00, sizeof(raw));
        raw.rxmode.max_rx_pkt_len = ETHER_MAX_LEN;
    }
};

/*
 * This class has dynamically informations.
 */
class port_stats {
public:
    const size_t id;
    struct rte_eth_stats raw;
    port_stats(size_t i) : id(i) {}
    void reset()  { rte_eth_stats_reset(id);       }
    void update() { rte_eth_stats_get  (id, &raw); }
};

/*
 * This class has dynamically informations.
 */
class link_stats {
public:
    const size_t id;
    struct rte_eth_link raw;
    link_stats(size_t i) : id(i) {}
    void update() { rte_eth_link_get_nowait(id, &raw); }
};

/*
 * This class has statically infomations.
 */
class dev_info {
public:
    const size_t id;
    struct rte_eth_dev_info raw;
    dev_info(size_t i) : id(i) {}
    void get()
    {
        rte_eth_dev_info_get(id, &raw);
    }
};
class ether_addr : public ::ether_addr {
public:
    const size_t id;
    ether_addr(size_t i) : id(i) {}
    void print(FILE* fd) const { fprintf(fd, "%s", toString().c_str()); }
    std::string toString() const
    {
        char buf[32];
        snprintf(buf, sizeof(buf),
                "%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8
                   ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8,
                addr_bytes[0], addr_bytes[1],
                addr_bytes[2], addr_bytes[3],
                addr_bytes[4], addr_bytes[5]);
        return buf;
    }
    void update() { rte_eth_macaddr_get(id, this); }
    void set(::ether_addr* addr)
    {
        int ret = rte_eth_dev_default_mac_addr_set(id, addr);
        if (ret < 0) {
            if (ret == -ENOTSUP) {
                throw slankdev::exception(
                        "rte_eth_dev_default_mac_addr_set: hardware doesn't suppoer");
            } else if (ret == -ENODEV) {
                throw slankdev::exception(
                        "rte_eth_dev_default_mac_addr_set: port invalid");
            } else if (ret == -EINVAL) {
                throw slankdev::exception(
                        "rte_eth_dev_default_mac_addr_set: MAC address is invalid");
            } else {
                throw slankdev::exception(
                        "rte_eth_dev_default_mac_addr_set: unknown error");
            }
        }
        update();
    }
    void add(::ether_addr* addr)
    {
        int ret = rte_eth_dev_mac_addr_add(id, addr, 0);
        if (ret < 0) {
            if (ret == -ENOTSUP) {
                throw slankdev::exception(
                "rte_eth_dev_mac_addr_add: hardware doesn't support this feature.");
            } else if (ret == -ENODEV) {
                throw slankdev::exception(
                    "rte_eth_dev_mac_addr_add: port is invalid.");
            } else if (ret == -ENOSPC) {
                throw slankdev::exception(
                    "rte_eth_dev_mac_addr_add: no more MAC addresses can be added.");
            } else if (ret == -EINVAL) {
                throw slankdev::exception(
                    "rte_eth_dev_mac_addr_add: MAC address is invalid.");
            } else {
                throw slankdev::exception("rte_eth_dev_mac_addr_add: unknown");
            }
        }
        update();
    }
    void del(::ether_addr* addr)
    {
        int ret = rte_eth_dev_mac_addr_remove(id, addr);
        if (ret < 0) {
            if (ret == -ENOTSUP) {
                throw slankdev::exception(
                        "rte_eth_dev_mac_addr_remove: hardware doesn't support.");
            } else if (ret == -ENODEV) {
                throw slankdev::exception(
                        "rte_eth_dev_mac_addr_remove: if port invalid.");
            } else if (ret == -EADDRINUSE) {
                std::string errstr = "rte_eth_dev_mac_addr_remove: ";
                errstr += "attempting to remove the default MAC address";
                throw slankdev::exception(errstr.c_str());
            }
        }
        update();
    }
};




} /* namespace ssnlib */

