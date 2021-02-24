// CVServices.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"

#include <direct.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

// Llibreries d'imatge

extern "C"
{
	#include "libjpeg-turbo\jpeglib.h"
}

#include "CVImage.h"


// =============================================================================
// CVException =====================================================================
// =============================================================================

// CVException =================================================================

CVException::CVException(const char *Format, ...)
{
	va_list al;
	va_start(al, Format);
	_vsnprintf(m_Message, 255, Format, al);
	m_Message[255] = 0;
	va_end(al);
}

// CVException =================================================================

CVException::CVException(const CVException& rhs) : exception(rhs)
{
	memcpy(m_Message, rhs.m_Message, sizeof m_Message);
}

// SetMessage ==================================================================

void CVException::SetMessage(const char *Format, va_list args)
{
	_vsnprintf(m_Message, 256, Format, args);
	m_Message[255] = 0;
	va_end(args);
}

// =============================================================================
// CVImage =====================================================================
// =============================================================================

// CVImage::CVImage ============================================================

CVImage::CVImage(const char *nombre,
				 unsigned szX,unsigned szY,unsigned cha,int OrigenX,int OrigenY)
{
	m_pNombre=new char[strlen(nombre)+1];
	strcpy(m_pNombre,nombre);
	m_szX=szX;
	m_szY=szY;
	m_OrigenX=OrigenX;
	m_OrigenY=OrigenY;
	m_Canales=cha;
	m_pRaster=new unsigned char[GetRasterSize()];
}

// CVImage::CVImage ============================================================

CVImage::CVImage(const CVImage &im)
{
	m_pNombre=new char[strlen(im.m_pNombre)+1];
	strcpy(m_pNombre,im.m_pNombre);
	m_szX=im.m_szX;
	m_szY=im.m_szY;
	m_Canales=im.m_Canales;
	m_OrigenX=im.m_OrigenX;
	m_OrigenY=im.m_OrigenY;

    int sz=GetRasterSize();
	m_pRaster=new unsigned char[sz];
    memcpy(m_pRaster,im.m_pRaster,sz);
}

// CVImage::~CVImage ===========================================================

CVImage::~CVImage()
{
	delete [] m_pNombre;
	delete [] m_pRaster;
}

// CVImage::new ================================================================

void *CVImage::operator new( size_t sz)
{
    return (void*) ::new char[sz];
}

// CVImage::delete =============================================================

void  CVImage::operator delete (void*p)
{
    ::delete p;
}

// CVImage::Get ================================================================

void CVImage::Get(BITMAPINFO &bmi) const
{
    //int szl;

    //szl=m_szX*m_Canales;   // Tamaño de una linea
	//szl=4*(szl/4+ ((szl & 3)!=0));
    //sz=szl*szY; // Tamaño global

    bmi.bmiHeader.biSize=40;
    bmi.bmiHeader.biWidth=m_szX;
    bmi.bmiHeader.biHeight=m_szY;
    bmi.bmiHeader.biPlanes=1;
    if (m_Canales==1) bmi.bmiHeader.biBitCount=8;
    else bmi.bmiHeader.biBitCount=24;
    bmi.bmiHeader.biCompression=0; /* no comprimido */
    bmi.bmiHeader.biSizeImage=0; //sz;
    bmi.bmiHeader.biXPelsPerMeter=0;
    bmi.bmiHeader.biYPelsPerMeter=0;
    bmi.bmiHeader.biClrUsed=256; //256;
    bmi.bmiHeader.biClrImportant=256; //256;
}

// CVImage::Set ================================================================
void CVImage::Set(HDC hdc, HBITMAP bitmap)
{
	BITMAPINFO bi;
	Get(bi);

	::GetDIBits(hdc, bitmap, 0, m_szY, GetRaster(), &bi, DIB_RGB_COLORS);
}

// CVImage::GetRasterSize ======================================================

int CVImage::GetRasterSize() const
{
    int szl=m_szX*m_Canales;   // Tamaño de una linea
	szl=4*(szl/4+ ((szl & 3)!=0));
    return szl*m_szY; // Tamaño global
}

// CVImage::GetLineSize ========================================================

int CVImage::GetLineSize() const
{
    int szl=m_szX*m_Canales;   // Tamaño de una linea
	szl=4*(szl/4+ ((szl & 3)!=0));
    return szl; 
}

// CVImage::GetPixel ===========================================================

int CVImage::GetPixel(int x,int y,int c) const
{
    x-=m_OrigenX;
    y-=m_OrigenY;
    if (x<0 || x>=(signed) m_szX || y<0 || y>= (signed) m_szY || c<0 || c>= (signed) m_Canales) return -1;
    INT_PTR szl=m_szX*m_Canales;   // Tamaño de una linea
	szl=4*(szl/4+ ((szl & 3)!=0));
    return *(m_pRaster+(m_szY-y-1)*szl+x*m_Canales+c);
}

// CVImage::GetPixelAddress ====================================================

unsigned char* CVImage::GetPixelAddress(int x,int y,int c) const
{
    x-=m_OrigenX;
    y-=m_OrigenY;
    if (x<0 || x>=(signed) m_szX || y<0 || y>= (signed) m_szY || c<0 || c>= (signed) m_Canales) return NULL;
    INT_PTR szl=m_szX*m_Canales;   // Tamaño de una linea
	szl=4*(szl/4+ ((szl & 3)!=0));
    return m_pRaster+(m_szY-y-1)*szl+x*m_Canales+c;
}


// CVImage::SetImRaster ========================================================

void CVImage::SetImRaster(const unsigned char *raster)
{


	long sz=m_szX*m_Canales;   // Tamaño de una linea
    sz=4*(sz/4+ ((sz & 3)!=0))*m_szY; // Tamaño global

    int pad=((m_szX*m_Canales) & 3);
    if (pad!=0) pad=4-pad;
	unsigned char *p=m_pRaster;
    if (m_Canales==1) {
        for (unsigned int y=m_szY-1; y>=0;--y) {
            for (unsigned int x=0;x<m_szX;++x) {
                *p++=raster[x + y * m_szX];
            }
            for (int i=pad; i-- > 0;) *p++=0;
        }
    }
    else {
		const unsigned char *pR = raster;
		const unsigned char *pG = raster+m_szX*m_szY;
		const unsigned char *pB = raster+m_szX*m_szY*2;

        for (unsigned int y=m_szY-1; y>=0;--y) {
            for (unsigned int x=0;x<m_szX;++x) {
                *p++=pB[x + y * m_szX];
                *p++=pG[x + y * m_szX];
                *p++=pR[x + y * m_szX];
            }
            for (int i=pad; i-- > 0;) *p++=0;
        }
    }
}

// CVIMage::GetChannel =========================================================

CVImage* CVImage::GetChannel(int ch) const
{
	CVImage *pImChannel = new CVImage("Channel", m_szX, m_szY, 1, m_OrigenX, m_OrigenY);
	long lsz = GetLineSize();
	long clsz = pImChannel->GetLineSize();
	for (int y = 0; y < m_szY; ++y) {
		unsigned char *pIm = GetRaster() + y * lsz + ch;
		unsigned char *pCh= pImChannel->GetRaster() + y * clsz;
		for (int x = 0; x < m_szX; ++x) {
			*pCh = *pIm;
			++pCh;
			pIm += m_Canales;
		}
	}
	return pImChannel;
}

// CVImage::CopyToBMPRaster ====================================================

void CVImage::CopyToBMPRaster(void *raster) const
{
    memcpy(raster,GetRaster(),GetRasterSize());
}


// swap_long ===================================================================

void swap_long(long *pl)
{
    union mylong {
        long lo;
        struct {
            char c1, c2, c3, c4;
        } ch;
    } ll;
    char ch_tmp;

    ll.lo = *pl;

    ch_tmp   = ll.ch.c1;
    ll.ch.c1 = ll.ch.c4;
    ll.ch.c4 = ch_tmp;
    ch_tmp   = ll.ch.c2;
    ll.ch.c2 = ll.ch.c3;
    ll.ch.c3 = ch_tmp;

    *pl = ll.lo;
}

void swap_ulong(unsigned long *pl)
{
    union mylong {
        unsigned long lo;
        struct {
            char c1, c2, c3, c4;
        } ch;
    } ll;
    char ch_tmp;

    ll.lo = *pl;

    ch_tmp   = ll.ch.c1;
    ll.ch.c1 = ll.ch.c4;
    ll.ch.c4 = ch_tmp;
    ch_tmp   = ll.ch.c2;
    ll.ch.c2 = ll.ch.c3;
    ll.ch.c3 = ch_tmp;

    *pl = ll.lo;
}

// CVImage::WriteBMP ===========================================================

void CVImage::WriteBMP(const char *name) const
{
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    RGBQUAD          aColors[256];

    int i;
    FILE *fp;
    int sz=m_szX*m_Canales;

    sz=4*(sz/4+ ((sz & 3)!=0))*m_szY;

    bmfh.bfType=19778;
	if (m_Canales==1) {
		bmfh.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*4+sz;
	}
	else {
        bmfh.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sz;
	}
    bmfh.bfReserved1=0;
    bmfh.bfReserved2=0;
	if (m_Canales==1) {
	    bmfh.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*4;
	}
	else {
	    bmfh.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	}

    bmih.biSize=40;
    bmih.biWidth=m_szX;
    bmih.biHeight=m_szY;
    bmih.biPlanes=1;
    bmih.biBitCount=8*m_Canales;
    bmih.biCompression=0; /* no comprimido */
    bmih.biSizeImage=sz;
    bmih.biXPelsPerMeter=0;
    bmih.biYPelsPerMeter=0;
    bmih.biClrUsed=0;
    bmih.biClrImportant=0;

    for (i=0; i<256;++i) {
        aColors[i].rgbBlue=i;
        aColors[i].rgbGreen=i;
        aColors[i].rgbRed=i;
        aColors[i].rgbReserved=0;
    }

    fp=fopen(name,"wb");
    if (!fp) {
		throw CVException("No se ha podido escribir la imagen %s",name);
	}
    fwrite(&bmfh,sizeof(bmfh),1,fp);
    fwrite(&bmih,sizeof(bmih),1,fp);
    if (m_Canales==1) fwrite(aColors,256*4,1,fp);
	fwrite(m_pRaster,sz,1,fp);
    fclose(fp);
}

// CVImage::Write ==============================================================

void CVImage::Write(const char *filename) const
{
	char ext[20];
	_splitpath(filename, NULL, NULL, NULL, ext);
	strlwr(ext);

	if (strcmp(ext, ".bmp") == 0) WriteBMP(filename);
	else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) WriteJpeg(filename,75);
	else throw CVException("CVImage::Write: Extensión de imagen desconocida");
}


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  char buffer[JMSG_LENGTH_MAX];
  (*cinfo->err->format_message) (cinfo, buffer); // Create the message

  throw CVException("%s",buffer);
}

// CVImage::WriteJpeg ===========================================================

void CVImage::WriteJpeg(const char *name, int quality) const
{
	// Abrir fichero imagen
    FILE * outfile=fopen(name,"wb");
    if (!outfile)
	{
		throw CVException("No se ha podido escribir la imagen %s",name);
	}

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	try {

		cinfo.err = jpeg_std_error(&jerr);
		jerr.error_exit = my_error_exit;

		jpeg_create_compress(&cinfo);

	  // Step 2: specify data destination (eg, a file)

		jpeg_stdio_dest(&cinfo, outfile);

	  // Step 3: set parameters for compression

		cinfo.image_width = m_szX;
		cinfo.image_height = m_szY;
		cinfo.input_components = m_Canales;       // # of color components per pixel

		if (m_Canales == 1)	cinfo.in_color_space = JCS_GRAYSCALE;
		else				cinfo.in_color_space = JCS_RGB;

		jpeg_set_defaults(&cinfo);	// paràmetres de compressió per defecte

		jpeg_set_quality(&cinfo, quality, TRUE ); // limit to baseline-JPEG values

		// Step 4: Start compressor

		jpeg_start_compress(&cinfo, TRUE);

		// Step 5: while (scan lines remain to be written)  jpeg_write_scanlines(...);

		int row_stride = m_szX*m_Canales; // JSAMPLEs per row in image_buffer ;

		JSAMPARRAY row_pointer =  (*cinfo.mem->alloc_sarray)	((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

		unsigned char *pix=GetRaster();
		int row = 0;

		unsigned cvlinsz=cinfo.input_components * cinfo.image_width;
		if (cvlinsz & 3) cvlinsz=(cvlinsz& ~3)+4;

		while (cinfo.next_scanline < cinfo.image_height)
		{
			// intercanvio les files
			int off = (cinfo.image_height - ++row) * cvlinsz;
			memcpy(row_pointer[0],&pix[off],row_stride);

			// swap pixels vermells i verds
			if (m_Canales == 3)
			{
				for (unsigned int i=0; i<3*cinfo.image_width; i+=3)
				{
					unsigned char aux = row_pointer[0][i+2];
					row_pointer[0][i+2] = row_pointer[0][i];
					row_pointer[0][i] = aux;
				}
			}

			jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}

		// Step 6: Finish compression

		jpeg_finish_compress(&cinfo);

		fclose(outfile);

		// Step 7: release JPEG compression object

  		jpeg_destroy_compress(&cinfo);

	}
	catch (CVException)
	{
		fclose(outfile);
  		jpeg_destroy_compress(&cinfo);
		throw;
	}

}


// CVReadImage =================================================================

CVImage *CVReadImage(const char *filename)
{
	char ext[20];
	_splitpath(filename, NULL, NULL, NULL, ext);
	strlwr(ext);

	if (strcmp(ext,".bmp")==0)
		return CVReadImageBMP(filename);
	else if (strcmp(ext,".jpg")==0 || strcmp(ext,".jpeg")==0)
		return CVReadImageJPEG(filename);
	return NULL;
}


// CVReadImageBMP ==============================================================

CVImage* CVReadImageBMP(const char *filename)
{
    int NCol;
    int NLin;
    int NChannels;
    FILE *fp;
	CVImage *cvim=NULL;
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
	RGBQUAD paleta[256];
	unsigned char *raster,*p;
    unsigned long LinSz;

    /* abrir fichero imagen */
    fp=fopen(filename,"rb");
    if (!fp) {
		throw CVException("No se ha podido leer la imagen %s",filename);
	}
    if (fread((void *) &bmfh,sizeof bmfh,1,fp)!=1 ||
        bmfh.bfType!=19778 ||
        fread((void *) &bmih,sizeof bmih,1,fp)!=1 ||
        bmih.biSize!=40 ||
        bmih.biPlanes!=1 ||
        (bmih.biBitCount!=1 && bmih.biBitCount!=8 && bmih.biBitCount!=24 && bmih.biBitCount!=32) ||
        (bmih.biCompression!=0 && bmih.biCompression!=BI_BITFIELDS)
        ) {
		fclose(fp);

		throw CVException("Formato no aceptado de la imagen %s",filename);
    };
    NCol=bmih.biWidth;
    NLin=bmih.biHeight;
	bool YInvertida=true;
	if (NLin<0) {
		NLin=-NLin;
		YInvertida=false;
	}
    if (bmih.biBitCount==8 || bmih.biBitCount==1) NChannels=1; else NChannels=3;

	CVImage *pCVIm;
	if (NChannels==1 && bmih.biBitCount==8) {
		int i;
		fread(paleta,256*sizeof(RGBQUAD),1,fp);
		for (i=0;i<256;++i) {
			if (paleta[i].rgbBlue!=i ||
				paleta[i].rgbRed!=i ||
				paleta[i].rgbGreen!=i) break;
		}
		if (i!=256) {
			int x,y,c,pad,padf;
			// Aplicar paleta
			NChannels=3;
			LinSz=NCol*NChannels;
			padf=4*((NCol & 3)!=0)-(NCol & 3);
			pad=LinSz;
			LinSz=4*(LinSz/4+ ((LinSz & 3)!=0));
			pad=LinSz-pad;
			pCVIm=new CVImage(filename,NCol,NLin,NChannels,0,0);
		    raster=pCVIm->GetRaster();
			p=raster;
			if (!YInvertida) raster+=LinSz*(NLin-1);
		    fseek(fp,bmfh.bfOffBits,0);
			for (y=0;y<NLin;++y) {
				p=raster;
				for (x=0;x<NCol;++x) {
					c=getc(fp);
					*p++=paleta[c].rgbBlue;
					*p++=paleta[c].rgbGreen;
					*p++=paleta[c].rgbRed;
				}
				for (i=0;i<padf;++i) getc(fp);
				if (YInvertida) raster+=LinSz;
				else raster-=LinSz;
			}
			fclose(fp);
			return pCVIm;
		}
	}

	switch (bmih.biBitCount) {
	case 1: {
		// Leer binaria
		int lsz=bmih.biSizeImage/NLin;
		LinSz=NCol*NChannels;
		LinSz=4*(LinSz/4+ ((LinSz & 3)!=0));
		pCVIm=new CVImage(filename,NCol,NLin,NChannels,0,0);
		raster=pCVIm->GetRaster();
		if (!YInvertida) raster+=LinSz*(NLin-1);
		fseek(fp,bmfh.bfOffBits,0);
		for (int y=0;y<NLin;++y) {
			p=raster;
			for (int x=0;;) {
				int bBits = fgetc(fp);
				*(p++) = (bBits&0x80) ? 255 : 0;
				if (++x>=NCol) break;
				*(p++) = (bBits&0x40) ? 255 : 0;
				if (++x>=NCol) break;
				*(p++) = (bBits&0x20) ? 255 : 0;
				if (++x>=NCol) break;
				*(p++) = (bBits&0x10) ? 255 : 0;
				if (++x>=NCol) break;
				*(p++) = (bBits&0x08) ? 255 : 0;
				if (++x>=NCol) break;
				*(p++) = (bBits&0x04) ? 255 : 0;
				if (++x>=NCol) break;
				*(p++) = (bBits&0x02) ? 255 : 0;
				if (++x>=NCol) break;
				*(p++) = (bBits&0x01) ? 255 : 0;
				if (++x>=NCol) break;
			}
			int leido=NCol/8+ ((NCol & 7)!=0);
			while (leido<lsz) {fgetc(fp); ++leido; }
			if (YInvertida) raster+=LinSz;
			else raster-=LinSz;
		}
		}
		break;
	case 8:
	case 24:
		LinSz=NCol*NChannels;
		LinSz=4*(LinSz/4+ ((LinSz & 3)!=0));
		pCVIm=new CVImage(filename,NCol,NLin,NChannels,0,0);
		raster=pCVIm->GetRaster();
		fseek(fp,bmfh.bfOffBits,0);
		if (YInvertida) {
			if (fread(raster,LinSz*NLin,1,fp)!=1) {
				fclose(fp);
				delete pCVIm;

				throw CVException("Error de lectura de la imagen en CVReadImageBMP");
			}
		}
		else {
			raster+=LinSz*(NLin-1);
			for (int y=0;y<NLin;++y) {
				if (fread(raster,LinSz,1,fp)!=1) {
					fclose(fp);
					delete pCVIm;

					throw CVException("Error de lectura de la imagen en CVReadImageBMP");
				}
				raster-=LinSz;
			}
		}
		break;
	case 32: {
		LinSz=NCol*3;
		pCVIm=new CVImage(filename,NCol,NLin,NChannels,0,0);
		raster=pCVIm->GetRaster();
		fseek(fp,bmfh.bfOffBits,0);
		int x,y;
		if (!YInvertida) raster+=LinSz*(NLin-1);
		for (y=0;y<NLin;++y) {
			p=raster;
			for (x=0;x<NCol;++x) {
				*p++=getc(fp);
				*p++=getc(fp);
				*p++=getc(fp);
				getc(fp);
			}
			if (YInvertida) raster+=LinSz;
			else raster-=LinSz;
		}
		}
		break;
	default:
		{
		throw CVException("CVReadImageBMP no admite biBitCount=%d",bmih.biBitCount);
		}
	}
    fclose(fp);
	return pCVIm;
}


// CVReadImageJPEG ==============================================================

CVImage* CVReadImageJPEG(const char *filename)
{

    // abrir fichero imagen
    FILE *infile=fopen(filename,"rb");
    if (!infile)
	{
		throw CVException(" No se ha podido leer la imagen %s", filename);
	}

	// Step 1: allocate and initialize JPEG decompression object

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

	try {

		cinfo.err = jpeg_std_error(&jerr);
		jerr.error_exit = my_error_exit;

		jpeg_create_decompress(&cinfo);

		// Step 2: specify data source (eg, a file)
		jpeg_stdio_src(&cinfo, infile);

		// Step 3: read file parameters with jpeg_read_header()
		jpeg_read_header(&cinfo, TRUE);


		// Step 4: set parameters for decompression

		// Step 5: Start decompressor

		jpeg_start_decompress(&cinfo);


		int row_stride = cinfo.output_width * cinfo.output_components;

		JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)	((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

		// Step 6: while (scan lines remain to be read) jpeg_read_scanlines(...);

		CVImage *pCVIm = new CVImage(filename,cinfo.output_width,cinfo.output_height,cinfo.num_components,0,0);
		unsigned char *pix=pCVIm->GetRaster();

		int row = 0;
		unsigned cvlinsz=cinfo.num_components * cinfo.output_width;
		if (cvlinsz & 3) cvlinsz=(cvlinsz& ~3)+4;
		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);

			// intercanvio files
			int off = (cinfo.output_height - ++row) * cvlinsz;

			if (cinfo.num_components == 3) // si és color -> passo de RGB a BGR
			{
				for (unsigned int i=0; i<3*cinfo.output_width; i+=3)
				{
					pix[off+i]   = buffer[0][i+2];
					pix[off+i+1] = buffer[0][i+1];
					pix[off+i+2] = buffer[0][i];
				}
			}
			else	// monocrom
			{
				memcpy(&pix[off],buffer[0],row_stride);
			}
		}

		// Step 7: Finish decompression
		jpeg_finish_decompress(&cinfo);

		// Step 8: Release JPEG decompression object
		jpeg_destroy_decompress(&cinfo);

		fclose(infile);

		return pCVIm;

	}
	catch (CVException)
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		throw;
	}

}

void RGBA_to_BGRA(BYTE* data,int width,int height)
{
	int   i;
	int   j;
	int   line_width;
	BYTE  r, g, b, a;
	BYTE* ptr;

	ptr = data;
	line_width = width * 4;

	for(i = 0; i < height; i++)
	{
		ptr = data + line_width*i;
		for(j = 0; j < width; j++)
		{
			r = ptr[0];
			g = ptr[1];
			b = ptr[2];
			a = ptr[3];
//			ptr[2] = (BYTE)( (r*a+1) >> 8 );
//			ptr[1] = (BYTE)( (g*a+1) >> 8 );
//			ptr[0] = (BYTE)( (b*a+1) >> 8 );
			ptr[2] = r;
			ptr[1] = g;
			ptr[0] = b;
			ptr += 4;
		}
	}

	return;
}

void BGRA_to_RGBA(BYTE* data,int width,int height, int pad)
{
	int   i;
	int   j;
	int   line_width;
	BYTE  r, g, b, a;
	BYTE* ptr;

	ptr = data;
	line_width = width * 4;

	for(i = 0; i < height; i++)
	{
		ptr = data + line_width*i;
		for(j = 0; j < width; j++)
		{
			b = ptr[0];
			g = ptr[1];
			r = ptr[2];
			a = ptr[3];
			ptr[0] = r;
			ptr[1] = g;
			ptr[2] = b;
			ptr += 4;
		}
	}

	return;
}



// CVImage::Draw ===============================================================
// Dibujar una imagen dentro del rectángulo del DC (x0,y0)-(x1,y1)

void CVImage::Draw(HDC hdc,int x0,int y0,int x1, int y1, int nStretchMode ) const
{
	struct {
		BITMAPINFO bmi; // Información de bitmap
		RGBQUAD pat[255]; // Espacio para la paleta
	} header;

	Get(header.bmi);
	for (int i=0; i<256;i++) {
		header.bmi.bmiColors[i].rgbBlue=i;
		header.bmi.bmiColors[i].rgbGreen=i;
		header.bmi.bmiColors[i].rgbRed=i;
		header.bmi.bmiColors[i].rgbReserved=0;
	}
	SetStretchBltMode(hdc, nStretchMode);
	int res=::StretchDIBits(hdc,
		x0,
		y0,
		x1-x0, // width of destination rectangle
		y1-y0, // height of destination rectangle
		0, 0,
		header.bmi.bmiHeader.biWidth,
		header.bmi.bmiHeader.biHeight,
		m_pRaster,
		&header.bmi,
		DIB_RGB_COLORS, SRCCOPY);
	/*
	CVMessageFBox("%d (%d %d) (%d %d) %dx%d %d %d %d %d",res,x0,y0,x1-x0,y1-y0,header.bmi.bmiHeader.biWidth,header.bmi.bmiHeader.biHeight,
		header.bmi.bmiHeader.biBitCount,
		header.bmi.bmiHeader.biPlanes,
		header.bmi.bmiHeader.biSizeImage,
		header.bmi.bmiHeader.biSize);
	*/
}

void CVImage::DrawRect(HDC hdc,int x0,int y0,int x1, int y1, int imX0, int imY0, int imX1, int imY1, int nStretchMode ) const
// Dibujar el corte de imagen (imX0,imY0)-(imX1,imY1) imagen dentro del rectángulo del DC (x0,y0)-(x1,y1)
{
	struct {
		BITMAPINFO bmi; // Información de bitmap
		RGBQUAD pat[255]; // Espacio para la paleta
	} header;

	Get(header.bmi);
	for (int i=0; i<256;i++) {
		header.bmi.bmiColors[i].rgbBlue=i;
		header.bmi.bmiColors[i].rgbGreen=i;
		header.bmi.bmiColors[i].rgbRed=i;
		header.bmi.bmiColors[i].rgbReserved=0;
	}
	SetStretchBltMode(hdc, nStretchMode);
	::StretchDIBits(hdc,
		x0,
		y0,
		x1-x0, // width of destination rectangle
		y1-y0, // height of destination rectangle
		imX0, imY0,
		imX1-imX0,
		imY1-imY0,
		m_pRaster,
		&header.bmi,
		DIB_RGB_COLORS, SRCCOPY);
}

// Dibujar una imagen a partir de las coordendas (x0,y0) del DC
void CVImage::Draw(HDC hdc,int x0,int y0, int nStretchMode ) const
{
	struct {
		BITMAPINFO bmi; // Información de bitmap
		RGBQUAD pat[255]; // Espacio para la paleta
	} header;

	Get(header.bmi);
	for (int i=0; i<256;i++) {
		header.bmi.bmiColors[i].rgbBlue=i;
		header.bmi.bmiColors[i].rgbGreen=i;
		header.bmi.bmiColors[i].rgbRed=i;
		header.bmi.bmiColors[i].rgbReserved=0;
	}

	SetStretchBltMode(hdc, nStretchMode);
	::SetDIBitsToDevice(hdc,
		x0,
		y0,
		header.bmi.bmiHeader.biWidth,
		header.bmi.bmiHeader.biHeight,
		0,0,
		0,header.bmi.bmiHeader.biHeight,
		m_pRaster,
		&header.bmi,
		DIB_RGB_COLORS);
}

// CVImage::DrawEx =============================================
// Dibuja la imagen dentro del rectángulo del DC (x0,y0)-(x1,y1) 
// manteniendo el aspect ratio y limitando la ampliación máxima

void CVImage::DrawEx(HDC hdc,int x0,int y0,int x1, int y1,
					 bool AspectRatio,double MaxEsc, int nStretchMode) const
{
	struct {
		BITMAPINFO bmi; // Información de bitmap
		RGBQUAD pat[255]; // Espacio para la paleta
	} header;

	Get(header.bmi);
	for (int i=0; i<256;i++) {
		header.bmi.bmiColors[i].rgbBlue=i;
		header.bmi.bmiColors[i].rgbGreen=i;
		header.bmi.bmiColors[i].rgbRed=i;
		header.bmi.bmiColors[i].rgbReserved=0;
	}
	SetStretchBltMode(hdc, nStretchMode);
	double escX,escY;
	escX=(double)(x1-x0)/header.bmi.bmiHeader.biWidth;
	escY=(double)(y1-y0)/header.bmi.bmiHeader.biHeight;
	if (AspectRatio) {
		escX=min(escY,escX);
		escY=escX;
	}
	if (MaxEsc>0.0) {
		escX=min(escX,MaxEsc);
		escY=min(escY,MaxEsc);
	}
	x1=int(x0+escX*header.bmi.bmiHeader.biWidth);
	y1=int(y0+escY*header.bmi.bmiHeader.biHeight);
	::StretchDIBits(hdc,
		x0,
		y0,
		x1-x0, // width of destination rectangle
		y1-y0, // height of destination rectangle
		0, 0,
		header.bmi.bmiHeader.biWidth,
		header.bmi.bmiHeader.biHeight,
		m_pRaster,
		&header.bmi,
		DIB_RGB_COLORS, SRCCOPY);
}

// FlipX =======================================================================

void CVImage::FlipX()
{
    int lsz=m_szX*m_Canales;
    lsz=4*(lsz/4+ ((lsz & 3)!=0)); // Tamaño línea

	unsigned char *p=m_pRaster;
	unsigned char *lim=m_pRaster+lsz*m_szY;
    if (m_Canales==1) {
		while (p<lim) {
			unsigned char *p0=p;
			unsigned char *p1=p+m_szX-1;
			while (p0<p1) {
				unsigned char tmp=*p0;
				*p0=*p1;
				*p1=tmp;
				++p0;
				--p1;
			}
			p+=lsz;
		}
	}
	else {
		while (p<lim) {
			unsigned char *p0=p;
			unsigned char *p1=p+m_szX*3-3;
			while (p0<p1) {
				unsigned char tmp=*p0;
				*p0=*p1;
				*p1=tmp;
				tmp=p0[1];
				p0[1]=p1[1];
				p1[1]=tmp;
				tmp=p0[2];
				p0[2]=p1[2];
				p1[2]=tmp;
				p0+=3;
				p1-=3;
			}
			p+=lsz;
		}
	}
}

// FlipY =======================================================================

void CVImage::FlipY()
{
    int lsz=m_szX*m_Canales;
    lsz=4*(lsz/4+ ((lsz & 3)!=0)); // Tamaño línea

	unsigned char *p0=m_pRaster;
	unsigned char *p1=m_pRaster+lsz*(GetszY()-1);
	while (p0<p1) {
		for (int x=0;x<lsz;++x) {
			unsigned char tmp=*p0;
			*p0=*p1;
			*p1=tmp;
			++p0;
			++p1;
		}
		p1-=lsz*2;
	}
}

// Rotate180 ====================================================================

void CVImage::Rotate180()
{
	/*
	FlipX();
	FlipY();
	*/
	int lsz=m_szX*m_Canales;
	lsz=4*(lsz/4+ ((lsz & 3)!=0)); // Tamaño línea
	if (m_Canales==1) {		
		unsigned char *p00=m_pRaster;
		unsigned char *p11=m_pRaster+lsz*(GetszY()-1)+m_szX-1;
		unsigned char *plim=m_pRaster+lsz*((GetszY()-1)/2);
		while (p00<=plim) {
			unsigned char *p0=p00;
			unsigned char *p1=p11;
			for (int x=0;x<lsz;++x) {
				unsigned char tmp=*p0;
				*p0=*p1;
				*p1=tmp;
				++p0;
				--p1;
			}
			p00+=lsz;
			p11-=lsz;
		}
	}
	else {
		unsigned char *p00=m_pRaster;
		unsigned char *p11=m_pRaster+lsz*(GetszY()-1)+(m_szX-1)*3;
		unsigned char *plim=m_pRaster+lsz*((GetszY()-1)/2);		
		while (p00<plim) {
			unsigned char *p0=p00;
			unsigned char *p1=p11;
			for (int x=0;x<lsz;++x) {
				unsigned char tmp=*p0;
				*p0=*p1;
				*p1=tmp;
				tmp=p0[1];
				p0[1]=p1[1];
				p1[1]=tmp;
				tmp=p0[2];
				p0[2]=p1[2];
				p1[2]=tmp;

				p0+=3;
				p1-=3;
			}
			p00+=lsz;
			p11-=lsz;
		}
	}
}


// CopyRotateRight =============================================================

void CVImage::CopyRotateRight(void* raster)
{
    int lsz=GetLineSize();
	int rlsz=m_szY*m_Canales;
    rlsz=4*(rlsz/4+ ((rlsz & 3)!=0)); // Tamaño línea
	if (m_Canales==1) {
		for (int y=0;y<m_szY;++y) {
			unsigned char* ps=((unsigned char*) m_pRaster)+y*lsz;
			unsigned char* pd=((unsigned char*) raster)+(m_szX-1)*rlsz+y;
			for (int x=0;x<m_szX;++x) {
				*pd=*ps;
				pd-=rlsz;
				ps++;
			}
		}
	}
	else {
		for (int y=0;y<m_szY;++y) {
			unsigned char* ps=((unsigned char*) m_pRaster)+y*lsz;
			unsigned char* pd=((unsigned char*) raster)+(m_szX-1)*rlsz+y*3;
			for (int x=0;x<m_szX;++x) {
				*pd=*ps;
				pd[1]=ps[1];
				pd[2]=ps[2];
				pd-=rlsz;
				ps+=3;
			}
		}
	}
}

// CopyRotateRight =============================================================

CVImage* CVImage::RotateRight()
{
	CVImage *pCVIm=new CVImage(m_pNombre,m_szY,m_szX,m_Canales,m_OrigenY,m_OrigenX);
    int lsz=GetLineSize();
	int rlsz=m_szY*m_Canales;
    rlsz=4*(rlsz/4+ ((rlsz & 3)!=0)); // Tamaño línea
	if (m_Canales==1) {
		for (int y=0;y<m_szY;++y) {
			unsigned char* ps=m_pRaster+y*lsz;
			unsigned char* pd=pCVIm->m_pRaster+(m_szX-1)*rlsz+y;
			for (int x=0;x<m_szX;++x) {
				*pd=*ps;
				pd-=rlsz;
				ps++;
			}
		}
	}
	else {
		for (int y=0;y<m_szY;++y) {
			unsigned char* ps=m_pRaster+y*lsz;
			unsigned char* pd=pd=pCVIm->m_pRaster+(m_szX-1)*rlsz+y*3;
			for (int x=0;x<m_szX;++x) {
				*pd=*ps;
				pd[1]=ps[1];
				pd[2]=ps[2];
				pd-=rlsz;
				ps+=3;
			}
		}
	}
	return pCVIm;
}

// CopyRotateLeft =============================================================

void CVImage::CopyRotateLeft(void* raster)
{
    int lsz=GetLineSize();
	int rlsz=m_szY*m_Canales;
    rlsz=4*(rlsz/4+ ((rlsz & 3)!=0)); // Tamaño línea
	if (m_Canales==1) {
		for (int y=0;y<m_szY;++y) {
			unsigned char* ps=((unsigned char*) m_pRaster)+y*lsz;
			unsigned char* pd=((unsigned char*) raster)+(m_szY-y-1);
			for (int x=0;x<m_szX;++x) {
				*pd=*ps;
				pd+=rlsz;
				ps++;
			}
		}
	}
	else {
		for (int y=0;y<m_szY;++y) {
			unsigned char* ps=((unsigned char*) m_pRaster)+y*lsz;
			unsigned char* pd=((unsigned char*) raster)+(m_szY-y-1)*3;
			for (int x=0;x<m_szX;++x) {
				*pd=*ps;
				pd[1]=ps[1];
				pd[2]=ps[2];
				pd+=rlsz;
				ps+=3;
			}
		}
	}
}

// RotateLeft =============================================================

CVImage* CVImage::RotateLeft()
{
	CVImage *pCVIm=new CVImage(m_pNombre,m_szY,m_szX,m_Canales,m_OrigenY,m_OrigenX);
    int lsz=GetLineSize();
	int rlsz=m_szY*m_Canales;
    rlsz=4*(rlsz/4+ ((rlsz & 3)!=0)); // Tamaño línea
	if (m_Canales==1) {
		for (int y=0;y<m_szY;++y) {
			unsigned char* ps=m_pRaster+y*lsz;
			unsigned char* pd=pCVIm->m_pRaster+(m_szY-y-1);
			for (int x=0;x<m_szX;++x) {
				*pd=*ps;
				pd+=rlsz;
				ps++;
			}
		}
	}
	else {
		for (int y=0;y<m_szY;++y) {
			unsigned char* ps=m_pRaster+y*lsz;
			unsigned char* pd=pCVIm->m_pRaster+(m_szY-y-1)*3;
			for (int x=0;x<m_szX;++x) {
				*pd=*ps;
				pd[1]=ps[1];
				pd[2]=ps[2];
				pd+=rlsz;
				ps+=3;
			}
		}
	}
	return pCVIm;
}


