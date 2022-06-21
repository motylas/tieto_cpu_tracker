#include <stdlib.h>
#include "cpu_raw_data_product.h"
#include <string.h>
#include <sys/sysinfo.h>

CpuRawDataProduct* cpu_raw_data_product_create(Reader* reader){
    CpuRawDataProduct* crd_product = malloc(sizeof(CpuRawDataProduct));
    if (crd_product == NULL)
        return NULL;

    fscanf(reader->file, "%s %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu",
        crd_product->name,
        &crd_product->user,
        &crd_product->nice,
        &crd_product->system,
        &crd_product->idle,
        &crd_product->iowait,
        &crd_product->irq,
        &crd_product->softirq,
        &crd_product->steal,
        &crd_product->guest,
        &crd_product->guest_nice
    );

    return crd_product;
}

size_t cpu_raw_data_product_get_full_product_size(void){
    size_t no_cores = (size_t) get_nprocs_conf();
    return (no_cores + 1) * sizeof(CpuRawDataProduct); 
}

void cpu_raw_data_product_destroy(CpuRawDataProduct* crd_product){
    if (crd_product == NULL)
        return;
    free(crd_product);
}
