/*
 * This program spits out the first 128 bytes of a file in hex, decimal and string
 * form.
 * Non-printable characters are printed out as periods
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CHUNKSIZE 128
unsigned char buf[CHUNKSIZE];

char* szerrmsgs[] = {
        "No errors on %s\n",
        "USAGE: %s <filename>\n ",
        "Could not open %s\n",
        "Seek error on %s\n",
        "Position error on %s\n",
        "Rewind error on %s\n",
        "Read error on %s\n"
};

void my_exit(FILE *fp, int err, char *pc){
    if(fp) fclose(fp);
    if(err) printf(szerrmsgs[err], pc);
    exit(err);
}

void main(int argc, char* argv[]) {
    int size, c = 0, x;
    FILE *fp = 0;

    if((argc < 2) || (!strcmp(argv[1], "?"))){
        my_exit(fp, 1, argv[0]);
    }

    if(fp = fopen(argv[1], "r")){
        // find out how large the file is
        // if < CHUNK SIZE, read all there is
        if(-1 != fseek(fp, 0, 2)){ // seek to end
            if(-1 != (size = ftell(fp))){
                if(size > CHUNKSIZE){
                    size = CHUNKSIZE;
                }
                if(-1 != fseek(fp, 0, 0)){ // seek to start
                    if(fread(buf, 1, size, fp) == (unsigned int)size){
                        // print this as 8 chars by 16 lines, first HEX, then decimal, then string
                        while(c < size){
                            // print HEX
                            printf("%3d: %02x%02x %02x%02x %02x%02x %02x%02x", c, buf[c], buf[c+1], buf[c+2], buf[c+3], buf[c+4], buf[c+5], buf[c+6], buf[c+7]);

                            // print DECIMAL
                            printf("%03d%03d %03d%03d %03d%03d %03d%03d ", buf[c], buf[c+1], buf[c+2], buf[c+3], buf[c+4], buf[c+5], buf[c+6], buf[c+7]);

                            // print String, check the chars and if not printable replace with periods
                            for (x = 0; x < 8; x++) {
                                if(!isprint(buf[c+x])){
                                    buf[c+x] = '.';
                                }
                            }

                            printf(" %c%c%c%c%c%c%c%c\n", buf[c], buf[c+1], buf[c+2], buf[c+3], buf[c+4], buf[c+5], buf[c+6], buf[c+7] );

                            c += 8;
                        }
                    }else{
                        my_exit(fp, 6, argv[0]);
                    }
                }else{
                    my_exit(fp, 5, argv[0]);
                }
            }else{
                my_exit(fp, 4, argv[0]);
            }
        }else{
            my_exit(fp, 3, argv[0]);
        }
    }else{
        my_exit(fp, 2, argv[1]);
    }

    my_exit(fp, 0, argv[1]); // no errors

}
