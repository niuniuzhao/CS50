#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue
            int g = round(((float) image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);

            // Update pixel values
            image[i][j].rgbtRed = g;
            image[i][j].rgbtGreen = g;
            image[i][j].rgbtBlue = g;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        // Since we are reflecting we only need to loop over half of width
        for (int j = 0; j < width / 2; j++)
        {
            // Initialise temp variables
            int tempRed = 0;
            int tempGreen = 0;
            int tempBlue = 0;

            // Swap pixels
            tempRed = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][width - 1 - j].rgbtRed = tempRed;

            tempGreen = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][width - 1 - j].rgbtGreen = tempGreen;

            tempBlue = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][width - 1 - j].rgbtBlue = tempBlue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Create a copy of image to read from
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Create a new image
    // For each pixel...
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialise variables
            int R = 0;
            int G = 0;
            int B = 0;
            int counter = 0;

            // For each pixel in the 3x3 (or less) neighbourhood
            for (int a = i - 1; a <= i + 1; a++)
            {
                for (int b = j - 1; b <= j + 1; b++)
                {
                    // Skip non-existent neighbours in corner and edge cases
                    if (a < 0 || a > height - 1 || b < 0 || b > width - 1)
                    {
                        continue;
                    }
                    // Get total R, G, B and count
                    else
                    {
                        R += copy[a][b].rgbtRed;
                        G += copy[a][b].rgbtGreen;
                        B += copy[a][b].rgbtBlue;
                        counter++;
                    }
                }
            }
            // Update pixels in original image
            image[i][j].rgbtRed = round((float) R / counter);
            image[i][j].rgbtGreen = round((float) G / counter);
            image[i][j].rgbtBlue = round((float) B / counter);
        }
    }
}

// Edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // Create a copy of image
    RGBTRIPLE copy[height][width];

    // Store Gx & Gy values in arrays
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // For each pixel...
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialise variables
            int GxR = 0;
            int GxG = 0;
            int GxB = 0;
            int GyR = 0;
            int GyG = 0;
            int GyB = 0;

            // For each pixel in the 3x3 (or less) neighbourhood
            for (int a = i - 1; a <= i + 1; a++)
            {
                for (int b = j - 1; b <= j + 1; b++)
                {
                    // Skip non-existent neighbours in corner and edge cases
                    if (a < 0 || a > height - 1 || b < 0 || b > width - 1)
                    {
                        continue;
                    }
                    // Multiply and sum Gx and Gy values
                    else
                    {
                        GxR += image[a][b].rgbtRed * Gx[a - (i - 1)][b - (j - 1)];
                        GyR += image[a][b].rgbtRed * Gy[a - (i - 1)][b - (j - 1)];
                        GxG += image[a][b].rgbtGreen * Gx[a - (i - 1)][b - (j - 1)];
                        GyG += image[a][b].rgbtGreen * Gy[a - (i - 1)][b - (j - 1)];
                        GxB += image[a][b].rgbtBlue * Gx[a - (i - 1)][b - (j - 1)];
                        GyB += image[a][b].rgbtBlue * Gy[a - (i - 1)][b - (j - 1)];
                    }
                }
            }
            // Use Sobel algorithm to combine Gx, Gy
            int SobelRed = round(sqrt((float) pow(GxR, 2) + pow(GyR, 2)));
            int SobelGreen = round(sqrt((float) pow(GxG, 2) + pow(GyG, 2)));
            int SobelBlue = round(sqrt((float) pow(GxB, 2) + pow(GyB, 2)));

            // Cap value at 255
            if (SobelRed > 255)
            {
                copy[i][j].rgbtRed = 255;
            }
            else
            {
                copy[i][j].rgbtRed = SobelRed;
            }

            if (SobelGreen > 255)
            {
                copy[i][j].rgbtGreen = 255;
            }
            else
            {
                copy[i][j].rgbtGreen = SobelGreen;
            }

            if (SobelBlue > 255)
            {
                copy[i][j].rgbtBlue = 255;
            }
            else
            {
                copy[i][j].rgbtBlue = SobelBlue;
            }
        }
    }

    // Overwrite original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
}
