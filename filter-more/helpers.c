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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //create a copy of the image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    //define Gx and Gy

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //iterate over the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //define variables
            double GxRed_sum = 0;
            double GxGreen_sum = 0;
            double GxBlue_sum = 0;

            double GyRed_sum = 0;
            double GyGreen_sum = 0;
            double GyBlue_sum = 0;

            //create weighted sum using Gx and Gy
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    //create if statement to ignore pixels that are outside the image array.
                    //as we are creating a ring of black pixels around the image (where R, G and B = 0) they wouldn't add
                    //anything to the sum anyway, so we can still ignore them.
                    if ((i + k) >= 0 && (i + k) < height && (j + l) >= 0 && (j + l) < width)
                    {
                        GxRed_sum += copy[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                        GxGreen_sum += copy[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                        GxBlue_sum += copy[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];

                        GyRed_sum += copy[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                        GyGreen_sum += copy[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                        GyBlue_sum += copy[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                    }
                }
            }

            //perform sobel algorithm
            double sobelRed_sum = sqrt((GxRed_sum * GxRed_sum) + (GyRed_sum * GyRed_sum));
            double sobelGreen_sum = sqrt((GxGreen_sum * GxGreen_sum) + (GyGreen_sum * GyGreen_sum));
            double sobelBlue_sum = sqrt((GxBlue_sum * GxBlue_sum) + (GyBlue_sum * GyBlue_sum));

            int sobelRed = round(sobelRed_sum);
            int sobelGreen = round(sobelGreen_sum);
            int sobelBlue = round(sobelBlue_sum);

            //if statements to make sure R, G, B values are capped at 255
            if (sobelRed > 255)
            {
                sobelRed = 255;
            }

            if (sobelGreen > 255)
            {
                sobelGreen = 255;
            }

            if (sobelBlue > 255)
            {
                sobelBlue = 255;
            }

            //update image
            image[i][j].rgbtRed = sobelRed;
            image[i][j].rgbtGreen = sobelGreen;
            image[i][j].rgbtBlue = sobelBlue;
        }
    }
    return;
}
