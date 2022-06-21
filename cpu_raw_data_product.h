#ifndef CPU_RAW_DATA_PRODUCT_H
#define CPU_RAW_DATA_PRODUCT_H

#include "reader.h"

typedef struct CpuRawDataProduct{
    char name[8];
    size_t user;
    size_t nice;
    size_t system;
    size_t idle;
    size_t iowait;
    size_t irq;
    size_t softirq;
    size_t steal;
    size_t guest;
    size_t guest_nice;
}CpuRawDataProduct;

CpuRawDataProduct* cpu_raw_data_product_create(Reader* reader);
size_t cpu_raw_data_product_get_full_product_size(void);
void cpu_raw_data_product_destroy(CpuRawDataProduct* crd_product);

#endif
