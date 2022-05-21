#include "ekg.h"

ekg_core* const EKG_CORE = new ekg_core();
EKG_Tessellator* const EKG_TESSELLATOR = new EKG_Tessellator();

void ekg_start_use_shader(GLuint shader_id) {
    glUseProgram(shader_id);
}

void ekg_end_use_shader() {
    glUseProgram(0);
}

void ekg_shader_proj_matrix(unsigned int shader_id, float *proj_matrix) {
    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "proj_matrix"), 1, GL_FALSE, proj_matrix);
    glUseProgram(0);
}

GLuint ekg_get_shader_attrib(const std::string &shader_name, unsigned int shader_id, const std::string &attrib_name) {
    GLint Attribute = glGetAttribLocation(shader_id, attrib_name.c_str());

    if (Attribute == -1) {
        ekg_log("Unknown attribute in shader: " + shader_name + " '" + attrib_name +
                "' (attribute)!");
        return 0;
    }

    return (GLuint) Attribute;
}

void ekg_set_shader_uniform_bool(unsigned int shader_id, const std::string &name, bool value) {
    GLint Flag = glGetUniformLocation(shader_id, name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + name + "'.");
        return;
    }

    glUniform1i(Flag, (int) value);
}

void ekg_set_shader_uniform_int(unsigned int shader_id, const std::string &name, int value) {
    GLint Flag = glGetUniformLocation(shader_id, name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + name + "'.");
        return;
    }

    glUniform1i(Flag, value);
}

void ekg_set_shader_uniform_float(unsigned int shader_id, const std::string &name, float value) {
    GLint Flag = glGetUniformLocation(shader_id, name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + name + "'.");
        return;
    }

    glUniform1f(Flag, value);
}

void ekg_set_shader_uniform_vec2(unsigned int shader_id, const std::string &name, const glm::vec2 &value) {
    GLint Flag = glGetUniformLocation(shader_id, name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + name + "'.");
        return;
    }

    glUniform2f(Flag, 1, value[0]);
}

void ekg_set_shader_uniform_vec2(unsigned int shader_id, const std::string &name, float x, float y) {
    GLint Flag = glGetUniformLocation(shader_id, name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + name + "'.");
        return;
    }

    glUniform2f(Flag, x, y);
}

void ekg_set_shader_uniform_vec3(unsigned int ShaderId, const std::string &Name, const glm::vec3 &Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform3fv(Flag, 1, &Value[0]);
}

void ekg_set_shader_uniform_vec3(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform3f(Flag, X, Y, Z);
}

void ekg_set_shader_uniform_vec4(unsigned int ShaderId, const std::string &Name, const glm::vec4 &Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform4fv(Flag, 1, &Value[0]);
}

void ekg_set_shader_uniform_vec4(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z, float W) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform4f(Flag, X, Y, Z, W);
}

void ekg_set_shader_uniform_mat2(unsigned int ShaderId, const std::string &Name, const glm::mat2 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniformMatrix2fv(Flag, 1, GL_FALSE, &Mat[0][0]);
}

void ekg_set_shader_uniform_mat3(unsigned int ShaderId, const std::string &Name, const glm::mat3 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniformMatrix3fv(Flag, 1, GL_FALSE, &Mat[0][0]);
}

void ekg_set_shader_uniform_mat4(unsigned int ShaderId, const std::string &Name, const glm::mat4 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        ekg_log("Shader waring: Uniform location does not exist '" + Name + "'.");
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

float ekg::screen_width = 0.0F;
float ekg::screen_height = 0.0F;

void ekg::init() {
    EKG_CORE->init();
    EKG_TESSELLATOR->Init();

    // Get device screen size.
    SDL_DisplayMode DisplayMode;
    SDL_GetDisplayMode(0, 0, &DisplayMode);

    ekg::screen_width = (float) DisplayMode.w;
    ekg::screen_height = (float) DisplayMode.h;

    // Call it was initialized property (or just initialized).
    ekg_log("Core initialized.");
}

void ekg::quit() {
    EKG_CORE->quit();
    EKG_TESSELLATOR->Quit();

    // Say it was quited successfully or just quit.
    ekg_log("Core quited successfully.");
}

void ekg::poll_event(SDL_Event Event) {
    EKG_CORE->on_event(Event);
}

void ekg::update(const float &DeltaTicks) {
    EKG_CORE->on_update(DeltaTicks);
}

void ekg::render(const float &PartialTicks) {
    EKG_CORE->OnRender(PartialTicks);
}

void ekg::init_font(const std::string &path, unsigned int initial_size) {
    EKG_CORE->font_renderer.SetFontPath(path);
    EKG_CORE->font_renderer.SetFontSize(initial_size);
}

void ekg::context() {
    float viewport[4];
    glGetFloatv(GL_VIEWPORT, viewport);

    float proj_ortho2d_matrix_4x4[16];
    EKG_Ortho2D(proj_ortho2d_matrix_4x4, 0.0F, viewport[2], viewport[3], 0.0F);

    // We set projection ortho.
    ekg_shader_proj_matrix(EKG_CORE->shader_manager.GetTessellatorShader(), proj_ortho2d_matrix_4x4);
}

ekg_font_renderer ekg::font_renderer() {
    return EKG_CORE->font_renderer;
}

ekg_shader_manager ekg::shader_manager() {
    return EKG_CORE->shader_manager;
}

unsigned int ekg::point_collide_dock(unsigned int flags, float point_x, float point_y, float min_offset, float max_offset, const ekg_rect &rect) {
    ekg_rect DockRect {};

    int DockColliding = -1;
    unsigned int DockType = dock::FULL;

    if (EKG_FlagContains(flags, DockType)) {
        DockRect = get_rect_dock(DockType, min_offset, max_offset, rect);
        return DockRect.CollideWithPoint(point_x, point_y) ? DockType : DockColliding;
    }

    DockType = dock::TOP;

    if (EKG_FlagContains(flags, DockType)) {
        DockRect = get_rect_dock(DockType, min_offset, max_offset, rect);
        DockColliding = DockRect.CollideWithPoint(point_x, point_y) ? DockType : DockColliding;
    }

    DockType = dock::BOTTOM;

    if (EKG_FlagContains(flags, DockType)) {
        DockRect = get_rect_dock(DockType, min_offset, max_offset, rect);
        DockColliding = DockRect.CollideWithPoint(point_x, point_y) ? DockType : DockColliding;
    }

    DockType = dock::LEFT;

    if (EKG_FlagContains(flags, DockType)) {
        DockRect = get_rect_dock(DockType, min_offset, max_offset, rect);
        DockColliding = DockRect.CollideWithPoint(point_x, point_y) ? DockType : DockColliding;
    }

    DockType = dock::RIGHT;

    if (EKG_FlagContains(flags, DockType)) {
        DockRect = get_rect_dock(DockType, min_offset, max_offset, rect);
        DockColliding = DockRect.CollideWithPoint(point_x, point_y) ? DockType : DockColliding;
    }

    DockType = dock::CENTER;

    if (EKG_FlagContains(flags, DockType)) {
        DockRect = get_rect_dock(DockType, min_offset, max_offset, rect);
        DockColliding = DockRect.CollideWithPoint(point_x, point_y) ? DockType : DockColliding;
    }

    return DockColliding;
}

ekg_rect ekg::get_rect_dock(unsigned int dock, float initial_offset, float size_offset, const ekg_rect &origin) {
    ekg_rect Rect;

    // Pos rect.
    float x = 0.0F;
    float y = 0.0F;

    // size rect.
    float w = 0.0F;
    float h = 0.0F;

    // Offset positions are normalized.
    float initial_offset_normal;
    float size_offset_normal;

    // division stuff.
    float division;

    switch (dock) {
        case dock::FULL: {
            initial_offset_normal = initial_offset == 0 ? initial_offset : initial_offset * origin.W;
            size_offset_normal = size_offset == 0 ? size_offset : size_offset * origin.H;

            x = origin.X + initial_offset_normal;
            y = origin.Y + size_offset_normal;
            w = origin.W - (initial_offset_normal * 2);
            h = origin.H - (size_offset_normal * 2);
            break;
        }

        case dock::LEFT: {
            division = origin.W == 0 ? 0 : (origin.W / 2.0F);

            initial_offset_normal = initial_offset == 0 ? initial_offset : initial_offset * (division);
            size_offset_normal = size_offset == 0 ? size_offset : size_offset * (division);

            x = origin.X + initial_offset_normal;
            y = origin.Y;
            w = division - size_offset_normal;
            h = origin.H;
            break;
        }

        case dock::RIGHT: {
            division = origin.W == 0 ? 0 : (origin.W / 2.0F);

            initial_offset_normal = initial_offset == 0 ? initial_offset : initial_offset * division;
            size_offset_normal = size_offset == 0 ? size_offset : size_offset * division;

            y = origin.Y;
            h = origin.H;

            w = division - size_offset_normal;
            x = origin.X + initial_offset_normal + origin.W - w;
            break;
        }

        case dock::TOP: {
            division = origin.H == 0 ? 0 : (origin.H / 2.0F);

            initial_offset_normal = initial_offset == 0 ? initial_offset : initial_offset * division;
            size_offset_normal = size_offset == 0 ? size_offset : size_offset * division;

            x = origin.X;
            y = origin.Y + initial_offset_normal;
            w = origin.W;
            h = division - size_offset_normal;
            break;
        }

        case dock::BOTTOM: {
            division = origin.H == 0 ? 0 : (origin.H / 2.0F);

            initial_offset_normal = initial_offset == 0 ? initial_offset : initial_offset * division;
            size_offset_normal = size_offset == 0 ? size_offset : size_offset * division;

            x = origin.X;
            w = origin.W;

            h = division - size_offset_normal;
            y = origin.Y + initial_offset_normal + origin.H - h;
            break;
        }

        case dock::CENTER: {
            division = origin.W == 0 ? 0 : (origin.W / 2.0F);
            initial_offset_normal = initial_offset == 0 ? initial_offset : initial_offset * division;
            size_offset_normal = size_offset == 0 ? size_offset : size_offset * division;

            x = origin.X + (division / 2.0F) - initial_offset_normal;
            w = division - size_offset_normal;

            // Center height segment code.
            division = origin.H == 0 ? 0 : (origin.H / 2.0F);
            initial_offset_normal = initial_offset == 0 ? initial_offset : initial_offset * division;
            size_offset_normal = size_offset == 0 ? size_offset : size_offset * division;

            y = origin.Y + (division / 2.0F) - initial_offset_normal;
            w = division - size_offset_normal;
        }
    }

    Rect.X = x;
    Rect.Y = y;
    Rect.W = w;
    Rect.H = h;

    return Rect;
}

void ekg::scaled_finger_pos(float &x, float &y) {
    x *= ekg::screen_width;
    y *= ekg::screen_height;
}

ekg_color_theme ekg::get_theme() {
    return EKG_CORE->color_theme;
}

ekg::frame ekg::create_frame(const std::string &name) {
    auto* element = new ekg_ui_element_frame();

    element->set_tag(name);
    element->set_id(EKG_CORE->next_id());
    element->place(10, 10);
    element->SetLimit(50, 50);
    element->set_width(250);
    element->set_height(250);
    element->set_visibility_flag(ekg::visibility::VISIBLE);
    element->sync_size();

    EKG_CORE->add_element_to_queue(element);
    ekg::frame interface(element);

    return interface;
}

ekg::button ekg::create_button(const std::string &name) {
    auto* element = new ekg_ui_element_button();

    element->set_size(6);
    element->set_tag(name);
    element->set_id(EKG_CORE->next_id());
    element->place(10, 10);
    element->align_text(ekg::dock::CENTER);
    element->align_box(ekg::dock::LEFT);
    element->set_visibility_flag(ekg::visibility::VISIBLE_ONCE);
    element->set_mode("normal");
    element->SetWidth(element->get_text_width());
    element->sync_size();

    EKG_CORE->add_element_to_queue(element);
    ekg::button interface(element);

    return interface;
}

ekg_ui_element_popup* ekg::Popup(const std::string &Name, float InitialPosX, float InitialPosY, const std::vector<std::string> &List) {
    // Instead we create a new popup, we need to verify what element is at top (focused);
    if (((InitialPosX != ekg::NOPOS && InitialPosY != ekg::NOPOS) && (ekg::current_focused_type() != "popup" && InitialPosX != ekg::ABSOLUTE && InitialPosY != ekg::ABSOLUTE)) && (ekg::task_on(
            ekg::task::BLOCKED) || (current_focused_type() != "frame" &&
            current_focused_type() != "NULL" && InitialPosX != ekg::NOPOS && InitialPosY != ekg::NOPOS))) {
        return NULL;
    }

    auto* Element = new ekg_ui_element_popup();

    Element->set_tag(Name);
    Element->set_id(EKG_CORE->next_id());
    Element->SetScale(2);
    Element->set_visibility_flag((InitialPosX != ekg::NOPOS && InitialPosY != ekg::NOPOS) ||
                                 (InitialPosX == ekg::ABSOLUTE && InitialPosY == ekg::ABSOLUTE)
                                 ? ekg::visibility::VISIBLE : ekg::visibility::INVISIBLE);
    Element->SetWidth(125);
    Element->place(InitialPosX, InitialPosY);
    Element->Insert(List);
    Element->sync_size();

    EKG_CORE->add_element_to_queue(Element);
    return Element;
}

ekg_ui_element_slider* ekg::Slider(const std::string &Name, float Value, float Min, float Max) {
    auto* Element = new ekg_ui_element_slider();

    Element->set_tag(Name);
    Element->Orientation("Horizontal");
    Element->set_id(EKG_CORE->next_id());
    Element->SetSize(125);
    Element->SetScale(6);
    Element->LabelAlign(ekg::dock::CENTER);
    Element->set_visibility_flag(ekg::visibility::VISIBLE_ONCE);
    Element->place(10, 10);
    Element->SetMin(Min);
    Element->SetMax(Max);
    Element->SetValue(Value);
    Element->sync_size();

    EKG_CORE->add_element_to_queue(Element);
    return Element;
}

ekg_ui_element_combobox* ekg::Combobox(const std::string &Name, const std::vector<std::string> &List) {
    auto* Element = new ekg_ui_element_combobox();

    Element->set_tag(Name);
    Element->set_id(EKG_CORE->next_id());
    Element->set_list(List);
    Element->set_current(" ");
    Element->align_text(ekg::dock::LEFT);
    Element->set_visibility_flag(ekg::visibility::VISIBLE_ONCE);
    Element->set_scale(6);
    Element->place(10, 10);
    Element->set_width(125);
    Element->sync_size();

    EKG_CORE->add_element_to_queue(Element);
    return Element;
}

ekg_ui_element_tab *ekg::Tab(const std::string &Name) {
    auto* Element = new ekg_ui_element_tab();

    Element->set_tag(Name);
    Element->set_id(EKG_CORE->next_id());
    Element->place(10, 10);
    Element->SetLimit(50, 50);
    Element->set_width(250);
    Element->SetButtonSize(3);
    Element->SetScale(3);
    Element->SetBorderOffset(10);
    Element->set_height(250);
    Element->TabSide(ekg::dock::TOP);
    Element->set_visibility_flag(ekg::visibility::VISIBLE);
    Element->sync_size();

    EKG_CORE->add_element_to_queue(Element);
    return Element;
}

ekg_abstract_element *ekg::find(unsigned int Id) {
    return EKG_CORE->get_element_by_id(Id);
}

std::vector<unsigned int> &ekg::children(ekg_abstract_element* element) {
    std::vector<unsigned int> Stack;

    if (element == NULL) {
        return Stack;
    }

    Stack = element->GetChildren().StackedIds;
    return Stack;
}

void ekg::kill(ekg_abstract_element* &element) {
    EKG_CORE->remove_element_by_id(element->get_id());
    element = NULL;
}

std::string ekg::current_focused_tag() {
    return EKG_CORE->get_focused_element_tag();
}

std::string ekg::current_focused_type() {
    return EKG_CORE->get_focused_element_info_class();
}

unsigned int ekg::current_focused_id() {
    return EKG_CORE->get_focused_element_id();
}

ekg_timing* ekg::timing() {
    return EKG_CORE->timing;
}

void ekg::task(unsigned int task_id, unsigned int id) {
    EKG_CORE->add_task_to_queue(task_id, id);
}

bool ekg::task_on(unsigned int task_id) {
    return EKG_CORE->verify_task(task_id);
}

Uint32 ekg::event::REGISTER = SDL_RegisterEvents(2);
Uint32 ekg::event::ELEMENT  = REGISTER++;
Uint32 ekg::event::POPUP    = REGISTER++;

ekg_event ekg::event::read(SDL_Event event) {
    if (event.type == POPUP) {
        ekg_event next_event;
        auto* callback = static_cast<std::string*>(event.user.data1);

        next_event.Type = event.user.code;
        next_event.popup.callback = *callback;

        return next_event;
    }

    return ekg_event();
}

void ekg::event::dispatch(Uint32 type, void *data1, void *data2) {
    SDL_Event event;

    event.type = type;
    event.user.code = type;

    event.user.data1 = data1;
    event.user.data2 = data2;

    SDL_PushEvent(&event);
}

std::string ekg::button::info_class() {
    return this->element->info_class();
}

float ekg::button::get_text_height() {
    return this->element->get_text_height();
}

float ekg::button::get_text_width() {
    return this->element->get_text_width();
}

void ekg::button::mode(const std::string &mode) {
    this->element->set_mode(mode);
}

void ekg::button::align_box(unsigned int dock, float offset) {
    this->element->align_box(dock);

    if (offset != NULL) {
        this->element->set_offset_box(offset);
    }
}

void ekg::button::align_text(unsigned int dock, float offset) {
    this->element->align_text(dock);

    if (offset != NULL) {
        this->element->set_offset_text(offset);
    }
}

bool ekg::button::box_checked() {
    return this->element->is_checked();
}

void ekg::button::set_box_check_state(bool state) {
    this->element->set_check_state(state);
}

bool ekg::button::clicked() {
    return this->element->is_clicked();
}

void ekg::button::set_click_state(bool state) {
    this->element->set_click_state(state);
}

float ekg::button::get_width() {
    return this->element->get_width();
}

void ekg::button::set_width(float Width) {
    return this->element->SetWidth(Width);
}

float ekg::button::get_size() {
    return this->element->get_size();
}

void ekg::button::set_size(float Size) {
    this->element->set_size(Size);
}

float ekg::button::get_height() {
    return this->element->get_height();
}

float ekg::button::get_x() {
    return this->element->get_x();
}

float ekg::button::get_y() {
    return this->element->get_y();
}

void ekg::button::place(float X, float Y) {
    this->element->place(X, Y);
}

ekg_stack ekg::button::get_children() {
    return this->element->GetChildren();
}

unsigned int ekg::button::get_id() {
    return this->element_id;
}

std::string ekg::button::get_mode() {
    return this->element->get_mode();
}

unsigned int ekg::frame::get_id() {
    return this->element_id;
}

std::string ekg::frame::info_class() {
    return this->element->info_class();
}

ekg_stack ekg::frame::get_children() {
    return this->element->GetChildren();
}

float ekg::frame::get_min_height() {
    return this->element->get_min_height();
}

float ekg::frame::get_min_width() {
    return this->element->get_min_width();
}

void ekg::frame::set_min_size(float min_width, float min_height) {
    this->element->SetLimit(min_width, min_height);
}

void ekg::frame::place(unsigned int element_in_id, float x, float y) {
    this->element->place(EKG_CORE->get_element_by_id(element_in_id), x, y);
}

void ekg::frame::resizable(unsigned int area, float offset) {
    this->element->resizable(area);

    if (offset != NULL) {
        this->element->set_resize_offset(offset);
    }
}

void ekg::frame::draggable(unsigned int area, float offset) {
    this->element->draggable(area);

    if (offset != NULL) {
        this->element->set_drag_offset(offset);
    }
}

float ekg::frame::get_height() {
    return this->element->get_height();
}

void ekg::frame::set_height(float Height) {
    this->element->set_height(Height);
}

float ekg::frame::get_width() {
    return this->element->get_width();
}

void ekg::frame::set_width(float width) {
    this->element->set_width(width);
}

void ekg::frame::place(float x, float y) {
    this->element->place(x, y);
}

float ekg::frame::get_y() {
    return this->element->get_y();
}

float ekg::frame::get_X() {
    return this->element->get_x();
}
