#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user-defined types

/* 
 * Structure to store information required for
 * decoding steganographic image file to secret data
 * Info and intermediate data is also stored
 */
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4 // Ensure this is enough for file extensions

typedef struct _DecodeInfo
{
    // Stego image info
    char *stego_image_fname; // File name of the stego image
    FILE *fptr_stego_image;  // File pointer for the stego image
    uint image_capacity;     // Capacity of the image
    char image_data[MAX_IMAGE_BUF_SIZE]; // Buffer for image data

    // Secret file info
    char *secret_fname;      // File name of the secret file
    FILE *fptr_secret;       // File pointer for the secret file
    char extn_secret_file[MAX_FILE_SUFFIX]; // Extension of the secret file
    long size_secret_file;   // Size of the secret file

} DecodeInfo;

/* Function prototypes */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get file pointers for input and output files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Decode data from image */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo);

/* Decode LSB from image */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode file extension size */
Status decode_file_extn_size(int *size, DecodeInfo *decInfo);

/* Decode size from LSB */
Status decode_size_from_lsb(int *size, DecodeInfo *decInfo);

/* Decode file extension from LSB of image */
Status decode_file_extn(char *file_extn, int size, DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(int *size, DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(int size, DecodeInfo *decInfo);

#endif 

