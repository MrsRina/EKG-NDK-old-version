
#include "EKG_tessellator.h"
#include "EKG_util.h"
#include "EKG.h"

void EKG_Tessellator::Init() {
    unsigned int TessellatorShaderId = EKG_CORE->ShaderManager.FindShader("Tessellator");
    EKG_StartUseShader(TessellatorShaderId);

    this->VertexList.push_back(0);
    this->VertexList.push_back(0);
    this->VertexList.push_back(0);

    // Position buffer.
    glGenBuffers(1, &this->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->VertexList.size(), &this->VertexList[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->ColorList.push_back(0);
    this->ColorList.push_back(0);
    this->ColorList.push_back(0);
    this->ColorList.push_back(0);

    // Color buffer.
    glGenBuffers(1, &this->ColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->ColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->ColorList.size(), &this->ColorList[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Fetch attribute from shader with object buffer.
    this->ColorAttribute = EKG_GetShaderAttribute("Tessellator", TessellatorShaderId, "VertexColor");
    this->VertexAttribute = EKG_GetShaderAttribute("Tessellator", TessellatorShaderId, "VertexPosition");

    EKG_EndUseShader();

    this->VertexList.clear();
    this->ColorList.clear();
}

void EKG_Tessellator::Draw() {
    unsigned int TessellatorShaderId = EKG_CORE->ShaderManager.GetTessellatorShader();

    // Use object shader.
    EKG_StartUseShader(TessellatorShaderId);
    EKG_SetShaderUniformBool(TessellatorShaderId, "ContainsTexture", this->ContainsTexture);

    // Call alpha channel.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (this->ContainsTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->TextureId);

        EKG_SetShaderUniformInt(TessellatorShaderId, "TextureSampler", 0);
        EKG_SetShaderUniformVec4(TessellatorShaderId, "DirectVertexColor", this->TextureColor.GetRedf(), this->TextureColor.GetGreenf(), this->TextureColor.GetBluef(), this->TextureColor.GetAlphaf());
    }

    // Enable vertex attribute for position color attribution.
    glEnableVertexAttribArray(this->VertexAttribute);
    glEnableVertexAttribArray(this->ColorAttribute);

    // Position buffer.
    glBindBuffer(GL_ARRAY_BUFFER, this->VertexBuffer);
    glVertexAttribPointer(this->VertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->VertexList.size(), &this->VertexList[0], GL_DYNAMIC_DRAW);

    // Color buffer.
    glBindBuffer(GL_ARRAY_BUFFER, this->ColorBuffer);
    glVertexAttribPointer(this->ColorAttribute, this->ContainsTexture ? 2 : 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->ColorList.size(), &this->ColorList[0], GL_DYNAMIC_DRAW);

    // Draw arrays.
    glDrawArrays(this->RenderType, 0, this->BufferSize);
    glDisableVertexAttribArray(this->VertexAttribute);
    glDisableVertexAttribArray(this->ColorAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (this->ContainsTexture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Un-use shader.
    EKG_EndUseShader();
}

void EKG_Tessellator::Vertex(double X, double Y, double Z) {
    this->VertexList.push_back(X);
    this->VertexList.push_back(Y);
    this->VertexList.push_back(Z);
}

void EKG_Tessellator::Color(unsigned int R, unsigned int G, unsigned int B) {
    if (this->ContainsTexture) {
        return;
    }

    this->ColorList.push_back(((float) R) / 255.0F);
    this->ColorList.push_back(((float) G) / 255.0F);
    this->ColorList.push_back(((float) B) / 255.0F);
    this->ColorList.push_back(1.0F);
}

void EKG_Tessellator::Color(unsigned int R, unsigned int G, unsigned int B, unsigned int A) {
    if (this->ContainsTexture) {
        return;
    }

    this->ColorList.push_back(((float) R) / 255.0F);
    this->ColorList.push_back(((float) G) / 255.0F);
    this->ColorList.push_back(((float) B) / 255.0F);
    this->ColorList.push_back(((float) A) / 255.0F);
}

void EKG_Tessellator::Color(EKG_Color Color) {
    if (this->ContainsTexture) {
        return;
    }

    this->ColorList.push_back(Color.GetRedf());
    this->ColorList.push_back(Color.GetBluef());
    this->ColorList.push_back(Color.GetGreenf());
    this->ColorList.push_back(Color.GetAlphaf());
}

void EKG_Tessellator::NewDraw(int DrawType, int DrawSize) {
    this->BufferSize = DrawSize;
    this->RenderType = DrawType;
    this->ContainsTexture = false;

    // Clean lists.
    this->VertexList.clear();
    this->ColorList.clear();
}

void EKG_Tessellator::SetRectColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A) {
    if (this->ContainsTexture) {
        this->TextureColor.Set(R, G, B, A);
    } else {
        // BearingY rect is 2 triangles (3 vertex by triangle).
        this->Color(R, G, B, A);
        this->Color(R, G, B, A);
        this->Color(R, G, B, A);
        this->Color(R, G, B, A);
        this->Color(R, G, B, A);
        this->Color(R, G, B, A);
    }
}

void EKG_Tessellator::Quit() {
    // Quit all buffers.
    glDeleteBuffers(1, &this->VertexBuffer);
    glDeleteBuffers(1, &this->ColorBuffer);
}

void EKG_Tessellator::BindTexture(GLuint Id) {
    this->TextureId = Id;
    this->ContainsTexture = true;
}

void EKG_Tessellator::TextCoord2f(float U, float V) {
    if (!this->ContainsTexture) {
        return;
    }

    this->ColorList.push_back(U);
    this->ColorList.push_back(V);
}

float EKG_Tessellator::GetTextureWidth() {
    if (this->ContainsTexture) {
        return this->TextureWidth;
    }

    return 0;
}

float EKG_Tessellator::GetTextureHeight() {
    if (this->ContainsTexture) {
        return this->TextureHeight;
    }

    return 0;
}

void EKG_Tessellator::BindTexture(const EKG_Texture &Texture) {
    this->TextureWidth = Texture.Width;
    this->TextureHeight = Texture.Height;

    this->BindTexture(Texture.Id);
}

void EKG_Tessellator::SetRectColor(EKG_Color Color) {
    this->SetRectColor(Color.R, Color.G, Color.B, Color.A);
}

void EKG_Tessellator::SetVertex(const std::vector<GLfloat> &NewVertexList) {
    this->VertexList = NewVertexList;
}

void EKG_Tessellator::SetUV(const std::vector<GLfloat> &UV) {
    this->ColorList = UV;
}

void EKG_FontRenderer::Init() {
    if (FT_Init_FreeType(&this->Library)) {
        EKG_Log("FT font not initialized.");
        return;
    }

    this->Reload();
}

void EKG_FontRenderer::SetFontPath(const std::string &Path) {
    this->FontPath = Path;
}

std::string EKG_FontRenderer::GetFontPath() {
    return this->FontPath;
}

void EKG_FontRenderer::SetFontSize(unsigned int FontSize) {
    this->Size = FontSize;
}

unsigned int EKG_FontRenderer::GetFontSize() {
    return this->Size;
}

void EKG_FontRenderer::Reload() {
    if (FT_New_Face(this->Library, this->FontPath.c_str(), 0, &this->Face)) {
        EKG_Log("Could not load font.");
        return;
    }

    FT_Set_Pixel_Sizes(this->Face, 0, this->Size);

    this->UseKerning = FT_HAS_KERNING(this->Face);
    this->GlyphSlot = this->Face->glyph;

    for (unsigned int I = 32; I < 128; I++) {
        if (FT_Load_Char(this->Face, I, FT_LOAD_RENDER)) {
            continue;
        }

        this->TextureWidth += (float) this->GlyphSlot->bitmap.width;
        this->TextureHeight = std::max(this->TextureHeight, float(this->GlyphSlot->bitmap.rows));
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create a bitmap texture for render in vbo.
    glGenTextures(1, &this->BitmapTextureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->BitmapTextureId);

    // Define image.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (int) this->TextureWidth, (int) this->TextureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, nullptr);

    float OffsetX = 0;

    // Generate bitmap using glTexSubImage2D.
    for (unsigned int I = 32; I < 128; I++) {
        if (FT_Load_Char(this->Face, I, FT_LOAD_RENDER)) {
            continue;
        }

        EKG_CharData Data {};

        Data.StoreX = float(OffsetX) / (float) this->TextureWidth;

        Data.W = (float) this->GlyphSlot->bitmap.width;
        Data.H = (float) this->GlyphSlot->bitmap.rows;

        Data.TextureLeft = this->GlyphSlot->bitmap_left;
        Data.TextureTop = this->GlyphSlot->bitmap_top;
        Data.TextureX = this->GlyphSlot->advance.x >> 6;

        glTexSubImage2D(GL_TEXTURE_2D, 0, OffsetX, 0, Data.W, Data.H, GL_ALPHA, GL_UNSIGNED_BYTE, this->GlyphSlot->bitmap.buffer);

        this->Chars[I] = Data;
        OffsetX += Data.W;
    }

    // Set parameters of texture.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind texture.
    glBindTexture(GL_TEXTURE_2D, 0);

    // Log that font renderer is successfully initialized.
    EKG_Log("Font renderer generated bitmap ok.");
}

void EKG_FontRenderer::DrawString(const std::string &String, float PositionX, float PositionY, const EKG_Color &Color) {
    std::vector<GLfloat> VertexVector, TextureVector;
    FT_Vector PreviousCharVector;

    float RenderX, RenderY, RenderW, RenderH;
    float TextureX, TextureY = 0.0F, TextureW, TextureH;

    const char* CharString = String.c_str();

    for (const char* I = CharString; *I; I++) {
        if (this->UseKerning && this->Previous && *I) {
            FT_Get_Kerning(this->Face, this->Previous, *I, 0, &PreviousCharVector);
            PositionX += (PreviousCharVector.x >> 6);
        }

        EKG_CharData Data = Chars[*I];

        RenderX = PositionX + (float) Data.TextureLeft;
        RenderY = PositionY + (this->TextureHeight - (float) Data.TextureTop);
        RenderW = Data.W;
        RenderH = Data.H;

        TextureX = Data.StoreX;
        TextureW = Data.W / (float) this->TextureWidth;
        TextureH = Data.H / (float) this->TextureHeight;

        float VertexList[] = {
                // First triangle.
                RenderX, RenderY, 0,
                RenderX, RenderY + RenderH, 0,
                RenderX + RenderW, RenderY + RenderH, 0,

                // Second triangle.
                RenderX + RenderW, RenderY + RenderH, 0,
                RenderX, RenderY, 0,
                RenderX + RenderW, RenderY, 0
        };

        float TextCoordList[] = {
                // First triangle.
                TextureX, TextureY,
                TextureX, TextureY + TextureH,
                TextureX + TextureW, TextureY + TextureH,

                // Second triangle.
                TextureX + TextureW, TextureY + TextureH,
                TextureX, TextureY,
                TextureX + TextureW, TextureY
        };

        VertexVector.insert(VertexVector.end(), VertexList, VertexList + 18);
        TextureVector.insert(TextureVector.end(), TextCoordList, TextCoordList + 12);

        PositionX += Data.TextureX;
        this->Previous = (int) *I;
    }

    // Draw string.
    EKG_TESSELLATOR->NewDraw(GL_TRIANGLES, 6 * (int) strlen(CharString));
    EKG_TESSELLATOR->BindTexture(this->BitmapTextureId);
    EKG_TESSELLATOR->SetRectColor(255 - Color.R, 255 - Color.G, 255 - Color.B, Color.A);
    EKG_TESSELLATOR->SetVertex(VertexVector);
    EKG_TESSELLATOR->SetUV(TextureVector);
    EKG_TESSELLATOR->Draw();
}

void EKG_FontRenderer::DrawStringClamped(const std::string &String, float PositionX, float PositionY, float W, const EKG_Color &Color) {
    std::vector<GLfloat> VertexVector, TextureVector;
    FT_Vector PreviousCharVector;

    float RenderX, RenderY, RenderW, RenderH, OriginX = PositionX;
    float TextureX, TextureY = 0.0F, TextureW, TextureH;

    const char* CharString = String.c_str();
    unsigned int CountChar = 0;

    for (const char* I = CharString; *I; I++) {
        if (this->UseKerning && this->Previous && *I) {
            FT_Get_Kerning(this->Face, this->Previous, *I, 0, &PreviousCharVector);
            PositionX += (PreviousCharVector.x >> 6);
        }

        EKG_CharData Data = Chars[*I];

        RenderX = PositionX + (float) Data.TextureLeft;
        RenderY = PositionY + (this->TextureHeight - (float) Data.TextureTop);
        RenderW = Data.W;
        RenderH = Data.H;

        if (RenderX >= OriginX + W) {
            continue;
        }

        CountChar++;

        TextureX = Data.StoreX;
        TextureW = Data.W / (float) this->TextureWidth;
        TextureH = Data.H / (float) this->TextureHeight;

        float VertexList[] = {
                // First triangle.
                RenderX, RenderY, 0,
                RenderX, RenderY + RenderH, 0,
                RenderX + RenderW, RenderY + RenderH, 0,

                // Second triangle.
                RenderX + RenderW, RenderY + RenderH, 0,
                RenderX, RenderY, 0,
                RenderX + RenderW, RenderY, 0
        };

        float TextCoordList[] = {
                // First triangle.
                TextureX, TextureY,
                TextureX, TextureY + TextureH,
                TextureX + TextureW, TextureY + TextureH,

                // Second triangle.
                TextureX + TextureW, TextureY + TextureH,
                TextureX, TextureY,
                TextureX + TextureW, TextureY
        };

        VertexVector.insert(VertexVector.end(), VertexList, VertexList + 18);
        TextureVector.insert(TextureVector.end(), TextCoordList, TextCoordList + 12);

        PositionX += Data.TextureX;
        this->Previous = (int) *I;
    }

    // Draw string.
    EKG_TESSELLATOR->NewDraw(GL_TRIANGLES, 6 * (int) CountChar);
    EKG_TESSELLATOR->BindTexture(this->BitmapTextureId);
    EKG_TESSELLATOR->SetRectColor(255 - Color.R, 255 - Color.G, 255 - Color.B, Color.A);
    EKG_TESSELLATOR->SetVertex(VertexVector);
    EKG_TESSELLATOR->SetUV(TextureVector);
    EKG_TESSELLATOR->Draw();
}

float EKG_FontRenderer::GetStringWidth(const std::string &String) {
    this->Previous = 0;
    FT_Vector Delta;

    float StartX = 0.0F;
    float RenderX = 0.0F;
    float StringWidth = 0.0F;

    for (const char* I = String.c_str(); *I; I++) {
        if (this->UseKerning && this->Previous && *I) {
            FT_Get_Kerning(this->Face, this->Previous, *I, 0, &Delta);
            StartX += (Delta.x >> 6);
        }

        EKG_CharData Data = Chars[*I];

        RenderX = StartX + (float) Data.TextureLeft;
        StartX += Data.TextureX;

        this->Previous = (int) *I;
        StringWidth = RenderX + Data.W;
    }

    return StringWidth;
}

float EKG_FontRenderer::GetStringHeight(const std::string &String) {
    this->Previous = 0;

    float RenderY;
    float Height = 0.0F;
    float StringHeight = 0.0F;

    for (const char* I = String.c_str(); *I; I++) {
        EKG_CharData Data;
        Data = Chars[*I];

        RenderY = (this->TextureHeight);
        this->Previous = (int) *I;

        StringHeight = RenderY + Data.H;

        if (StringHeight > Height) {
            Height = StringHeight;
        }
    }

    return StringHeight;
}

void EKG_FontRenderer::Quit() {
    FT_Done_Face(this->Face);
    FT_Done_FreeType(this->Library);
}
