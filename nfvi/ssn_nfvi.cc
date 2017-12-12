
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

#include <ssn_nfvi.h>
#include <slankdev/string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <mutex>
#include <crow.h>
#include <glob.h>

#include <ssn_log.h>
#include <ssn_port.h>
#include <ssn_common.h>
#include <ssn_timer.h>
#include <ssn_vnf_catalog.h>
#include <ssn_rest_api.h>
#include <slankdev/exception.h>
#include <slankdev/signal.h>
#include <slankdev/filefd.h>
#include <slankdev/hexdump.h>

size_t get_socket_id_from_pci_addr(const char* pciaddr_)
{
  char c;
  glob_t globbuf;
  ssn_pci_address addr;
  addr.set(pciaddr_);

  const std::string _path = slankdev::format(
        "/sys/devices/pci*/*/%04x\\:%02x\\:%02x.%01x/numa_node",
          addr.dom, addr.bus, addr.dev, addr.fun);
  int ret = glob(_path.c_str(), 0, NULL, &globbuf);
  if (globbuf.gl_pathc != 1) throw slankdev::exception("Fuckin");
  const std::string path = globbuf.gl_pathv[0];
  globfree(&globbuf);

  slankdev::filefd file;
  file.fopen(path.c_str(), "r");
  file.fread(&c, 1, 1);

  size_t socket_id = c - '0';
  return socket_id;
}

ssn_nfvi* _nfvip = nullptr;

bool _ssn_system_running_flag = true;
static void signal_handler(int signum)
{
  if (signum == SIGINT || signum == SIGTERM) {
    if (_nfvip) {
      printf("recv signal(%d), then exit...\n", signum);
      _nfvip->stop();
      _ssn_system_running_flag = false;
    }
  }
}

void ssn_nfvi::add_timer(ssn_timer* tim)
{
  tims.push_back(tim);
  timer_sched->add(tim);
}

void ssn_nfvi::del_timer(ssn_timer* tim)
{
  size_t n_tims = tims.size();
  for (size_t i=0; i<n_tims; i++) {
    if (tims[i] == tim) {
      tims.erase(tims.begin() + i);
      timer_sched->del(tim);
      delete tim;
      return ;
    }
  }
  std::string err;
  err = slankdev::format("ssn_nfvi::del_timer: not found timer %p", tim);
  throw slankdev::exception(err.c_str());
}

void ssn_nfvi::run(uint16_t rest_server_port)
{
  /*
   * Launch REST API server
   */
  std::thread rest_api(rest_api_thread, this, &app, rest_server_port);

  /*
   * Running loop
   */
  running = true;
  while (running) {
    slankdev::signal(SIGINT, signal_handler);
    slankdev::signal(SIGTERM, signal_handler);
    sleep(1);
  }

  /*
   * Undeploy all vnfs
   */
  const size_t n_vnf = vnfs.size();
  for (size_t i=0; i<n_vnf; i++) {
    if (vnfs[i]->is_running())
      vnfs[i]->undeploy();
  }

  rest_api.join();
}

void ssn_nfvi::stop()
{
  app.stop();
  running = false;
}

void ssn_nfvi::debug_dump(FILE* fp) const
{
  using std::string;

  printf("\n");

  printf("[+] vnfs (n:name, b:n_block, p:n_port, r:is_running)\n");
  const size_t n_vnfs = vnfs.size();
  for (size_t i=0; i<n_vnfs; i++) {
    const auto* vnf = vnfs[i];
    string name = vnf->name.c_str();
    size_t n_ports  = vnf->n_ports();
    size_t n_blocks = vnf->n_blocks();
    string run = vnf->is_running()?"true":"false";
    printf("vnfs[%zd]: n=%s b=%zd p=%zd r=%s \n",
        i, name.c_str(), n_blocks, n_ports, run.c_str());
  }

  printf("\n");

  printf("[+] ports (n:name)\n");
  const size_t n_ports = ports.size();
  for (size_t i=0; i<n_ports; i++) {
    printf("ports[%zd] n=%s \n", i, ports[i]->name.c_str());
  }

  printf("\n");

  printf("[+] port-pathc-panels (n:name)\n");
  const size_t n_ppps = ppps.size();
  for (size_t i=0; i<n_ppps; i++) {
    printf("ppps[%zd] n=%s \n", i, ppps[i]->name.c_str());
  }

  printf("\n");

  printf("[+] vnf catalog (n:name)\n");
  const size_t n_vcat = vnf_catalog.size();
  for (size_t i=0; i<n_vcat; i++) {
    printf("vcat[%zd] n=%s \n", i, vnf_catalog[i].name.c_str());
  }

}

ssn_nfvi::ssn_nfvi(int argc, char** argv, ssn_log_level ll)
  : timer_sched(nullptr), running(false)
{
  startup_time = time(nullptr);
  printf("TIME: %s", ctime(&startup_time));

  ssn_log_set_level(ll);
  ssn_init(argc, argv);
  const size_t n_ports = ssn_dev_count();
  for (size_t i=0; i<n_ports; i++) {
    dpdk::eth_dev_detach(i);
  }

  const size_t n_socket = ssn_socket_count();
  for (size_t i=0; i<n_socket; i++) {
    std::string name = slankdev::format("NFVi%zd", i);
    mp[i] = dpdk::mp_alloc(name.c_str(), i, 8191 * 4);
    printf("ALLOCATE MEMORY POOL on socket%zd\n", i);
  }

  timer_sched = new ssn_timer_sched(lcoreid_timer);
  timer_thread_tid = ssn_native_thread_launch(
      ssn_timer_sched_poll_thread, timer_sched, lcoreid_timer);

  uint64_t one_sec = ssn_timer_get_hz();
  add_timer(new ssn_timer(_timercallback, this, one_sec));

  _nfvip = this;
}

ssn_nfvi::~ssn_nfvi()
{
  /*
   * Free Objects about Timer
   */
  size_t n_tims = tims.size();
  for (size_t i=0; i<n_tims; i++) {
    ssn_timer* tim = tims[i];
    del_timer(tim);
  }
  delete timer_sched;

  /*
   * Free VNF Objects
   */
  size_t n_vnf = vnfs.size();
  for (size_t i=0; i<n_vnf; i++) {
    delete vnfs[i];
  }

  /*
   * Free Port Objects
   */
  size_t n_port = ports.size();
  for (size_t i=0; i<n_port; i++) {
    delete ports[i];
  }

  /*
   * Free ppp objects
   */
  size_t n_ppp = ppps.size();
  for (size_t i=0; i<n_ppp; i++) {
    delete ppps[i];
  }

  const size_t n_socket = ssn_socket_count();
  for (size_t i=0; i<n_socket; i++) {
    rte_mempool_free(mp[i]);
  }
  ssn_fin();
}

ssn_vnf_port* ssn_nfvi::find_port(const char* name)
{
  const size_t n_port = ports.size();
  for (size_t i=0; i<n_port; i++) {
    if (ports[i]->name == name) {
      return ports[i];
    }
  }
  return nullptr;
}

ssn_vnf* ssn_nfvi::find_vnf(const char* name)
{
  const size_t n_vnf = vnfs.size();
  for (size_t i=0; i<n_vnf; i++) {
    if (vnfs[i]->name == name) {
      return vnfs[i];
    }
  }
  return nullptr;
}

ssn_vnf_port_patch_panel* ssn_nfvi::find_ppp(const char* name)
{
  const size_t n_ppp = ppps.size();
  for (size_t i=0; i<n_ppp; i++) {
    if (ppps[i]->name == name) {
      return ppps[i];
    }
  }
  return nullptr;
}

void ssn_nfvi::vnf_register_to_catalog(const char* cname, ssn_vnfallocfunc_t f)
{
  const size_t n_ele = vnf_catalog.size();
  for (size_t i=0; i<n_ele; i++) {
    if (vnf_catalog.at(i).name == cname) {
      std::string err = "ssn_nfvi::vnf_register_to_catalog: ";
      err += slankdev::format("cname already registerd (%s)", cname);
      throw slankdev::exception(err.c_str());
    }
  }
  vnf_catalog.register_vnf(cname, f);
}

ssn_vnf_port_patch_panel*
ssn_nfvi::ppp_alloc(const char* iname, ssn_vnf_port* r, ssn_vnf_port* l)
{
  if (find_ppp(iname)) return nullptr;

  ssn_vnf_port_virt* r_ = dynamic_cast<ssn_vnf_port_virt*>(r);
  ssn_vnf_port_virt* l_ = dynamic_cast<ssn_vnf_port_virt*>(l);
  if (!r_ || !l_) return nullptr;

  if (r_->patched() || l_->patched()) return nullptr;

  ssn_vnf_port_patch_panel* ppp = new ssn_vnf_port_patch_panel(iname, r, l);
  if (!ppp) return nullptr;

  ppps.push_back(ppp);
  return ppp;
}

ssn_vnf_port* ssn_nfvi::port_alloc_virt(const char* iname)
{
  ssn_vnf_port* port = new ssn_vnf_port_virt(iname);
  port->config_hw(8,8);

  this->ports.push_back(port);
  return port;
}

ssn_vnf_port* ssn_nfvi::port_alloc_pci(const char* iname, const char* pciaddr)
{
  size_t socket_id = get_socket_id_from_pci_addr(pciaddr);
  rte_mempool* mp = get_mp(socket_id);

  ssn_vnf_port_dpdk* port = new ssn_vnf_port_dpdk(iname, ppmd_pci(pciaddr));
  port->set_mp(mp);
  port->config_hw(8,8);

  this->ports.push_back(port);
  return port;
}

ssn_vnf_port* ssn_nfvi::port_alloc_tap(const char* iname, const char* ifname)
{
  size_t socket_id = 0; // TODO
  rte_mempool* mp = get_mp(socket_id);

  ssn_vnf_port_dpdk* port = new ssn_vnf_port_dpdk(iname, vpmd_tap(ifname));
  port->set_mp(mp);
  port->config_hw(8,8);

  this->ports.push_back(port);
  return port;
}

ssn_vnf* ssn_nfvi::vnf_alloc_from_catalog(const char* cname, const char* iname)
{
  if (find_vnf(iname)) return nullptr;

  ssn_vnf* vnf = vnf_catalog.alloc_vnf(cname, iname);
  if (!vnf) return nullptr;

  vnfs.push_back(vnf);
  return vnf;
}

void ssn_nfvi::del_port(ssn_vnf_port* port)
{
  size_t n_ele = ports.size();
  for (size_t i=0; i<n_ele; i++) {
    if (ports[i] == port) {
      ports.erase(ports.begin() + i);
      delete port;
      return ;
    }
  }
  std::string err = "ssn_nfvi::del_port: ";
  err += slankdev::format("not found port (%s)", port->name.c_str());
  throw slankdev::exception(err.c_str());
}

void ssn_nfvi::del_vnf(ssn_vnf* vnf)
{
  size_t n_ele = vnfs.size();
  for (size_t i=0; i<n_ele; i++) {
    if (vnfs[i] == vnf) {
      vnfs.erase(vnfs.begin() + i);
      delete vnf;
      return ;
    }
  }
  std::string err = "ssn_nfvi::del_vnf: ";
  err += slankdev::format("not found vnf (%s)", vnf->name.c_str());
  throw slankdev::exception(err.c_str());
}

void ssn_nfvi::del_ppp(ssn_vnf_port_patch_panel* ppp)
{
  size_t n_ele = vnfs.size();
  for (size_t i=0; i<n_ele; i++) {
    if (ppps[i] == ppp) {
      ppps.erase(ppps.begin() + i);
      delete ppp;
      return ;
    }
  }
  std::string err = "ssn_nfvi::del_ppp: ";
  err += slankdev::format("not found ppp (%s)", ppp->name.c_str());
  throw slankdev::exception(err.c_str());
}

