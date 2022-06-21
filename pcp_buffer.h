#ifndef PCPBUFFER_H
#define PCPBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct PCPBuffer PCPBuffer;
PCPBuffer* pcpbuffer_create(size_t packet_size, size_t max_no_packets);
void pcpbuffer_destroy(PCPBuffer* pcpbuffer);
bool pcpbuffer_is_full(PCPBuffer* buffer);
bool pcpbuffer_is_empty(PCPBuffer* buffer);
void pcpbuffer_put(PCPBuffer* pcpbuffer, u_int8_t* product);
u_int8_t* pcpbuffer_get(PCPBuffer* pcpbuffer);
void pcpbuffer_lock(PCPBuffer* pcpbuffer);
void pcpbuffer_unlock(PCPBuffer* pcpbuffer);
void pcpbuffer_call_producer(PCPBuffer* pcpbuffer);
void pcpbuffer_call_consumer(PCPBuffer* pcpbuffer);
void pcpbuffer_wait_for_producer(PCPBuffer* pcpbuffer);
void pcpbuffer_wait_for_consumer(PCPBuffer* pcpbuffer);

#endif
