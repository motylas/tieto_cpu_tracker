#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "reader.h"
#include "pcp_buffer.h"
#include "cpu_raw_data_product.h"
#include <stdio.h>

void* reader_thread(void* args);

#endif
