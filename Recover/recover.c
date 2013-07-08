/**
 * Filename: recover.c
 * Author: Thomas Frei
 * Created 07.07.2013
 * 
 * Description: Recovers Deleted JPG's From a memory card
 * Link: http://nifty.stanford.edu/2010/malan-csi/
 */

#include <stdio.h>

/**
 * Function 
 * Name: makeFile
 * Description: Generates a new JPG File Dynamically
 */
FILE *makeFile(int name){

	// Generate name of new file
	char image_name[8];
	sprintf(image_name, "%03i.jpg",name);
			
	// Open new File
	FILE *output = fopen(image_name, "w");

	// Error ?
	if (output == NULL){
		printf("Could not open %s for Writing\n", image_name);
		return NULL;
	}

	return output;
}

/**
 * Function
 * Name: Main
 * Description: Recovers Deleted JPG's From 'card.raw'
 */
int main(void){

	FILE *rawFile;						// The File To Read
	FILE *output = NULL;				// The Recovered Image
	int fileOpen = 0;					// Flag
	int counter = 0;					// File Counter
	unsigned char buffer[512];			// Read Buffer

	// Open The File
	rawFile = fopen("card.raw", "r");

	// Error ?
	if (rawFile == NULL){
		printf("Could not open file for Reading\n");
		return 1;
	}
	
	// Loop Until EOF
	while(!feof(rawFile)){
		
		// Read 2 Bytes
		fread(buffer, sizeof(char), 512, rawFile);
		
		// 0xFF and 0xD8 are the JPEG SOI Markers
		// see https://en.wikipedia.org/wiki/JPEG
		if (buffer[0] == 0xff && buffer[1] == 0xd8){
			
			counter++;

			// Check if a file is already open
			if (fileOpen == 1){
				fclose(output);
			}

			// Generate new File
			output = makeFile(counter);

			// Write Data 
			fwrite(buffer, sizeof(char),512, output);
			
			// Set Flag
			fileOpen = 1;

			// Found end of file
		} else if (fileOpen == 1 && buffer[0] == 0xFF && buffer[1] == 0xD9){
			
			// Write last 2 Bytes		
			fwrite(buffer, sizeof(char),512, output);
			fclose(output);

			// Set Flag
			fileOpen = 0;
		} else {
			if (fileOpen == 1){
				fwrite(buffer, sizeof(char),512, output);
			}
		}
	}

	// Close Open Files
	fclose(rawFile);

	printf("%d images recovered\n",counter);
		
	return 0;
}