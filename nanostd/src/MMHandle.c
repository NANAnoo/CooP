//
// Created by ZhangHao on 2023/8/16.
//
#include "MMHandle.h"
#include <libc.h>
#include <sys/mman.h>
extern errno_t errno;

IMPL_CLASS(MMHandle , CObject)
M_NEW(MMHandle, const char *STORAGE_ID, int read) {
    if (STORAGE_ID && strlen(STORAGE_ID) > 0) {
        this->MID = STORAGE_ID;
        this->fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (this->fd != -1) {
            if (ftruncate(this->fd, 4096) != -1) {
                this->ptr = mmap(NULL, 4096,
                                 read ? PROT_READ : PROT_WRITE, MAP_SHARED, this->fd, 0);
            }
        }
        if (this->ptr == NULL || this->ptr == MAP_FAILED) {
            this->ptr = MAP_FAILED;
            this->fd = shm_unlink(this->MID);
            this->MID = NULL;
            this->fd = -1;
            printf("Create Handle Failed: %s : %s\n", strerror(errno), STORAGE_ID);
        }
    }
    return this;
}

MF_IMPL(MMHandle)
int, isValid) {
    return this->fd != -1 && this->ptr != MAP_FAILED && this->MID != NULL;
}

MF_IMPL(MMHandle)
void, SetData, void *data, size_t len) {
    if (MCall(this, MMHandle, isValid)) {
        ((char *)this->ptr)[0] = 'a';
        memcpy(this->ptr, data, len < 4096 ? len : 4096);
    }
}

MF_IMPL(MMHandle)
void , GetData, void *out, size_t size) {
    if (MCall(this, MMHandle, isValid)) {
        memcpy(out, this->ptr, size);
    }
}

Destructor(MMHandle) {
    if (MCall(this, MMHandle, isValid)) {
        munmap(this->ptr, 4096);
        this->fd = shm_unlink(this->MID);
    }
}
