#include "pcp_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

struct PCPBuffer{
    size_t packet_size;
    size_t max_no_packets;
    size_t current_no_packets;
    size_t head;
    size_t tail;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
    u_int8_t buffer[]; /* FAM */
};

PCPBuffer* pcpbuffer_create(size_t packet_size, size_t max_no_packets){
    if (packet_size == 0)
        return NULL;
    if (max_no_packets == 0)
        return NULL;
    
    PCPBuffer* pcpbuffer;
    pcpbuffer = malloc(sizeof(PCPBuffer) + packet_size * max_no_packets);
    if (pcpbuffer == NULL)
        return NULL;
    
    *pcpbuffer = (PCPBuffer){.packet_size = packet_size,
                             .max_no_packets = max_no_packets,
                             .current_no_packets = 0,
                             .head = 0,
                             .tail = 0,
                             .mutex = PTHREAD_MUTEX_INITIALIZER,
                             .can_produce = PTHREAD_COND_INITIALIZER,
                             .can_consume = PTHREAD_COND_INITIALIZER
    };
    return pcpbuffer;
}

void pcpbuffer_destroy(PCPBuffer* pcpbuffer){
    pthread_cond_destroy(&pcpbuffer->can_consume);
    pthread_cond_destroy(&pcpbuffer->can_produce);
    pthread_mutex_destroy(&pcpbuffer->mutex);

    free(pcpbuffer);
}

bool pcpbuffer_is_full(PCPBuffer* buffer){
    return buffer->current_no_packets == buffer->max_no_packets;
}

bool pcpbuffer_is_empty(PCPBuffer* buffer){
    return buffer->current_no_packets == 0;
}

void pcpbuffer_put(PCPBuffer* pcpbuffer, u_int8_t* product){
    if (pcpbuffer_is_full(pcpbuffer))
        return;

    size_t buffer_start = pcpbuffer->packet_size * pcpbuffer->head;

    memcpy(&pcpbuffer->buffer[buffer_start], product, pcpbuffer->packet_size);
    pcpbuffer->head = (pcpbuffer->head + 1) % pcpbuffer->max_no_packets;
    pcpbuffer->current_no_packets++;
}

u_int8_t* pcpbuffer_get(PCPBuffer* pcpbuffer){
    if (pcpbuffer_is_empty(pcpbuffer))
        return NULL;
    
    u_int8_t* product = malloc(sizeof(pcpbuffer->packet_size));

    size_t buffer_end = pcpbuffer->packet_size * pcpbuffer->tail;
    memcpy(product, &pcpbuffer->buffer[buffer_end], pcpbuffer->packet_size);
    pcpbuffer->tail = (pcpbuffer->tail + 1) % pcpbuffer->max_no_packets;
    pcpbuffer->current_no_packets--;

    return product;
}

void pcpbuffer_lock(PCPBuffer* pcpbuffer){
    pthread_mutex_lock(&pcpbuffer->mutex);
}

void pcpbuffer_unlock(PCPBuffer* pcpbuffer){
    pthread_mutex_unlock(&pcpbuffer->mutex);
}

void pcpbuffer_call_producer(PCPBuffer* pcpbuffer){
    pthread_cond_signal(&pcpbuffer->can_produce);
}

void pcpbuffer_call_consumer(PCPBuffer* pcpbuffer){
    pthread_cond_signal(&pcpbuffer->can_consume);
}

void pcpbuffer_wait_for_producer(PCPBuffer* pcpbuffer){
    pthread_cond_wait(&pcpbuffer->can_consume, &pcpbuffer->mutex);
}

void pcpbuffer_wait_for_consumer(PCPBuffer* pcpbuffer){
    pthread_cond_wait(&pcpbuffer->can_produce, &pcpbuffer->mutex);
}
