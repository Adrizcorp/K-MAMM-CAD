
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




IBMP * image_analize;


double histo16bits[65536];
double histo8bits[256];

char command[2000];

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
    unsigned int nbits;
    float bits=log(result->max)/log(2);
    nbits=round_d(bits);
    float dividedby=0;
    if(nbits<16){
        dividedby=result->max;
    }
    else {
        dividedby=result->max-(result->average+result->media);
    }
    printf("min=%d max=%d divided=%d average=%d media=%d bits=%d",result->min,result->max,(int)dividedby,result->average,result->media,nbits);

    unsigned char intensity=0;
    unsigned int pixel_2_show;
    float pixel_f;
    unsigned int pixel_I;


    for(y=0;y<image->h;y++){
        for(x=0;x<image->w;x++){
            value=get_pixel(x,y,image) ;

            pixel_f=(float)value/(dividedby);
            pixel_f=pixel_f*255;
            pixel_I=pixel_f;
            intensity=pixel_I;
            pixel_2_show=0;
            pixel_2_show=intensity<<16 | intensity<<8 | intensity;
            set_pixel(x,y,pixel_2_show,image) ;
        }
    }
}



IBMP * open_LJPEG( char **argv){
    char file_str[1000];
    char command[1000];
    char c[100];
    int width, height;


    short * buffer;

    IBMP * image1;


    size_t result;
    sprintf(command,"Ljpeg.exe -d -s %s",argv[1]);
    system(command);

    sprintf(file_str,"%s.txt",argv[1]);

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
    printf("%d,%d",width, height);
    int temp=width;
    if(width>height){

        width=height;
        height=temp;
    }

    fclose(fptr);

    sprintf(file_str,"%s.1",argv[1]);

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
    do{
    result = fread (buffer,2,width*height,fptr);
    printf("%d\n",result);
    }while(result!=0);


    fclose (fptr);


    sprintf(command,"del /Q %s.1",argv[1]);
    system(command);
    sprintf(command,"del /Q %s.txt",argv[1]);
    system(command);

    image1=create_surface(width,height);


    int _i,_j;
    unsigned short pixel;
    unsigned char half1;
    unsigned char half2;
    for(_i=0; _i<height; _i++){
        for(_j=0; _j<width; _j++){
            half2=buffer[_i*(width)+_j] & 0xff;
            half1=(buffer[_i*(width)+_j]>>8) & 0xff;
            pixel=(half2<<8) | half1;
            set_pixel(_j,_i,pixel,image1);
        }

    }

    image_features imagen_f;
    extract_feactures(&imagen_f,image1,HIST16);
    normalize_image(&imagen_f,image1);




    free (buffer);

    return image1;
}


int main(int argc, char **argv)
{
    char file_str_output1[2000];
    char file_str_output2[2000];

    if(argc<2){
        fputs ("Too few arguments, read the README",stderr); exit (2);

    }

    sprintf(file_str_output1,"%s.bmp",argv[2]);
    sprintf(file_str_output2,"%s.jpeg",argv[2]);

    //
    image_analize=open_LJPEG( argv);
    save_bmp_file(image_analize,file_str_output1);
    sprintf(command,"bmp2jpeg.exe %s %s 70",file_str_output1,file_str_output2);
    system(command);

    return 0;
}


