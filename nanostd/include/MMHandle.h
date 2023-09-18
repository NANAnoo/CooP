#include "CClass.h"

CLASS(MMHandle, CObject)
const char* MID;
void *ptr;
int fd;
)
    int MF(MMHandle, isValid);
    void MF(MMHandle, SetData, void *, size_t);
    void MF(MMHandle, GetData, void *, size_t);
CLASS_END(MMHandle, CObject)

M_NEW(MMHandle, const char *path, int read);