//
// Created by jonat on 10/6/2023.
//
#include <unistd.h>
#include "png.h"
#include "dmi.h"
#include <getopt.h>

int main(int argc, char **argv){
    extern char *optarg;
    extern int optind;
    //getopt_long();
    //getopt(gfg, gfg, gtfg);
    png_structp read_png_ptr;
    png_infop read_info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, pixels_per_byte;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }
    FILE *input_fp;

    if(!check_if_png("Base_Black.dmi",&input_fp)){
        printf("The file you are attempting to read is not a valid PNG file!\n");
        return 0;
    }
    else
        printf("lo. . .\n");

    return 1;
}
