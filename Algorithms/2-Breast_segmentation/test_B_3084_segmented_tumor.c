
#include "includes/AdrizDrawing.h"
typedef struct {
    double * histogram;
    int size;
} histograma;


typedef struct {
    int max;
    int min;
    int average;
    int media;
    histograma histo;
} image_features;



IBMP * image1;
IBMP * imageR;
IBMP * imageG;
IBMP * imageG;
IBMP * imageB;
IBMP * image_gray_scale;
IBMP * image_sobel;
IBMP * image_canny;
IBMP * image_negative;
IBMP * image_sepia;
IBMP * image1_thresholded;

double histo16bits[65536];
double histo8bits[256];

#define HIST16 1
#define HIST8 0

int round_d(double x)
{
    float val=( x >= 0.0) ? (x + 0.5) : (x - (int)(x-1) + 0.5) + (int)(x-1);
    return (int)val;
}


void histogram(image_features * result,IBMP * image,char type_histo){

    int x,y;

    int value;

    if(type_histo){
        result->histo.histogram=&histo16bits[0];
        result->histo.size=65536;
    }
    else{
        result->histo.histogram=&histo8bits[0];
        result->histo.size=256;
    }


    for(y=0;y< result->histo.size;y++){
        result->histo.histogram[y]=0;
    }

    for(y=0;y<image->h;y++){
        for(x=0;x<image->w;x++){
            value=get_pixel(x,y,image) ;
            result->histo.histogram[value]=histo16bits[value]+0.00001;
        }
    }

}

void get_media(image_features * result,IBMP * image,char type_histo){
    int y,x;
    int value;
    double media=0.0;
    double dividedby=0;

    histogram(result,image,type_histo);

    for(y=0;y<image->h;y++){
        for(x=0;x<image->w;x++){
            value=get_pixel(x,y,image) ;
             dividedby+=histo16bits[value];
        }
    }
    printf("%f\n",dividedby);

    for(y=0;y<image->h;y++){
        for(x=0;x<image->w;x++){
            value=get_pixel(x,y,image) ;
             media+=histo16bits[value]*value/dividedby;
        }
    }
    printf("%f\n",media);


    result->media=round_d(media);


}

void extract_feactures(image_features * result,IBMP * image,char type_histo){

    int x,y;
    int value;

    result->min=0;
    result->max=0;
    result->average=0;
    for(y=0;y<image->h;y++){
        for(x=0;x<image->w;x++){
            value=get_pixel(x,y,image) ;
            if(value<=result->min){
                result->min=value;
            }
            if(value>=result->max){
                result->max=value;
            }
            result->average=result->average+value;
            result->average=result->average>>1;
        }
    }

    get_media(result,image,type_histo);
}


void normalize_image(image_features * result,IBMP * image){

    int x,y;
    int value;

    float dividedby=0;
    dividedby=result->max-result->media;
    printf("min=%d max=%d divided=%d average=%d media=%d ",result->min,result->max,(int)dividedby,result->average,result->media);

    unsigned char intensity=0;
    unsigned int pixel_2_show;
    float pixel_f;
    unsigned int pixel_I;


    for(y=0;y<image->h;y++){
        for(x=0;x<image->w;x++){
            value=get_pixel(x,y,image) ;

            pixel_f=(float)value/(dividedby-1);
            pixel_f=pixel_f*255;
            pixel_I=pixel_f;
            intensity=pixel_I;
            pixel_2_show=0;
            pixel_2_show=intensity<<16 | intensity<<8 | intensity;
            set_pixel(x,y,pixel_2_show,image) ;
        }
    }
}


int main(int argc, char **argv)
{
    char file_str[100];
    char c[100];
    int width, height;
    long lSize;
    short * buffer;
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
    unsigned short pixel;
    unsigned int pix;
    unsigned char half1;
    unsigned char half2;
    for(_i=0; _i<height; _i++){
        for(_j=0; _j<width; _j++){
            half2=buffer[_i*(width)+_j] & 0xff;
            half1=(buffer[_i*(width)+_j]>>8) & 0xff;
            pixel=(half2<<8) | half1;

//            pixel_f=(float)pixel/4095;
//            pixel_f=pixel_f*255;
//            pixel_I=(unsigned int)pixel_f;
//            intensity=pixel_I;
//            pixel_2_show=intensity<<16 | intensity<<8 | intensity;

//            if(intensity>90){
//                set_pixel(_j,_i,0xffffff,image1_thresholded);
//            }
            set_pixel(_j,_i,pixel,image1);
        }

    }

    image_features imagen_f;
    extract_feactures(&imagen_f,image1,HIST16);
    save_bmp_file(image1,"Results/test_not_normalized.bmp");
    normalize_image(&imagen_f,image1);
    save_bmp_file(image1,"Results/test_normalized.bmp");



    save_bmp_file(image1,"Results/test.bmp");
    save_bmp_file(image1_thresholded,"Results/test_thresholded.bmp");
//    canny_edge_detection(image1,image_canny, 45, 50, 2.0f);
//    save_bmp_file(image_canny,"Results/test_c.bmp");

//    image_to_negative(image1,image_negative);
//    image_to_sobel(image1_thresholded,image_sobel,1);
//    save_bmp_file(image_sobel,"Results/test_e.bmp");
//    save_bmp_file(image_negative,"Results/test_n.bmp");
//    image_to_sobel(image_negative,image_sobel,1);
//    save_bmp_file(image_sobel,"Results/test_e2.bmp");



//    for(_i=0; _i<height; _i++){
//        for(_j=0; _j<width; _j++){
//            pixel=buffer[_i*(width)+_j]& 0xffff;

//            intensity=pixel>>8;
//            pixel_2_show=intensity<<16 | intensity<<8 | intensity;
//            if(_i==0){
//                printf(" %0.1f, %d\n",pixel_f,pixel_2_show);
//            }
//            set_pixel(_j,_i,intensity<<16 | intensity<<8 | intensity,image1);
//        }

//    }



//    save_bmp_file(image1,"Results/test2.bmp");
//    image_to_negative(image1,image_negative);
//    image_to_sobel(image1,image_sobel,1);
//    save_bmp_file(image_sobel,"Results/test2_e.bmp");
//    save_bmp_file(image_negative,"Results/test2_n.bmp");
//    image_to_sobel(image_negative,image_sobel,1);
//    save_bmp_file(image_sobel,"Results/test2_e2.bmp");


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


