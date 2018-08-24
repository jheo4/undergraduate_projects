#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
#include "core/net/rime/chameleon.h"
#include "net/mac/framer-802154.h"
#include "net/netstack.h"
#include "sys/ctimer.h"
#include "net/mac/mac-sequence.h"
#include "cpu/cc26xx-cc13xx/ti-lib.h"
#include "cpu/cc26xx-cc13xx/rf-core/ieee-mode.c"

/*---------------------------------------------------------------------------*/
// Jin_Glossy mode
#ifndef __JIN_GLOSSY__
#define __JIN_GLOSSY__
#endif
/*---------------------------------------------------------------------------*/
static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from);
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
static struct ctimer ct;
static struct ctimer ApplicationTimer;
/*---------------------------------------------------------------------------*/
#define JIN_CHANNEL broadcast.c.channel
#define INITIATOR 1
#define RECEIVER 2
#define TESTER 3
#define N_TX 1
#define GLOSSY_PHASE_FACTOR 4
/*---------------------------------------------------------------------------*/
static int curTX = 0;
static int nodeState = 0;
static bool jinHop = false;
static bool CheckDI = false;

static int maxSeqNum[26]; // a to z 
static int minHopCount;


// for Testing.. 
static int jinTestCount = 0;
#define MAX_TEST_COUNT 1440
/*---------------------------------------------------------------------------*/
// Functions for Glossy phase & application work
void jinSend();
void jinGlossyPhase();
void jinOn();
void jinOff();
void jinApplicationWork();
/*---------------------------------------------------------------------------*/
// Setting 
//////////////////////////////
// Contiki RIME Stack
// NULL MAC
// NULL RDC
// FRAMER802154
//////////////////////////////
void jinSend(){
  chameleon_create(&JIN_CHANNEL);
  packetbuf_compact();
  packetbuf_set_addr(PACKETBUF_ADDR_SENDER, &linkaddr_node_addr);
  packetbuf_set_attr(PACKETBUF_ATTR_FRAME_TYPE, FRAME802154_DATAFRAME);
  NETSTACK_FRAMER.create();
  NETSTACK_RADIO.prepare(packetbuf_hdrptr(), packetbuf_totlen());
  NETSTACK_RADIO.transmit(packetbuf_totlen());
}
/*---------------------------------------------------------------------------*/
void jinGlossyPhase(){
  jinOn();

  ti_lib_timer_disable(GPT0_BASE, TIMER_B);
  ti_lib_timer_disable(GPT0_BASE, TIMER_A);
  if(nodeState == INITIATOR && jinTestCount < MAX_TEST_COUNT){
    ctimer_set(&ct, CLOCK_SECOND/GLOSSY_PHASE_FACTOR, jinGlossyPhase, NULL);

    // first a : seq number
    // second a : hop count
    packetbuf_copyfrom("aaGlossy", 8);
    jinHop = true;
    jinSend();
    curTX++;
    jinTestCount++;
    leds_toggle(LEDS_RED);
  }
  ti_lib_timer_enable(GPT0_BASE, TIMER_B);
  ti_lib_timer_enable(GPT0_BASE, TIMER_A);
  printf("End glossy phase..\r\n");
}
/*---------------------------------------------------------------------------*/
void jinOn(){
  curTX = 0;
  jinHop = false;
  // Radio On / Off
  //NETSTACK_RADIO.on();
}
/*---------------------------------------------------------------------------*/
void jinOff(){
  // Radio On / Off
  //NETSTACK_RADIO.off();
}
/*---------------------------------------------------------------------------*/
void jinApplicationWork(){
  // Tester Node is usally the farthest node to check the hop count & the relay counter
  printf("Application work!\r\n");
  if(nodeState == TESTER){
    for(int i = 0 ; i < 26; i++) printf("%d ",maxSeqNum[i]);
    printf("\r\n");
    printf("minHopCount : %d \r\n", minHopCount);
  }
  ctimer_set(&ApplicationTimer, CLOCK_SECOND*3, jinApplicationWork, NULL);
}
/*---------------------------------------------------------------------------*/