
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define KERNEL_FORMAT     "[تطبيق]"
#define KERNEL_LOAD_BASE  0x100000ULL

typedef struct
{
    char     format[128];
    uint64_t entry_point;
    uint64_t image_size;
    uint64_t flags;
} aros_kernel_header_t;

int main(int argc, char *argv[])
{

    uint64_t entry_offset = 0x53b;

    if (argc >= 2) {
        /* قراءة عنوان KernelMain المطلق من command line */
        uint64_t abs_addr = strtoull(argv[1], NULL, 0);
        if (abs_addr >= KERNEL_LOAD_BASE) {
            entry_offset = abs_addr - KERNEL_LOAD_BASE;
        } else {
            /* المستخدم أعطى offset مباشرة */
            entry_offset = abs_addr;
        }
    }

    printf("[maker] KERNEL_LOAD_BASE = 0x%llx\n",
           (unsigned long long)KERNEL_LOAD_BASE);
    printf("[maker] entry_offset     = 0x%llx\n",
           (unsigned long long)entry_offset);

    FILE *in = fopen("kernel.bin", "rb");
    if (!in) {
        fprintf(stderr, "[ERROR] لا يمكن فتح kernel.bin\n");
        return 1;
    }

    FILE *out = fopen("النواة.تطبيق", "wb");
    if (!out) {
        fprintf(stderr, "[ERROR] لا يمكن إنشاء النواة.تطبيق\n");
        fclose(in);
        return 1;
    }

    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    rewind(in);

    printf("[maker] image_size = %ld bytes\n", size);

    aros_kernel_header_t hdr;
    memset(&hdr, 0, sizeof(hdr));
    strcpy(hdr.format, KERNEL_FORMAT);
    hdr.entry_point = entry_offset;
    hdr.image_size  = (uint64_t)size;
    hdr.flags       = 0;

    fwrite(&hdr, sizeof(hdr), 1, out);

    char buf[4096];
    while (!feof(in)) {
        size_t n = fread(buf, 1, sizeof(buf), in);
        if (n > 0) fwrite(buf, 1, n, out);
    }

    fclose(in);
    fclose(out);

    printf("[maker] النواة.تطبيق تم إنشاؤها بنجاح\n");
    return 0;
}
