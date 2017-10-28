
/*
 * MIT License
 *
 * Copyright (c) 2017 Susanow
 * Copyright (c) 2017 Hiroki SHIROKURA
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

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <slankdev/string.h>
#include <slankdev/exception.h>
#include <slankdev/util.h>
#include <slankdev/string.h>
#include <dpdk/dpdk.h>

#include <ssn_port.h>
#include <ssn_common.h>
#include <ssn_log.h>
#include <ssn_vnf_v02_l2fwd1b.h>
#include "lib.h"
#include "ssn_nfvi.h"

int main(int argc, char** argv)
{
  ssn_nfvi nfvi(argc, argv);
  rte_mempool* mp = nfvi.get_mp();

  /*-------------------------------------------------------------------------*/

  size_t pci0_pid = append_pci_nic("0000:01:00.0");
  size_t pci1_pid = append_pci_nic("0000:01:00.1");
  size_t tap0_pid = append_tap_pmd("tap0");
  size_t tap1_pid = append_tap_pmd("tap1");
  ssn_vnf_port_dpdk tap0(tap0_pid, 4, 4, mp); // dpdk0
  ssn_vnf_port_dpdk tap1(tap1_pid, 4, 4, mp); // dpdk1
  ssn_vnf_port_dpdk pci0(pci0_pid, 4, 4, mp); // dpdk0
  ssn_vnf_port_dpdk pci1(pci1_pid, 4, 4, mp); // dpdk1

  /*-------------------------------------------------------------------------*/
#if 1
  ssn_vnf_l2fwd1b vnf0("vnf0");
  vnf0.attach_port(0, &pci0);
  vnf0.attach_port(1, &pci1);
  vnf0.reset_allport_acc();
  vnf0.set_coremask(0, 0b00000010);
  vnf0.deploy();

  ssn_vnf_l2fwd1b vnf1("vnf1");
  vnf1.attach_port(0, &tap0);
  vnf1.attach_port(1, &tap1);
  vnf1.reset_allport_acc();
  vnf1.set_coremask(0, 0b00000100);
  vnf1.deploy();

  getchar(); vnf1.undeploy();
  getchar(); vnf0.undeploy();
#endif
  /*-------------------------------------------------------------------------*/
}


