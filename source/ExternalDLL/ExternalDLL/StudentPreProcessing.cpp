#include "StudentPreProcessing.h"
#include "IntensityImageStudent.h"
#include "ImageFactory.h"
#include "basetimer.h"
#include <iostream>
#include <fstream>


IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	return nullptr;
}

// kernels maken en toevoegen
// waarden uit opdrachtomschrijving
int default_kernel[9*9] = { 
	 0, 0, 0,  1,  1,  1, 0, 0, 0, 
	 0, 0, 0,  1,  1,  1, 0, 0, 0, 
	 0, 0, 0,  1,  1,  1, 0, 0, 0, 
	 1, 1, 1, -4, -4, -4, 1, 1, 1, 
	 1, 1, 1, -4, -4, -4, 1, 1, 1, 
	 1, 1, 1, -4, -4, -4, 1, 1, 1, 
	 0, 0, 0,  1,  1,  1, 0, 0, 0, 
	 0, 0, 0,  1,  1,  1, 0, 0, 0, 
	 0, 0, 0,  1,  1,  1, 0, 0, 0 
};

// deze wordt gebruikt voorlopig
// komt uit de lessheets
int kernel[3 * 3] = {
	0, 1, 0,
	1,-4, 1,
	0, 1, 0
};

int kernel1[3 * 3] = {
	1, 1, 1,
	1,-8, 1,
	1, 1, 1
};

// Mexican hat
int kernel2[3 * 3] = {
	0.5, 1, 0.5,
	1 , -6, 1,
	0.5, 1, 0.5
};


IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	IntensityImage* IM = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());
	int offset = 0; // offset voor randen
	int kernelWidth = 3; // breedte van de kernel
	int blockSize = 3; 
	int imageWidth = image.getWidth();
	int pixSum = 0; // som van pixelwaarden
	int pixVal = 0;
	
	// loop door de image heen
	for (int x = offset; x < (image.getWidth() - offset); x++) {
		for (int y = offset; y < (image.getHeight() - offset); y++) {
			pixSum = 0;
			// loop door kernel heen per pixel
			for (int kx = 0; kx < kernelWidth; kx++) {
				for (int ky = 0; ky < kernelWidth; ky++) {
					pixVal = 0;
					if (kernel[ky * kernelWidth + kx] == 0) {
						continue;
					}
					for (int bx = 0; bx < blockSize; bx++) {
						for (int by = 0; by < blockSize; by++) {
							// bereken de waarde na het toepassen van de kernel
							pixVal += image.getPixel( (x - offset + bx + (kx * blockSize)) +
									(imageWidth * (y - offset + by + (ky * blockSize))));
						}
					}
					pixSum += pixVal * kernel[ky * kernelWidth + kx];
				}
			}
			// set pixelwaarde binnen itensity range
			if (pixSum < 0) {
				pixSum = 0;
			}
			else if (pixSum > 255) {
				pixSum = 255;
			}
			// bouw nieuwe intensityimage
			IM->setPixel(x + imageWidth * y, pixSum);
		}
	}
	return IM;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	IntensityImage* IM = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());
	int size = image.getWidth() * image.getHeight();
	int threshold = 220; // drempelwaarde, kan aangepast worden
	for (int i = 0; i < size; i++) {
		Intensity in = image.getPixel(i);

		if (in > threshold) {
			IM->setPixel(i, 0);
		}
		else {
			IM->setPixel(i, 255);
		}
	}
	return IM;
}