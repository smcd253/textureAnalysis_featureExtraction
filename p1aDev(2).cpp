#include "p1.hpp"

using namespace std;

// input: ./p1aDev HW4_images/texture1.raw

/*************************************************************************************************************************************************/
// FUNCTION DESCRIPTION: reflects the requested value over the x axis if it is beyond image boundaries (effectively neighbor padding)
// RETURNS: the desired or reflected pixel location
// PARAMETER (int width): width of image
// PARAMETER (int i): y value of pixel
// PARAMETER (int nh): the requested y delta
/*************************************************************************************************************************************************/
int reflectY(int height, int i, int nh)
{
	if(i + nh >= 0)
	{
		if (i + nh < height - 1)
			return i + nh;
		else
			return i - nh;
	}
	else
		return i - nh;
}

/*************************************************************************************************************************************************/
// FUNCTION DESCRIPTION: reflects the requested value over the y axis if it is beyond image boundaries (effectively neighbor padding)
// RETURNS: the desired or reflected pixel location
// PARAMETER (int width): width of image
// PARAMETER (int j): x value of pixel
// PARAMETER (int nw): the requested x delta
/*************************************************************************************************************************************************/
int reflectX(int width, int j, int nw)
{
	if(j + nw >= 0)
	{
		if (j + nw < width - 1)
			return j + nw;
		else
			return j - nw;
	}
	else
		return j - nw;
}

/*************************************************************************************************************************************************/
// FUNCTION DESCRIPTION: takes in image minus mean and calculates the energy sum for each filter in the filter bank, dividing by the image size to
// return the feature vector for fitler k
// RETURNS: energySum, the sum of all pixels convolved with each filter 
// PARAMETER (int **image_minusMean): the de illuminated image data built from source image data minus the mean value of all pixels
// PARAMETER (int height): height of image
// PARAMETER (int width): width of image
// PARAMETER (int k): filter number
/*************************************************************************************************************************************************/
float buildFeatureVector(int **image_minusMean, int height, int width, int k)
{
	int filteredImage[width][height];
	int energySum = 0;
	
	// loop over entire image
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			// loop through filter variations
			for (int m = 0; m < 5; m++)
			{	
				for (int n = 0; n < 5; n++)
				{
					filteredImage[i][j] += filter_bank[k][m][n] * image_minusMean[reflectX(width, i, m)][reflectY(height, j, n)];
				}
			}
			energySum += filteredImage[i][j]*filteredImage[i][j];
		}
	}
	return energySum / (height * width);
}

int main(int argc, char *argv[])
{
	FILE *file;
	int height = 128;
	int width = 128;
	
	unsigned char sourceImageData[width][height];


    // read in file data
    if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(sourceImageData, sizeof(unsigned char), height*width, file);
	fclose(file);


    // add together pixel values to find average pixel value for input image
    float featureVector[12][25]={0.0};
	float featureVector_6[6][25]={0.0};
	float kmean_center_old[4][25]={0.0};
	float kmean_center_new[4][25]={0.0};

	float featureVector3d[12][3]={	
								{3.6739,0.0096,-0.0744},{2.1889,0.1305,-0.0493},{-0.8910,0.0961,-0.0247},
								{-5.5780,0.0307,0.0327},{-5.4875,0.0396,0.0309},{-5.0620,0.0236,0.0387},
								{8.0285,-0.0666,-0.0382},{8.0473,-0.0295,0.0482},{6.9885,0.0106,0.0887},
								{-3.9062,-0.0844,-0.0171},{-4.1417,-0.0782,-0.0113},{-3.8605,-0.0822,-0.0242}
								};
								
	float kmean_center_old3d[4][3]={0.0};
	float kmean_center_new3d[4][3]={0.0};
	
	
	float energy_sum=0;
	

	// throw image into int array
	int image_minusMean[width][height];

	for(int i=0; i<width; i++)
		for(int j=0; j<height; j++)
			image_minusMean[i][j] = (int)sourceImageData[i][j];

	// subtract image mean to reduce illumination effects
	float pixelSum = 0;
	int imageMean = 0;

	for(int i=0; i<width; i++)
		for(int j=0; j<height; j++)
			pixelSum += (float)sourceImageData[i][j];

	// mean
    imageMean = (int)(pixelSum/(height*width));

	// subtract mean from each pixel
	for(int i=0; i<width+4; i++)
		for(int j=0; j<height+4; j++)
			image_minusMean[i][j] -= imageMean;

	// build the feature vector of input image for each filter
	for(int k=0; k<25; k++)
	{
		featureVector[0][k] = buildFeatureVector((int **)image_minusMean, height, width, k);
	}

    for(int j=0; j<25; j++)
			printf("featureVector[0][%d] = %f\n", j, featureVector[0][j]);
    

    /****************************************normal feature vector******************************************/
	// float featureVector_max[25] = {0};
	// for(int j=0; j<25; j++)
	// {
	// 	featureVector_max[j] = featureVector[0][j];
	// 	for(int i=1; i<12; i++)
	// 	{
	// 		if(featureVector_max[j]<featureVector[i][j])
	// 			featureVector_max[j] = featureVector[i][j];
	// 	}
	// }

	// for(int i=0; i<12; i++)
	// {
	// 	for(int j=0; j<25; j++)
	// 		featureVector[i][j] = featureVector[i][j]/featureVector_max[j];
	// }

/*
	for(int i=0; i<12; i++)
	{
		for(int j=0; j<25; j++)
			cout << featureVector[i][j] << ",";
		cout << endl;
	}
	cout << endl;*/
	return 0;
}