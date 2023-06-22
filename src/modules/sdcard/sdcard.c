#include "modules/sdcard/sdcard.h"

FATFS FatFs;
FIL fil;
FRESULT fres;

FRESULT mount_sd()
{
    return f_mount(&FatFs, "", 1);
}

FRESULT unmount_sd()
{
    return f_mount(NULL, "", 0);
}

uint32_t getSize_sd()
{
    FATFS *pfs;
    DWORD fre_clust;
    f_getfree("", &fre_clust, &pfs);
    return (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
}

uint32_t getFreeSpace_sd()
{
    FATFS *pfs;
    DWORD fre_clust;
    f_getfree("", &fre_clust, &pfs);
    return (uint32_t)(fre_clust * pfs->csize * 0.5);
}