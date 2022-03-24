#include "EKG.h"

EKG_Core* const EKG_CORE = new EKG_Core();
EKG_Tessellator* const EKG_TESSELLATOR = new EKG_Tessellator();

void EKG_StartUseShader(GLuint ShaderId) {
    glUseProgram(ShaderId);
}

void EKG_EndUseShader() {
    glUseProgram(0);
}

void EKG_ShaderProjMatrix(unsigned int ShaderId, float *ProjMatrix) {
    glUseProgram(ShaderId);
    glUniformMatrix4fv(glGetUniformLocation(ShaderId, "ProjMatrix"), 1, GL_FALSE, ProjMatrix);
    glUseProgram(0);
}

GLuint EKG_GetShaderAttribute(const std::string &ShaderName, unsigned int ShaderId, const std::string &AttributeName) {
    GLint Attribute = glGetAttribLocation(ShaderId, AttributeName.c_str());

    if (Attribute == -1) {
        EKG_Log("Unknown attribute in shader: " + ShaderName + " '" + AttributeName + "' (attribute)!");
        return 0;
    }

    return (GLuint) Attribute;
}

void EKG_SetShaderUniformBool(unsigned int ShaderId, const std::string &Name, bool Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform1i(Flag, (int) Value);
}

void EKG_SetShaderUniformInt(unsigned int ShaderId, const std::string &Name, int Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform1i(Flag, Value);
}

void EKG_SetShaderUniformFloat(unsigned int ShaderId, const std::string &Name, float Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform1f(Flag, Value);
}

void EKG_SetShaderUniformVec2(unsigned int ShaderId, const std::string &Name, const glm::vec2 &Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform2f(Flag, 1, Value[0]);
}

void EKG_SetShaderUniformVec2(unsigned int ShaderId, const std::string &Name, float X, float Y) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform2f(Flag, X, Y);
}

void EKG_SetShaderUniformVec3(unsigned int ShaderId, const std::string &Name, const glm::vec3 &Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform3fv(Flag, 1, &Value[0]);
}

void EKG_SetShaderUniformVec3(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform3f(Flag, X, Y, Z);
}

void EKG_SetShaderUniformVec4(unsigned int ShaderId, const std::string &Name, const glm::vec4 &Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform4fv(Flag, 1, &Value[0]);
}

void EKG_SetShaderUniformVec4(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z, float W) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform4f(Flag, X, Y, Z, W);
}

void EKG_SetShaderUniformMat2(unsigned int ShaderId, const std::string &Name, const glm::mat2 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniformMatrix2fv(Flag, 1, GL_FALSE, &Mat[0][0]);
}

void EKG_SetShaderUniformMat3(unsigned int ShaderId, const std::string &Name, const glm::mat3 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniformMatrix3fv(Flag, 1, GL_FALSE, &Mat[0][0]);
}

void EKG_SetShaderUniformMat4(unsigned int ShaderId, const std::string &Name, const glm::mat4 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniformMatrix4fv(Flag, 1, GL_FALSE, &Mat[0][0]);
}

bool EKG_FlagContains(unsigned int Flags, unsigned int Flag) {
    return Flags & (Flag);
}

void EKG_FlagRemove(unsigned int &Flags, unsigned int Flag) {
    Flags &= ~(Flag);
}

void EKG_FlagAdd(unsigned int &Flags, unsigned int Flag) {
    Flags |= Flag;
}

void EKG_ColorTheme::Frame(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue) {
    switch (Flag) {
        case BACKGROUND: {
            this->FrameBackground[0] = RedValue;
            this->FrameBackground[1] = GreenValue;
            this->FrameBackground[2] = BlueValue;
            this->FrameBackground[3] = AlphaValue;
            break;
        }

        case HIGHLIGHT: {
            this->FrameHighlight[0] = RedValue;
            this->FrameHighlight[1] = GreenValue;
            this->FrameHighlight[2] = BlueValue;
            this->FrameHighlight[3] = AlphaValue;
            break;
        }

        case FOCUSED: {
            this->FrameFocused[0] = RedValue;
            this->FrameFocused[1] = GreenValue;
            this->FrameFocused[2] = BlueValue;
            this->FrameFocused[3] = AlphaValue;
            break;
        }

        case PRESSED: {
            this->FramePressed[0] = RedValue;
            this->FramePressed[1] = GreenValue;
            this->FramePressed[2] = BlueValue;
            this->FramePressed[3] = AlphaValue;
            break;
        }

        case ACTIVY: {
            this->FrameActivy[0] = RedValue;
            this->FrameActivy[1] = GreenValue;
            this->FrameActivy[2] = BlueValue;
            this->FrameActivy[3] = AlphaValue;
            break;
        }
    }
}

void EKG_ColorTheme::Container(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue) {
    switch (Flag) {
        case BACKGROUND: {
            this->ContainerBackground[0] = RedValue;
            this->ContainerBackground[1] = GreenValue;
            this->ContainerBackground[2] = BlueValue;
            this->ContainerBackground[3] = AlphaValue;
            break;
        }

        case HIGHLIGHT: {
            this->ContainerHighlight[0] = RedValue;
            this->ContainerHighlight[1] = GreenValue;
            this->ContainerHighlight[2] = BlueValue;
            this->ContainerHighlight[3] = AlphaValue;
            break;
        }

        case FOCUSED: {
            this->ContainerFocused[0] = RedValue;
            this->ContainerFocused[1] = GreenValue;
            this->ContainerFocused[2] = BlueValue;
            this->ContainerFocused[3] = AlphaValue;
            break;
        }

        case PRESSED: {
            this->ContainerPressed[0] = RedValue;
            this->ContainerPressed[1] = GreenValue;
            this->ContainerPressed[2] = BlueValue;
            this->ContainerPressed[3] = AlphaValue;
            break;
        }

        case ACTIVY: {
            this->ContainerActivy[0] = RedValue;
            this->ContainerActivy[1] = GreenValue;
            this->ContainerActivy[2] = BlueValue;
            this->ContainerActivy[3] = AlphaValue;
            break;
        }
    }
}

void EKG_ColorTheme::Widget(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue)  {
    switch (Flag) {
        case BACKGROUND: {
            this->WidgetBackground[0] = RedValue;
            this->WidgetBackground[1] = GreenValue;
            this->WidgetBackground[2] = BlueValue;
            this->WidgetBackground[3] = AlphaValue;
            break;
        }

        case HIGHLIGHT: {
            this->WidgetHighlight[0] = RedValue;
            this->WidgetHighlight[1] = GreenValue;
            this->WidgetHighlight[2] = BlueValue;
            this->WidgetHighlight[3] = AlphaValue;
            break;
        }

        case FOCUSED: {
            this->WidgetFocused[0] = RedValue;
            this->WidgetFocused[1] = GreenValue;
            this->WidgetFocused[2] = BlueValue;
            this->WidgetFocused[3] = AlphaValue;
            break;
        }

        case PRESSED: {
            this->WidgetPressed[0] = RedValue;
            this->WidgetPressed[1] = GreenValue;
            this->WidgetPressed[2] = BlueValue;
            this->WidgetPressed[3] = AlphaValue;
            break;
        }

        case ACTIVY: {
            this->WidgetActivy[0] = RedValue;
            this->WidgetActivy[1] = GreenValue;
            this->WidgetActivy[2] = BlueValue;
            this->WidgetActivy[3] = AlphaValue;
            break;
        }
    }
}

void EKG_ColorTheme::OnInit() {
    Frame(BACKGROUND, 255, 255, 255, 50);
    Frame(HIGHLIGHT, 0, 0, 0, 0);
    Frame(PRESSED, 255, 255, 255, 50);
    Frame(FOCUSED, 0, 0, 0, 0);
    Frame(ACTIVY, 0, 0, 0, 0);

    Container(BACKGROUND, 255, 0, 0, 50);
    Container(HIGHLIGHT, 0, 0, 0, 0);
    Container(PRESSED, 0, 0, 0, 0);
    Container(FOCUSED, 0, 0, 0, 0);
    Container(ACTIVY, 0, 0, 0, 0);

    Widget(BACKGROUND, 0, 0, 0, 100);
    Widget(HIGHLIGHT, 255, 0, 255, 50);
    Widget(PRESSED, 255, 0, 255, 100);
    Widget(FOCUSED, 0, 0, 0, 0);
    Widget(ACTIVY, 255, 255, 255, 50);
}

bool EKG_ColorTheme::IsWireframeFrameEnabled() {
    return this->WireframeFrame;
}

bool EKG_ColorTheme::IsWireframeButtonEnabled() {
    return this->WireframeFrame;
}

bool EKG::ContextOkay = false;

float EKG::DeviceScreenWidth = 0.0F;
float EKG::DeviceScreenHeight = 0.0F;

void EKG::Init() {
    EKG_CORE->Init();
    EKG_TESSELLATOR->Init();

    // Get device screen size.
    SDL_DisplayMode DisplayMode;
    SDL_GetDisplayMode(0, 0, &DisplayMode);

    EKG::DeviceScreenWidth = (float) DisplayMode.w;
    EKG::DeviceScreenHeight = (float) DisplayMode.h;

    // Call it was initialized property (or just initialized).
    EKG_Log("Core initialized.");
}

void EKG::Quit() {
    EKG_CORE->Quit();
    EKG_TESSELLATOR->Quit();

    // Say it was quited successfully or just quit.
    EKG_Log("Core quited successfully.");
}

void EKG::Event(SDL_Event Event) {

}

void EKG::Update(float DeltaTicks) {
    EKG_CORE->OnUpdate(DeltaTicks);
}

void EKG::Render(float PartialTicks) {
    EKG_CORE->OnRender(PartialTicks);
}

void EKG::InitFont(const std::string &Path, unsigned int InitialSize) {
    EKG_CORE->FontRenderer.SetFontPath(Path);
    EKG_CORE->FontRenderer.SetFontSize(InitialSize);
}

void EKG::Context() {
    float Viewport[4];
    glGetFloatv(GL_VIEWPORT, Viewport);

    float ProjectionMatrix4x4[16];
    EKG_Ortho2D(ProjectionMatrix4x4, 0.0F, Viewport[2], Viewport[3], 0.0F);

    // We set projection ortho.
    EKG_ShaderProjMatrix(EKG_CORE->ShaderManager.GetTessellatorShader(), ProjectionMatrix4x4);
}

EKG_FontRenderer EKG::GetFontRenderer() {
    return EKG_CORE->FontRenderer;
}

EKG_ShaderManager EKG::GetShaderManager() {
    return EKG_CORE->ShaderManager;
}

EKG::Dock EKG::PointCollideDock(unsigned int Flags, float PointX, float PointY, float MinOffset, float MaxOffset, const EKG_Rect &Rect) {
    EKG_Rect DockRect {};

    Dock DockColliding = Dock::UNKNOWN;
    Dock DockType = Dock::FULL;

    if (EKG_FlagContains(Flags, DockType)) {
        DockRect = GetRectDock(DockType, MinOffset, MaxOffset, Rect);
        return DockRect.CollideWithPoint(PointX, PointY) ? DockType : DockColliding;
    }

    DockType = Dock::TOP;

    if (EKG_FlagContains(Flags, DockType)) {
        DockRect = GetRectDock(DockType, MinOffset, MaxOffset, Rect);
        DockColliding = DockRect.CollideWithPoint(PointX, PointY) ? DockType : DockColliding;
    }

    DockType = Dock::BOTTOM;

    if (EKG_FlagContains(Flags, DockType)) {
        DockRect = GetRectDock(DockType, MinOffset, MaxOffset, Rect);
        DockColliding = DockRect.CollideWithPoint(PointX, PointY) ? DockType : DockColliding;
    }

    DockType = Dock::LEFT;

    if (EKG_FlagContains(Flags, DockType)) {
        DockRect = GetRectDock(DockType, MinOffset, MaxOffset, Rect);
        DockColliding = DockRect.CollideWithPoint(PointX, PointY) ? DockType : DockColliding;
    }

    DockType = Dock::RIGHT;

    if (EKG_FlagContains(Flags, DockType)) {
        DockRect = GetRectDock(DockType, MinOffset, MaxOffset, Rect);
        DockColliding = DockRect.CollideWithPoint(PointX, PointY) ? DockType : DockColliding;
    }

    DockType = Dock::CENTER;

    if (EKG_FlagContains(Flags, DockType)) {
        DockRect = GetRectDock(DockType, MinOffset, MaxOffset, Rect);
        DockColliding = DockRect.CollideWithPoint(PointX, PointY) ? DockType : DockColliding;
    }

    return DockColliding;
}

EKG_Rect EKG::GetRectDock(EKG::Dock p_Dock, float InitialOffset, float SizeOffset, const EKG_Rect &Origin) {
    EKG_Rect Rect;

    // Pos rect.
    float X = 0.0F;
    float Y = 0.0F;

    // Size rect.
    float W = 0.0F;
    float H = 0.0F;

    // Offset positions are normalized.
    float InitialOffsetFixed;
    float SizeOffsetFixed;

    // Division stuff.
    float Division;

    switch (p_Dock) {
        case FULL: {
            InitialOffsetFixed = InitialOffset == 0 ? InitialOffset : InitialOffset * Origin.W;
            SizeOffsetFixed = SizeOffset == 0 ? SizeOffset : SizeOffset * Origin.H;

            X = Origin.X + InitialOffsetFixed;
            Y = Origin.Y + SizeOffsetFixed;
            W = Origin.W - (InitialOffsetFixed * 2);
            H = Origin.H - (SizeOffsetFixed * 2);
            break;
        }

        case LEFT: {
            Division = Origin.W == 0 ? 0 : (Origin.W / 2.0F);

            InitialOffsetFixed = InitialOffset == 0 ? InitialOffset : InitialOffset * (Division);
            SizeOffsetFixed = SizeOffset == 0 ? SizeOffset : SizeOffset * (Division);

            X = Origin.X + InitialOffsetFixed;
            Y = Origin.Y;
            W = Division - SizeOffsetFixed;
            H = Origin.H;
            break;
        }

        case RIGHT: {
            Division = Origin.W == 0 ? 0 : (Origin.W / 2.0F);

            InitialOffsetFixed = InitialOffset == 0 ? InitialOffset : InitialOffset * Division;
            SizeOffsetFixed = SizeOffset == 0 ? SizeOffset : SizeOffset * Division;

            Y = Origin.Y;
            H = Origin.H;

            W = Division - SizeOffsetFixed;
            X = Origin.X + InitialOffsetFixed + Origin.W - W;
            break;
        }

        case TOP: {
            Division = Origin.H == 0 ? 0 : (Origin.H / 2.0F);

            InitialOffsetFixed = InitialOffset == 0 ? InitialOffset : InitialOffset * Division;
            SizeOffsetFixed = SizeOffset == 0 ? SizeOffset : SizeOffset * Division;

            X = Origin.X;
            Y = Origin.Y + InitialOffsetFixed;
            W = Origin.W;
            H = Division - SizeOffsetFixed;
            break;
        }

        case BOTTOM: {
            Division = Origin.H == 0 ? 0 : (Origin.H / 2.0F);

            InitialOffsetFixed = InitialOffset == 0 ? InitialOffset : InitialOffset * Division;
            SizeOffsetFixed = SizeOffset == 0 ? SizeOffset : SizeOffset * Division;

            X = Origin.X;
            W = Origin.W;

            H = Division - SizeOffsetFixed;
            Y = Origin.Y + InitialOffsetFixed + Origin.H - H;
            break;
        }

        case CENTER: {
            Division = Origin.W == 0 ? 0 : (Origin.W / 2.0F);
            InitialOffsetFixed = InitialOffset == 0 ? InitialOffset : InitialOffset * Division;
            SizeOffsetFixed = SizeOffset == 0 ? SizeOffset : SizeOffset * Division;

            X = Origin.X + (Division / 2.0F) - InitialOffsetFixed;
            W = Division - SizeOffsetFixed;

            // Center height segment code.
            Division = Origin.H == 0 ? 0 : (Origin.H / 2.0F);
            InitialOffsetFixed = InitialOffset == 0 ? InitialOffset : InitialOffset * Division;
            SizeOffsetFixed = SizeOffset == 0 ? SizeOffset : SizeOffset * Division;

            Y = Origin.Y + (Division / 2.0F) - InitialOffsetFixed;
            W = Division - SizeOffsetFixed;
        }
        case UNKNOWN: {
            break;
        }
    }

    Rect.X = X;
    Rect.Y = Y;
    Rect.W = W;
    Rect.H = H;

    return Rect;
}

void EKG::ScaledFingerPos(float &X, float &Y) {
    X *= EKG::DeviceScreenWidth;
    Y *= EKG::DeviceScreenHeight;
}

EKG_Frame *EKG::Frame(const std::string &Name, float InitialPosX, float InitialPosY, float InitialSizeWidth, float InitialSizeHeight) {
    auto* Element = new EKG_Frame();

    Element->SetTag(Name);
    Element->SetId(EKG_CORE->NewId());
    Element->SetX(InitialPosX);
    Element->SetY(InitialPosY);
    Element->SetWidth(InitialSizeWidth);
    Element->SetHeight(InitialSizeHeight);
    Element->SyncPos();
    Element->SyncSize();

    return Element;
}
