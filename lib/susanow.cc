

#include <susanow.h>
#include <unistd.h>
#include <lthread.h>
#include <slankdev/extra/dpdk.h>

void ssn_init(int argc, char** argv)
{
  ssn_sys_init(argc, argv);
  ssn_green_thread_init();
  ssn_timer_init();
  ssn_port_init();
}
void ssn_fin()
{
  rte_eal_mp_wait_lcore();
  ssn_green_thread_fin();
  ssn_timer_fin();
  ssn_port_fin();
}

void ssn_sleep(size_t msec)
{
  if (is_green_thread(rte_lcore_id())) lthread_sleep(msec*1000000);
  else usleep(msec * 1000);
}
void ssn_yield() { lthread_yield(); }
size_t ssn_lcore_id() { return rte_lcore_id(); }

bool is_green_thread(size_t lcore_id) { return ssn_get_lcore_state(lcore_id) == SSN_LS_RUNNING_GREEN; }
bool is_tthread(size_t lcore_id) { return ssn_get_lcore_state(lcore_id) == SSN_LS_RUNNING_TIMER; }



