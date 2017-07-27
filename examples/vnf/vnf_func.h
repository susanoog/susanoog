
#pragma once
#include "func.h"



class func_rx : public func {
 public:

  func_rx(std::vector<stageio_rx*>& _rx,
          std::vector<stageio_tx*>& _tx) : func(_rx, _tx) {}
  virtual void poll_exe() override
  {
    size_t nb_ports = ssn_dev_count();
    run = true;
    while (run) {
      for (size_t p=0; p<nb_ports; p++) {
        rte_mbuf* mbufs[32];
        size_t recvlen = rx[p]->rx_burst(mbufs, 32);
        if (recvlen == 0) continue;
        size_t enqlen = tx[p]->tx_burst(mbufs, recvlen);
        if (recvlen > enqlen) {
          slankdev::rte_pktmbuf_free_bulk(&mbufs[enqlen], recvlen-enqlen);
        }
      }
    }
  }
  virtual void stop() override { run = false; }
};



class func_wk : public func {
 public:

  func_wk(std::vector<stageio_rx*>& _rx,
          std::vector<stageio_tx*>& _tx) : func(_rx, _tx) {}
  virtual void poll_exe() override
  {
    ssn_log(SSN_LOG_INFO, "func_wk: INCLUDE DELAY\r\n");
    size_t nb_ports = ssn_dev_count();
    rte_mbuf* mbufs[32];
    while (run) {
      for (size_t p=0; p<nb_ports; p++) {
        size_t deqlen = rx[p]->rx_burst(mbufs, 32);
        for (size_t i=0; i<deqlen; i++) {
          // for (size_t j=0; j<100; j++) ; // DELAY
          for (size_t j=0; j<40; j++) ; // DELAY
          int ret = tx[p^1]->tx_shot(mbufs[i]);
          if (ret < 0) rte_pktmbuf_free(mbufs[i]);
        }
      }
    }
  }
  virtual void stop() override { run = false; }
};



class func_tx : public func {
 public:

  func_tx(std::vector<stageio_rx*>& _rx,
          std::vector<stageio_tx*>& _tx) : func(_rx, _tx) {}
  virtual void poll_exe() override
  {
    size_t nb_ports = rx.size();
    while (run) {
      for (size_t p=0; p<nb_ports; p++) {
        rte_mbuf* mbufs[32];
        size_t deqlen = rx[p]->rx_burst(mbufs, 32);
        if (deqlen == 0) continue;
        // printf("stop4 deqlen=%zd p=%zd\n", deqlen, p);
        size_t sendlen = tx[p]->tx_burst(mbufs, deqlen);
        // printf("stop10\n");
        if (deqlen > sendlen) {
          slankdev::rte_pktmbuf_free_bulk(&mbufs[sendlen], deqlen-sendlen);
        }
      }
    }
  }
  virtual void stop() override { run = false; }
};


