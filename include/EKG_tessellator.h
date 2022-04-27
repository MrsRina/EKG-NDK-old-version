/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_includes.h"
#include "EKG_util.h"

/**
 * Char data used in font renderer.
 */
struct EKG_CharData {
    /* Values that reference positions in bitmap. */
    float StoreX, W, H, TextureX, TextureLeft, TextureTop;
};

/**
 * Render fast shapes, lines, points using modern way (VBO, shaders...).
 */
class EKG_Tessellator {
private:
    /* Settings of current buffer. */
    int BufferSize;
    int RenderType;

    /* Metrics of bind texture. */
    float TextureWidth, TextureHeight;

    /* Texture flag and id. */
    bool ContainsTexture;
    GLint TextureId;

    /* Data vertex and color (UV). */
    std::vector<GLfloat> VertexList, ColorList;

    /* VBOs id and attributes. */
    GLuint VertexBuffer, ColorBuffer, VertexAttribute, ColorAttribute;

    /* Extra color field used to texture. */
    EKG_Color TextureColor = EKG_Color(255, 255, 255, 255);
public:
    EKG_Tessellator() {}

    /* Start of main methods. */
    void Init();
    void Quit();
    /* End of main methods. */

    /* Start of action methods. */
    void Vertex(double X, double Y, double Z);
    void TextCoord2f(float U, float V);
    void Color(unsigned int R, unsigned int G, unsigned int B);
    void Color(unsigned int R, unsigned int G, unsigned int B, unsigned int A);
    void Color(EKG_Color Color);
    void BindTexture(GLuint Id);
    void BindTexture(const EKG_Texture &Texture);
    void NewDraw(int DrawType, int DrawSize);
    void Draw();
    /* End of actions methods. */

    /* Start of getters and setters. */
    float GetTextureWidth();
    float GetTextureHeight();

    void SetRectColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A);
    void SetRectColor(EKG_Color Color);

    void SetVertex(const std::vector<GLfloat> &NewVertexList);
    void SetUV(const std::vector<GLfloat> &UV);
    /* End of getters and setters. */
};

/**
 * Render font used in EKG.
 */
class EKG_FontRenderer {
    /* Start of FT main variables. */
    FT_Library Library;
    FT_Face Face;
    FT_GlyphSlot GlyphSlot;
    FT_Bool UseKerning;
    FT_UInt Previous;
    /* End of FT main variables. */

    /* Metrics of bitmap texture. */
    float TextureWidth, TextureHeight;

    /* Chars of bitmap list. */
    EKG_CharData Chars[128];

    /* Bitmap texture id. */
    unsigned int BitmapTextureId;

    /* Path and font size. */
    std::string FontPath;
    unsigned int Size;
public:
    /* Start of main methods. */
    void Init();
    void Reload();
    void Quit();

    void DrawString(const std::string& String, float X, float Y, const EKG_Color &Color);
    void DrawStringClamped(const std::string &String, float X, float Y, float W, const EKG_Color &Color);
    /* End of main methods. */

    /* Start of setters and getters. */
    void SetFontPath(const std::string &Path);
    std::string GetFontPath();

    void SetFontSize(unsigned int FontSize);
    unsigned int GetFontSize();

    float GetStringWidth(const std::string &String);
    float GetStringHeight(const std::string &String);
    /* End of setters and getters. */
};