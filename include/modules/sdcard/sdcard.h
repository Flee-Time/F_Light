#ifndef __SDCARD_H
#define __SDCARD_H

#include "modules/fatfs/fatfs.h"
#include "stm32f4xx_hal.h"

// VOLUME MANAGEMENT COMMANDS

/** The mount_sd fucntion registers file system object to the FatFs module.
 *
 * @return     FileSystem Result
 */
FRESULT mount_sd();

/** The unmount_sd fucntion unregisters file system object to the FatFs module.
 *
 * @return     FileSystem Result
 */
FRESULT unmount_sd();

/** The getSize_sd function gets size of the volume.
 *
 *
 * @return     device status
 */
uint32_t getSize_sd();

/** The getFreeSpace_sd function gets size of the free space on the volume.
 *
 * @return     device status
 */
uint32_t getFreeSpace_sd();

/** The format_sd fucntion creates an FAT32 volume on the logical drive.
 *
 * @return     FileSystem Result
 */
FRESULT format_sd();

// FILE/DIRECTORY MANAGMENT COMMANDS

/** The checkFile_sd function checks the existence of a file or sub-directory.
 *
 * @param      FILE_NAME  - The name of the file or folder to be checked if its there.
 * 
 * @return     FileSystem Result
 */
FRESULT checkFile_sd(TCHAR FILE_NAME);

/** The mkdir_sd function creates a new directory.
 *
 * @param      DIR_NAME  - The name of the folder to be created in the current directory.
 * 
 * @return     FileSystem Result
 */
FRESULT mkdir_sd(TCHAR DIR_NAME);

/** The renameFile_sd function renames and/or moves a file or sub-directory.
 *
 * @param      OLD_FIL_NAME  - The current file name of the file to be changed.
 * @param      NEW_NAME      - The new name for the file.
 *
 * @return     FileSystem Result
 */
FRESULT renameFile_sd(TCHAR OLD_FIL_NAME, TCHAR NEW_NAME);

/** The remveFile_sd function removes a file or sub-directory.
 *
 * @param      FILE_PATH  - The path of the file to be deleted.
 *
 * @return     FileSystem Result
 */
FRESULT removeFile_sd(TCHAR FILE_PATH);

/** The chdir_sd function changes the current directory of a drive.
 *
 * @param      FILE_PATH  - Path of the directory to be entered.
 *
 * @return     FileSystem Result
 */
FRESULT chdir_sd(TCHAR FILE_PATH);

/** The getPath_sd function retrieves the current directory and current drive.
 *
 * @param      BUFF  - The buffer for the current path to be written.
 *
 * @return     FileSystem Result
 */
FRESULT getPath_sd(TCHAR* BUFF[128]);

#endif