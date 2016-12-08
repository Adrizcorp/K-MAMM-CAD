
#include "includes/AdrizDrawing.h"


IBMP * image_analize;
IBMP * image_analize2;
IBMP * image_analize3;
IBMP * image_analize_edge;


char command[2000];

int main(int argc, char **argv)
{
    char file_str_output1[2000];
    char file_str_output2[2000];
    char file_str_output3[2000];
    int type;
    if(argc<3){
        fputs ("Too few arguments, read the README",stderr); exit (2);

    }

    sprintf(file_str_output1,"%s",argv[1]);
    sprintf(file_str_output2,"%s.bmp",argv[2]);
    sprintf(file_str_output3,"%s.jpeg",argv[2]);
    sscanf(argv[3],"%d",&type);
    if(type>1 || type<0){
        fputs ("Type Does not exist",stderr); exit (2);
    }

    //open the image

    int width, height,bits;

    /*Se crean las imagenes temporarles*/
    get_size_bmp(&width,&height,&bits, file_str_output1);
    image_analize3= create_surface(width,height);
    load_bmp(image_analize3, file_str_output1);
    image_analize= create_surface(width>>1,height>>1);
    resizeimage(image_analize3,image_analize);

    //save_bmp_file(image_analize,file_str_output2);

    image_analize2= create_surface(image_analize->w,image_analize->h);
    image_analize_edge= create_surface(image_analize->w,image_analize->h);
    //Apply Otsu Segmentation at 50%
    otsu_th(image_analize,image_analize2,0.5);
    //Filter Image horizontaly
    filter_image(image_analize2,image_analize2,1,0,100,240);
    //Filter Image Verticaly
    filter_image(image_analize2,image_analize2,0,0,100,240);
    //Apply Otsu Segmentation at 50%
    //Apply Borders sobel with threshold of 15
    image_to_sobel(image_analize2,image_analize_edge, 15);
    //Filter Image horizontaly
    filter_image(image_analize_edge,image_analize_edge,1,0,2,30);
    //Filter Image Verticaly
    filter_image(image_analize_edge,image_analize_edge,0,0,2,230);
    onecolor2othercolor( image_analize_edge, WHITE_24, RED_24);
    digital_operation(image_analize, image_analize2,AND_OP);
    digital_operation(image_analize_edge, image_analize2,OR_OP);
    digital_operation(image_analize, image_analize_edge,OR_OP);


    if(type){
        save_bmp_file(image_analize2,file_str_output2);
    }
    else{
        save_bmp_file(image_analize_edge,file_str_output2);
    }

    sprintf(command,"bmp2jpeg.exe %s %s 70",file_str_output2,file_str_output3);
    system(command);
//    sprintf(command,"del /Q %s",file_str_output2);
//    system(command);




    return 0;
}


