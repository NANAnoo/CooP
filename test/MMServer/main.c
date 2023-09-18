#include <MMHandle.h>
#include <libc.h>

const char *map_file = "/test_map";
int main() {
    MMHandle *handle = NEW(MMHandle, map_file, 0);
    if (handle && MCall(handle, MMHandle, isValid)) {
        char buff[1024];
        do {
            size_t len = read(STDIN_FILENO, buff, 1024);
            MCall(handle, MMHandle, SetData, buff, len);
        } while (buff[0] != '.');
        DELETE(handle);
    }
    return 0;
}