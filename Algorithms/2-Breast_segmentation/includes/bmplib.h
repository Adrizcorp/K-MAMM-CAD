#ifndef BMPLIB_H
#define BMPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>


#define  WIDTH 800
#define  HEIGHT 480

#define ISLITTLEENDIAN 1

#define WORD unsigned short
#define DWORD unsigned int
#define LONG int
#define BYTE unsigned char
#define BOOL int
#define FALSE 0
#define TRUE 1

typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} BITMAPINFOHEADER;



typedef struct tagBuf
{
    union {
        unsigned int buf : 32;
        unsigned char vbuf[4];
    } BufU;
    int buflen;
} BUF;

#define BITMAPFILEHEADER_SIZE ( sizeof( WORD ) * 3 + sizeof( DWORD ) * 2 )
#define BITMAPINFOHEADER_SIZE ( sizeof( WORD ) * 2 + sizeof( DWORD ) * 5 + sizeof( LONG ) * 4 )

typedef struct tagInternalColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} ICOLOR;

typedef struct tagInternalBMP
{
    int width;
    int height;
    int BitPerPix;
    ICOLOR *pColor;
    unsigned char *pPix;
} IBMP;



IBMP* BmpIO_CreateBitmap( int width, int height, int BitPerPixcel );
IBMP* BmpIO_Load( FILE *infile );
char LoadHeader(
    FILE *infile,
    unsigned int *ctsize,
    int *blen,
    int *pWidth,
    int *pHeight);

 BOOL LoadBody1( FILE *infile, int BitLen, IBMP *pBmp );
 BOOL LoadBody24( FILE *infile, IBMP *pBmp );
 void FrushBuf_ipt( BUF *pBuf, FILE *infile );
 int ReadBuf( BUF *pBuf, int len, FILE *infile );
 size_t int_fread( void *buffer, size_t size, size_t count, FILE *stream );
 BOOL WriteHeader( FILE *outfile, const IBMP *pBmp );
 BOOL WriteBody1( FILE *outfile, const IBMP *pBmp );
 BOOL WriteBody24( FILE *outfile, const IBMP *pBmp );
 void FrushBuf_opt( BUF *pBuf, FILE *outfile );
 void WriteBuf( BUF *pBuf, int BitLen, unsigned char c, FILE *outfile );
 size_t int_fwrite( const void *buffer, size_t size, size_t count, FILE *stream );

int BmpIO_Save( FILE *outfile, const IBMP *pBmp );
void BmpIO_DeleteBitmap( IBMP *pBmp );
int BmpIO_GetWidth( const IBMP *pBmp );
int BmpIO_GetHeight( const IBMP *pBmp );
int BmpIO_GetBitPerPixcel( const IBMP *pBmp );
unsigned char BmpIO_GetR( int x, int y, const IBMP *pBmp );
unsigned char BmpIO_GetG( int x, int y, const IBMP *pBmp );
unsigned char BmpIO_GetB( int x, int y, const IBMP *pBmp );


void BmpIO_SetR( int x, int y, IBMP *pBmp, unsigned char v );
void BmpIO_SetG( int x, int y, IBMP *pBmp, unsigned char v );
void BmpIO_SetB( int x, int y, IBMP *pBmp, unsigned char v );


unsigned char BmpIO_GetColorTableR( int idx, const IBMP *pBmp );
unsigned char BmpIO_GetColorTableG( int idx, const IBMP *pBmp );
unsigned char BmpIO_GetColorTableB( int idx, const IBMP *pBmp );
void BmpIO_SetColorTableR( int idx, const IBMP *pBmp, unsigned char v );
void BmpIO_SetColorTableG( int idx, const IBMP *pBmp, unsigned char v );
void BmpIO_SetColorTableB( int idx, const IBMP *pBmp, unsigned char v );
unsigned char BmpIO_GetPixcel( int x, int y, const IBMP *pBmp );
void BmpIO_SetPixcel( int x, int y, const IBMP *pBmp, unsigned char v );
int BmpIO_TranseTo24BitColor( IBMP *pBmp );

void LoadBody24_2_framereader( FILE *infile,int x, int y, int width, int height, IBMP * pBmp);
int PixIdx( int w, int h, const IBMP *pBmp );




#endif // BMP_H
