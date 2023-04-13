#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//global variable for jpeg block size AND DEFINE BYTE
int JPEGBLOCKSIZE = 512;
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //make sure user inputs one command line argument
    if (argc != 2)
    {
        printf("Please use one command line argument to specify filename/n");
        return 1;
    }

    //define file name pointer for readability
    char *input_file_name = argv[1];

    //open file specified by the user in the command line argument
    FILE *file = fopen(input_file_name, "r");

    //error message if file can't be read
    if (file == NULL)
    {
        printf("Error, cannot open %s\n", input_file_name);
        return 1;
    }

    //initialise variables

    //define buffer, e.g an array of bytes (8 bits each) with 512 entries
    BYTE buffer[JPEGBLOCKSIZE];

    //array to store the file names of the jpegs
    char output_filename[8];

    //counter to keep track of the jpeg number
    int file_counter = 0;

    //image file to write to
    FILE *img = NULL;


    while (fread(&buffer, JPEGBLOCKSIZE, 1, file) == 1)
    {
        //in the explanation video they use 'buffer[3] & 0xf0 == 0xe0' as the end condition but i think my version will
        //work too. If it doesn't work then check here.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
            //close previous file (if you're not on the first JPEG)
            if (file_counter != 0)
            {
                fclose(img);
            }

            //create jpeg filename that will increase the number of each JPEG
            sprintf(output_filename, "%03i.jpg", file_counter);

            //open file to write JPEG too
            img = fopen(output_filename, "w");

            //increase number to be used for the next file
            file_counter++;

        }

        //after finding the start of a new jpeg we need to continue writing to the new file until we find another one

        //if loop to only start wrting when we've found our first jpeg
        if (file_counter != 0)
        {
            fwrite(&buffer, JPEGBLOCKSIZE, 1, img);
        }

    }

    //close remaining files
    fclose(file);
    fclose(img);

    return 0;


}