#pragma once

#include "../../vxui.h"
#include "../../vxui_menu.h"

struct vxui_demo_command_deck_theme
{
    vxui_color app_background_base;
    vxui_color app_background_accent;
    vxui_color hero_surface_fill;
    vxui_color hero_surface_border;
    vxui_color primary_panel_fill;
    vxui_color primary_panel_border;
    vxui_color secondary_panel_fill;
    vxui_color secondary_panel_border;
    vxui_color utility_fill;
    vxui_color utility_border;
    vxui_color focused_row_fill;
    vxui_color focused_row_border;
    vxui_color passive_row_fill;
    vxui_color passive_row_border;
    vxui_color badge_fill;
    vxui_color badge_text;
    vxui_color title_text;
    vxui_color section_text;
    vxui_color body_text;
    vxui_color muted_text;
    vxui_color utility_text;
    vxui_color accent_cool;
    vxui_color warning_text;
    vxui_color success_text;
    vxui_color alert_text;
    vxui_color action_fill;
    vxui_color action_border;
    vxui_color action_text;
    vxui_color stat_track;
    vxui_color stat_fill;
    vxui_color disabled_fill;
    vxui_color disabled_text;
    float surface_scanline_alpha;
    float title_focus_decor_alpha;
    float form_focus_decor_alpha;
};

inline const vxui_demo_command_deck_theme& vxui_demo_command_deck_theme_tokens()
{
    static constexpr vxui_demo_command_deck_theme theme = {
        .app_background_base = { 7, 11, 19, 255 },
        .app_background_accent = { 86, 124, 164, 52 },
        .hero_surface_fill = { 23, 34, 52, 244 },
        .hero_surface_border = { 67, 92, 124, 50 },
        .primary_panel_fill = { 16, 27, 43, 248 },
        .primary_panel_border = { 68, 94, 126, 120 },
        .secondary_panel_fill = { 19, 31, 48, 246 },
        .secondary_panel_border = { 54, 79, 108, 80 },
        .utility_fill = { 12, 20, 32, 244 },
        .utility_border = { 74, 102, 132, 60 },
        .focused_row_fill = { 68, 110, 145, 255 },
        .focused_row_border = { 176, 229, 244, 248 },
        .passive_row_fill = { 13, 21, 34, 245 },
        .passive_row_border = { 66, 90, 118, 60 },
        .badge_fill = { 176, 132, 56, 240 },
        .badge_text = { 255, 244, 214, 255 },
        .title_text = { 245, 248, 255, 255 },
        .section_text = { 255, 221, 158, 255 },
        .body_text = { 222, 231, 242, 255 },
        .muted_text = { 166, 185, 208, 255 },
        .utility_text = { 231, 239, 248, 255 },
        .accent_cool = { 166, 219, 216, 255 },
        .warning_text = { 255, 214, 156, 255 },
        .success_text = { 164, 220, 190, 255 },
        .alert_text = { 232, 130, 130, 255 },
        .action_fill = { 27, 46, 70, 248 },
        .action_border = { 115, 170, 198, 196 },
        .action_text = { 231, 239, 248, 255 },
        .stat_track = { 28, 43, 64, 255 },
        .stat_fill = { 121, 214, 186, 255 },
        .disabled_fill = { 16, 23, 32, 184 },
        .disabled_text = { 113, 125, 143, 255 },
        .surface_scanline_alpha = 0.024f,
        .title_focus_decor_alpha = 0.10f,
        .form_focus_decor_alpha = 0.08f,
    };
    return theme;
}

inline Clay_Color vxui_demo_clay_color( vxui_color color )
{
    return ( Clay_Color ) {
        ( float ) color.r,
        ( float ) color.g,
        ( float ) color.b,
        ( float ) color.a,
    };
}

inline Clay_BorderElementConfig vxui_demo_panel_border( vxui_color color, uint16_t width = 1 )
{
    return ( Clay_BorderElementConfig ) {
        .color = vxui_demo_clay_color( color ),
        .width = CLAY_BORDER_ALL( width ),
    };
}

inline vxui_label_cfg vxui_demo_text_style( uint32_t font_id, float font_size, vxui_color color )
{
    vxui_label_cfg cfg = {};
    cfg.font_id = font_id;
    cfg.font_size = font_size;
    cfg.color = color;
    return cfg;
}

inline vxui_value_cfg vxui_demo_value_style( uint32_t font_id, float font_size, vxui_color color, const char* format = nullptr )
{
    vxui_value_cfg cfg = {};
    cfg.font_id = font_id;
    cfg.font_size = font_size;
    cfg.color = color;
    cfg.format = format;
    return cfg;
}

inline void vxui_demo_apply_title_menu_theme( vxui_menu_style& style )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    style.corner_radius = 9.0f;
    style.border_width = 1.0f;
    style.badge_padding_x = 8.0f;
    style.badge_padding_y = 3.0f;
    style.focus_decor = VXUI_MENU_FOCUS_DECOR_GLOW;
    style.focus_decor_padding = 3.0f;
    style.focus_decor_alpha = theme.title_focus_decor_alpha;
    style.panel_fill_color = theme.primary_panel_fill;
    style.row_fill_color = theme.passive_row_fill;
    style.row_focus_fill_color = theme.focused_row_fill;
    style.row_disabled_fill_color = theme.disabled_fill;
    style.row_border_color = theme.passive_row_border;
    style.row_focus_border_color = theme.focused_row_border;
    style.text_color = theme.body_text;
    style.focused_text_color = theme.title_text;
    style.disabled_text_color = theme.disabled_text;
    style.secondary_text_color = theme.muted_text;
    style.section_text_color = theme.section_text;
    style.badge_fill_color = theme.badge_fill;
    style.badge_text_color = theme.badge_text;
    style.prompt_fill_color = theme.action_fill;
    style.prompt_text_color = theme.action_text;
    style.arrow_color = theme.focused_row_border;
    style.slider_track_color = theme.stat_track;
    style.slider_fill_color = theme.stat_fill;
}

inline void vxui_demo_apply_form_menu_theme( vxui_menu_style& style )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    style.corner_radius = 9.0f;
    style.border_width = 1.0f;
    style.badge_padding_x = 8.0f;
    style.badge_padding_y = 3.0f;
    style.focus_decor = VXUI_MENU_FOCUS_DECOR_GLOW;
    style.focus_decor_padding = 2.0f;
    style.focus_decor_alpha = theme.form_focus_decor_alpha;
    style.panel_fill_color = theme.primary_panel_fill;
    style.row_fill_color = theme.passive_row_fill;
    style.row_focus_fill_color = theme.focused_row_fill;
    style.row_disabled_fill_color = theme.disabled_fill;
    style.row_border_color = theme.passive_row_border;
    style.row_focus_border_color = theme.focused_row_border;
    style.text_color = theme.body_text;
    style.focused_text_color = theme.utility_text;
    style.disabled_text_color = theme.disabled_text;
    style.secondary_text_color = theme.muted_text;
    style.section_text_color = theme.section_text;
    style.badge_fill_color = theme.badge_fill;
    style.badge_text_color = theme.badge_text;
    style.prompt_fill_color = theme.action_fill;
    style.prompt_text_color = theme.action_text;
    style.arrow_color = theme.focused_row_border;
    style.slider_track_color = theme.stat_track;
    style.slider_fill_color = theme.stat_fill;
}
