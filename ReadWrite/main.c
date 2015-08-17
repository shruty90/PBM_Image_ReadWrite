/*
Reading and Writing PBM files in C( without the use of any external libraries/packages)
Author: Shruty Janakiraman
e-mail: shrutyorama@gmail.com
Input: binary image 


*/




#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mainheader.h"


int width = 0;
int height = 0;

int main(int argc, char *argv[])
{
	char *input = NULL;
	char *output = NULL;
	
	unsigned char **InputImg = NULL;
	if (argc == 3)
	{
		input = argv[1];
		output = argv[2];
	}
	else
	fprintf(stdout, "\n invalid number of arguments");		
	InputImg = ReadPMB(input);
	if (InputImg==NULL)
	{
		fprintf(stderr, "\n error reading file");
		exit(1);
	}	

	WritePMB(InputImg, output, width, height);
	for (int i = 0; i < height; i++)
	{
		free((void*)InputImg[i]);
	}
	free(InputImg);
	return(0);
}

unsigned char**ReadPMB(char *input)
{		
	int ch = 0;
	int flag = 0;
	int num = 0;
	int get_height = 0; 
	FILE *fp = NULL;
	fp = fopen(input, "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "\n error reading file");
		exit(1);
	}
	 //parse the header to extract the width and height of the image
	    ch = getc(fp);	
		if (ch != 'P'&&ch != EOF)
			fprintf(stderr, "\n not a PBM file");		
		ch = getc(fp);
		if (ch != '4'&&ch != EOF)
			fprintf(stderr, "\n not a PBM file");
		while (ch != EOF&& flag!=2)
		{
			ch = getc(fp);
			if (isspace(ch))
			{
				while (ch != '\n')
				{
					ch = getc(fp);
					if (isdigit(ch))
					{
						ungetc(ch, fp);
						if (flag == 0)
						{
							fscanf(fp, "%d", &width);
							flag++;
							continue;
						}
						if (flag == 1)
						{							
							fscanf(fp, "%d", &height);
							flag++;
							break;
						}
					}//end of isdigit
				}
				continue;
			}

			if (ch == '#')
			{
				while (ch != '\n')
				{
					ch = getc(fp);
				}
				continue;
			}
				ungetc(ch, fp);
				fscanf(fp, "%d", &width);
				flag++;
				continue;		
			
		}//end of while		
		while ((ch = getc(fp)) != '\n' && ch!=EOF);
	//********************************************************************************************************************************//
		ch = getc(fp);
		ungetc(ch, fp);
		//allocate memory for image array
	unsigned char **Image = (unsigned char**)malloc(height*sizeof(unsigned char*));	
	for (int i = 0; i < height; i++)
	{
		Image[i] = (unsigned char*)malloc(width*sizeof(unsigned char*));
	}
	if (Image == NULL)
	{
		fprintf(stderr, "\n memory allocation failure");
		exit(1);
	}	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Image[i][j] = 0;
		}
	}	
	
	while (!feof(fp))
	{
		for (int i = 0; i < height; i++)
		{
			num = fread(Image[i], sizeof(unsigned char), width, fp);
			if (ferror(fp))			
			{
				perror("read error");
				break;
			}
		}
		
	}  			
	fclose(fp);	
	return(Image);

}

void WritePMB(unsigned char**Output, char *output,int width, int height)
{
	FILE* f = NULL;
	int num1 = 0;
	//check for pointer validity
	if (output == NULL)
		fprintf(stderr, "\n invalid file output name");
	//open the output file in write mode
	f = fopen(output, "wb");
	if (f==NULL)
		fprintf(stderr, "\n cannot open file ");
	//print the width and height
	fprintf(f, "P4\n%d %d\n",width,height);		
	for (int i = 0; i < height; i++)
	{
		num1=fwrite(Output[i], sizeof(unsigned char), width, f);
		if (ferror(f))
		{
			perror("write error");
			break;
		}
	}	
	fclose(f);
	
}