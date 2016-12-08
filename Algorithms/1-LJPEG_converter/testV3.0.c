
#include "includes/AdrizDrawing.h"


IBMP * image1;
IBMP * imageR;
IBMP * imageG;
IBMP * imageB;
IBMP * image_gray_scale;
IBMP * image_sobel;
IBMP * image_canny;
IBMP * image_negative;
IBMP * image_sepia;
IBMP * image1_thresholded;


int main(int argc, char **argv)
{
    char file_str[100];
    char c[100];
    int width, height;
    long lSize;
    unsigned short * buffer;
    size_t result;
    sprintf(file_str,"Imagenes/%s.txt",argv[1]);

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

    sprintf(file_str,"Imagenes/%s.1",argv[1]);

    if ((fptr = fopen(file_str, "rb")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // allocate memory to contain the whole file:
    buffer = (unsigned short *) malloc (sizeof(unsigned short )*width*height);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer,2,width*height,fptr);
    if (result != width*height) {fputs ("Reading error",stderr); exit (3);}

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose (fptr);


    image1=create_surface(width,height);
    image1_thresholded=create_surface(width,height);
    image_sobel=create_surface(width,height);
    image_negative=create_surface(width,height);
    image_canny=create_surface(width,height);


    int _i,_j;
    unsigned char intensity=0;
    unsigned short pixel;
    unsigned int pixel_2_show;
    float pixel_f;
    unsigned int pixel_I;
    unsigned char half1;
    unsigned char half2;
    for(_i=0; _i<height; _i++){
        for(_j=0; _j<width; _j++){
            half2=buffer[_i*(width)+_j] & 0xff;
            half1=(buffer[_i*(width)+_j]>>8) & 0xff;
            pixel=(half2<<8) | half1;
            pixel_f=(float)pixel/4095;
            pixel_f=pixel_f*255;
            pixel_I=(unsigned int)pixel_f;
            intensity=pixel_I;
            pixel_2_show=intensity<<16 | intensity<<8 | intensity;

            if(intensity>90){
                set_pixel(_j,_i,0xffffff,image1_thresholded);
            }
            set_pixel(_j,_i,pixel_2_show,image1);
        }

    }



    save_bmp_file(image1,"Results/test.bmp");
    save_bmp_file(image1_thresholded,"Results/test_thresholded.bmp");
//    canny_edge_detection(image1,image_canny, 45, 50, 2.0f);
//    save_bmp_file(image_canny,"Results/test_c.bmp");

    image_to_negative(image1,image_negative);
    image_to_sobel(image1_thresholded,image_sobel,1);
    save_bmp_file(image_sobel,"Results/test_e.bmp");
    save_bmp_file(image_negative,"Results/test_n.bmp");
    image_to_sobel(image_negative,image_sobel,1);
    save_bmp_file(image_sobel,"Results/test_e2.bmp");



    for(_i=0; _i<height; _i++){
        for(_j=0; _j<width; _j++){
            pixel=buffer[_i*(width)+_j]& 0xffff;

            intensity=pixel>>8;
            pixel_2_show=intensity<<16 | intensity<<8 | intensity;
            if(_i==0){
                printf(" %0.1f, %d\n",pixel_f,pixel_2_show);
            }
            set_pixel(_j,_i,intensity<<16 | intensity<<8 | intensity,image1);
        }

    }



    save_bmp_file(image1,"Results/test2.bmp");
    image_to_negative(image1,image_negative);
    image_to_sobel(image1,image_sobel,1);
    save_bmp_file(image_sobel,"Results/test2_e.bmp");
    save_bmp_file(image_negative,"Results/test2_n.bmp");
    image_to_sobel(image_negative,image_sobel,1);
    save_bmp_file(image_sobel,"Results/test2_e2.bmp");


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
    //
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



