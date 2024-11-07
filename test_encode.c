#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int main(int argc,char *argv[])
{
    
    EncodeInfo E1;
    //validate CLA
    if (argc<2)
    {
        printf("Error: Insufficient arguments.\n");
        printf("Usage:\n");
        printf("  For Encoding: ./a.out -e beautiful.bmp secret.txt [stegano.bmp]\n");
        printf("  For Decoding: ./a.out -d stegano.bmp [decoded.txt]\n");
        return 1;
    }
    else
    {

        int ret=check_operation_type(argv);
        if (ret == e_encode)
        {
            printf("Encoding is selected\n");
            //read and validate encode arguments
            if(read_and_validate_encode_args(argv,&E1)== e_success)
            {
                    printf("Info : Read and validate args is success\n");
                    if (do_encoding(&E1) == e_success)
                    {
                        printf("Encoding is success\n");
                    }
            }
            else
            {
                    printf("Info : Read and validate args is failed\n");
                    return 5;
            }
            
        }
        else if (ret == e_decode )
        {
            DecodeInfo D1;
            printf("Decoding is selected\n");
            //read and validate decode arguments
            if (read_and_validate_decode_args(argv,&D1) == e_success)
            {
            printf("Info : Read and validate args is success\n");
            if ( do_decoding(&D1)== e_success)
            {
                    printf("Decoding is success\n");
            }
            }
            else
            {
                    printf("Info : Read and validate args is failed\n");
                    return 5;
            }     
        }
        else
        {
            printf("Error : Unsupported type\n");
            printf("Usage : For Encoding : ./a.out -e beautiful.bmp secret.txt [stegano.bmp]\n");
            printf("        For Decoding : ./a.out -d stegano.bmp [decoded.txt]\n");
        }
    }
    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1],"-e")==0) 
    return e_encode;
    else if (strcmp(argv[1],"-d")==0)
    return e_decode;
    else
    return e_unsupported;
}

     

