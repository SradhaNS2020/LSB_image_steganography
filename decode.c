#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    if (argv[3] != NULL)
    {
        if (strcmp(strstr(argv[3], "."), ".txt") == 0)
        {
            decInfo->secret_fname = argv[3];
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        decInfo->secret_fname = "default.txt";
    }
    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    // Open stego image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }

    // Open output file
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    // Error handling
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
        return e_failure;
    }
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;
    for (int i = 0; i < 8; i++)
    {
        *data |= ((image_buffer[i] & 0x01) << i);
    }
    return e_success;
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        if (fread(decInfo->image_data, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            fprintf(stderr, "Error: Failed to read image data\n");
            return e_failure;
        }
        if (decode_byte_from_lsb(&data[i], decInfo->image_data) != e_success)
        {
            fprintf(stderr, "Error: Failed to decode byte from image data\n");
            return e_failure;
        }
    }
    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    char str[strlen(magic_string) + 1];
    decode_data_from_image(str, strlen(magic_string), decInfo);
    str[strlen(magic_string)] = '\0';
    if (strcmp(magic_string, str) == 0)
    {
        printf("Verified magic string successfully\n");
        return e_success;
    }
    else
    {
        printf("Failed to verify magic string in image\n");
        return e_failure;
    }
}

Status decode_size_from_lsb(int *size, DecodeInfo *decInfo)
{
    *size = 0;
    char str[32];
    if (fread(str, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        fprintf(stderr, "Error: Failed to read size from image\n");
        return e_failure;
    }
    for (int i = 0; i < 32; i++)
    {
        *size |= ((str[i] & 1) << i);
    }
    return e_success;
}

Status decode_file_extn_size(int *size, DecodeInfo *decInfo)
{
    return decode_size_from_lsb(size, decInfo);
}

Status decode_file_extn(char *file_extn, int size, DecodeInfo *decInfo)
{
    char str[size + 1];
    if (decode_data_from_image(str, size, decInfo) != e_success)
    {
        return e_failure;
    }
    str[size] = '\0';
    if (strcmp(str, file_extn) == 0)
    {
        printf("File extension verified successfully\n");
        return e_success;
    }
    else
    {
        printf("Failed to verify file extension\n");
        return e_failure;
    }
}

Status decode_secret_file_size(int *size, DecodeInfo *decInfo)
{
    return decode_size_from_lsb(size, decInfo);
}

Status decode_secret_file_data(int size, DecodeInfo *decInfo)
{
    char str[size];
    if (decode_data_from_image(str, size, decInfo) != e_success)
    {
        return e_failure;
    }
    if (fwrite(str, 1, size, decInfo->fptr_secret) != size) 
    {
        fprintf(stderr, "Error: Failed to write secret data to file.\n");
        return e_failure;
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_success)
    {
        printf("Open file is success\n");
        if (decode_magic_string(MAGIC_STRING, decInfo) == e_success)
        {
            printf("Decoded magic string successfully\n");
            strcpy(decInfo->extn_secret_file, strstr(decInfo->secret_fname, "."));
            int extn_size = strlen(decInfo->extn_secret_file);
            if (decode_file_extn_size(&extn_size, decInfo) == e_success) 
            {
                printf("Decoded secret file extension size successfully\n");
                if (decode_file_extn(decInfo->extn_secret_file, extn_size, decInfo) == e_success)
                {
                    printf("Decoded secret File extension successfully\n");
                    int secret_file_size;
                    if (decode_secret_file_size(&secret_file_size, decInfo) == e_success) 
                    {
                        printf("Decoded secret file size successfully\n");
                        if (decode_secret_file_data(secret_file_size, decInfo) == e_success)
                        {
                            printf("Decoded secret file data successfully\n");
                        }
                        else
                        {
                            printf("Failed to decode secret file data\n");
                            return e_failure; 
                        }
                    }
                    else
                    {
                        printf("Failed to decode secret file size\n");
                        return e_failure; 
                    }
                }
                else
                {
                    printf("Failed to decode file extension\n");
                    return e_failure; 
                }
            }
            else
            {
                printf("Failed to decode file extension size\n");
                return e_failure; 
            }
        }
        else
        {
            printf("Failed to decode magic string\n");
            return e_failure; 
        }

        return e_success;
    }
    else
    {
        printf("Failed to open file\n");
        return e_failure; 
    }
}

