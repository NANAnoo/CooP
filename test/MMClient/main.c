//
// Created by ZhangHao on 2023/8/16.
//
#include <MMHandle.h>
#include <libc.h>

const char *map_file = "/test_map";
int main() {
    MMHandle *handle = NEW(MMHandle, map_file, 1);
    if (handle && MCall(handle, MMHandle, isValid)) {
        char buff[1024];
        do {
            MCall(handle, MMHandle, GetData, buff, 1024);
            printf("%s \n", buff);
        } while (buff[0] != '.');
        DELETE(handle);
    }
    return 0;
}