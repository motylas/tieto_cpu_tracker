#include "thread_manager.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <sys/sysinfo.h>

static pthread_t tid[1];
static PCPBuffer* readerBuffer;
static char path[] = "/proc/stat";

void* reader_thread(void* arg){
    (void)(arg);
    Reader* reader = reader_create(path);
    if (reader == NULL)
        return NULL;

    while(true){
        reader_reopen(reader);
        CpuRawDataProduct* cpr_product = NULL;
        size_t full_product_size = cpu_raw_data_product_get_full_product_size();
        u_int8_t* full_product = malloc(full_product_size);


        for (size_t i = 0; i < 5; i++){
            cpr_product = cpu_raw_data_product_create(reader);
            memcpy(&full_product[i * sizeof(CpuRawDataProduct)], cpr_product, sizeof(CpuRawDataProduct));
            cpu_raw_data_product_destroy(cpr_product);
        }

        pcpbuffer_lock(readerBuffer);
            if (pcpbuffer_is_full(readerBuffer)){
                pcpbuffer_wait_for_consumer(readerBuffer);
            }
            pcpbuffer_put(readerBuffer, full_product);
            pcpbuffer_call_consumer(readerBuffer);
        pcpbuffer_unlock(readerBuffer);
        free(full_product);
    }


    reader_destroy(reader);
    return NULL;
}



int main(void){
    size_t one_package_size = cpu_raw_data_product_get_full_product_size();
    readerBuffer = pcpbuffer_create(one_package_size, 10);
    pthread_create(&tid[0], NULL, reader_thread, NULL);
    pthread_join(tid[0], NULL);

    pcpbuffer_destroy(readerBuffer);
    return 0;
}
