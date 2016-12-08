
#include "includes/AdrizDrawing.h"


IBMP * image1;
IBMP * imageR;
IBMP * imageG;
IBMP * imageB;
IBMP * image_gray_scale;
IBMP * image_sobel;
IBMP * image_negative;
IBMP * image_sepia;


int main(int argc, char **argv)
{
    char file_str[100];
    char c[100];
    int width, height;
    long lSize;
    unsigned short * buffer;
    size_t result;
    sprintf(file_str,"%s","Imagenes/B_3084_1.RIGHT_CC.LJPEG.txt");

    FILE *fptr;

    if ((fptr = fopen(file_str, "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // reads text until newline
    fscanf(fptr,"%[^\n]", c);

    sscanf(c,"%d\t%d",&width,&height);

    fclose(fptr);


    sprintf(file_str,"%s","Imagenes/B_3084_1.RIGHT_CC.LJPEG.1");

    if ((fptr = fopen(file_str, "rb")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // allocate memory to contain the whole file:
    buffer = (unsigned short *) malloc (sizeof(unsigned short )*width*height);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
    printf("%d\n",width*height);
    // copy the file into the buffer:
    result = fread (buffer,2,width*height,fptr);
    if (result != width*height) {fputs ("Reading error",stderr); exit (3);}

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose (fptr);


    image1=create_surface(width,height);
    image_sobel=create_surface(width,height);
    int _i,_j;
    unsigned char R,G,B=0;
    unsigned int pixel;
    unsigned short pixel2;
    for(_i=0; _i<height; _i++){

        for(_j=0; _j<width; _j++){
            pixel2=(short)buffer[_i*(width)+_j];
            R=((pixel2 & 0xff))& 0xff;
            G=R;
            B=R;
            pixel=(R<<16) | (G<<8)|(B<<0);
            set_pixel(_j,_i,pixel,image1);
        }

    }

    save_bmp_file(image1,"Results/test.bmp");

    image_to_sobel(image1,image_sobel);
    save_bmp_file(image_sobel,"Results/test2.bmp");

    free (buffer);


    //    char file_str[100];
    //    if(argc>=1 && argv[1]!=NULL){
    //        sprintf(file_str,"%s",argv[1]);
    //    }
    //    else{
    //        sprintf(file_str,"%s","Imagenes/tifa.bmp");
    //    }

    //    int width, height,bits;

    //    /*Se crean las imagenes temporarles*/
    //    get_size_bmp(&width,&height,&bits, file_str);
    //    image1=create_surface(width,height);
    //    imageR=create_surface(width,height);
    //    imageG=create_surface(width,height);
    //    imageB=create_surface(width,height);
    //    image_gray_scale=create_surface(width,height);
    //    image_sobel=create_surface(width,height);
    //    image_negative=create_surface(width,height);
    //    image_sepia=create_surface(width,height);
    //    load_bmp(image1,file_str);


    //    /*se divide la imagen en su componene RGB*/
    //image_to_RGB(image1, imageR,imageG,imageB);
    //    /*se convierte la imagen a escala de grises*/
    //    image_to_grayscale(image1,image_gray_scale,LUMINOSITY);
    //    /*se convierte la imagen a bordes*/
    //    image_to_sobel(image1,image_sobel);
    //    /*se convierte la imagen a su negativo*/
    //    image_to_negative(image1,image_negative);
    //    /*se convierte la imagen a color sepia*/
    //    image_to_sepia(image1,image_sepia);

    //    /*guarda los resultados en diferentes archivos*/

    //    save_bmp_file(imageR,"Results/R_bayonetta.bmp");
    //    save_bmp_file(imageG,"Results/G_bayonetta.bmp");
    //    save_bmp_file(imageB,"Results/B_bayonetta.bmp");
    //    save_bmp_file(image_gray_scale,"Results/gra_bayonetta.bmp");
    //    save_bmp_file(image_sobel,"Results/sobel_bayonetta.bmp");
    //    save_bmp_file(image_negative,"Results/negative_bayonetta.bmp");
    //    save_bmp_file(image_sepia,"Results/sepia_bayonetta.bmp");


    return 0;
}


