

#pragma once

#include <exception>
using namespace std;

// =============================================================================
// CVException =================================================================
// =============================================================================


class CVException : public exception {
public:
private:
	// Campos
	char m_Message[256];
protected:
	void SetMessage(const char *Format, va_list args);
public:
	//CVException() {}
	CVException(const char *Format, ...);
	CVException(const CVException& rhs);
	virtual const char *what() const throw() { return m_Message; }
};

// =============================================================================
// CVImage =====================================================================
// =============================================================================

class CVImage {
private:
	unsigned m_szX,m_szY,m_Canales; // Tamaño de la imagen
	unsigned char *m_pRaster; // pixeles de la imagen (formato BMP de windows)
	int m_OrigenX,m_OrigenY; // Origen de coordenadas de la imagen
	char *m_pNombre; // Nombre de la imagen
public:
	CVImage(const char *nombre,
		unsigned szX,unsigned szY,unsigned cha,int OrigenX=0,int OrigenY=0);
	~CVImage();
    CVImage(const CVImage &im);
	const char *GetName() const { return m_pNombre; }
	unsigned char *GetRaster() const {return m_pRaster; }
    int GetRasterSize() const ;
    int GetLineSize() const ;
    void CopyToBMPRaster(void *raster) const;

	void FlipY();
	void FlipX();
	void Rotate180();
	CVImage *RotateRight();
	CVImage *RotateLeft();
	void CopyRotateRight(void* raster);
	void CopyRotateLeft(void* raster);
	
	int GetOrigenX() const { return m_OrigenX; }
	int GetOrigenY() const { return m_OrigenY; }
	void SetOrigen(int x, int y) { m_OrigenX = x; m_OrigenY = y; }
	int GetszX() const { return m_szX; }
	int GetszY() const { return m_szY; }
	int Width() const { return m_szX; }
	int Height() const { return m_szY; }
	int GetCha() const { return m_Canales; }
	CVImage* GetChannel(int ch) const;
	void Get(BITMAPINFO &bmi) const;
	
	void Write(const char *Name) const;
	void WriteBMP(const char *Name) const;
	void WriteJpeg(const char *Name, int quality) const;
    
	void Draw(HDC hdc,int x0,int y0,int x1,int y1, int nStretchMode =COLORONCOLOR /*HALFTONE*/) const;
	void DrawRect(HDC hdc,int x0,int y0,int x1, int y1, int imX0, int imY0, int imX1, int imY1, int nStretchMode = COLORONCOLOR /*HALFTONE*/) const;
	void Draw(HDC hdc,int x0,int y0, int nStretchMode =COLORONCOLOR /*HALFTONE*/) const;
	void DrawEx(HDC hdc,int x0,int y0,int x1, int y1,
					 bool AspectRatio,double MaxEsc, int nStretchMode=COLORONCOLOR /*HALFTONE*/) const;
    void *operator new( size_t sz);
    void  operator delete (void*p);
    int GetPixel(int x,int y,int c) const;
	unsigned char* GetPixelAddress(int x,int y,int c) const;
	void Set(HDC hdc, HBITMAP bitmap);
	void SetImRaster(const unsigned char *raster);
};

CVImage *CVReadImage(const char *filename);
CVImage *CVReadImageBMP(const char *filename);
CVImage *CVReadImageJPEG(const char *filename);
