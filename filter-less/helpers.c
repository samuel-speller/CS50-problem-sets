#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //find average of RBG values
            float greyscale_float = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) /
                                    3.0;
            //round this number to the nearest int
            int greyscale_round = round(greyscale_float);

            //set RGB values to this average
            image[i][j].rgbtRed = greyscale_round;
            image[i][j].rgbtGreen = greyscale_round;
            image[i][j].rgbtBlue = greyscale_round;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate sepia RGB values using supplied alorithm
            float sepiaRed_float = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue;

            //round float value
            int sepiaRed = round(sepiaRed_float);

            //cap value at 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            //do the same for green and blue
            float sepiaGreen_float = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue;
            int sepiaGreen = round(sepiaGreen_float);

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            float sepiaBlue_float = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue;
            int sepiaBlue = round(sepiaBlue_float);

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            //input new pixel colours into the ouput file
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        //only cycle through half the width, otherwise you will switch the pixels and then switch them back!
        for (int j = 0; j < (width / 2); j++)
        {
            //switch pixels
            RGBTRIPLE temp = image [i][j];
            image [i][j] = image[i][width - j - 1];
            image [i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a copy of the image so the blur function doesn't blur itself
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image [i][j];
        }
    }

    //blur algorithm
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //create ints for the sum of all the colours
            float blurredRed_tot = 0;
            float blurredGreen_tot = 0;
            float blurredBlue_tot = 0;

            //variable to keep track of the number of pixels you are adding together (some will be ignored as they
            //are outside the image array)
            int pixel_count = 0;

            //create nested loops to iterate over all the surounding pixels
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    //create if statement to ignore pixels beyond the limits of the image
                    if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width)
                    {
                        blurredRed_tot += copy[i + k][j + l].rgbtRed;
                        blurredGreen_tot += copy[i + k][j + l].rgbtGreen;
                        blurredBlue_tot += copy[i + k][j + l].rgbtBlue;

                        pixel_count++;
                    }
                }
            }

            //find the average value of RGB from the totals
            int blurredRed = round(blurredRed_tot / pixel_count);
            int blurredGreen = round(blurredGreen_tot / pixel_count);
            int blurredBlue = round(blurredBlue_tot / pixel_count);

            //change the image pixels to the blurred values
            image[i][j].rgbtRed = blurredRed;
            image[i][j].rgbtGreen = blurredGreen;
            image[i][j].rgbtBlue = blurredBlue;
        }
    }

    return;
}
