/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "ekg_core.h"
#include "ekg_tessellator.h"

#include "ekg_ui_element_frame.h"
#include "ekg_ui_element_button.h"
#include "ekg_ui_element_slider.h"
#include "ekg_ui_element_popup.h"
#include "ekg_ui_element_combobox.h"
#include "ekg_ui_element_tab.h"

/* Start of shader util functions. */
void ekg_start_use_shader(GLuint shader_id);
void ekg_end_use_shader();
void ekg_shader_proj_matrix(unsigned int shader_id, float* proj_matrix);
GLuint ekg_get_shader_attrib(const std::string &shader_name, unsigned int shader_id, const std::string &attrib_name);
void ekg_set_shader_uniform_bool(unsigned int shader_id, const std::string &name, bool value);
void ekg_set_shader_uniform_int(unsigned int shader_id, const std::string &name, int value);
void ekg_set_shader_uniform_float(unsigned int shader_id, const std::string &name, float value);
void ekg_set_shader_uniform_vec2(unsigned int shader_id, const std::string &name, const glm::vec2 &value);
void ekg_set_shader_uniform_vec2(unsigned int shader_id, const std::string &name, float x, float y);
void ekg_set_shader_uniform_vec3(unsigned int ShaderId, const std::string &Name, const glm::vec3 &Value);
void ekg_set_shader_uniform_vec3(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z);
void ekg_set_shader_uniform_vec4(unsigned int ShaderId, const std::string &Name, const glm::vec4 &Value);
void ekg_set_shader_uniform_vec4(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z, float W);
void ekg_set_shader_uniform_mat2(unsigned int ShaderId, const std::string &Name, const glm::mat2 &Mat);
void ekg_set_shader_uniform_mat3(unsigned int ShaderId, const std::string &Name, const glm::mat3 &Mat);
void ekg_set_shader_uniform_mat4(unsigned int ShaderId, const std::string &Name, const glm::mat4 &Mat);
/* End of shader util functions. */

/* Start of helper functions. */
bool EKG_FlagContains(unsigned int Flags, unsigned int Flag);
void EKG_FlagRemove(unsigned int &Flags, unsigned int Flag);
void EKG_FlagAdd(unsigned int &Flags, unsigned int Flag);
/* End of helper functions. */

/**
 * popup event.
 **/
typedef struct {
    std::string callback;
} popup_callback;

/**
 * event structure.
 **/
struct ekg_event {
    Sint32 Type;

    popup_callback popup;
};

// Global runtime variable.
extern ekg_core* const EKG_CORE;
extern EKG_Tessellator* const EKG_TESSELLATOR;

/**
 * Main class to call and build GUIs.
 */
class ekg {
public:
    /**
     * Coordinates used in ekg.
     **/
    struct dock {
    public:
        static const unsigned int FULL    = 1 << 1;
        static const unsigned int TOP     = 1 << 2;
        static const unsigned int BOTTOM  = 1 << 3;
        static const unsigned int LEFT    = 1 << 4;
        static const unsigned int RIGHT   = 1 << 5;
        static const unsigned int CENTER  = 1 << 6;
    };

    /**
     * task in UI core.
     **/
    struct task {
        const static unsigned int REFRESH = 1 << 1;
        const static unsigned int REORDER = 1 << 2;
        const static unsigned int BLOCKED = 1 << 3;
        const static unsigned int REFOCUS = 1 << 4;
        const static unsigned int FREE    = 1 << 5;
    };

    /**
     * Get elements value released in popup.
     **/
    struct event {
        static Uint32 REGISTER;
        static Uint32 POPUP;
        static Uint32 ELEMENT;

        static void dispatch(Uint32 type, void* data1, void* data2);
        static ekg_event read(SDL_Event event);
    };

    /**
     * element context.
     **/
    enum visibility {
        EXISTED, VISIBLE_ONCE, INVISIBLE_ONCE, LOW_PRIORITY, INVISIBLE, VISIBLE
    };

    /* Metrics of device. */
    static float screen_width, screen_height;

    /* Flags used in environment. */
    static const int NOPOS    = -256;
    static const int ABSOLUTE = -255;

    /* Start of UI helpers method. */
    static unsigned int point_collide_dock(unsigned int flags, float point_x, float point_y, float min_offset, float max_offset, const ekg_rect &rect);
    static ekg_rect get_rect_dock(unsigned int dock, float initial_offset, float size_offset, const ekg_rect &origin);
    static void scaled_finger_pos(float &x, float &y);

    static std::string current_focused_tag();
    static std::string current_focused_type();
    static unsigned int current_focused_id();

    static void task(unsigned int task_id, unsigned int id = 0);
    static bool task_on(unsigned int task_id);
    static ekg_timing* timing();
    /* End of UI helpers method. */

    /* Start of static instance methods. */
    static ekg_shader_manager shader_manager();
    static ekg_font_renderer font_renderer();
    static ekg_color_theme get_theme();
    /* End of static instance methods. */

    /* Start of main ekg static methods (needed use). */
    static void init_font(const std::string &path, unsigned int initial_size);
    static void init();
    static void quit();
    /* End of main ekg static methods. */

    /* Start of setup/concurrent static methods. */
    static ekg_ui_element_slider* Slider(const std::string &Name, float Value, float Min, float Max);
    static ekg_ui_element_popup* Popup(const std::string &Name, float InitialPosX, float InitialPosY, const std::vector<std::string> &List);
    static ekg_ui_element_combobox* Combobox(const std::string &Name, const std::vector<std::string> &List);
    static ekg_ui_element_tab* Tab(const std::string &Name);

    static std::vector<unsigned int> &children(ekg_abstract_element* element);
    static ekg_abstract_element* find(unsigned int Id);

    static void kill(ekg_abstract_element* &element);
    static void context();
    static void poll_event(SDL_Event Event);
    static void update(const float &DeltaTicks);
    static void render(const float &PartialTicks);
    /* End of setup/concurrent static methods. */

    /* Start of "interfaces" of GUI. */
    struct frame {
    private:
        ekg_ui_element_frame* element;
        unsigned int element_id;
    public:
        frame() {}
        frame(ekg_ui_element_frame* element) {
            this->element = element;
            this->element_id = element->get_id();
        }

        float get_X();
        float get_y();

        void place(float x, float y);

        void set_width(float width);
        float get_width();

        void set_height(float Height);
        float get_height();

        void draggable(unsigned int area, float offset = NULL);
        void resizable(unsigned int area, float offset = NULL);
        void place(unsigned int element_in_id, float x, float y);

        void set_min_size(float min_width, float min_height);

        float get_min_width();
        float get_min_height();

        ekg_stack get_children();
        std::string info_class();
        unsigned int get_id();
    };

    struct button {
    private:
        ekg_ui_element_button* element;
        unsigned int element_id;
    public:
        button() {}
        button(ekg_ui_element_button* element_in) {
            this->element = element_in;
            this->element_id = element_in->get_id();
        }

        float get_x();
        float get_y();

        void place(float X, float Y);

        void set_size(float Size);
        float get_size();

        void set_width(float Width);
        float get_width();

        float get_height();

        void set_click_state(bool state);
        bool clicked();

        void set_box_check_state(bool state);
        bool box_checked();

        void align_text(unsigned int dock, float offset = NULL);
        void align_box(unsigned int Dock, float Offset = NULL);

        void mode(const std::string &mode);
        std::string get_mode();

        float get_text_width();
        float get_text_height();

        ekg_stack get_children();
        std::string info_class();
        unsigned int get_id();
    };

    static ekg::frame create_frame(const std::string &name);
    static ekg::button create_button(const std::string &name);
    /* End of "interfaces" of GUI. */
};