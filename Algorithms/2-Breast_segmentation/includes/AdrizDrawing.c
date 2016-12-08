
/*
Autor: Holguer A Becerra.
Based on Terasic Libraries
Year 2013-2
Course: Programacion 1 UPB- Bucaramanaga
*/

#include "AdrizDrawing.h"

#ifdef ___BMP___
#else
#include <string.h>
#endif


void set_pixel(int x, int y, int color,void *pBmp){
#ifdef ___BMP___
    IBMP * frame= (IBMP*)pBmp;
    if(x>=frame->width || x<0){
        return;
    }
    if(y>=frame->height || y<0){
        return;
    }
    BmpIO_SetR(x,frame->height-y,frame,(color>>16)&0xff);
    BmpIO_SetG(x,frame->height-y,frame,(color>>8)&0xff);
    BmpIO_SetB(x,frame->height-y,frame,(color)&0xff);
#else
    SDL_Surface * frame= (SDL_Surface*)pBmp;
    if(x>=frame->w || x<0){
        return;
    }
    if(y>=frame->h || y<0){
        return;
    }
    *((Uint32*) frame->pixels + y*frame->pitch/BPP + x) = color;
#endif


}

unsigned int get_pixel(int x, int y,void *pBmp){

#ifdef ___BMP___
    IBMP * frame= (IBMP*)pBmp;
    int  red, blue, green;
    if(x>=frame->width || x<0){
        return 0;
    }
    if(y>=frame->height || y<0){
        return 0;
    }
    red=BmpIO_GetR( x, frame->height-y, frame );
    green=BmpIO_GetG( x, frame->height-y, frame );
    blue=BmpIO_GetB( x, frame->height-y, frame );
    return (red<<16 | green<<8 | blue);
#else
    SDL_Surface * frame= (SDL_Surface*)pBmp;
    if(x>=frame->w || x<0){
        return 0;
    }
    if(y>=frame->h || y<0){
        return 0;
    }
    Uint32 *pixels = (Uint32 *)frame->pixels;
    return pixels[ ( y * frame->w ) + x ];
#endif


}


void clean_screen(void* display, int color){
#ifdef ___BMP___
    IBMP * frame= (IBMP*)display;
    paint_block(0,0,frame->width,frame->height,color,display);
#else
    SDL_Surface * frame= (SDL_Surface*)display;
    SDL_FillRect(frame,NULL, color);
#endif

}

void vid_draw_sloped_line( unsigned short horiz_start,
                           unsigned short vert_start,
                           unsigned short horiz_end,
                           unsigned short vert_end,
                           unsigned short width,
                           int color,
                           void* display)
{
    // Find the vertical and horizontal distance between the two points
    int horiz_delta = abs(horiz_end-horiz_start);
    int vert_delta = abs(vert_end-vert_start);

    // Find out what direction we are going
    int horiz_incr, vert_incr;
    if (horiz_start > horiz_end) { horiz_incr=-1; } else { horiz_incr=1; }
    if (vert_start > vert_end) { vert_incr=-1; } else { vert_incr=1; }

    // Find out which axis is always incremented when drawing the line
    // If it's the horizontal axis
    if (horiz_delta >= vert_delta) {
        int dPr   = vert_delta<<1;
        int dPru  = dPr - (horiz_delta<<1);
        int P     = dPr - horiz_delta;

        // Process the line, one horizontal point at at time
        for (; horiz_delta >= 0; horiz_delta--) {
            // plot the pixel
            set_pixel(horiz_start, vert_start, color, display);
            // If we're moving both up and right
            if (P > 0) {
                horiz_start+=horiz_incr;
                vert_start+=vert_incr;
                P+=dPru;
            } else {
                horiz_start+=horiz_incr;
                P+=dPr;
            }
        }
        // If it's the vertical axis
    } else {
        int dPr   = horiz_delta<<1;
        int dPru  = dPr - (vert_delta<<1);
        int P     = dPr - vert_delta;

        // Process the line, one vertical point at at time
        for (; vert_delta>=0; vert_delta--) {
            // plot the pixel
            set_pixel(horiz_start, vert_start, color, display);
            // If we're moving both up and right
            if (P > 0) {
                horiz_start+=horiz_incr;
                vert_start+=vert_incr;
                P+=dPru;
            } else {
                vert_start+=vert_incr;
                P+=dPr;
            }
        }
    }
}

void vid_draw_horiz_line (short Hstart, short Hend, int V, int color, void* display)
{
    int count=0;
    while(Hend>=Hstart){
        set_pixel(Hstart+count, V, color, display);
        Hend--;
        count++;
    }

}


void draw_line(int horiz_start, int vert_start, int horiz_end, int vert_end, int width, int color, void *display){
    if( vert_start == vert_end )
    {
        vid_draw_horiz_line( (unsigned short)horiz_start,
                             (unsigned short)horiz_end,
                             (unsigned short)vert_start,
                             color,
                             display );
    }
    else
    {
        vid_draw_sloped_line( (unsigned short)horiz_start,
                              (unsigned short)vert_start,
                              (unsigned short)horiz_end,
                              (unsigned short)vert_end,
                              (unsigned short)width,
                              color,
                              display );
    }

}



void paint_block (int Hstart,int Vstart, int Hend, int Vend, int color, void* display)
{

    int x,y;

    for(y=Vstart; y<Vend;y++){
        for(x=Hstart; x<Hend;x++){
            set_pixel(x,y, color,display);
        }
    }
}


int draw_box (int horiz_start, int vert_start, int horiz_end, int vert_end, int color, int fill, void* display){

    // If we want to fill in our box
    if (fill) {
        paint_block (horiz_start, vert_start, horiz_end, vert_end, color, display);
        // If we're not filling in the box, just draw four lines.
    } else {
        draw_line(horiz_start, vert_start, horiz_start, vert_end-1, 1, color, display);
        draw_line(horiz_end-1, vert_start, horiz_end-1, vert_end-1, 1, color, display);
        draw_line(horiz_start, vert_start, horiz_end-1, vert_start, 1, color, display);
        draw_line(horiz_start, vert_end-1, horiz_end-1, vert_end-1, 1, color, display);
    }

    return (0);


}

void vid_round_corner_points( int cx, int cy, int x, int y,
                              int straight_width, int straight_height, int color,
                              char fill, void* display)
{

    // If we're directly above, below, left and right of center (0 degrees), plot those 4 pixels
    if (x == 0) {
        // bottom
        set_pixel(cx, cy + y + straight_height, color, display);
        set_pixel(cx + straight_width, cy + y + straight_height, color, display);
        // top
        set_pixel(cx, cy - y, color, display);
        set_pixel(cx + straight_width, cy - y, color, display);

        if(fill) {
            draw_line(cx - y, cy, cx + y + straight_width, cy, 1, color, display);
            draw_line(cx - y, cy + straight_height, cx + y + straight_width, cy + straight_height, 1, color, display);
        } else {
            //right
            set_pixel(cx + y + straight_width, cy, color, display);
            set_pixel(cx + y + straight_width, cy + straight_height, color, display);
            //left
            set_pixel(cx - y, cy, color, display);
            set_pixel(cx - y, cy + straight_height, color, display);
        }

    } else
        // If we've reached the 45 degree points (x=y), plot those 4 pixels
        if (x == y) {
            if(fill) {
                draw_line(cx - x, cy + y + straight_height, cx + x + straight_width, cy + y + straight_height, 1, color, display); // lower
                draw_line(cx - x, cy - y, cx + x + straight_width, cy - y, 1, color, display); // upper

            } else {
                set_pixel(cx + x + straight_width, cy + y + straight_height, color, display); // bottom right
                set_pixel(cx - x, cy + y + straight_height, color, display); // bottom left
                set_pixel(cx + x + straight_width, cy - y, color, display); // top right
                set_pixel(cx - x, cy - y, color, display); // top left
            }
        } else
            // If we're between 0 and 45 degrees plot 8 pixels.
            if (x < y) {
                if(fill) {
                    draw_line(cx - x, cy + y + straight_height, cx + x + straight_width, cy + y + straight_height, 1, color, display);
                    draw_line(cx - y, cy + x + straight_height, cx + y + straight_width, cy + x + straight_height, 1, color, display);
                    draw_line(cx - y, cy - x, cx + y + straight_width, cy - x, 1, color, display);
                    draw_line(cx - x, cy - y, cx + x + straight_width, cy - y, 1, color, display);
                } else {
                    set_pixel(cx + x + straight_width, cy + y + straight_height, color, display);
                    set_pixel(cx - x, cy + y + straight_height, color, display);
                    set_pixel(cx + x + straight_width, cy - y, color, display);
                    set_pixel(cx - x, cy - y, color, display);
                    set_pixel(cx + y + straight_width, cy + x + straight_height, color, display);
                    set_pixel(cx - y, cy + x + straight_height, color, display);
                    set_pixel(cx + y + straight_width, cy - x, color, display);
                    set_pixel(cx - y, cy - x, color, display);
                }
            }
}

int draw_circle(int Hcenter, int Vcenter, int radius, int color, char fill, void* display){
    int x = 0;
    int y = radius;
    int p = (5 - radius*4)/4;

    // Start the circle with the top, bottom, left, and right pixels.
    vid_round_corner_points(Hcenter, Vcenter, x, y, 0, 0, color, fill, display);

    // Now start moving out from those points until the lines meet
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2*x+1;
        } else {
            y--;
            p += 2*(x-y)+1;
        }
        vid_round_corner_points(Hcenter, Vcenter, x, y, 0, 0, color, fill, display);
    }
    return (0);

}



int draw_round_corner_box ( int horiz_start, int vert_start, int horiz_end, int vert_end,
                            int radius, int color, int fill, void* display){

    unsigned int x, y;
    int p;
    int diameter;
    int temp;
    unsigned int width, height, straight_width, straight_height;

    // Make sure the start point us up and left of the end point
    if( horiz_start > horiz_end )
    {
        temp = horiz_end;
        horiz_end = horiz_start;
        horiz_start = temp;
    }

    if( vert_start > vert_end )
    {
        temp = vert_end;
        vert_end = vert_start;
        vert_start = temp;
    }

    // These are the overall dimensions of the box
    width = horiz_end - horiz_start;
    height = vert_end - vert_start;

    // Make sure our radius isnt more than the shortest dimension
    // of the box, or it'll screw us all up
    if( radius > ( width / 2 ))
        radius = width / 2;

    if( radius > ( height / 2 ))
        radius = height / 2;

    // We use the diameter for some calculations, so we'll pre calculate it here.
    diameter = ( radius * 2 );

    // These are the lengths of the straight portions of the box edges.
    straight_width = width - diameter;
    straight_height = height - diameter;

    x = 0;
    y = radius;
    p = (5 - radius*4)/4;

    // Start the corners with the top, bottom, left, and right pixels.
    vid_round_corner_points( horiz_start + radius, vert_start + radius, x, y,
                             straight_width, straight_height, color, fill, display );

    // Now start moving out from those points until the lines meet
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2*x+1;
        } else {
            y--;
            p += 2*(x-y)+1;
        }
        vid_round_corner_points( horiz_start + radius, vert_start + radius, x, y,
                                 straight_width, straight_height, color, fill, display);
    }

    // If we want to fill in our box
    if (fill) {
        paint_block (horiz_start, vert_start + radius, horiz_end, vert_end - radius, color, display);
        // If we're not filling in the box, just draw four lines.
    } else {
        draw_line(horiz_start, vert_start + radius, horiz_start, vert_end - radius , 1, color, display); //left
        draw_line(horiz_end, vert_start + radius, horiz_end, vert_end - radius , 1, color, display); //right
        draw_line(horiz_start + radius, vert_start, horiz_end - radius , vert_start, 1, color, display); //top
        draw_line(horiz_start + radius, vert_end, horiz_end - radius , vert_end, 1, color, display); //bottom
    }

    return (0);

}
/******************************************************************
*  Function: min3
*
*  Purpose:  Returns the minimum value of 3 parameters
*            Used for drawing filled shapes
*
******************************************************************/
__inline__ int max3( int a, int b, int c )
{
    if( a < b )
        a = b;
    if( a < c )
        a = c;

    return a;
}

/******************************************************************
*  Function: min3
*
*  Purpose:  Returns the minimum value of 3 parameters
*            Used for drawing filled shapes.
*
******************************************************************/
__inline__ int min3( int a, int b, int c )
{
    if( a > b )
        a = b;
    if( a > c )
        a = c;

    return a;
}

/******************************************************************
*  Function: max_diff3
*
*  Purpose:  Returns the positive max difference between 3
*            parameters.  Used for drawing filled shapes
*
******************************************************************/
__inline__ int max_diff3(int a, int b, int c)
{
    int max, min;

    max = max3( a, b, c );
    min = min3( a, b, c );
    return (max - min);
}


inline void vid_put_pixel_in_span_map( int x, int y, int *span_array )
{
    if (span_array[y*2] == -1)
    {
        span_array[y*2] = x;
        span_array[(y*2)+1] = x;
    }
    else if( span_array[y*2] > x )
        span_array[y*2] = x;
    else if( span_array[(y*2)+1] < x )
        span_array[(y*2)+1] = x;
}

void vid_bres_scan_edges( int x1, int y1, int x2, int y2, int *span_array)
{

    int x_incr, y_incr;
    int y_delta, x_delta;

    // Assure we always draw left to right
    if( x1 > x2 )
    {
        int tempx = x2;
        x2 = x1;
        x1 = tempx;
        int tempy = y2;
        y2 = y1;
        y1 = tempy;
    }

    // Find the vertical and horizontal distance between the two points
    y_delta = abs(y1-y2);
    x_delta = (x2-x1);

    // Find out what direction we are going
    if (y1 > y2) { y_incr=-1; } else { y_incr=1; }
    x_incr=1;

    // Find out which axis is always incremented when drawing the line
    // If it's the horizontal axis
    if (x_delta >= y_delta) {
        int dPr   = y_delta<<1;
        int dPru  = dPr - (x_delta<<1);
        int P     = dPr - x_delta;

        // Process the line, one horizontal point at at time
        for (; x_delta >= 0; x_delta--) {
            // map the pixel
            vid_put_pixel_in_span_map(x1, y1, span_array);
            // If we're moving along both axis
            if (P > 0) {
                x1+=x_incr;
                y1+=y_incr;
                P+=dPru;
            } else {
                x1+=x_incr;
                P+=dPr;
            }
        }
    }
    else // If it's the vertical axis
    {
        int dPr   = x_delta<<1;
        int dPru  = dPr - (y_delta<<1);
        int P     = dPr - y_delta;

        // Process the line, one vertical point at at time
        for (; y_delta>=0; y_delta--) {
            // plot the pixel
            vid_put_pixel_in_span_map(x1, y1, span_array);
            // If we're moving along both axis
            if (P > 0) {
                x1+=x_incr;
                y1+=y_incr;
                P+=dPru;
            } else {
                y1+=y_incr;
                P+=dPr;
            }
        }
    }
}


void draw_triangle(triangle_struct* tri, void* display){
#ifdef ___BMP___
    IBMP * frame= (IBMP*)display;
    int w,h;
    w=frame->width;
    h=frame->height;
#else
    SDL_Surface * frame= (SDL_Surface*)display;
    int w,h;
    w=frame->w;
    h=frame->h;
#endif

    int i;

    // Outline it first
    draw_line( tri->vertex_x[0], tri->vertex_y[0],
            tri->vertex_x[1], tri->vertex_y[1], 1,
            tri->col, display);
    draw_line( tri->vertex_x[1], tri->vertex_y[1],
            tri->vertex_x[2], tri->vertex_y[2], 1,
            tri->col, display);
    draw_line( tri->vertex_x[2], tri->vertex_y[2],
            tri->vertex_x[0], tri->vertex_y[0], 1,
            tri->col, display);
    //  vid_draw_line(tri->bx, tri->by, tri->cx, tri->cy, 1, tri->col, display);
    //  vid_draw_line(tri->cx, tri->cy, tri->ax, tri->ay, 1, tri->col, display);

    if(tri->fill == DO_FILL)
    {
        tri->top_y = min3(tri->vertex_y[0], tri->vertex_y[1], tri->vertex_y[2]);
        tri->bottom_y = max3(tri->vertex_y[0], tri->vertex_y[1], tri->vertex_y[2]);
        tri->spans_needed = max_diff3(tri->vertex_y[0], tri->vertex_y[1], tri->vertex_y[2]);
        tri->max_span = max_diff3(tri->vertex_x[0], tri->vertex_x[1], tri->vertex_x[2]);
        tri->span_array = malloc(h * 4 * 2);

        //init the span array
        for( i = tri->top_y; i <= tri->bottom_y; i++)
        {
            tri->span_array[i*2] = -1;
            tri->span_array[(i*2) + 1] = -1;
        }

        // Scan-convert the triangle
        vid_bres_scan_edges( tri->vertex_x[0], tri->vertex_y[0],
                tri->vertex_x[1], tri->vertex_y[1], tri->span_array);
        vid_bres_scan_edges( tri->vertex_x[1], tri->vertex_y[1],
                tri->vertex_x[2], tri->vertex_y[2], tri->span_array);
        vid_bres_scan_edges( tri->vertex_x[2], tri->vertex_y[2],
                tri->vertex_x[0], tri->vertex_y[0], tri->span_array);

        // Render the polygon
        for( i = tri->top_y; i <= tri->bottom_y; i++ )
        {
            //      vid_draw_horiz_line (tri->span_array[i*2], tri->span_array[(i*2)+1], i, tri->col, display);
            draw_line (tri->span_array[i*2], i, tri->span_array[(i*2)+1], i, 1, tri->col, display);

        }
        free(tri->span_array);
    }
}



int print_char (int horiz_offset, int vert_offset, int color, char character, char *font, void* display){
    int i, j;

    char temp_char, char_row;

    // Convert the ASCII value to an array offset
    temp_char = (character - 0x20);

    //Each character is 8 pixels wide and 11 tall.
    for(i = 0; i < 11; i++) {
        char_row = *(font + (temp_char * FONT_10PT_ROW) + i);
        for (j = 0; j < 8; j++) {
            //If the font table says the pixel in this location is on for this character, then set it.
            if (char_row & (((unsigned char)0x80) >> j)) {
                set_pixel((horiz_offset + j), (vert_offset + i), color, display); // plot the pixel
            }
        }
    }
    return(0);

}

int print_string(int horiz_offset, int vert_offset, int color, char *font, void* display, char string[]){

    int i = 0;
    int original_horiz_offset;

    original_horiz_offset = horiz_offset;

    // Print until we hit the '\0' char.
    while (string[i]) {
        //Handle newline char here.
        if (string[i] == '\n') {
            horiz_offset = original_horiz_offset;
            vert_offset += 12;
            i++;
            continue;
        }
        // Lay down that character and increment our offsets.
        print_char (horiz_offset, vert_offset, color, string[i], font, display);
        i++;
        horiz_offset += 8;
    }
    return (0);

}



__inline__ int seperate_color_channels(int color_depth, unsigned char * color, unsigned char * red, unsigned char * green, unsigned char * blue)
{
    int return_value;  // error checking in case anyone is listening
    unsigned short temp_color = 0;

    if (color_depth == 16)  // R(5):G(6):B(5)
    {
        temp_color = *color++;        // get the first half of the 16 bit color
        temp_color += (*color << 8);  // get the second half of the 16 bit color
        *blue = temp_color & 0x1F;
        *green = (temp_color >> 5) & 0x3F;
        *red = (temp_color >> 11) & 0x1F;
        return_value = 1;
    }
    else if ((color_depth == 24) || (color_depth == 32))  // R(8):G(8):B(8) packed/unpacked
    {
        *blue = *color++;
        *green = *color++;
        *red = *color;
        return_value = 1;
    }
    else  // not a valid color depth for this function
    {
        *red = 0;
        *green = 0;
        *blue = 0;
        return_value = 0;
    }
    return return_value;
}


__inline__ int merge_color_channels(int color_depth, unsigned char red, unsigned char green, unsigned char blue, unsigned char * color)
{
    int return_value;  // error checking in case anyone is listening
    unsigned short temp_color;

    if (color_depth == 16)  // R(5):G(6):B(5)
    {
        temp_color = (blue & 0x1F) | ((green & 0x3F) << 5) | ((red & 0x1F) << 11);  // 16 bit color format
        *color++ = temp_color & 0xFF;
        *color = (temp_color >> 8) & 0xFF;
        return_value = 1;
    }
    else if ((color_depth == 24) || (color_depth == 32))  // R(8):G(8):B(8) packed/unpacked
    {
        *color++ = blue;
        *color++ = green;
        *color = red;
        return_value = 1;
    }
    else  // not a valid color depth for this function
    {
        *color++ = 0;
        *color++ = 0;
        *color = 0;
        return_value = 0;
    }
    return return_value;
}

__inline__ int read_from_frame (int horiz, int vert, unsigned char *red, unsigned char *green, unsigned char *blue, void * display)
{
#ifdef ___BMP___
    IBMP * frame= (IBMP*)display;
    *red=BmpIO_GetR(horiz,frame->height-vert,frame);
    *green=BmpIO_GetG(horiz,frame->height-vert,frame);
    *blue=BmpIO_GetB(horiz,frame->height-vert,frame);
#else
    SDL_Surface * frame= (SDL_Surface*)display;
    unsigned int color =get_pixel(horiz,vert,frame);
    *red=color>>16 & 0xff;
    *green=color>>8 & 0xff;
    *blue=color & 0xff;
#endif

    return 1;
}


__inline__ int alpha_blending (int horiz_offset, int vert_offset, int background_color, unsigned char alpha, unsigned char *red, unsigned char *green, unsigned char *blue, void * display)
{
    int return_value;
    unsigned char background_red, background_green, background_blue;
    unsigned int blended_red, blended_green, blended_blue;  // short since there may be overflow to be clipped

    // set red, green, and blue of the background color
    if(background_color == CLEAR_BACKGROUND)
    {
        read_from_frame (horiz_offset, vert_offset, &background_red, &background_green, &background_blue, display);
    }
    else
    {
        seperate_color_channels(24, (unsigned char *)&background_color, &background_red, &background_green, &background_blue);
    }

    // these blended colors may need to be clipped to the maximum amounts the color depth supports
    blended_red = (((*red) * alpha) + (background_red * (255 - alpha)))/255;
    blended_green = (((*green) * alpha) + (background_green * (255 - alpha)))/255;
    blended_blue = (((*blue) * alpha) + (background_blue * (255 - alpha)))/255;


    *red = (blended_red > 0xFF)? 0xFF: blended_red;
    *green = (blended_green > 0xFF)? 0xFF: blended_green;
    *blue = (blended_blue > 0xFF)? 0xFF: blended_blue;
    return_value = 1;

    return return_value;
}





int print_char_alpha (int horiz_offset, int vert_offset, int color, char character, int background_color, struct abc_font_struct font[], void * display){
    int i, j;
    unsigned char * alpha;
    unsigned char original_red, original_blue, original_green;
    unsigned char red, green, blue;
    int new_color;

    // Assign the pointer of the font bitmap
    alpha = font[character-33].char_alpha_map;

    // set red, green, and blue of the font color
    seperate_color_channels(24, (char *)&color, &original_red, &original_green, &original_blue);

    for(i = 0; i < font[character-33].bounds_height; i++) {
        for (j = 0; j < font[character-33].bounds_width; j++) {
            red = original_red;
            green = original_green;
            blue = original_blue;

            // send in the font alpha channel and the colour channels for the blending to occur
            alpha_blending ((horiz_offset + j), (vert_offset + i), background_color, *alpha, &red, &green, &blue, display);

            // take the blended pixel and merge the color channels back into a single pixel value
            merge_color_channels(24, red, green, blue, (char *)&new_color);

            // write the new pixel value out to memory overwriting the current background
            set_pixel((horiz_offset + j), (vert_offset + i), new_color, display);

            alpha++;
        }
    }

    return(0);

}

int get_size_width_text(struct abc_font_struct font[], char string[]){
    int i = 0;
    int tab;
    int horiz_offset=0;


    // Print until we hit the '\0' char.
    while (string[i]) {

        // Lay down that character and increment our offsets.
        if(string[i] == 32)  // this is a space in ASCII
        {
            horiz_offset += font[45 - 33].bounds_width;  // using the width of the '-' character for the space width since it's not present in the font
        }
        else if(string[i] == '\t')  // this is a tab
        {
            for( tab = 0; tab < TAB_SPACING; tab++ )
            {
                horiz_offset += font[45 - 33].bounds_width;  // using the width of the '-' character for the space width since it's not present in the font
            }
        }
        else
        {
            horiz_offset += font[string[i] - 33].bounds_width;
        }
        i++;
    }
    return (horiz_offset);

}

int print_string_alpha(int horiz_offset, int vert_offset, int color, int background_color, struct abc_font_struct font[], void * display, char string[]){

    int i = 0;
    int tab;
    int original_horiz_offset;

    original_horiz_offset = horiz_offset;

    // Print until we hit the '\0' char.
    while (string[i]) {
        //Handle newline char here.
        if (string[i] == '\n') {
            horiz_offset = original_horiz_offset;
            vert_offset += font['|' - 33].bounds_height;  // we'll use "|" to add the line to line spacing
            i++;
            continue;
        }
        // Lay down that character and increment our offsets.
        if(string[i] == 32)  // this is a space in ASCII
        {
            if(background_color != CLEAR_BACKGROUND)  // need to fill in this spot (setting foreground to background color so that it looks like a drawn in box)
            {
                print_char_alpha (horiz_offset, vert_offset, background_color, '-', background_color, font, display);
            }
            horiz_offset += font[45 - 33].bounds_width;  // using the width of the '-' character for the space width since it's not present in the font
        }
        else if(string[i] == '\t')  // this is a tab
        {
            for( tab = 0; tab < TAB_SPACING; tab++ )
            {
                if(background_color != CLEAR_BACKGROUND)  // need to fill in this spot (setting foreground to background color so that it looks like a drawn in box)
                {
                    print_char_alpha (horiz_offset, vert_offset, background_color, '-', background_color, font, display);
                }
                horiz_offset += font[45 - 33].bounds_width;  // using the width of the '-' character for the space width since it's not present in the font
            }
        }
        else
        {
            print_char_alpha (horiz_offset, vert_offset, color, string[i], background_color, font, display);
            horiz_offset += font[string[i] - 33].bounds_width;
        }
        i++;
    }
    return (0);


}




void draw_bmp(void *display_global, char * filename,int x_pos, int y_pos){

#ifdef ___BMP___
    IBMP * frame= (IBMP*)display_global;

    char pBmp;
    // Reading BMP

    int width, height, dwidth, dheight, x, y;
    int dx = 0;
    int dy = 0;
    FILE * infile;
    char * cp_pic_name = (char*)malloc(100);
    sprintf(cp_pic_name,"%s",filename);
    infile = fopen(cp_pic_name,"rb");
    if (infile == NULL){

        printf("open file[%s] failed!\n",filename);

        return;
    }



    pBmp = LoadHeader(infile, &dwidth, &dheight,&width,&height);


    if ( NULL == pBmp )
    {

        return;
    }


    dx = x_pos;
    dy  = y_pos;


    LoadBody24_2_framereader( infile,dx, dy, width, height, frame);


    // se cierra el archivo :)
    fclose(infile);
#else
    SDL_Surface * frame= (SDL_Surface*)display_global;
    SDL_Surface * image = SDL_LoadBMP(filename);
    if (image == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }
    SDL_Rect src, dest;

    src.x = 0;
    src.y = 0;
    src.w = image->w;
    src.h = image->h;

    dest.x = x_pos;
    dest.y = y_pos;
    dest.w = image->w;
    dest.h = image->h;
    SDL_BlitSurface(image, &src, frame, &dest);

    SDL_FreeSurface(image);
#endif



}


void load_bmp(void *display_global, char * filename){
#ifdef ___BMP___
    IBMP * frame= (IBMP*)display_global;
    char pBmp;
    // Reading BMP

    int width, height, dwidth, dheight, x, y;
    int dx = 0;
    int dy = 0;
    FILE * infile;
    char * cp_pic_name = (char*)malloc(100);
    sprintf(cp_pic_name,"%s",filename);
    infile = fopen(cp_pic_name,"rb");
    if (infile == NULL){

        printf("open file[%s] failed!\n",filename);

        return;
    }



    pBmp = LoadHeader(infile, &dwidth, &dheight,&width,&height);

    if ( NULL == pBmp )
    {

        return;
    }



    dx = 0;
    dy = 0;


    LoadBody24_2_framereader( infile,dx, dy, width, height, frame);


    // se cierra el archivo :)
    fclose(infile);
#else
    SDL_Surface * frame= (SDL_Surface*)display_global;
    SDL_Surface * image;
    image = SDL_LoadBMP(filename);
    if (image == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }
    SDL_Rect src, dest;

    src.x = 0;
    src.y = 0;
    src.w = image->w;
    src.h = image->h;

    dest.x = 0;
    dest.y = 0;
    dest.w = image->w;
    dest.h = image->h;
    SDL_BlitSurface(image, &src, frame, &dest);

    SDL_FreeSurface(image);
#endif



}


/*you can transform to grayscale using the next options: channel(RED_CHANNEL, GREEN_CHANNEL, BLUE_CHANNEL and LUMINOSITY)*/
void image_to_grayscale(void * source, void * destiny,char channel){
#ifdef ___BMP___
    IBMP * frame_source= (IBMP*)source;
    IBMP * frame_destiny= (IBMP*)destiny;
#else
    SDL_Surface * frame_source= (SDL_Surface*)source;
    SDL_Surface * frame_destiny= (SDL_Surface*)destiny;
#endif


    int x,y;
    int color,new_color;
    int lumi;

    int w,h;
#ifdef ___BMP___
    h= frame_source->height;
    w= frame_source->width;
#else
    w= frame_source->w;
    h= frame_source->h;
#endif


    for(y=0;y<h;y++){
        for(x=0;x<w;x++){
            color=get_pixel(x,y,frame_source);

            switch(channel){
            case RED_CHANNEL:
                lumi=(color>>16) & 0xff;
                break;
            case GREEN_CHANNEL:
                lumi=(color>>8) & 0xff;
                break;
            case BLUE_CHANNEL:
                lumi=color & 0xff;
                break;
            case LUMINOSITY:
                lumi=(int)((float)0.299*((color>>16) & 0xff) + (float)0.587*((color>>8) & 0xff) +(float)0.114*((color) & 0xff));
                break;
            default: break;
            }
            merge_color_channels(24,lumi,lumi,lumi,&new_color);
            set_pixel(x,y,new_color,frame_destiny);
        }
    }

}

void image_to_sepia( void* img, void* dst){
    int x,y;
    int color;
    int new_color;

    image_to_grayscale(img, dst,LUMINOSITY);

#ifdef ___BMP___
    IBMP * frame_source= (IBMP*)img;
#else
    SDL_Surface * frame_source= (SDL_Surface*)img;
#endif

    int w,h;
#ifdef ___BMP___
    h= frame_source->height;
    w= frame_source->width;
#else
    w= frame_source->w;
    h= frame_source->h;
#endif

    for(y=0;y<h;y++){
        for(x=0;x<w;x++){
            color=get_pixel(x,y,dst);
            unsigned char new_r = ((color>>16)&0xff)*1;
            unsigned char new_g= ((color>>8)&0xff)*0.95;
            unsigned char new_b=((color)&0xff)*0.82;
            merge_color_channels(24,new_r,new_g,new_b,&new_color);
            set_pixel(x,y,new_color,dst);
        }
    }

}

void image_to_negative( void* img, void* dst){
    int x,y;
    int color;
#ifdef ___BMP___
    IBMP * frame_source= (IBMP*)img;
#else
    SDL_Surface * frame_source= (SDL_Surface*)img;
#endif

    int w,h;
#ifdef ___BMP___
    h= frame_source->height;
    w= frame_source->width;
#else
    w= frame_source->w;
    h= frame_source->h;
#endif

    for(y=0;y<h;y++){
        for(x=0;x<w;x++){
            color=get_pixel(x,y,img);
            set_pixel(x,y,~color,dst);
        }
    }
}




void image_to_sobel( void* img, void* dst, int threshold)
{
#ifdef ___BMP___
    IBMP * frame_source= (IBMP*)img;
#else
    SDL_Surface * frame_source= (SDL_Surface*)img;
#endif

    int w,h;
#ifdef ___BMP___
    h= frame_source->height;
    w= frame_source->width;
#else
    w= frame_source->w;
    h= frame_source->h;
#endif
    // iterators
    unsigned int x, y;
    for(x = 1; x < w - 1; x++){
        for(y = 1; y <h - 1; y++){
            // initialise Gx and Gy to 0
            int Gx = 0;
            int Gy = 0;
            unsigned int intensity = 0;
            int pixel;

            // Left column
            pixel = get_pixel(x-1,y-1,img);
            intensity = ((pixel>>16)&0xff) + ((pixel>>8)&0xff) + ((pixel)&0xff);
            Gx += -intensity;
            Gy += -intensity;

            pixel = get_pixel(x-1,y,img);
            intensity = ((pixel>>16)&0xff) + ((pixel>>8)&0xff) + ((pixel)&0xff);
            Gx += -2 * intensity;

            pixel = get_pixel(x-1,y+1,img);
            intensity = ((pixel>>16)&0xff) + ((pixel>>8)&0xff) + ((pixel)&0xff);
            Gx += -intensity;
            Gy += +intensity;

            // middle column
            pixel = get_pixel(x,y-1,img);
            intensity = ((pixel>>16)&0xff) + ((pixel>>8)&0xff) + ((pixel)&0xff);
            Gy += -2 * intensity;


            pixel = get_pixel(x,y+1,img);
            intensity = ((pixel>>16)&0xff) + ((pixel>>8)&0xff) + ((pixel)&0xff);
            Gy += +2 * intensity;

            // right column
            pixel = get_pixel(x+1,y-1,img);
            intensity = ((pixel>>16)&0xff) + ((pixel>>8)&0xff) + ((pixel)&0xff);
            Gx += +intensity;
            Gy += -intensity;

            pixel = get_pixel(x+1,y,img);
            intensity = ((pixel>>16)&0xff) + ((pixel>>8)&0xff) + ((pixel)&0xff);
            Gx += +2 * intensity;

            pixel = get_pixel(x+1,y+1,img);
            intensity = ((pixel>>16)&0xff) + ((pixel>>8)&0xff) + ((pixel)&0xff);
            Gx += +intensity;
            Gy += +intensity;

            // calculate the gradient length
            unsigned int length = (unsigned int)sqrt( (float)(Gx * Gx) + (float)(Gy * Gy) );
            int new_color;

            // normalise the length to 0 to 255
            length = length / threshold;

            // draw the pixel on the edge image
            merge_color_channels(24,length,length,length,&new_color);

            set_pixel(x,y,new_color,dst);
        }
    }
}


// if normalize is true, map pixels to range 0..MAX_BRIGHTNESS
void convolution(unsigned int *in, unsigned int *out, const float *kernel,
                 const int nx, const int ny, const int kn,
                 const char normalize)
{

    assert(kn % 2 == 1);
    assert(nx > kn && ny > kn);
    const int khalf = kn / 2;
    float min = FLT_MAX, max = -FLT_MAX;
    int m,n,j,i;
    if (normalize)
        for ( m = khalf; m < nx - khalf; m++)
            for ( n = khalf; n < ny - khalf; n++) {
                float pixel = 0.0;
                size_t c = 0;
                for ( j = -khalf; j <= khalf; j++)
                    for ( i = -khalf; i <= khalf; i++) {
                        pixel += in[(n - j) * nx + m - i] * kernel[c];
                        c++;
                    }
                if (pixel < min)
                    min = pixel;
                if (pixel > max)
                    max = pixel;
            }

    for ( m = khalf; m < nx - khalf; m++)
        for ( n = khalf; n < ny - khalf; n++) {
            float pixel = 0.0;
            size_t c = 0;
            for ( j = -khalf; j <= khalf; j++)
                for ( i = -khalf; i <= khalf; i++) {
                    pixel += in[(n - j) * nx + m - i] * kernel[c];
                    c++;
                }

            if (normalize)
                pixel = MAX_BRIGHTNESS * (pixel - min) / (max - min);
            out[n * nx + m] = (unsigned int )pixel;
        }
}



/*
 * gaussianFilter:
 * http://www.songho.ca/dsp/cannyedge/cannyedge.html
 * determine size of kernel (odd #)
 * 0.0 <= sigma < 0.5 : 3
 * 0.5 <= sigma < 1.0 : 5
 * 1.0 <= sigma < 1.5 : 7
 * 1.5 <= sigma < 2.0 : 9
 * 2.0 <= sigma < 2.5 : 11
 * 2.5 <= sigma < 3.0 : 13 ...
 * kernelSize = 2 * int(2*sigma) + 3;
 */
void gaussian_filter(unsigned int *in, unsigned int *out,
                     const int nx, const int ny, const float sigma)
{
    int i,j;
    const int n = 2 * (int)(2 * sigma) + 3;
    const float mean = (float)floor(n / 2.0);
    float kernel[n * n]; // variable length array

    fprintf(stderr, "gaussian_filter: kernel size %d, sigma=%g\n",
            n, sigma);
    size_t c = 0;
    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++) {
            kernel[c] = exp(-0.5 * (pow((i - mean) / sigma, 2.0) +
                                    pow((j - mean) / sigma, 2.0)))
                    / (2 * M_PI * sigma * sigma);
            c++;
        }

    convolution(in, out, kernel, nx, ny, n, 1);
}



/*
 * Links:
 * http://en.wikipedia.org/wiki/Canny_edge_detector
 * http://www.tomgibara.com/computer-vision/CannyEdgeDetector.java
 * http://fourier.eng.hmc.edu/e161/lectures/canny/node1.html
 * http://www.songho.ca/dsp/cannyedge/cannyedge.html
 *
 * Note: T1 and T2 are lower and upper thresholds.
 */
void canny_edge_detection(void *source,
                          void *destiny,
                          const int tmin, const int tmax,
                          const float sigma)
{

#ifdef ___BMP___
    IBMP * frame_source= (IBMP*)source;
#else
    SDL_Surface * frame_source= (SDL_Surface*)source;
#endif

    int w,h;
#ifdef ___BMP___
        IBMP * frame= (IBMP*)pBmp;
    h= frame_source->height;
    w= frame_source->width;
#else
    w= frame_source->w;
    h= frame_source->h;
    SDL_Surface * frame_in= (SDL_Surface*)source;
    Uint32 *pixels_in = (Uint32 *)frame_in->pixels;


#endif




    const int nx = w;
    const int ny = h;
    int j, i;
    unsigned int *G = calloc(nx * ny , sizeof(unsigned int));
    unsigned int *after_Gx = calloc(nx * ny , sizeof(unsigned int));
    unsigned int *after_Gy = calloc(nx * ny, sizeof(unsigned int));
    unsigned int *nms = calloc(nx * ny, sizeof(unsigned int));
    unsigned int *out = malloc(nx*ny * sizeof(unsigned int));

    if (G == NULL || after_Gx == NULL || after_Gy == NULL ||
            nms == NULL || out == NULL) {
        fprintf(stderr, "canny_edge_detection:"
                " Failed memory allocation(s).\n");
        exit(1);
    }

    gaussian_filter(pixels_in, out, nx, ny, sigma);

    const float Gx[] = {-1, 0, 1,
                        -2, 0, 2,
                        -1, 0, 1};

    convolution(out, after_Gx, Gx, nx, ny, 3, 0);

    const float Gy[] = { 1, 2, 1,
                         0, 0, 0,
                         -1,-2,-1};

    convolution(out, after_Gy, Gy, nx, ny, 3, 0);

    for ( i = 1; i < nx - 1; i++)
        for ( j = 1; j < ny - 1; j++) {
            const int c = i + nx * j;
            // G[c] = abs(after_Gx[c]) + abs(after_Gy[c]);
            G[c] = (unsigned int)hypot(after_Gx[c], after_Gy[c]);
        }

    // Non-maximum suppression, straightforward implementation.
    for ( i = 1; i < nx - 1; i++)
        for ( j = 1; j < ny - 1; j++) {
            const int c = i + nx * j;
            const int nn = c - nx;
            const int ss = c + nx;
            const int ww = c + 1;
            const int ee = c - 1;
            const int nw = nn + 1;
            const int ne = nn - 1;
            const int sw = ss + 1;
            const int se = ss - 1;

            const float dir = (float)(fmod(atan2(after_Gy[c],
                                                 after_Gx[c]) + M_PI,
                                           M_PI) / M_PI) * 8;

            if (((dir <= 1 || dir > 7) && G[c] > G[ee] &&
                 G[c] > G[ww]) || // 0 deg
                    ((dir > 1 && dir <= 3) && G[c] > G[nw] &&
                     G[c] > G[se]) || // 45 deg
                    ((dir > 3 && dir <= 5) && G[c] > G[nn] &&
                     G[c] > G[ss]) || // 90 deg
                    ((dir > 5 && dir <= 7) && G[c] > G[ne] &&
                     G[c] > G[sw]))   // 135 deg
                nms[c] = G[c];
            else
                nms[c] = 0;
        }

    // Reuse array
    // used as a stack. nx*ny/2 elements should be enough.
    int *edges = (int*) after_Gy;
    memset(out, 0, sizeof(unsigned int) * nx * ny);
    memset(edges, 0, sizeof(unsigned int) * nx * ny);

    // Tracing edges with hysteresis . Non-recursive implementation.
    size_t c = 1;
    for ( j = 1; j < ny - 1; j++)
        for ( i = 1; i < nx - 1; i++) {
            if (nms[c] >= tmax && out[c] == 0) { // trace edges
                out[c] = MAX_BRIGHTNESS;
                int nedges = 1;
                edges[0] = c;

                do {
                    nedges--;
                    const int t = edges[nedges];

                    int nbs[8]; // neighbours
                    nbs[0] = t - nx;     // nn
                    nbs[1] = t + nx;     // ss
                    nbs[2] = t + 1;      // ww
                    nbs[3] = t - 1;      // ee
                    nbs[4] = nbs[0] + 1; // nw
                    nbs[5] = nbs[0] - 1; // ne
                    nbs[6] = nbs[1] + 1; // sw
                    nbs[7] = nbs[1] - 1; // se
                    int k;
                    for ( k = 0; k < 8; k++)
                        if (nms[nbs[k]] >= tmin && out[nbs[k]] == 0) {
                            out[nbs[k]] = MAX_BRIGHTNESS;
                            edges[nedges] = nbs[k];
                            nedges++;
                        }
                } while (nedges > 0);
            }
            c++;
        }


    for ( j = 1; j < ny - 1; j++){
        for ( i = 1; i < nx - 1; i++) {
            set_pixel(i,j, out[j * nx + i], destiny);
        }
    }

    free(after_Gx);
    free(after_Gy);
    free(G);
    free(nms);


}

/**/


double histo16bits[65536];
double histo8bits[256];


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





char command[2000];



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

int hist8bits_int[256];
double prob8bits[256], omega8bits[256]; /* prob of graylevels */
double myu8bits[256];   /* mean value for separation */
double sigma8bits[256]; /* inter-class variance */

/* binarization by Otsu's method
based on maximization of inter-class variance */
int otsu_th(void * img, void *dst, float scale)
{

  double max_sigma; /* inter-class variance */
  int i, x, y; /* Loop variable */
  int threshold; /* threshold for binarization */
  unsigned char color_8;
  int color_32;
#ifdef ___BMP___
  IBMP * frame_source= (IBMP*)img;
#else
  SDL_Surface * frame_source= (SDL_Surface*)img;
#endif

  int w,h;
#ifdef ___BMP___
  h= frame_source->height;
  w= frame_source->width;
#else
  w= frame_source->w;
  h= frame_source->h;
#endif



  printf("Otsu's binarization process starts now.\n");

  /* Histogram generation */
  for (i = 0; i < 256; i++) {
      hist8bits_int[i] = 0;
  }
  for (y = 0; y < h; y++){
    for (x = 0; x < w; x++) {
        color_32=get_pixel(x,y,img);
        color_8=color_32 & 0xff;
        hist8bits_int[color_8]++;
    }
  }
  /* calculation of probability density */
  for ( i = 0; i < 256; i ++ ) {
    prob8bits[i] = (double)hist8bits_int[i] / (h * w);
  }

  /* omega & myu generation */
  omega8bits[0] = prob8bits[0];
  myu8bits[0] = 0.0;       /* 0.0 times prob[0] equals zero */
  for (i = 1; i < 256; i++) {
    omega8bits[i] = omega8bits[i-1] + prob8bits[i];
    myu8bits[i] = myu8bits[i-1] + i*prob8bits[i];
  }

  /* sigma maximization
     sigma stands for inter-class variance
     and determines optimal threshold value */
  threshold = 0;
  max_sigma = 0.0;
  for (i = 0; i < 256-1; i++) {
    if (omega8bits[i] != 0.0 && omega8bits[i] != 1.0){
      sigma8bits[i] = pow(myu8bits[256-1]*omega8bits[i] - myu8bits[i], 2) /
    (omega8bits[i]*(1.0 - omega8bits[i]));
    }
    else{
      sigma8bits[i] = 0.0;
    }
    if (sigma8bits[i] > max_sigma) {
      max_sigma = sigma8bits[i];
      threshold = i;
    }
  }

  threshold=(threshold*scale);
  printf("\nthreshold value = %d\n", threshold);

  /* binarization output into image2 */
  for (y = 0; y < h; y++){
      for (x = 0; x < w; x++){
          color_32=get_pixel(x,y,img);
          color_8=color_32 & 0xff;
          if (color_8 > threshold){
              set_pixel(x,y,WHITE_24,dst);
          }
          else{
              set_pixel(x,y,BLACK_24,dst);
          }
      }
  }


  return threshold;
}

#define MAX_WINDOW_SIZE 1000

void filter_image(void * img, void *dst, char horizontaly, char edge_image, int window_size, int threshold){


    int  x, y; /* Loop variable */
    int i;
    unsigned char color_8;
    int color_32;
    unsigned char window[MAX_WINDOW_SIZE];
  #ifdef ___BMP___
    IBMP * frame_source= (IBMP*)img;
  #else
    SDL_Surface * frame_source= (SDL_Surface*)img;
  #endif

    int w,h;
  #ifdef ___BMP___
    h= frame_source->height;
    w= frame_source->width;
  #else
    w= frame_source->w;
    h= frame_source->h;
  #endif

    int average;

    if(window_size>MAX_WINDOW_SIZE){
        window_size=MAX_WINDOW_SIZE;
    }

    if(horizontaly)
    {
        for (y = 0; y < h; y++){
            for(i=0;i<window_size;i++){
                color_32=get_pixel(i,y,img);
                color_8=color_32 & 0xff;
                window[i]=color_8;
            }
            for (x = 0; x < w; x++){
                color_32=get_pixel(x,y,img);
                color_8=color_32 & 0xff;
                window[x%window_size]=color_8;
                average=0;
                for(i=0;i<window_size;i++){
                    average+=window[i];
                }
                average=average/window_size;
                //set_pixel(x,y,average&0xff,dst);
                if (average > threshold){
                    set_pixel(x,y,WHITE_24,dst);
                }
                else{
                    set_pixel(x,y,BLACK_24,dst);
                }
            }
        }
    }
    else{

        for (x = 0; x < w; x++){
            for(i=0;i<window_size;i++){
                color_32=get_pixel(x,i,img);
                color_8=color_32 & 0xff;
                window[i]=color_8;
            }
            average=0;
            for (y = 0; y < h; y++){
                color_32=get_pixel(x,y,img);
                color_8=color_32 & 0xff;
                window[y%window_size]=color_8;
                average=0;
                for(i=0;i<window_size;i++){
                    average+=window[i];
                }
                average=average/window_size;
                if (average > threshold){
                    set_pixel(x,y,edge_image?BLACK_24 :WHITE_24,dst);
                }
                else{
                    set_pixel(x,y,edge_image?WHITE_24:BLACK_24,dst);
                }
            }
        }
    }

}

void onecolor2othercolor( void* img, int color, int color2){
    int x,y;
    int color_d;
    #ifdef ___BMP___
        IBMP * frame_source= (IBMP*)img;
    #else
        SDL_Surface * frame_source= (SDL_Surface*)img;
    #endif

        int w,h;
    #ifdef ___BMP___
        h= frame_source->height;
        w= frame_source->width;
    #else
        w= frame_source->w;
        h= frame_source->h;
    #endif



    for(y=0;y<h;y++){
        for(x=0;x<w;x++){
            color_d=get_pixel(x,y,img) & 0xffffff;
            if(color_d>=color){
                set_pixel(x,y,color2,img);
            }else{
                set_pixel(x,y,color_d,img);
            }
        }
    }


}
void digital_operation(void * img, void *dst, char op){
    int x,y;
    int color,  color2, color3;
#ifdef ___BMP___
    IBMP * frame_source= (IBMP*)img;
#else
    SDL_Surface * frame_source= (SDL_Surface*)img;
#endif

    int w,h;
#ifdef ___BMP___
    h= frame_source->height;
    w= frame_source->width;
#else
    w= frame_source->w;
    h= frame_source->h;
#endif



for(y=0;y<h;y++){
    for(x=0;x<w;x++){
        color=get_pixel(x,y,img) & 0xffffff;
        color2=get_pixel(x,y,dst)& 0xffffff;
        switch(op){
        case AND_OP :
            color3=color & color2;
            break;
        case XOR_OP :
            color3=color ^ color2;
            break;
        case OR_OP:
            color3=color | color2;
            break;
        default:
            color3=color;
            break;
        }
        set_pixel(x,y,color3,dst);
    }
}




}


void image_to_RGB(void * source, void * destinyR,void * destinyG,void * destinyB){
    int x,y;
    int color;
#ifdef ___BMP___
    IBMP * frame_source= (IBMP*)source;
#else
    SDL_Surface * frame_source= (SDL_Surface*)source;
#endif

    int w,h;
#ifdef ___BMP___
    h= frame_source->height;
    w= frame_source->width;
#else
    w= frame_source->w;
    h= frame_source->h;
#endif

    for(y=0;y<h;y++){
        for(x=0;x<w;x++){
            color=get_pixel(x,y,frame_source);
            set_pixel(x,y,((color) & 0xff0000),destinyR);
            set_pixel(x,y,((color) & 0x00ff00),destinyG);
            set_pixel(x,y,(color & 0x0000ff),destinyB);
        }
    }

}

void get_size_bmp(int * width,int *height,int * bits, char * filename){
#ifdef ___BMP___
    char pBmp;
    // Reading BMP

    int  dwidth, dheight, x, y;
    int dx = 0;
    int dy = 0;
    FILE * infile;
    char * cp_pic_name = (char*)malloc(100);
    sprintf(cp_pic_name,"%s",filename);
    infile = fopen(cp_pic_name,"rb");
    if (infile == NULL){

        printf("open file[%s] failed!\n",filename);

        return;
    }



    pBmp = LoadHeader(infile, &dwidth, bits,width,height);
    if ( NULL == pBmp )
    {

        return;
    }

    // se cierra el archivo :)
    fclose(infile);
#else
    SDL_Surface * temp=SDL_LoadBMP(filename);
    if (temp == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }
    *width=temp->w;
    *height=temp->h;
    *bits=24;
#endif


}

char save_bmp_file(void *display_global, char * filename){

#ifdef ___BMP___
    IBMP * frame_source= (IBMP*)display_global;
    FILE * infile;
    char * cp_pic_name = (char*)malloc(100);
    sprintf(cp_pic_name,"%s",filename);
    infile = fopen(cp_pic_name,"wb+");

    if (infile == NULL){

        printf("open file failed!\n");

        return 0;
    }
    BmpIO_Save(infile,frame_source);
    fclose(infile);


    return 1;
#else
    SDL_Surface * frame_source= (SDL_Surface*)display_global;
    SDL_SaveBMP(frame_source,filename);
#endif


}


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
                unsigned char alpha)
{

    int x_b,x_init,x_end;
    int y_b,y_init,y_end;
    int x_des,y_des;
    int color;
    int back_color;
    unsigned char red, green, blue;
    unsigned char blended_red, blended_blue, blended_green;
    unsigned char back_red, back_blue, back_green;
    int new_color;

    x_init=x_source;
    y_init=y_source;
    x_end=x_source+width;
    y_end=y_source+height;
    x_des=x_destiny;
    y_des=y_destiny;

    for (x_b = x_init; x_b<x_end; x_b++) {
        for (y_b = y_init; y_b<y_end; y_b++) {

            color = get_pixel(x_b,y_b,source);


            back_color=get_pixel(x_des,y_des,destiny);

            new_color=back_color;
            if(color!=alpha_color)
            {
                // THE SW WAY
                back_red=(background_color & 0xff0000)>>16;
                back_green=(background_color & 0x00ff00)>>8;
                back_blue=background_color & 0x0000ff;
                red=(color & 0xff0000)>>16;
                green=(color & 0x00ff00)>>8;
                blue=(color & 0x0000ff);
                blended_red = (((red) * alpha) + (back_red * (255 - alpha)))/255;
                blended_green = (((green) * alpha) + (back_green * (255 - alpha)))/255;
                blended_blue = (((blue) * alpha) + (back_blue * (255 - alpha)))/255;
                new_color=(blended_red << 16) |(blended_green<<8) | (blended_blue);
            }
            set_pixel(x_des, y_des, new_color, destiny);
            y_des++;

        }// end for 2
        y_des=y_destiny;
        x_des++;

    }//
    return 1;
}

#ifdef ___BMP___
IBMP *  create_surface(int w, int h){

    return BmpIO_CreateBitmap(w,h,24);
}
#else
SDL_Surface *  create_surface(int w, int h){

    return SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
}

SDL_Surface *  Show_screen(int w,int h, char * name_window){
    /*inicializo la libreria SDL*/
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) { return 1; }
    SDL_WM_SetCaption(name_window, "Holguer A Becerra");
    SDL_EnableUNICODE( SDL_ENABLE );
    return SDL_SetVideoMode(w, h, DEPTH, SDL_HWSURFACE);


}



int ___text_get_enable_SDL_ADRIZ=0;

char Adriz_SDL_Get_Char(SDL_Event * event){

    char i_=0;

    i_=(char)event->key.keysym.unicode;

    return i_;
}


int AdrizGet_String(char * text,int x, int y,char input_type,int backcolor,struct abc_font_struct font[],SDL_Event * event,SDL_Surface *screen){
    static char temp_text[50];
    static int free_buffer=0;
    static int tilt_cursor=0;
    static unsigned int tilt_cursor_time=0;
    SDL_Surface * frame_text;

    static Uint16 last__=0;
    static Uint16 now__=0;
    int count_pos;

    static unsigned int time_flag=0;

    int return_val=0;

    if(___text_get_enable_SDL_ADRIZ==1){
        if(free_buffer==0){
            free_buffer=1;
            memset(temp_text,'\0',sizeof(temp_text));
        }

        count_pos=strlen(temp_text);


        if(time_flag<SDL_GetTicks()){
            last__=0;
            time_flag=SDL_GetTicks()+150;
        }

        now__=event->key.keysym.unicode;
        if(event->type==SDL_KEYDOWN && last__!=now__){
            last__=now__;
            time_flag=SDL_GetTicks()+150;
            if(sizeof(temp_text)!=count_pos){

                char char_=(char)event->key.keysym.unicode;;

                switch (input_type) {
                case NUMBERS_TYPE:
                    if( ( event->key.keysym.unicode >= (Uint16)'0' ) && ( event->key.keysym.unicode <= (Uint16)'9' ) )
                    {
                        //Append the character
                        temp_text[count_pos]=(char)event->key.keysym.unicode;
                    }
                    break;
                case STANDARD_TYPE:
                    //If the key is a space
                    if( (event->key.keysym.unicode==(Uint16)' '))
                    {
                        //Append the character
                        temp_text[count_pos]=(char)event->key.keysym.unicode;
                    }
                    //If the key is a number
                    else if( ( event->key.keysym.unicode >= (Uint16)'0' ) && ( event->key.keysym.unicode <= (Uint16)'9' ) )
                    {
                        //Append the character
                        temp_text[count_pos]=(char)event->key.keysym.unicode;
                    }
                    //If the key is a uppercase letter
                    else if( ( event->key.keysym.unicode >= (Uint16)'A' ) && ( event->key.keysym.unicode <= (Uint16)'Z' ) )
                    {
                        //Append the character
                        temp_text[count_pos]=(char)event->key.keysym.unicode;
                    }
                    //If the key is a lowercase letter
                    else if( ( event->key.keysym.unicode >= (Uint16)'a' ) && ( event->key.keysym.unicode <= (Uint16)'z' ) )
                    {
                        //Append the character
                        temp_text[count_pos]=(char)event->key.keysym.unicode;
                    }

                    break;
                case FREE_TYPE:

                    if(char_>21 && char_<128){
                        //Append the character
                        temp_text[count_pos]=(char)event->key.keysym.unicode;
                    }
                    break;
                default:
                    if(char_>21 && char_<128){
                        //Append the character
                        temp_text[count_pos]=(char)event->key.keysym.unicode;
                    }
                }


            }

        }

        if(event->type==SDL_KEYDOWN){
            //If backspace was pressed and the string isn't blank
            if( ( event->key.keysym.sym == SDLK_BACKSPACE ) && ( count_pos != 0 ) )
            {
                //Remove a character from the end
                count_pos--;
                temp_text[count_pos]='\0';
                SDL_Delay(50);

            }
        }


        frame_text=create_surface(1+get_size_width_text(font,temp_text),font['|' - 33].bounds_height);

        clean_screen(frame_text,backcolor);

        print_string_alpha(0,0,~backcolor,backcolor,font,frame_text,temp_text);

        // tilt cursor
        if(tilt_cursor_time<SDL_GetTicks()){
            tilt_cursor=(tilt_cursor==0)?1:0;
            tilt_cursor_time=SDL_GetTicks()+500;
        }
        if(tilt_cursor==1){
            draw_box(frame_text->w-2,0,frame_text->w,frame_text->h,~backcolor,DO_FILL,frame_text);
        }

        frame2frame(frame_text,screen,0,0,frame_text->w,frame_text->h,x,y,-1,-1,0xff);
        if((event->key.keysym.sym==SDLK_RETURN) || (event->key.keysym.sym==SDLK_KP_ENTER)){
            strcpy(text,temp_text);
            ___text_get_enable_SDL_ADRIZ=0;
            free_buffer=0;
            return_val=1;
            tilt_cursor=0;
        }


    }
    else{
        free_buffer=0;
    }

    return return_val;
}

void Adriz_SDL_Enable_Get_Text(void){
    ___text_get_enable_SDL_ADRIZ=1;
}

void Adriz_SDL_Disable_Get_Text(void){
    ___text_get_enable_SDL_ADRIZ=0;
}

int SDL_Get_String(char * text,int x, int y,char input_type,int backcolor,struct abc_font_struct font[],SDL_Event * event,SDL_Surface *screen){
    if(___text_get_enable_SDL_ADRIZ==0){
        Adriz_SDL_Enable_Get_Text();
    }
    AdrizGet_String(text,x,y,input_type,backcolor,font,event,screen);
    return ___text_get_enable_SDL_ADRIZ;
}

#endif


/******************************************************************
*  Data: cour10_font
*
*  Purpose: Data array that represents a 10-point courier font.
*
******************************************************************/
char cour10_font_array[95][11] =

{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00},
 {0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x14, 0x14, 0x7E, 0x28, 0x28, 0x28, 0xFC, 0x50, 0x50, 0x00, 0x00},
 {0x10, 0x38, 0x44, 0x40, 0x38, 0x04, 0x44, 0x38, 0x10, 0x00, 0x00},
 {0x40, 0xA2, 0x44, 0x08, 0x10, 0x20, 0x44, 0x8A, 0x04, 0x00, 0x00},
 {0x30, 0x40, 0x40, 0x20, 0x60, 0x92, 0x94, 0x88, 0x76, 0x00, 0x00},
 {0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x08},
 {0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20},
 {0x00, 0x00, 0x6C, 0x38, 0xFE, 0x38, 0x6C, 0x00, 0x00, 0x00, 0x00},
 {0x00, 0x10, 0x10, 0x10, 0xFE, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00},
 {0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
 {0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00},
 {0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00},
 {0x10, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00},
 {0x38, 0x44, 0x04, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7C, 0x00, 0x00},
 {0x38, 0x44, 0x04, 0x04, 0x18, 0x04, 0x04, 0x44, 0x38, 0x00, 0x00},
 {0x08, 0x18, 0x18, 0x28, 0x28, 0x48, 0x7C, 0x08, 0x1C, 0x00, 0x00},
 {0x7C, 0x40, 0x40, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38, 0x00, 0x00},
 {0x18, 0x20, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00},
 {0x7C, 0x44, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x00, 0x00},
 {0x38, 0x44, 0x44, 0x44, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00},
 {0x38, 0x44, 0x44, 0x44, 0x3C, 0x04, 0x04, 0x08, 0x30, 0x00, 0x00},
 {0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00},
 {0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x00},
 {0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x7C, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x00, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00},
 {0x38, 0x44, 0x04, 0x04, 0x08, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00},
 {0x3C, 0x42, 0x9A, 0xAA, 0xAA, 0xAA, 0x9C, 0x40, 0x38, 0x00, 0x00},
 {0x30, 0x10, 0x10, 0x28, 0x28, 0x44, 0x7C, 0x44, 0xEE, 0x00, 0x00},
 {0xFC, 0x42, 0x42, 0x42, 0x7C, 0x42, 0x42, 0x42, 0xFC, 0x00, 0x00},
 {0x3C, 0x42, 0x80, 0x80, 0x80, 0x80, 0x80, 0x42, 0x3C, 0x00, 0x00},
 {0xF8, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0xF8, 0x00, 0x00},
 {0xFE, 0x42, 0x40, 0x48, 0x78, 0x48, 0x40, 0x42, 0xFE, 0x00, 0x00},
 {0xFE, 0x42, 0x40, 0x48, 0x78, 0x48, 0x40, 0x40, 0xF0, 0x00, 0x00},
 {0x3C, 0x42, 0x80, 0x80, 0x80, 0x8E, 0x82, 0x42, 0x3C, 0x00, 0x00},
 {0xEE, 0x44, 0x44, 0x44, 0x7C, 0x44, 0x44, 0x44, 0xEE, 0x00, 0x00},
 {0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00},
 {0x1E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x44, 0x38, 0x00, 0x00},
 {0xE6, 0x44, 0x48, 0x48, 0x50, 0x70, 0x48, 0x44, 0xE6, 0x00, 0x00},
 {0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0xFE, 0x00, 0x00},
 {0xC6, 0x44, 0x6C, 0x6C, 0x54, 0x54, 0x44, 0x44, 0xEE, 0x00, 0x00},
 {0xCE, 0x44, 0x64, 0x64, 0x54, 0x4C, 0x4C, 0x44, 0xE4, 0x00, 0x00},
 {0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38, 0x00, 0x00},
 {0xFC, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 0xF0, 0x00, 0x00},
 {0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38, 0x36, 0x00},
 {0xFC, 0x42, 0x42, 0x42, 0x7C, 0x48, 0x48, 0x44, 0xE6, 0x00, 0x00},
 {0x7C, 0x82, 0x80, 0x80, 0x7C, 0x02, 0x02, 0x82, 0x7C, 0x00, 0x00},
 {0xFE, 0x92, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00},
 {0xEE, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00},
 {0xEE, 0x44, 0x44, 0x44, 0x28, 0x28, 0x28, 0x10, 0x10, 0x00, 0x00},
 {0xEE, 0x44, 0x44, 0x44, 0x54, 0x54, 0x54, 0x28, 0x28, 0x00, 0x00},
 {0xEE, 0x44, 0x28, 0x28, 0x10, 0x28, 0x28, 0x44, 0xEE, 0x00, 0x00},
 {0xEE, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00},
 {0xFE, 0x84, 0x08, 0x08, 0x10, 0x20, 0x20, 0x42, 0xFE, 0x00, 0x00},
 {0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x38, 0x00, 0x00},
 {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00},
 {0x1C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1C, 0x00, 0x00},
 {0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00},
 {0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x78, 0x04, 0x7C, 0x84, 0x84, 0x7A, 0x00, 0x00},
 {0xC0, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x42, 0xFC, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x7C, 0x82, 0x80, 0x80, 0x82, 0x7C, 0x00, 0x00},
 {0x0C, 0x04, 0x04, 0x7C, 0x84, 0x84, 0x84, 0x84, 0x7E, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x7C, 0x82, 0xFE, 0x80, 0x82, 0x7C, 0x00, 0x00},
 {0x30, 0x40, 0x40, 0xF0, 0x40, 0x40, 0x40, 0x40, 0xF0, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x7E, 0x84, 0x84, 0x84, 0x7C, 0x04, 0x04, 0x78},
 {0xC0, 0x40, 0x40, 0x58, 0x64, 0x44, 0x44, 0x44, 0xEE, 0x00, 0x00},
 {0x08, 0x00, 0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00},
 {0x08, 0x00, 0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x70},
 {0xC0, 0x40, 0x40, 0x4C, 0x48, 0x50, 0x70, 0x48, 0xC6, 0x00, 0x00},
 {0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0xE8, 0x54, 0x54, 0x54, 0x54, 0xD6, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0xD8, 0x64, 0x44, 0x44, 0x44, 0xEE, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x7C, 0x82, 0x82, 0x82, 0x82, 0x7C, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0xFC, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x40, 0xE0},
 {0x00, 0x00, 0x00, 0x7E, 0x84, 0x84, 0x84, 0x7C, 0x04, 0x0E, 0x00},
 {0x00, 0x00, 0x00, 0xEC, 0x32, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0x7C, 0x82, 0x70, 0x0C, 0x82, 0x7C, 0x00, 0x00},
 {0x00, 0x20, 0x20, 0x78, 0x20, 0x20, 0x20, 0x24, 0x18, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0xCC, 0x44, 0x44, 0x44, 0x4C, 0x36, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0xEE, 0x44, 0x44, 0x28, 0x28, 0x10, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0xEE, 0x44, 0x54, 0x54, 0x28, 0x28, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0xEE, 0x44, 0x38, 0x38, 0x44, 0xEE, 0x00, 0x00},
 {0x00, 0x00, 0x00, 0xEE, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x60},
 {0x00, 0x00, 0x00, 0xFC, 0x88, 0x10, 0x20, 0x44, 0xFC, 0x00, 0x00},
 {0x0C, 0x10, 0x10, 0x10, 0x10, 0x60, 0x10, 0x10, 0x10, 0x10, 0x0C},
 {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
 {0x60, 0x10, 0x10, 0x10, 0x10, 0x0C, 0x10, 0x10, 0x10, 0x10, 0x60},
 {0x00, 0x00, 0x62, 0x92, 0x8C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

//Pointer to our font table
char* cour10_font = &cour10_font_array[0][0];

