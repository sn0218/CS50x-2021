#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float avg_pixel_value = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate the average pixel value
            avg_pixel_value = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;

            // set each colour value to the average value
            image[i][j].rgbtRed = (int) round(avg_pixel_value);
            image[i][j].rgbtGreen = (int) round(avg_pixel_value);
            image[i][j].rgbtBlue = (int) round(avg_pixel_value);
        }
        // reset the avg_pixel_value for the next pixel
        avg_pixel_value = 0;
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // swap pixels on horizontally opposite sides
        if (width % 2 == 0)
        {
            // even no. of pixel in each row of image
            for (int j = 0; j < width / 2; j++)
            {
                RGBTRIPLE tmp = image[i][j];
                image[i][j] = image[i][width - 1 - j];
                image[i][width - 1 - j] = tmp;
            }
        }
        else
        {
            // odd no. of pixel in each row of image
            for (int j = 0; j < width / 2; j++)
            {
                RGBTRIPLE tmp = image[i][j];
                image[i][j] = image[i][width - 1];
                image[i][width - 1] = tmp;
            }
        }

    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float avg_red = 0.0;
    float avg_green = 0.0;
    float avg_blue = 0.0;
    RGBTRIPLE tmp[height][width]; // create tmp array to store the original pixel value of the image

    // copy the original pixel value of image and store to tmp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // calculate the average pixel value of middle pixel
            if (i > 0 && i < (height - 1) && j > 0 && j < (width - 1))
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i - 1][j - 1].rgbtRed + tmp[i - 1][j].rgbtRed + tmp[i - 1][j + 1].rgbtRed +
                           tmp[i][j - 1].rgbtRed + tmp[i][j + 1].rgbtRed + tmp[i + 1][j - 1].rgbtRed + tmp[i + 1][j].rgbtRed + tmp[i + 1][j + 1].rgbtRed)
                          / 9.0;

                avg_green = (tmp[i][j].rgbtGreen + tmp[i - 1][j - 1].rgbtGreen + tmp[i - 1][j].rgbtGreen + tmp[i - 1][j + 1].rgbtGreen +
                             tmp[i][j - 1].rgbtGreen + tmp[i][j + 1].rgbtGreen + tmp[i + 1][j - 1].rgbtGreen + tmp[i + 1][j].rgbtGreen +
                             tmp[i + 1][j + 1].rgbtGreen) / 9.0;

                avg_blue = (tmp[i][j].rgbtBlue + tmp[i - 1][j - 1].rgbtBlue + tmp[i - 1][j].rgbtBlue + tmp[i - 1][j + 1].rgbtBlue +
                            tmp[i][j - 1].rgbtBlue + tmp[i][j + 1].rgbtBlue + tmp[i + 1][j - 1].rgbtBlue + tmp[i + 1][j].rgbtBlue +
                            tmp[i + 1][j + 1].rgbtBlue) / 9.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);

            }
            // edge case on top
            else if (i == 0 && j > 0 && j < (width - 1))
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i][j - 1].rgbtRed + tmp[i][j + 1].rgbtRed + tmp[i + 1][j - 1].rgbtRed + tmp[i + 1][j].rgbtRed +
                           tmp[i + 1][j + 1].rgbtRed) / 6.0;
                avg_green = (tmp[i][j].rgbtGreen + tmp[i][j - 1].rgbtGreen + tmp[i][j + 1].rgbtGreen + tmp[i + 1][j - 1].rgbtGreen +
                             tmp[i + 1][j].rgbtGreen + tmp[i + 1][j + 1].rgbtGreen) / 6.0;
                avg_blue = (tmp[i][j].rgbtBlue + tmp[i][j - 1].rgbtBlue + tmp[i][j + 1].rgbtBlue + tmp[i + 1][j - 1].rgbtBlue +
                            tmp[i + 1][j].rgbtBlue + tmp[i + 1][j + 1].rgbtBlue) / 6.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);
            }
            // edge case on bottom
            else if (i == (height - 1) && j > 0 && j < (width - 1))
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i][j - 1].rgbtRed + tmp[i][j + 1].rgbtRed + tmp[i - 1][j - 1].rgbtRed +
                           tmp[i - 1][j].rgbtRed + tmp[i - 1][j + 1].rgbtRed) / 6.0;
                avg_green = (tmp[i][j].rgbtGreen + tmp[i][j - 1].rgbtGreen + tmp[i][j + 1].rgbtGreen + tmp[i - 1][j - 1].rgbtGreen +
                             tmp[i - 1][j].rgbtGreen + tmp[i - 1][j + 1].rgbtGreen) / 6.0;
                avg_blue = (tmp[i][j].rgbtBlue + tmp[i][j - 1].rgbtBlue + tmp[i][j + 1].rgbtBlue + tmp[i - 1][j - 1].rgbtBlue +
                            tmp[i - 1][j].rgbtBlue + tmp[i - 1][j + 1].rgbtBlue) / 6.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);
            }
            // edge case on left
            else if (j == 0 && i > 0 && i < (width - 1))
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i - 1][j].rgbtRed + tmp[i + 1][j].rgbtRed + tmp[i - 1][j + 1].rgbtRed +
                           tmp[i][j + 1].rgbtRed + tmp[i + 1][j + 1].rgbtRed) / 6.0;
                avg_green = (tmp[i][j].rgbtGreen + tmp[i - 1][j].rgbtGreen + tmp[i + 1][j].rgbtGreen + tmp[i - 1][j + 1].rgbtGreen +
                             tmp[i][j + 1].rgbtGreen + tmp[i + 1][j + 1].rgbtGreen) / 6.0;
                avg_blue = (tmp[i][j].rgbtBlue + tmp[i - 1][j].rgbtBlue + tmp[i + 1][j].rgbtBlue + tmp[i - 1][j + 1].rgbtBlue +
                            tmp[i][j + 1].rgbtBlue + tmp[i + 1][j + 1].rgbtBlue) / 6.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);
            }
            // edge case on right
            else if (j == (width - 1) && i > 0 && i < (height - 1))
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i - 1][j].rgbtRed + tmp[i + 1][j].rgbtRed + tmp[i - 1][j - 1].rgbtRed +
                           tmp[i][j - 1].rgbtRed + tmp[i + 1][j - 1].rgbtRed) / 6.0;
                avg_green = (tmp[i][j].rgbtGreen + tmp[i - 1][j].rgbtGreen + tmp[i + 1][j].rgbtGreen + tmp[i - 1][j - 1].rgbtGreen +
                             tmp[i][j - 1].rgbtGreen + tmp[i + 1][j - 1].rgbtGreen) / 6.0;
                avg_blue = (tmp[i][j].rgbtBlue + tmp[i - 1][j].rgbtBlue + tmp[i + 1][j].rgbtBlue + tmp[i - 1][j - 1].rgbtBlue +
                            tmp[i][j - 1].rgbtBlue + tmp[i + 1][j - 1].rgbtBlue) / 6.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);
            }
            // corner top left
            else if (i == 0 && j == 0)
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i][j + 1].rgbtRed + tmp[i + 1][j].rgbtRed + tmp[i + 1][j + 1].rgbtRed) / 4.0;
                avg_green = (tmp[i][j].rgbtGreen + tmp[i][j + 1].rgbtGreen + tmp[i + 1][j].rgbtGreen + tmp[i + 1][j + 1].rgbtGreen) / 4.0;
                avg_blue = (tmp[i][j].rgbtBlue + tmp[i][j + 1].rgbtBlue + tmp[i + 1][j].rgbtBlue + tmp[i + 1][j + 1].rgbtBlue) / 4.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);
            }
            // corner top right
            else if (i == 0 && j == (width - 1))
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i][j - 1].rgbtRed + tmp[i + 1][j - 1].rgbtRed + tmp[i + 1][j].rgbtRed) / 4.0;
                avg_green = (tmp[i][j].rgbtGreen + tmp[i][j - 1].rgbtGreen + tmp[i + 1][j - 1].rgbtGreen + tmp[i + 1][j].rgbtGreen) / 4.0;
                avg_blue = (tmp[i][j].rgbtBlue + tmp[i][j - 1].rgbtBlue + tmp[i + 1][j - 1].rgbtBlue + tmp[i + 1][j].rgbtBlue) / 4.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);
            }
            // corner bottom left
            else if (i == (height - 1) && j == 0)
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i - 1][j].rgbtRed + tmp[i - 1][j + 1].rgbtRed + tmp[i][j + 1].rgbtRed) / 4.0;
                avg_green = (tmp[i][j].rgbtGreen + tmp[i - 1][j].rgbtGreen + tmp[i - 1][j + 1].rgbtGreen + tmp[i][j + 1].rgbtGreen) / 4.0;
                avg_blue = (tmp[i][j].rgbtBlue + tmp[i - 1][j].rgbtBlue + tmp[i - 1][j + 1].rgbtBlue + tmp[i][j + 1].rgbtBlue) / 4.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);
            }
            // corner bottom right
            else if (i == (height - 1) && j == (width - 1))
            {
                avg_red = (tmp[i][j].rgbtRed + tmp[i - 1][j].rgbtRed + tmp[i - 1][j - 1].rgbtRed + tmp[i][j - 1].rgbtRed) / 4.0;
                avg_green = (tmp[i][j].rgbtGreen + tmp[i - 1][j].rgbtGreen + tmp[i - 1][j - 1].rgbtGreen + tmp[i][j - 1].rgbtGreen) / 4.0;
                avg_blue = (tmp[i][j].rgbtBlue + tmp[i - 1][j].rgbtBlue + tmp[i - 1][j - 1].rgbtBlue + tmp[i][j - 1].rgbtBlue) / 4.0;

                image[i][j].rgbtRed = (int) round(avg_red);
                image[i][j].rgbtGreen = (int) round(avg_green);
                image[i][j].rgbtBlue = (int) round(avg_blue);
            }

        }

        // reset the average value of each channel
        avg_red = 0.0;
        avg_blue = 0.0;
        avg_green = 0.0;
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Edge detection algorithm from box blur algorithm to scan every neighbouring pixel within the radius 
    
    RGBTRIPLE tmp[height][width];
    
    // copy the original pixel value of image and store to tmp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }
    
    // create the Gx and Gy convolution matrix 
    int Gx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    
    int Gy[3][3] = 
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    
    // iterate through every pixel of the image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // create the coordinate arrays to store the pixel positions around the current pixel
            int row_coords[] = {row - 1, row, row + 1};
            int col_coords[] = {col - 1, col, col + 1};
            float gx_r = 0, gx_g = 0, gx_b = 0;
            float gy_r = 0, gy_g = 0, gy_b = 0;
            
            // iterate through the current pixel and every neigbouring pixel within the radius of 1 pixel
            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int cur_row = row_coords[r];
                    int cur_col = col_coords[c];
                    
                    // validate the current pixel if it is in bound
                    if (cur_row >= 0 && cur_row < height && cur_col >= 0 && cur_col < width)
                    {
                        RGBTRIPLE cur_pixel = tmp[cur_row][cur_col];
                        
                        gx_r += Gx[r][c] * cur_pixel.rgbtRed;
                        gx_g += Gx[r][c] * cur_pixel.rgbtGreen;
                        gx_b += Gx[r][c] * cur_pixel.rgbtBlue;
                        
                        gy_r += Gy[r][c] * cur_pixel.rgbtRed;
                        gy_g += Gy[r][c] * cur_pixel.rgbtGreen;
                        gy_b += Gy[r][c] * cur_pixel.rgbtBlue;
                    }
                }
            }
            
            // calculate the sobel filter value of each channel of the pixel
            // beware of the postion of the declaration of red green blue
            int red = round(sqrt(pow(gx_r, 2) + pow(gy_r, 2)));
            int green = round(sqrt(pow(gx_g, 2) + pow(gy_g, 2)));
            int blue = round(sqrt(pow(gx_b, 2) + pow(gy_b, 2)));
        
            // Cap the channel value at 255 if it is greater than 255
            if (red > 255)
            {
                red = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            
            image[row][col].rgbtRed = red;
            image[row][col].rgbtGreen = green;
            image[row][col].rgbtBlue = blue;
        }
        
    }
    
    return;
    
    /* 
    // Slow blur algorithm of 9 different cases where the pixel located in

    // create the GX and GY array
    float GX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float GY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    typedef struct
    {
        float red;
        float blue;
        float green;
    } kernel;

    kernel gx, gy;

    int red, green, blue;

    RGBTRIPLE tmp[height][width]; // create tmp array to store the original pixel value of the image

    // copy the original pixel value of image and store to tmp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // calculate the average pixel value of middle pixel
            if (i > 0 && i < (height - 1) && j > 0 && j < (width - 1))
            {
                // calculate particular value for each channel
                gx.red = tmp[i - 1][j - 1].rgbtRed * GX[0] + tmp[i - 1][j].rgbtRed * GX[1] + tmp[i - 1][j + 1].rgbtRed * GX[2] +
                         tmp[i][j - 1].rgbtRed * GX[3] + tmp[i][j].rgbtRed * GX[4] + tmp[i][j + 1].rgbtRed * GX[5] +
                         tmp[i + 1][j - 1].rgbtRed * GX[6] +  tmp[i + 1][j].rgbtRed * GX[7] + tmp[i + 1][j + 1].rgbtRed * GX[8];

                gx.green = tmp[i - 1][j - 1].rgbtGreen * GX[0] + tmp[i - 1][j].rgbtGreen * GX[1] + tmp[i - 1][j + 1].rgbtGreen * GX[2] +
                           tmp[i][j - 1].rgbtGreen * GX[3] + tmp[i][j].rgbtGreen * GX[4] + tmp[i][j + 1].rgbtGreen * GX[5] +
                           tmp[i + 1][j - 1].rgbtGreen * GX[6] +  tmp[i + 1][j].rgbtGreen * GX[7] + tmp[i + 1][j + 1].rgbtGreen * GX[8];

                gx.blue = tmp[i - 1][j - 1].rgbtBlue * GX[0] + tmp[i - 1][j].rgbtBlue * GX[1] + tmp[i - 1][j + 1].rgbtBlue * GX[2] +
                          tmp[i][j - 1].rgbtBlue * GX[3] + tmp[i][j].rgbtBlue * GX[4] + tmp[i][j + 1].rgbtBlue * GX[5] +
                          tmp[i + 1][j - 1].rgbtBlue * GX[6] +  tmp[i + 1][j].rgbtBlue * GX[7] + tmp[i + 1][j + 1].rgbtBlue * GX[8];

                gy.red = tmp[i - 1][j - 1].rgbtRed * GY[0] + tmp[i - 1][j].rgbtRed * GY[1] + tmp[i - 1][j + 1].rgbtRed * GY[2] +
                         tmp[i][j - 1].rgbtRed * GY[3] + tmp[i][j].rgbtRed * GY[4] + tmp[i][j + 1].rgbtRed * GY[5] +
                         tmp[i + 1][j - 1].rgbtRed * GY[6] +  tmp[i + 1][j].rgbtRed * GY[7] + tmp[i + 1][j + 1].rgbtRed * GY[8];

                gy.green = tmp[i - 1][j - 1].rgbtGreen * GY[0] + tmp[i - 1][j].rgbtGreen * GY[1] + tmp[i - 1][j + 1].rgbtGreen * GY[2] +
                           tmp[i][j - 1].rgbtGreen * GY[3] + tmp[i][j].rgbtGreen * GY[4] + tmp[i][j + 1].rgbtGreen * GY[5] +
                           tmp[i + 1][j - 1].rgbtGreen * GY[6] +  tmp[i + 1][j].rgbtGreen * GY[7] + tmp[i + 1][j + 1].rgbtGreen * GY[8];

                gy.blue = tmp[i - 1][j - 1].rgbtBlue * GY[0] + tmp[i - 1][j].rgbtBlue * GY[1] + tmp[i - 1][j + 1].rgbtBlue * GY[2] +
                          tmp[i][j - 1].rgbtBlue * GY[3] + tmp[i][j].rgbtBlue * GY[4] + tmp[i][j + 1].rgbtBlue * GY[5] +
                          tmp[i + 1][j - 1].rgbtBlue * GY[6] +  tmp[i + 1][j].rgbtBlue * GY[7] + tmp[i + 1][j + 1].rgbtBlue * GY[8];

                // calculate the sobel filter value
                red = round(sqrt(pow(gx.red, 2) + pow(gy.red, 2)));
                green = round(sqrt(pow(gx.green, 2) + pow(gy.green, 2)));
                blue = round(sqrt(pow(gx.blue, 2) + pow(gy.blue, 2)));

                if (red > 225)
                {
                    red = 225;
                }

                else if (green > 225)
                {
                    green = 225;
                }

                else if (blue > 225)
                {
                    blue = 225;
                }


                image[i][j].rgbtRed = red;
                image[i][j].rgbtGreen = green;
                image[i][j].rgbtBlue = blue;

            }
            // corner top left
            else if (i == 0 && j == 0)
            {
                // calculate particular value for each channel
                gx.red = tmp[i][j].rgbtRed * GX[0] + tmp[i][j + 1].rgbtRed * GX[1] + tmp[i][j + 2].rgbtRed * GX[2] +
                         tmp[i + 1][j].rgbtRed * GX[3] + tmp[i + 1][j + 1].rgbtRed * GX[4] + tmp[i + 1][j + 2].rgbtRed * GX[5] +
                         tmp[i + 2][j].rgbtRed * GX[6] + tmp[i + 2][j + 1].rgbtRed * GX[7] + tmp[i + 2][j + 2].rgbtRed * GX[8];

                gx.green = tmp[i][j].rgbtGreen * GX[0] + tmp[i][j + 1].rgbtGreen * GX[1] + tmp[i][j + 2].rgbtGreen * GX[2] +
                         tmp[i + 1][j].rgbtGreen * GX[3] + tmp[i + 1][j + 1].rgbtGreen * GX[4] + tmp[i + 1][j + 2].rgbtGreen * GX[5] +
                         tmp[i + 2][j].rgbtGreen * GX[6] + tmp[i + 2][j + 1].rgbtGreen * GX[7] + tmp[i + 2][j + 2].rgbtGreen * GX[8];

                gx.blue = tmp[i][j].rgbtBlue * GX[0] + tmp[i][j + 1].rgbtBlue * GX[1] + tmp[i][j + 2].rgbtBlue * GX[2] +
                         tmp[i + 1][j].rgbtBlue * GX[3] + tmp[i + 1][j + 1].rgbtBlue * GX[4] + tmp[i + 1][j + 2].rgbtBlue * GX[5] +
                         tmp[i + 2][j].rgbtBlue * GX[6] + tmp[i + 2][j + 1].rgbtBlue * GX[7] + tmp[i + 2][j + 2].rgbtBlue * GX[8];

                gy.red = tmp[i][j].rgbtRed * GY[0] + tmp[i][j + 1].rgbtRed * GY[1] + tmp[i][j + 2].rgbtRed * GY[2] +
                         tmp[i + 1][j].rgbtRed * GY[3] + tmp[i + 1][j + 1].rgbtRed * GY[4] + tmp[i + 1][j + 2].rgbtRed * GY[5] +
                         tmp[i + 2][j].rgbtRed * GY[6] + tmp[i + 2][j + 1].rgbtRed * GY[7] + tmp[i + 2][j + 2].rgbtRed * GY[8];

                gy.green = tmp[i][j].rgbtGreen * GY[0] + tmp[i][j + 1].rgbtGreen * GY[1] + tmp[i][j + 2].rgbtGreen * GY[2] +
                         tmp[i + 1][j].rgbtGreen * GY[3] + tmp[i + 1][j + 1].rgbtGreen * GY[4] + tmp[i + 1][j + 2].rgbtGreen * GY[5] +
                         tmp[i + 2][j].rgbtGreen * GY[6] + tmp[i + 2][j + 1].rgbtGreen * GY[7] + tmp[i + 2][j + 2].rgbtGreen * GY[8];

                gy.blue = tmp[i][j].rgbtBlue * GY[0] + tmp[i][j + 1].rgbtBlue * GY[1] + tmp[i][j + 2].rgbtBlue * GY[2] +
                         tmp[i + 1][j].rgbtBlue * GY[3] + tmp[i + 1][j + 1].rgbtBlue * GY[4] + tmp[i + 1][j + 2].rgbtBlue * GY[5] +
                         tmp[i + 2][j].rgbtBlue * GY[6] + tmp[i + 2][j + 1].rgbtBlue * GX[7] + tmp[i + 2][j + 2].rgbtBlue * GX[8];


                // calculate the sobel filter value
                red = round(sqrt(pow(gx.red, 2) + pow(gy.red, 2)));
                green = round(sqrt(pow(gx.green, 2) + pow(gy.green, 2)));
                blue = round(sqrt(pow(gx.blue, 2) + pow(gy.blue, 2)));

                if (red > 225)
                {
                    red = 225;
                }

                else if (green > 225)
                {
                    green = 225;
                }

                else if (blue > 225)
                {
                    blue = 225;
                }


                image[i][j].rgbtRed = red;
                image[i][j].rgbtGreen = green;
                image[i][j].rgbtBlue = blue;
            }




        }

    }

    */
}


