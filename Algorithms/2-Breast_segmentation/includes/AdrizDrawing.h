

/*
Autor: Holguer A Becerra.
Based on Terasic Libraries and SDL Libraries
Year 2013-2
Course: Programacion 1 UPB- Bucaramanaga
*/


#ifndef ADRIZDRAWING_H
#define ADRIZDRAWING_H

//#define ___BMP___

#ifdef ___BMP___
    #include "bmplib.h"
#else
    #include <assert.h>
    #include "SDL/SDL.h"
    #define BPP 4
    #define DEPTH 32
    typedef SDL_Surface IBMP;
#endif

#define DO_FILL 1
#define DO_NOT_FILL 0
#define TAB_SPACING 2
#define CLEAR_BACKGROUND -1
#define FONT_10PT_ROW 11
#define FONT_10PT_COLUMN 8
#define RED_CHANNEL 0
#define GREEN_CHANNEL 1
#define BLUE_CHANNEL 2
#define LUMINOSITY 3

typedef struct {
  int   vertex_x[3];
  int   vertex_y[3];
  int   spans_needed;
  int   max_span;
  int   top_y;
  int   bottom_y;
  int   col;
  int   fill;
  int   center_z;
  int   *span_array;
} triangle_struct;



struct abc_font_struct {
  unsigned long extents_width;
  unsigned long extents_height;
  unsigned long extents_ascent;
  unsigned long extents_descent;
  unsigned long bounds_width;
  unsigned long bounds_height;
  unsigned char *char_alpha_map;
  unsigned long reserved;
};


extern struct abc_font_struct tahomabold_32[];
extern struct abc_font_struct tahomabold_20[];

extern char* cour10_font;

#ifdef ___BMP___
    IBMP *  create_surface(int w, int h);
#else
    #define NUMBERS_TYPE 0
    #define STANDARD_TYPE 1
    #define FREE_TYPE 2

    SDL_Surface *  create_surface(int w, int h);
    SDL_Surface *  Show_screen(int w,int h, char * name_window);
    char Adriz_SDL_Get_Char(SDL_Event * event);
    int AdrizGet_String(char * text,int x, int y,char input_type,int backcolor,struct abc_font_struct font[],SDL_Event *event,SDL_Surface *screen);
    void Adriz_SDL_Enable_Get_Text(void);
    void Adriz_SDL_Disable_Get_Text(void);
    int SDL_Get_String(char * text,int x, int y,char input_type,int backcolor,struct abc_font_struct font[],SDL_Event * event,SDL_Surface *screen);

#endif
void set_pixel(int x, int y, int color,void *pBmp);
unsigned int get_pixel(int x, int y,void *pBmp);
void draw_line(int horiz_start, int vert_start, int horiz_end, int vert_end, int width, int color, void *pBmp);
int draw_box (int horiz_start, int vert_start, int horiz_end, int vert_end, int color, int fill, void* display);
int draw_circle(int Hcenter, int Vcenter, int radius, int color, char fill, void* display);
int draw_round_corner_box ( int horiz_start, int vert_start, int horiz_end, int vert_end,int radius, int color, int fill, void* display);
int print_char (int horiz_offset, int vert_offset, int color, char character, char *font, void* display);
int print_string(int horiz_offset, int vert_offset, int color, char *font, void* display, char string[]);
int get_size_width_text(struct abc_font_struct font[], char string[]);
int print_string_alpha(int horiz_offset, int vert_offset, int color, int background_color, struct abc_font_struct font[], void * display, char string[]);
int print_char_alpha (int horiz_offset, int vert_offset, int color, char character, int background_color, struct abc_font_struct font[], void * display);
void draw_triangle(triangle_struct* tri, void* display);
void paint_block (int Hstart,int Vstart, int Hend, int Vend, int color, void* display);
void clean_screen(void* display, int color);
void draw_bmp(void *display_global, char * filename,int x_pos, int y_pos);
void load_bmp(void *display_global, char * filename);
void image_to_grayscale(void * source, void * destiny,char channel);
void image_to_sobel( void* img, void* dst, int threshold);

void image_to_negative( void* img, void* dst);
void image_to_sepia( void* img, void* dst);
void image_to_RGB(void * source, void * destinyR,void * destinyG,void * destinyB);


//Canny
#define MAX_BRIGHTNESS 0xffffff
// C99 doesn't define M_PI (GNU-C99 does)
#define M_PI 3.14159265358979323846264338327
#define FLT_MAX     3.40282347E+38F
#define FLT_MIN     1.17549435E-38F
void convolution(unsigned int *in, unsigned int *out, const float *kernel,
                 const int nx, const int ny, const int kn,
                 const char normalize);
void gaussian_filter(unsigned int *in, unsigned int *out,
                     const int nx, const int ny, const float sigma);

void canny_edge_detection(void *source,
                          void *destiny,
                          const int tmin, const int tmax,
                          const float sigma);


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


#define HIST16 1
#define HIST8 0


int round_d(double x);

void histogram(image_features * result,IBMP * image,char type_histo);

void get_media(image_features * result,IBMP * image,char type_histo);

void extract_feactures(image_features * result,IBMP * image,char type_histo);

void normalize_image(image_features * result,IBMP * image);

IBMP * open_LJPEG( char **argv);

//Otsu segmentation
int otsu_th(void * img, void *dst, float scale);

#define AND_OP 0
#define XOR_OP 1
#define OR_OP 2

void digital_operation(void * img, void *dst, char op);
void onecolor2othercolor( void* img, int color, int color2);
void filter_image(void * img, void *dst, char horizontaly, char edge_image, int window_size, int threshold);


/*
 * Function Name: copyrecframe2other
 * Function: Copy complete frame to other, or copy only the section that you prefer
 * Developer: Holguer A Becerra Daza
 * */

int frame2frame(void *source,
               void *destiny,
               int x_source,
               int y_source,
               int width,
               int height,
               int x_destiny,
               int y_destiny,
               unsigned int alpha_color,
               int background_color,
               unsigned char alpha);


void get_size_bmp(int * width,int *height,int * bits, char * filename);
char save_bmp_file(void *display_global, char * filename);

#define ALICEBLUE_24    0xF0F8FF
#define ANTIQUEWHITE_24   0xFAEBD7
#define AQUA_24   0x00FFFF
#define AQUAMARINE_24   0x7FFFD4
#define AZURE_24    0xF0FFFF
#define BEIGE_24    0xF5F5DC
#define BISQUE_24   0xFFE4C4
#define BLACK_24    0x000000
#define BLANCHEDALMOND_24   0xFFEBCD
#define BLUE_24   0x0000FF
#define BLUEVIOLET_24   0x8A2BE2
#define BROWN_24    0xA52A2A
#define BURLYWOOD_24    0xDEB887
#define CADETBLUE_24    0x5F9EA0
#define CHARTREUSE_24   0x7FFF00
#define CHOCOLATE_24    0xD2691E
#define CORAL_24    0xFF7F50
#define CORNFLOWERBLUE_24   0x6495ED
#define CORNSILK_24   0xFFF8DC
#define CRIMSON_24    0xDC143C
#define CYAN_24   0x00FFFF
#define DARKBLUE_24   0x00008B
#define DARKCYAN_24   0x008B8B
#define DARKGOLDENROD_24    0xB8860B
#define DARKGRAY_24   0xA9A9A9
#define DARKGREY_24   0xA9A9A9
#define DARKGREEN_24    0x006400
#define DARKKHAKI_24    0xBDB76B
#define DARKMAGENTA_24    0x8B008B
#define DARKOLIVEGREEN_24   0x556B2F
#define DARKORANGE_24   0xFF8C00
#define DARKORCHID_24   0x9932CC
#define DARKRED_24    0x8B0000
#define DARKSALMON_24   0xE9967A
#define DARKSEAGREEN_24   0x8FBC8F
#define DARKSLATEBLUE_24    0x483D8B
#define DARKSLATEGRAY_24    0x2F4F4F
#define DARKSLATEGREY_24    0x2F4F4F
#define DARKTURQUOISE_24    0x00CED1
#define DARKVIOLET_24   0x9400D3
#define DEEPPINK_24   0xFF1493
#define DEEPSKYBLUE_24    0x00BFFF
#define DIMGRAY_24    0x696969
#define DIMGREY_24    0x696969
#define DODGERBLUE_24   0x1E90FF
#define FIREBRICK_24    0xB22222
#define FLORALWHITE_24    0xFFFAF0
#define FORESTGREEN_24    0x228B22
#define FUCHSIA_24  0xFF00FF
#define GAINSBORO_24    0xDCDCDC
#define GHOSTWHITE_24   0xF8F8FF
#define GOLD_24   0xFFD700
#define GOLDENROD_24    0xDAA520
#define GRAY_24   0x808080
#define GREY_24   0x808080
#define GREEN_24    0x008000
#define GREENYELLOW_24    0xADFF2F
#define HONEYDEW_24   0xF0FFF0
#define HOTPINK_24    0xFF69B4
#define INDIANRED_24    0xCD5C5C
#define INDIGO_24     0x4B0082
#define IVORY_24    0xFFFFF0
#define KHAKI_24    0xF0E68C
#define LAVENDER_24   0xE6E6FA
#define LAVENDERBLUSH_24    0xFFF0F5
#define LAWNGREEN_24    0x7CFC00
#define LEMONCHIFFON_24   0xFFFACD
#define LIGHTBLUE_24    0xADD8E6
#define LIGHTCORAL_24   0xF08080
#define LIGHTCYAN_24    0xE0FFFF
#define LIGHTGOLDENRODYELLOW_24   0xFAFAD2
#define LIGHTGRAY_24    0xD3D3D3
#define LIGHTGREY_24    0xD3D3D3
#define LIGHTGREEN_24   0x90EE90
#define LIGHTPINK_24    0xFFB6C1
#define LIGHTSALMON_24    0xFFA07A
#define LIGHTSEAGREEN_24    0x20B2AA
#define LIGHTSKYBLUE_24   0x87CEFA
#define LIGHTSLATEGRAY_24   0x778899
#define LIGHTSLATEGREY_24   0x778899
#define LIGHTSTEELBLUE_24   0xB0C4DE
#define LIGHTYELLOW_24    0xFFFFE0
#define LIME_24   0x00FF00
#define LIMEGREEN_24    0x32CD32
#define LINEN_24    0xFAF0E6
#define MAGENTA_24    0xFF00FF
#define MAROON_24   0x800000
#define MEDIUMAQUAMARINE_24   0x66CDAA
#define MEDIUMBLUE_24   0x0000CD
#define MEDIUMORCHID_24   0xBA55D3
#define MEDIUMPURPLE_24   0x9370D8
#define MEDIUMSEAGREEN_24_24    0x3CB371
#define MEDIUMSLATEBLUE_24    0x7B68EE
#define MEDIUMSPRINGGREEN_24    0x00FA9A
#define MEDIUMTURQUOISE_24    0x48D1CC
#define MEDIUMVIOLETRED_24    0xC71585
#define MIDNIGHTBLUE_24   0x191970
#define MINTCREAM_24    0xF5FFFA
#define MISTYROSE_24    0xFFE4E1
#define MOCCASIN_24   0xFFE4B5
#define NAVAJOWHITE_24    0xFFDEAD
#define NAVY_24   0x000080
#define OLDLACE_24    0xFDF5E6
#define OLIVE_24    0x808000
#define OLIVEDRAB_24    0x6B8E23
#define ORANGE_24   0xFFA500
#define ORANGERED_24    0xFF4500
#define ORCHID_24   0xDA70D6
#define PALEGOLDENROD_24    0xEEE8AA
#define PALEGREEN_24    0x98FB98
#define PALETURQUOISE_24    0xAFEEEE
#define PALEVIOLETRED_24    0xD87093
#define PAPAYAWHIP_24   0xFFEFD5
#define PEACHPUFF_24    0xFFDAB9
#define PERU_24   0xCD853F
#define PINK_24   0xFFC0CB
#define PLUM_24   0xDDA0DD
#define POWDERBLUE_24   0xB0E0E6
#define PURPLE_24   0x800080
#define RED_24    0xFF0000
#define ROSYBROWN_24    0xBC8F8F
#define ROYALBLUE_24    0x4169E1
#define SADDLEBROWN_24    0x8B4513
#define SALMON_24   0xFA8072
#define SANDYBROWN_24   0xF4A460
#define SEAGREEN_24   0x2E8B57
#define SEASHELL_24   0xFFF5EE
#define SIENNA_24   0xA0522D
#define SILVER_24   0xC0C0C0
#define SKYBLUE_24    0x87CEEB
#define SLATEBLUE_24    0x6A5ACD
#define SLATEGRAY_24    0x708090
#define SLATEGREY_24    0x708090
#define SNOW_24   0xFFFAFA
#define SPRINGGREEN_24    0x00FF7F
#define STEELBLUE_24    0x4682B4
#define TAN_24    0xD2B48C
#define TEAL_24   0x008080
#define THISTLE_24    0xD8BFD8
#define TOMATO_24   0xFF6347
#define TURQUOISE_24    0x40E0D0
#define VIOLET_24   0xEE82EE
#define WHEAT_24    0xF5DEB3
#define WHITE_24    0xFFFFFF
#define WHITESMOKE_24   0xF5F5F5
#define YELLOW_24   0xFFFF00
#define YELLOWGREEN_24    0x9ACD32


#endif // ADRIZDRAWING_H
