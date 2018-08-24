#include "cpu/cc26xx-cc13xx/rf-core/rf-core.c"
/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);
/*---------------------------------------------------------------------------*/

static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  printf("broadcast message received from %d.%d: '%s'\r\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
  jinHop =false;
  //jinOff();
}
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(example_broadcast_process, ev, data)
{
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();
  broadcast_open(&broadcast, 129, &broadcast_call);
  leds_on(LEDS_RED);
  ti_lib_gpio_set_output_enable_dio(27, GPIO_OUTPUT_ENABLE);

  // Node State Setting...
  nodeState = INITIATOR;
  //nodeState = RECEIVER;
  //nodeState = TESTER;

  // For Tester...
  for(int i = 0 ; i < 26; i++){
    maxSeqNum[i] = 0;
  }

  ctimer_set(&ApplicationTimer, CLOCK_SECOND, jinApplicationWork, NULL);
  if(nodeState == INITIATOR) ctimer_set(&ct, CLOCK_SECOND/GLOSSY_PHASE_FACTOR, jinGlossyPhase, NULL);
  
  printf("end setting \r\n");
  PROCESS_END();
}

