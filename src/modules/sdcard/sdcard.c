#include "modules/sdcard/sdcard.h"

FATFS FatFs;
FIL fil;
FILINFO fno;
FRESULT fres;

char SDpath[4];

// Volume managment commands.

FRESULT mount_sd()
{
    return f_mount(&FatFs, (TCHAR const*)SDpath, 1);
}

FRESULT unmount_sd()
{
    return f_mount(NULL, (TCHAR const*)SDpath, 0);
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

FRESULT format_sd()
{
    // I dont know for certain if this code works, need to test it in the future.
    char f_buffer[128];
    f_mkfs((TCHAR const*)SDpath, FM_FAT32, 0, f_buffer, sizeof f_buffer);
}

// File/Directory Management commands.

FRESULT checkFile_sd(TCHAR FILE_NAME)
{
    return f_stat(FILE_NAME, &fno);
}

FRESULT mkdir_sd(TCHAR DIR_NAME)
{
    return f_mkdir(DIR_NAME);
}

FRESULT renameFile_sd(TCHAR OLD_FIL_NAME, TCHAR NEW_NAME)
{
    return f_rename(OLD_FIL_NAME, NEW_NAME);
}

FRESULT removeFile_sd(TCHAR FILE_PATH)
{
    return f_unlink(FILE_PATH);
}

FRESULT chdir_sd(TCHAR FILE_PATH)
{
    return f_chdir(FILE_PATH);
}

FRESULT getPath_sd(TCHAR* BUFF[128])
{
    return f_getcwd(BUFF, sizeof BUFF);
}