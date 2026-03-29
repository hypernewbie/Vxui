#pragma once

#include "vxui.h"

#define VXUI_MENU_STYLE_FOOTER_PROMPT_BAR ( 1u << 0 )
#define VXUI_MENU_STYLE_SECONDARY_TEXT ( 1u << 1 )
#define VXUI_MENU_STYLE_AUTO_SCROLL_TO_FOCUS ( 1u << 2 )
#define VXUI_MENU_STYLE_VALUE_ARROWS ( 1u << 3 )
#define VXUI_MENU_STYLE_FOCUS_FRAME ( 1u << 4 )

typedef enum vxui_menu_layout_mode
{
    VXUI_MENU_LAYOUT_FORM = 0,
    VXUI_MENU_LAYOUT_CENTERED = 1,
} vxui_menu_layout_mode;

typedef enum vxui_menu_value_lane_mode
{
    VXUI_MENU_VALUE_LANE_GROW = 0,
    VXUI_MENU_VALUE_LANE_FIXED = 1,
    VXUI_MENU_VALUE_LANE_FIT = 2,
} vxui_menu_value_lane_mode;

typedef struct vxui_menu_state
{
    float scroll_current;
    float scroll_target;
    float scroll_velocity;
    uint32_t last_focused_row_id;
    int last_emitted_row_count;
    int selected_section_index;
} vxui_menu_state;

typedef struct vxui_menu_badge_cfg
{
    uint32_t font_id;
    float font_size;
    float padding_x;
    float padding_y;
    float corner_radius;
    float min_width;
    vxui_color fill_color;
    vxui_color text_color;
    bool hidden;
} vxui_menu_badge_cfg;

typedef struct vxui_menu_row_cfg
{
    const char* value_key;
    const char* secondary_key;
    const char* badge_text_key;
    vxui_menu_badge_cfg badge;
    uint32_t font_id;
    float font_size;
    vxui_color text_color;
    bool hidden;
    bool disabled;
} vxui_menu_row_cfg;

typedef struct vxui_menu_section_cfg
{
    const char* secondary_key;
    uint32_t font_id;
    float font_size;
    vxui_color text_color;
    bool hidden;
    bool centered;
} vxui_menu_section_cfg;

typedef struct vxui_menu_prompt_bar_cfg
{
    const char* const* action_names;
    const char* const* label_keys;
    int count;
    float gap;
    bool hidden;
} vxui_menu_prompt_bar_cfg;

typedef struct vxui_menu_style
{
    uint32_t flags;
    vxui_menu_layout_mode layout_mode;
    vxui_menu_value_lane_mode value_lane_mode;
    float row_height;
    float row_gap;
    float section_gap;
    float label_lane_width;
    float value_lane_width;
    float padding_x;
    float padding_y;
    float lane_gap;
    float secondary_gap;
    float corner_radius;
    float border_width;
    float badge_padding_x;
    float badge_padding_y;
    float prompt_gap;
    float scroll_stiffness;
    float scroll_damping;
    uint32_t body_font_id;
    uint32_t title_font_id;
    uint32_t badge_font_id;
    float body_font_size;
    float secondary_font_size;
    float title_font_size;
    float badge_font_size;
    uint32_t focus_trait_id;
    float focus_trait_padding;
    float focus_trait_alpha;
    vxui_color panel_fill_color;
    vxui_color row_fill_color;
    vxui_color row_focus_fill_color;
    vxui_color row_disabled_fill_color;
    vxui_color row_border_color;
    vxui_color row_focus_border_color;
    vxui_color text_color;
    vxui_color focused_text_color;
    vxui_color disabled_text_color;
    vxui_color secondary_text_color;
    vxui_color section_text_color;
    vxui_color badge_fill_color;
    vxui_color badge_text_color;
    vxui_color prompt_fill_color;
    vxui_color prompt_text_color;
    vxui_color arrow_color;
    vxui_color slider_track_color;
    vxui_color slider_fill_color;
} vxui_menu_style;

typedef struct vxui_menu_cfg
{
    const vxui_menu_style* style;
    const char* title_key;
    const char* subtitle_key;
    float viewport_height;
} vxui_menu_cfg;

vxui_menu_style vxui_menu_style_br_title( void );
vxui_menu_style vxui_menu_style_br_panel( void );
vxui_menu_style vxui_menu_style_form( void );
vxui_menu_style vxui_menu_style_compact( void );

void vxui_menu_begin( vxui_ctx* ctx, vxui_menu_state* state, const char* id, vxui_menu_cfg cfg );
void vxui_menu_end( vxui_ctx* ctx, vxui_menu_state* state );
void vxui_menu_section( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* title_key, vxui_menu_section_cfg cfg );
void vxui_menu_label( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* label_key, vxui_menu_row_cfg cfg );
void vxui_menu_action( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* label_key, vxui_action_fn fn, vxui_menu_row_cfg row_cfg, vxui_action_cfg action_cfg );
void vxui_menu_option( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* label_key, int* index, const char** option_keys, int count, vxui_menu_row_cfg row_cfg, vxui_option_cfg option_cfg );
void vxui_menu_slider( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* label_key, float* value, float min_value, float max_value, vxui_menu_row_cfg row_cfg, vxui_slider_cfg slider_cfg );
void vxui_menu_badge( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* text_key, vxui_menu_badge_cfg cfg );
void vxui_menu_prompt_bar( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const vxui_menu_prompt_bar_cfg* cfg );

#ifdef VXUI_MENU_IMPL

#ifndef VXUI_IMPL
#error "VXUI_MENU_IMPL requires VXUI_IMPL in the same translation unit."
#endif

#include <cmath>
#include <cstdio>
#include <cstring>

enum
{
    VXUI_MENU__MAX_SCOPE_DEPTH = 8,
};

typedef struct vxui_menu__scope
{
    vxui_ctx* ctx;
    vxui_menu_state* state;
    const char* id;
    vxui_menu_style style;
    vxui_menu_cfg cfg;
    uint32_t viewport_id;
    uint32_t content_id;
    int row_count;
    int focused_row_index;
} vxui_menu__scope;

static thread_local vxui_menu__scope vxui_menu__scopes[ VXUI_MENU__MAX_SCOPE_DEPTH ];
static thread_local int vxui_menu__scope_count = 0;

static Clay_Color vxui_menu__to_clay_color( vxui_color color )
{
    return Clay_Color { ( float ) color.r, ( float ) color.g, ( float ) color.b, ( float ) color.a };
}

static uint16_t vxui_menu__u16( float value )
{
    if ( value <= 0.0f ) {
        return 0;
    }
    if ( value >= 65535.0f ) {
        return 65535u;
    }
    return ( uint16_t ) std::lround( ( double ) value );
}

static uint32_t vxui_menu__mix_id( uint32_t base, uint32_t salt )
{
    uint32_t value = base ^ ( salt * 0x9E3779B9u ) ^ 0x7F4A7C15u;
    if ( value == 0 ) {
        value = salt ? salt : 1u;
    }
    return value;
}

static vxui_menu__scope* vxui_menu__current_scope( void )
{
    if ( vxui_menu__scope_count <= 0 ) {
        return nullptr;
    }
    return &vxui_menu__scopes[ vxui_menu__scope_count - 1 ];
}

static vxui_menu_style vxui_menu__sanitize_style( vxui_menu_style style )
{
    vxui_menu_style defaults = vxui_menu_style_form();

    if ( style.flags == 0 ) {
        style.flags = defaults.flags;
    }
    if ( style.row_height <= 0.0f ) {
        style.row_height = defaults.row_height;
    }
    if ( style.row_gap <= 0.0f ) {
        style.row_gap = defaults.row_gap;
    }
    if ( style.section_gap <= 0.0f ) {
        style.section_gap = defaults.section_gap;
    }
    if ( style.label_lane_width <= 0.0f && style.layout_mode == VXUI_MENU_LAYOUT_FORM ) {
        style.label_lane_width = defaults.label_lane_width;
    }
    if ( style.value_lane_width <= 0.0f && style.value_lane_mode == VXUI_MENU_VALUE_LANE_FIXED ) {
        style.value_lane_width = defaults.value_lane_width;
    }
    if ( style.padding_x <= 0.0f ) {
        style.padding_x = defaults.padding_x;
    }
    if ( style.padding_y <= 0.0f ) {
        style.padding_y = defaults.padding_y;
    }
    if ( style.lane_gap <= 0.0f ) {
        style.lane_gap = defaults.lane_gap;
    }
    if ( style.secondary_gap <= 0.0f ) {
        style.secondary_gap = defaults.secondary_gap;
    }
    if ( style.corner_radius <= 0.0f ) {
        style.corner_radius = defaults.corner_radius;
    }
    if ( style.border_width <= 0.0f ) {
        style.border_width = defaults.border_width;
    }
    if ( style.badge_padding_x <= 0.0f ) {
        style.badge_padding_x = defaults.badge_padding_x;
    }
    if ( style.badge_padding_y <= 0.0f ) {
        style.badge_padding_y = defaults.badge_padding_y;
    }
    if ( style.prompt_gap <= 0.0f ) {
        style.prompt_gap = defaults.prompt_gap;
    }
    if ( style.scroll_stiffness <= 0.0f ) {
        style.scroll_stiffness = defaults.scroll_stiffness;
    }
    if ( style.scroll_damping <= 0.0f ) {
        style.scroll_damping = defaults.scroll_damping;
    }
    if ( style.body_font_size <= 0.0f ) {
        style.body_font_size = defaults.body_font_size;
    }
    if ( style.secondary_font_size <= 0.0f ) {
        style.secondary_font_size = defaults.secondary_font_size;
    }
    if ( style.title_font_size <= 0.0f ) {
        style.title_font_size = defaults.title_font_size;
    }
    if ( style.badge_font_size <= 0.0f ) {
        style.badge_font_size = defaults.badge_font_size;
    }
    if ( style.focus_trait_padding <= 0.0f ) {
        style.focus_trait_padding = defaults.focus_trait_padding;
    }
    if ( style.focus_trait_alpha <= 0.0f ) {
        style.focus_trait_alpha = defaults.focus_trait_alpha;
    }

    if ( style.panel_fill_color.a == 0 ) style.panel_fill_color = defaults.panel_fill_color;
    if ( style.row_fill_color.a == 0 ) style.row_fill_color = defaults.row_fill_color;
    if ( style.row_focus_fill_color.a == 0 ) style.row_focus_fill_color = defaults.row_focus_fill_color;
    if ( style.row_disabled_fill_color.a == 0 ) style.row_disabled_fill_color = defaults.row_disabled_fill_color;
    if ( style.row_border_color.a == 0 ) style.row_border_color = defaults.row_border_color;
    if ( style.row_focus_border_color.a == 0 ) style.row_focus_border_color = defaults.row_focus_border_color;
    if ( style.text_color.a == 0 ) style.text_color = defaults.text_color;
    if ( style.focused_text_color.a == 0 ) style.focused_text_color = defaults.focused_text_color;
    if ( style.disabled_text_color.a == 0 ) style.disabled_text_color = defaults.disabled_text_color;
    if ( style.secondary_text_color.a == 0 ) style.secondary_text_color = defaults.secondary_text_color;
    if ( style.section_text_color.a == 0 ) style.section_text_color = defaults.section_text_color;
    if ( style.badge_fill_color.a == 0 ) style.badge_fill_color = defaults.badge_fill_color;
    if ( style.badge_text_color.a == 0 ) style.badge_text_color = defaults.badge_text_color;
    if ( style.prompt_fill_color.a == 0 ) style.prompt_fill_color = defaults.prompt_fill_color;
    if ( style.prompt_text_color.a == 0 ) style.prompt_text_color = defaults.prompt_text_color;
    if ( style.arrow_color.a == 0 ) style.arrow_color = defaults.arrow_color;
    if ( style.slider_track_color.a == 0 ) style.slider_track_color = defaults.slider_track_color;
    if ( style.slider_fill_color.a == 0 ) style.slider_fill_color = defaults.slider_fill_color;

    return style;
}

static uint32_t vxui_menu__row_id( const vxui_menu__scope* scope, const char* id )
{
    return vxui_idi( scope && scope->id ? scope->id : "", ( int ) vxui_id( id ? id : "" ) );
}

static bool vxui_menu__visible( const vxui_menu_row_cfg* cfg )
{
    return !cfg || !cfg->hidden;
}

static vxui_color vxui_menu__row_text_color( const vxui_menu_style* style, bool focused, bool disabled, bool secondary )
{
    if ( disabled ) {
        return secondary ? style->secondary_text_color : style->disabled_text_color;
    }
    if ( secondary ) {
        return style->secondary_text_color;
    }
    return focused ? style->focused_text_color : style->text_color;
}

static uint32_t vxui_menu__row_font_id( const vxui_menu_style* style, const vxui_menu_row_cfg* cfg )
{
    if ( cfg && cfg->font_id != 0 ) {
        return cfg->font_id;
    }
    return style->body_font_id;
}

static float vxui_menu__row_font_size( const vxui_menu_style* style, const vxui_menu_row_cfg* cfg )
{
    if ( cfg && cfg->font_size > 0.0f ) {
        return cfg->font_size;
    }
    return style->body_font_size;
}

static void vxui_menu__emit_text_leaf( vxui_ctx* ctx, uint32_t owner_id, const char* text, uint32_t font_id, float font_size, vxui_color color )
{
    const char* resolved = text ? vxui__resolve_text( ctx, text ) : "";
    CLAY_AUTO_ID( vxui__text_leaf_decl( Clay_ElementDeclaration {} ) ) {
        vxui__emit_text(
            ctx,
            resolved,
            vxui__effective_font_id( ctx, font_id ),
            vxui__effective_font_size( ctx, font_size ),
            color,
            owner_id );
    }
}

static void vxui_menu__attach_focus_trait( vxui_ctx* ctx, uint32_t row_id, const vxui_menu_style* style )
{
    if ( !ctx || !style || style->focus_trait_id == 0 ) {
        return;
    }

    uint32_t prev_decl_id = ctx->current_decl_id;
    ctx->current_decl_id = row_id;

    switch ( style->focus_trait_id ) {
        case VXUI_TRAIT_GLOW: {
            vxui__trait_glow_params params = {
                .padding = style->focus_trait_padding,
                .alpha = style->focus_trait_alpha,
            };
            vxui__attach_trait( ctx, style->focus_trait_id, &params, sizeof( params ) );
        } break;

        case VXUI_TRAIT_PULSE: {
            vxui__trait_pulse_params params = {
                .speed = 2.0f,
                .scale = 0.05f,
                .alpha = style->focus_trait_alpha,
            };
            vxui__attach_trait( ctx, style->focus_trait_id, &params, sizeof( params ) );
        } break;

        default:
            break;
    }

    ctx->current_decl_id = prev_decl_id;
}

static void vxui_menu__begin_common_row( vxui_menu__scope* scope, uint32_t row_id, bool focused, bool disabled, bool centered )
{
    const vxui_menu_style* style = &scope->style;
    vxui_color fill = disabled ? style->row_disabled_fill_color : ( focused ? style->row_focus_fill_color : style->row_fill_color );
    vxui_color border = focused ? style->row_focus_border_color : style->row_border_color;
    Clay_BorderWidth border_width = {};
    if ( ( style->flags & VXUI_MENU_STYLE_FOCUS_FRAME ) != 0u ) {
        uint16_t width = vxui_menu__u16( style->border_width );
        border_width = { width, width, width, width, width };
    }

    Clay__OpenElementWithId( vxui__clay_id_from_hash( row_id ) );
    Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .padding = {
                vxui_menu__u16( style->padding_x ),
                vxui_menu__u16( style->padding_x ),
                vxui_menu__u16( style->padding_y ),
                vxui_menu__u16( style->padding_y ),
            },
            .childGap = vxui_menu__u16( style->secondary_gap ),
            .childAlignment = {
                .x = centered ? CLAY_ALIGN_X_CENTER : CLAY_ALIGN_X_LEFT,
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_menu__to_clay_color( fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( style->corner_radius ),
        .border = {
            .color = vxui_menu__to_clay_color( border ),
            .width = border_width,
        },
    } );

    if ( focused ) {
        vxui_menu__attach_focus_trait( scope->ctx, row_id, style );
    }
}

static void vxui_menu__end_common_row( void )
{
    Clay__CloseElement();
}

static void vxui_menu__begin_top_lane( const vxui_menu__scope* scope, uint32_t row_id, bool centered )
{
    Clay__OpenElementWithId( vxui__clay_id_from_hash( vxui_menu__mix_id( row_id, 1u ) ) );
    Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .childGap = vxui_menu__u16( scope->style.lane_gap ),
            .childAlignment = {
                .x = centered ? CLAY_ALIGN_X_CENTER : CLAY_ALIGN_X_LEFT,
                .y = CLAY_ALIGN_Y_CENTER,
            },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } );
}

static void vxui_menu__end_top_lane( void )
{
    Clay__CloseElement();
}

static void vxui_menu__begin_lane( const vxui_menu__scope* scope, uint32_t row_id, uint32_t salt, bool label_lane )
{
    const vxui_menu_style* style = &scope->style;
    Clay_SizingAxis width = CLAY_SIZING_FIT( 0 );
    if ( style->layout_mode == VXUI_MENU_LAYOUT_FORM ) {
        if ( label_lane ) {
            width = CLAY_SIZING_FIXED( style->label_lane_width );
        } else {
            switch ( style->value_lane_mode ) {
                case VXUI_MENU_VALUE_LANE_GROW:
                    width = CLAY_SIZING_GROW( 0 );
                    break;
                case VXUI_MENU_VALUE_LANE_FIXED:
                    width = CLAY_SIZING_FIXED( style->value_lane_width );
                    break;
                case VXUI_MENU_VALUE_LANE_FIT:
                default:
                    width = CLAY_SIZING_FIT( 0 );
                    break;
            }
        }
    } else if ( !label_lane ) {
        width = CLAY_SIZING_FIT( 0 );
    } else {
        width = CLAY_SIZING_GROW( 0 );
    }

    Clay__OpenElementWithId( vxui__clay_id_from_hash( vxui_menu__mix_id( row_id, salt ) ) );
    Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
        .layout = {
            .sizing = { width, CLAY_SIZING_FIT( 0 ) },
            .childAlignment = {
                .x = scope->ctx->rtl ? CLAY_ALIGN_X_RIGHT : ( scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED ? CLAY_ALIGN_X_CENTER : CLAY_ALIGN_X_LEFT ),
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } );
}

static void vxui_menu__end_lane( void )
{
    Clay__CloseElement();
}

static void vxui_menu__emit_badge_inline( const vxui_menu__scope* scope, uint32_t row_id, const char* text_key, const vxui_menu_badge_cfg* cfg )
{
    if ( !text_key || ( cfg && cfg->hidden ) ) {
        return;
    }

    const vxui_menu_style* style = &scope->style;
    float padding_x = cfg && cfg->padding_x > 0.0f ? cfg->padding_x : style->badge_padding_x;
    float padding_y = cfg && cfg->padding_y > 0.0f ? cfg->padding_y : style->badge_padding_y;
    float radius = cfg && cfg->corner_radius > 0.0f ? cfg->corner_radius : style->corner_radius;
    float min_width = cfg && cfg->min_width > 0.0f ? cfg->min_width : 0.0f;
    uint32_t font_id = cfg && cfg->font_id != 0 ? cfg->font_id : style->badge_font_id;
    float font_size = cfg && cfg->font_size > 0.0f ? cfg->font_size : style->badge_font_size;
    vxui_color fill = cfg && cfg->fill_color.a != 0 ? cfg->fill_color : style->badge_fill_color;
    vxui_color text = cfg && cfg->text_color.a != 0 ? cfg->text_color : style->badge_text_color;

    Clay__OpenElementWithId( vxui__clay_id_from_hash( vxui_menu__mix_id( row_id, 17u ) ) );
    Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
        .layout = {
            .sizing = { min_width > 0.0f ? CLAY_SIZING_FIXED( min_width ) : CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
            .padding = {
                vxui_menu__u16( padding_x ),
                vxui_menu__u16( padding_x ),
                vxui_menu__u16( padding_y ),
                vxui_menu__u16( padding_y ),
            },
            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
        },
        .backgroundColor = vxui_menu__to_clay_color( fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( radius ),
    } );
    vxui_menu__emit_text_leaf( scope->ctx, row_id, text_key, font_id, font_size, text );
    Clay__CloseElement();
}

static void vxui_menu__emit_label_and_value(
    const vxui_menu__scope* scope,
    uint32_t row_id,
    const char* label_key,
    const char* value_text,
    const vxui_menu_row_cfg* row_cfg,
    bool focused,
    bool disabled,
    bool interactive_value )
{
    vxui_color label_color = row_cfg && row_cfg->text_color.a != 0 ? row_cfg->text_color : vxui_menu__row_text_color( &scope->style, focused, disabled, false );
    vxui_color value_color = vxui_menu__row_text_color( &scope->style, focused, disabled, false );
    uint32_t font_id = vxui_menu__row_font_id( &scope->style, row_cfg );
    float font_size = vxui_menu__row_font_size( &scope->style, row_cfg );
    bool centered = scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED;
    bool has_value = value_text && value_text[ 0 ] != '\0';

    vxui_menu__begin_top_lane( scope, row_id, centered );
    auto emit_label_lane = [&]() {
        vxui_menu__begin_lane( scope, row_id, 2u, true );
        vxui_menu__emit_text_leaf( scope->ctx, row_id, label_key, font_id, font_size, label_color );
        vxui_menu__end_lane();
    };
    auto emit_value_lane = [&]() {
        if ( !has_value && !( row_cfg && row_cfg->badge_text_key ) ) {
            return;
        }
        vxui_menu__begin_lane( scope, row_id, 3u, false );
        if ( has_value ) {
            Clay__OpenElementWithId( vxui__clay_id_from_hash( vxui_menu__mix_id( row_id, 11u ) ) );
            Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
                .layout = {
                    .sizing = { CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
                    .childGap = vxui_menu__u16( scope->style.prompt_gap ),
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                },
            } );
            if ( interactive_value && ( scope->style.flags & VXUI_MENU_STYLE_VALUE_ARROWS ) != 0u && focused && !disabled ) {
                vxui_menu__emit_text_leaf( scope->ctx, row_id, scope->ctx->rtl ? ">" : "<", font_id, font_size, scope->style.arrow_color );
            }
            vxui_menu__emit_text_leaf( scope->ctx, row_id, value_text, font_id, font_size, value_color );
            if ( interactive_value && ( scope->style.flags & VXUI_MENU_STYLE_VALUE_ARROWS ) != 0u && focused && !disabled ) {
                vxui_menu__emit_text_leaf( scope->ctx, row_id, scope->ctx->rtl ? "<" : ">", font_id, font_size, scope->style.arrow_color );
            }
            Clay__CloseElement();
        }
        if ( row_cfg && row_cfg->badge_text_key ) {
            vxui_menu__emit_badge_inline( scope, row_id, row_cfg->badge_text_key, &row_cfg->badge );
        }
        vxui_menu__end_lane();
    };

    if ( scope->ctx->rtl && has_value ) {
        emit_value_lane();
        emit_label_lane();
    } else {
        emit_label_lane();
        emit_value_lane();
    }
    vxui_menu__end_top_lane();

    if ( row_cfg && row_cfg->secondary_key && ( scope->style.flags & VXUI_MENU_STYLE_SECONDARY_TEXT ) != 0u ) {
        vxui_color secondary = vxui_menu__row_text_color( &scope->style, focused, disabled, true );
        vxui_menu__emit_text_leaf( scope->ctx, row_id, row_cfg->secondary_key, font_id, scope->style.secondary_font_size, secondary );
    }
}

static void vxui_menu__register_non_focusable( vxui_ctx* ctx, vxui_decl_kind kind, uint32_t row_id )
{
    vxui__register_decl( ctx, kind, row_id, 0, 0, 0, 0, false, false, true, nullptr, nullptr );
    vxui__get_anim_state( ctx, row_id, true );
    ctx->current_decl_id = row_id;
}

static void vxui_menu__track_row( vxui_menu__scope* scope, uint32_t row_id )
{
    int row_index = scope->row_count;
    scope->row_count += 1;
    if ( scope->state ) {
        scope->state->last_emitted_row_count = scope->row_count;
        if ( scope->ctx->focused_id == row_id ) {
            scope->state->last_focused_row_id = row_id;
            scope->focused_row_index = row_index;
        }
    }
}

vxui_menu_style vxui_menu_style_form( void )
{
    return ( vxui_menu_style ) {
        .flags = VXUI_MENU_STYLE_SECONDARY_TEXT | VXUI_MENU_STYLE_AUTO_SCROLL_TO_FOCUS | VXUI_MENU_STYLE_VALUE_ARROWS | VXUI_MENU_STYLE_FOCUS_FRAME,
        .layout_mode = VXUI_MENU_LAYOUT_FORM,
        .value_lane_mode = VXUI_MENU_VALUE_LANE_FIT,
        .row_height = 34.0f,
        .row_gap = 8.0f,
        .section_gap = 16.0f,
        .label_lane_width = 220.0f,
        .value_lane_width = 160.0f,
        .padding_x = 12.0f,
        .padding_y = 8.0f,
        .lane_gap = 12.0f,
        .secondary_gap = 4.0f,
        .corner_radius = 10.0f,
        .border_width = 1.0f,
        .badge_padding_x = 8.0f,
        .badge_padding_y = 4.0f,
        .prompt_gap = 8.0f,
        .scroll_stiffness = VXUI_DEFAULT_STIFFNESS,
        .scroll_damping = VXUI_DEFAULT_DAMPING,
        .body_font_size = 24.0f,
        .secondary_font_size = 18.0f,
        .title_font_size = 30.0f,
        .badge_font_size = 16.0f,
        .focus_trait_id = VXUI_TRAIT_GLOW,
        .focus_trait_padding = 4.0f,
        .focus_trait_alpha = 0.24f,
        .panel_fill_color = { 12, 16, 24, 192 },
        .row_fill_color = { 22, 28, 40, 220 },
        .row_focus_fill_color = { 32, 46, 68, 240 },
        .row_disabled_fill_color = { 18, 22, 28, 160 },
        .row_border_color = { 60, 86, 114, 120 },
        .row_focus_border_color = { 110, 180, 255, 255 },
        .text_color = { 230, 236, 244, 255 },
        .focused_text_color = { 255, 255, 255, 255 },
        .disabled_text_color = { 132, 140, 150, 255 },
        .secondary_text_color = { 164, 172, 182, 255 },
        .section_text_color = { 164, 198, 255, 255 },
        .badge_fill_color = { 70, 96, 132, 255 },
        .badge_text_color = { 245, 250, 255, 255 },
        .prompt_fill_color = { 20, 26, 34, 220 },
        .prompt_text_color = { 235, 240, 248, 255 },
        .arrow_color = { 148, 208, 255, 255 },
        .slider_track_color = { 32, 40, 56, 255 },
        .slider_fill_color = { 96, 176, 255, 255 },
    };
}

vxui_menu_style vxui_menu_style_br_panel( void )
{
    vxui_menu_style style = vxui_menu_style_form();
    style.flags |= VXUI_MENU_STYLE_FOOTER_PROMPT_BAR;
    style.row_height = 36.0f;
    style.row_gap = 10.0f;
    style.label_lane_width = 240.0f;
    style.corner_radius = 12.0f;
    style.row_fill_color = { 18, 24, 36, 224 };
    style.row_focus_fill_color = { 34, 52, 76, 244 };
    style.row_border_color = { 76, 108, 144, 144 };
    style.row_focus_border_color = { 156, 214, 255, 255 };
    style.section_text_color = { 196, 220, 255, 255 };
    style.focus_trait_alpha = 0.30f;
    return style;
}

vxui_menu_style vxui_menu_style_br_title( void )
{
    vxui_menu_style style = vxui_menu_style_form();
    style.flags |= VXUI_MENU_STYLE_FOOTER_PROMPT_BAR;
    style.layout_mode = VXUI_MENU_LAYOUT_CENTERED;
    style.value_lane_mode = VXUI_MENU_VALUE_LANE_FIT;
    style.row_height = 40.0f;
    style.row_gap = 14.0f;
    style.section_gap = 20.0f;
    style.label_lane_width = 0.0f;
    style.padding_x = 20.0f;
    style.padding_y = 10.0f;
    style.corner_radius = 14.0f;
    style.title_font_size = 40.0f;
    style.body_font_size = 28.0f;
    style.secondary_font_size = 20.0f;
    style.focus_trait_id = VXUI_TRAIT_PULSE;
    style.focus_trait_alpha = 0.16f;
    style.row_fill_color = { 18, 22, 34, 150 };
    style.row_focus_fill_color = { 42, 60, 88, 220 };
    style.row_border_color = { 64, 90, 128, 96 };
    style.row_focus_border_color = { 178, 228, 255, 255 };
    style.text_color = { 228, 234, 244, 255 };
    style.focused_text_color = { 255, 255, 255, 255 };
    style.section_text_color = { 210, 228, 255, 255 };
    return style;
}

vxui_menu_style vxui_menu_style_compact( void )
{
    vxui_menu_style style = vxui_menu_style_form();
    style.row_height = 28.0f;
    style.row_gap = 6.0f;
    style.section_gap = 10.0f;
    style.label_lane_width = 180.0f;
    style.padding_x = 8.0f;
    style.padding_y = 6.0f;
    style.corner_radius = 8.0f;
    style.body_font_size = 20.0f;
    style.secondary_font_size = 15.0f;
    style.badge_font_size = 14.0f;
    return style;
}

void vxui_menu_begin( vxui_ctx* ctx, vxui_menu_state* state, const char* id, vxui_menu_cfg cfg )
{
    if ( !ctx || !state || !id || vxui_menu__scope_count >= VXUI_MENU__MAX_SCOPE_DEPTH ) {
        return;
    }

    vxui_menu_style style = cfg.style ? *cfg.style : vxui_menu_style_form();
    style = vxui_menu__sanitize_style( style );

    if ( state->scroll_target < 0.0f ) {
        state->scroll_target = 0.0f;
    }
    vxui__spring_step( state->scroll_target, &state->scroll_current, &state->scroll_velocity, style.scroll_stiffness, style.scroll_damping, ctx->delta_time );

    vxui_menu__scope* scope = &vxui_menu__scopes[ vxui_menu__scope_count++ ];
    *scope = {};
    scope->ctx = ctx;
    scope->state = state;
    scope->id = id;
    scope->style = style;
    scope->cfg = cfg;
    scope->viewport_id = vxui_idi( id, 1 );
    scope->content_id = vxui_idi( id, 2 );
    scope->focused_row_index = -1;

    state->last_emitted_row_count = 0;

    Clay__OpenElementWithId( vxui__clay_id_from_hash( scope->viewport_id ) );
    Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), cfg.viewport_height > 0.0f ? CLAY_SIZING_FIXED( cfg.viewport_height ) : CLAY_SIZING_GROW( 0 ) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_menu__to_clay_color( style.panel_fill_color ),
        .cornerRadius = CLAY_CORNER_RADIUS( style.corner_radius ),
        .clip = {
            .horizontal = true,
            .vertical = true,
            .childOffset = { 0.0f, -state->scroll_current },
        },
    } );

    Clay__OpenElementWithId( vxui__clay_id_from_hash( scope->content_id ) );
    Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .childGap = vxui_menu__u16( style.row_gap ),
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } );

    if ( cfg.title_key && cfg.title_key[ 0 ] != '\0' ) {
        uint32_t title_id = vxui_menu__mix_id( scope->viewport_id, 101u );
        vxui_menu__register_non_focusable( ctx, VXUI_DECL_LABEL, title_id );
        vxui_menu__begin_common_row( scope, title_id, false, false, true );
        vxui_menu__emit_text_leaf( ctx, title_id, cfg.title_key, style.title_font_id, style.title_font_size, style.section_text_color );
        if ( cfg.subtitle_key && cfg.subtitle_key[ 0 ] != '\0' ) {
            vxui_menu__emit_text_leaf( ctx, title_id, cfg.subtitle_key, style.body_font_id, style.secondary_font_size, style.secondary_text_color );
        }
        vxui_menu__end_common_row();
        vxui_menu__track_row( scope, title_id );
    }
}

void vxui_menu_end( vxui_ctx* ctx, vxui_menu_state* state )
{
    vxui_menu__scope* scope = vxui_menu__current_scope();
    if ( !scope || scope->ctx != ctx || scope->state != state ) {
        return;
    }

    Clay__CloseElement();
    Clay__CloseElement();

    if ( ( scope->style.flags & VXUI_MENU_STYLE_AUTO_SCROLL_TO_FOCUS ) != 0u ) {
        vxui_rect focus_bounds = {};
        vxui_rect viewport_bounds = {};
        vxui_rect content_bounds = {};
        bool have_focus_bounds = vxui_find_anim_bounds( ctx, ctx->focused_id, &focus_bounds ) && focus_bounds.w > 0.0f && focus_bounds.h > 0.0f;
        if ( !have_focus_bounds && ctx->focused_id != 0 ) {
            Clay_ElementData focused = Clay_GetElementData( vxui__clay_id_from_hash( ctx->focused_id ) );
            if ( focused.found ) {
                focus_bounds = vxui__rect_from_clay_box( focused.boundingBox );
                have_focus_bounds = focus_bounds.w > 0.0f && focus_bounds.h > 0.0f;
            }
        }
        if ( have_focus_bounds ) {
            Clay_ElementData viewport = Clay_GetElementData( vxui__clay_id_from_hash( scope->viewport_id ) );
            Clay_ElementData content = Clay_GetElementData( vxui__clay_id_from_hash( scope->content_id ) );
            if ( viewport.found ) {
                viewport_bounds = vxui__rect_from_clay_box( viewport.boundingBox );
                float margin = scope->style.padding_y + scope->style.row_gap;
                if ( focus_bounds.y < viewport_bounds.y + margin ) {
                    state->scroll_target -= ( viewport_bounds.y + margin ) - focus_bounds.y;
                }
                if ( focus_bounds.y + focus_bounds.h > viewport_bounds.y + viewport_bounds.h - margin ) {
                    state->scroll_target += ( focus_bounds.y + focus_bounds.h ) - ( viewport_bounds.y + viewport_bounds.h - margin );
                }
            }
            if ( content.found && viewport.found ) {
                content_bounds = vxui__rect_from_clay_box( content.boundingBox );
                float estimated_content_height = scope->row_count > 0
                    ? ( float ) scope->row_count * scope->style.row_height + ( float ) ( scope->row_count - 1 ) * scope->style.row_gap
                    : 0.0f;
                float max_target = content_bounds.h - viewport_bounds.h;
                if ( estimated_content_height - viewport_bounds.h > max_target ) {
                    max_target = estimated_content_height - viewport_bounds.h;
                }
                if ( max_target < 0.0f ) {
                    max_target = 0.0f;
                }
                if ( state->scroll_target < 0.0f ) {
                    state->scroll_target = 0.0f;
                }
                if ( state->scroll_target > max_target ) {
                    state->scroll_target = max_target;
                }
            }
        } else if ( scope->focused_row_index >= 0 ) {
            float viewport_height = scope->cfg.viewport_height > 0.0f ? scope->cfg.viewport_height : 0.0f;
            float margin = scope->style.padding_y + scope->style.row_gap;
            float row_stride = scope->style.row_height + scope->style.row_gap;
            float row_top = ( float ) scope->focused_row_index * row_stride;
            float row_bottom = row_top + scope->style.row_height;
            if ( viewport_height > 0.0f ) {
                if ( row_top < state->scroll_target + margin ) {
                    state->scroll_target = row_top - margin;
                }
                if ( row_bottom > state->scroll_target + viewport_height - margin ) {
                    state->scroll_target = row_bottom - viewport_height + margin;
                }
                float max_target = ( float ) scope->row_count * row_stride - viewport_height;
                if ( max_target < 0.0f ) {
                    max_target = 0.0f;
                }
                if ( state->scroll_target < 0.0f ) {
                    state->scroll_target = 0.0f;
                }
                if ( state->scroll_target > max_target ) {
                    state->scroll_target = max_target;
                }
            }
        }
    }

    state->last_focused_row_id = ctx->focused_id;
    state->last_emitted_row_count = scope->row_count;
    vxui_menu__scope_count -= 1;
}

void vxui_menu_section( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* title_key, vxui_menu_section_cfg cfg )
{
    ( void ) state;
    vxui_menu__scope* scope = vxui_menu__current_scope();
    if ( !scope || scope->ctx != ctx || cfg.hidden ) {
        return;
    }

    uint32_t row_id = vxui_menu__row_id( scope, id );
    vxui_menu__register_non_focusable( ctx, VXUI_DECL_LABEL, row_id );
    vxui_menu__begin_common_row( scope, row_id, false, false, cfg.centered || scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED );
    vxui_menu__emit_text_leaf(
        ctx,
        row_id,
        title_key,
        cfg.font_id != 0 ? cfg.font_id : scope->style.title_font_id,
        cfg.font_size > 0.0f ? cfg.font_size : scope->style.title_font_size,
        cfg.text_color.a != 0 ? cfg.text_color : scope->style.section_text_color );
    if ( cfg.secondary_key && ( scope->style.flags & VXUI_MENU_STYLE_SECONDARY_TEXT ) != 0u ) {
        vxui_menu__emit_text_leaf( ctx, row_id, cfg.secondary_key, scope->style.body_font_id, scope->style.secondary_font_size, scope->style.secondary_text_color );
    }
    vxui_menu__end_common_row();
    vxui_menu__track_row( scope, row_id );
}

void vxui_menu_label( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* label_key, vxui_menu_row_cfg cfg )
{
    ( void ) state;
    vxui_menu__scope* scope = vxui_menu__current_scope();
    if ( !scope || scope->ctx != ctx || !vxui_menu__visible( &cfg ) ) {
        return;
    }

    uint32_t row_id = vxui_menu__row_id( scope, id );
    vxui_menu__register_non_focusable( ctx, VXUI_DECL_LABEL, row_id );
    vxui_menu__begin_common_row( scope, row_id, false, cfg.disabled, scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED );
    vxui_menu__emit_label_and_value( scope, row_id, label_key, cfg.value_key ? cfg.value_key : "", &cfg, false, cfg.disabled, false );
    vxui_menu__end_common_row();
    vxui_menu__track_row( scope, row_id );
}

void vxui_menu_action( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* label_key, vxui_action_fn fn, vxui_menu_row_cfg row_cfg, vxui_action_cfg action_cfg )
{
    ( void ) state;
    vxui_menu__scope* scope = vxui_menu__current_scope();
    if ( !scope || scope->ctx != ctx || !vxui_menu__visible( &row_cfg ) ) {
        return;
    }

    uint32_t row_id = vxui_menu__row_id( scope, id );
    action_cfg.disabled = action_cfg.disabled || row_cfg.disabled;
    action_cfg.no_focus_ring = true;
    vxui__register_action( ctx, row_id, fn, action_cfg );
    vxui__get_anim_state( ctx, row_id, true );
    ctx->current_decl_id = row_id;

    bool focused = ctx->focused_id == row_id && !action_cfg.disabled;
    vxui_menu__begin_common_row( scope, row_id, focused, action_cfg.disabled, scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED );
    vxui_menu__emit_label_and_value( scope, row_id, label_key, row_cfg.value_key ? row_cfg.value_key : "", &row_cfg, focused, action_cfg.disabled, false );
    vxui_menu__end_common_row();
    vxui_menu__track_row( scope, row_id );
}

void vxui_menu_option( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* label_key, int* index, const char** option_keys, int count, vxui_menu_row_cfg row_cfg, vxui_option_cfg option_cfg )
{
    ( void ) state;
    vxui_menu__scope* scope = vxui_menu__current_scope();
    if ( !scope || scope->ctx != ctx || !vxui_menu__visible( &row_cfg ) ) {
        return;
    }

    uint32_t row_id = vxui_menu__row_id( scope, id );
    bool interactive = !row_cfg.disabled;
    vxui_option_cfg zero_option_cfg = {};
    bool wrap = option_cfg.wrap || std::memcmp( &option_cfg, &zero_option_cfg, sizeof( option_cfg ) ) == 0;

    vxui__register_decl(
        ctx,
        VXUI_DECL_OPTION,
        row_id,
        interactive ? option_cfg.nav_up : 0,
        interactive ? option_cfg.nav_down : 0,
        interactive ? option_cfg.nav_left : 0,
        interactive ? option_cfg.nav_right : 0,
        interactive,
        !interactive,
        true,
        nullptr,
        option_cfg.userdata );
    vxui__get_anim_state( ctx, row_id, true );
    if ( interactive ) {
        vxui__push_input_owner( ctx, row_id, false, false, true, true );
    }
    ctx->current_decl_id = row_id;

    if ( index && count > 0 ) {
        if ( *index < 0 ) {
            *index = 0;
        }
        if ( *index >= count ) {
            *index = count - 1;
        }
    }

    if ( interactive && ctx->focused_id == row_id && index && count > 0 ) {
        bool changed = false;
        if ( ( ctx->pending_nav_mask & ( 1u << VXUI_DIR_LEFT ) ) != 0u ) {
            changed = vxui__option_step( index, count, -1, wrap ) || changed;
            ctx->pending_nav_mask &= ~( 1u << VXUI_DIR_LEFT );
        }
        if ( ( ctx->pending_nav_mask & ( 1u << VXUI_DIR_RIGHT ) ) != 0u ) {
            changed = vxui__option_step( index, count, +1, wrap ) || changed;
            ctx->pending_nav_mask &= ~( 1u << VXUI_DIR_RIGHT );
        }
        if ( changed && option_cfg.on_change ) {
            option_cfg.on_change( ctx, *index, option_cfg.userdata );
        }
    }

    const char* value_key = ( index && option_keys && count > 0 ) ? option_keys[ *index ] : "";
    bool focused = interactive && ctx->focused_id == row_id;
    vxui_menu__begin_common_row( scope, row_id, focused, !interactive, scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED );
    vxui_menu__emit_label_and_value( scope, row_id, label_key, value_key, &row_cfg, focused, !interactive, true );
    vxui_menu__end_common_row();
    vxui_menu__track_row( scope, row_id );
}

void vxui_menu_slider( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* label_key, float* value, float min_value, float max_value, vxui_menu_row_cfg row_cfg, vxui_slider_cfg slider_cfg )
{
    ( void ) state;
    vxui_menu__scope* scope = vxui_menu__current_scope();
    if ( !scope || scope->ctx != ctx || !vxui_menu__visible( &row_cfg ) ) {
        return;
    }

    uint32_t row_id = vxui_menu__row_id( scope, id );
    bool interactive = !row_cfg.disabled;
    float range = max_value - min_value;
    float step = slider_cfg.step != 0.0f ? slider_cfg.step : ( range != 0.0f ? range / 20.0f : 0.0f );
    if ( step < 0.0f ) {
        step = -step;
    }

    vxui__register_decl( ctx, VXUI_DECL_SLIDER, row_id, 0, 0, 0, 0, interactive, !interactive, true, nullptr, slider_cfg.userdata );
    vxui__get_anim_state( ctx, row_id, true );
    if ( interactive ) {
        vxui__push_input_owner( ctx, row_id, false, false, true, true );
    }
    ctx->current_decl_id = row_id;

    if ( value ) {
        if ( *value < min_value ) {
            *value = min_value;
        }
        if ( *value > max_value ) {
            *value = max_value;
        }
    }

    if ( interactive && ctx->focused_id == row_id && value ) {
        float next = *value;
        if ( ( ctx->pending_nav_mask & ( 1u << VXUI_DIR_LEFT ) ) != 0u ) {
            next -= step;
            ctx->pending_nav_mask &= ~( 1u << VXUI_DIR_LEFT );
        }
        if ( ( ctx->pending_nav_mask & ( 1u << VXUI_DIR_RIGHT ) ) != 0u ) {
            next += step;
            ctx->pending_nav_mask &= ~( 1u << VXUI_DIR_RIGHT );
        }
        if ( next < min_value ) {
            next = min_value;
        }
        if ( next > max_value ) {
            next = max_value;
        }
        if ( next != *value ) {
            *value = next;
            if ( slider_cfg.on_change ) {
                slider_cfg.on_change( ctx, *value, slider_cfg.userdata );
            }
        }
    }

    float fill = 0.0f;
    if ( value && range > 0.0f ) {
        fill = vxui__clamp01( ( *value - min_value ) / range );
    }

    char formatted[ 64 ] = {};
    if ( value ) {
        const char* format = slider_cfg.format ? slider_cfg.format : "%.2f";
        std::snprintf( formatted, sizeof( formatted ), format, *value );
    }

    bool focused = interactive && ctx->focused_id == row_id;
    vxui_menu__begin_common_row( scope, row_id, focused, !interactive, scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED );

    vxui_color label_color = row_cfg.text_color.a != 0 ? row_cfg.text_color : vxui_menu__row_text_color( &scope->style, focused, !interactive, false );
    uint32_t font_id = vxui_menu__row_font_id( &scope->style, &row_cfg );
    float font_size = vxui_menu__row_font_size( &scope->style, &row_cfg );
    bool centered = scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED;

    vxui_menu__begin_top_lane( scope, row_id, centered );
    auto emit_label_lane = [&]() {
        vxui_menu__begin_lane( scope, row_id, 2u, true );
        vxui_menu__emit_text_leaf( ctx, row_id, label_key, font_id, font_size, label_color );
        vxui_menu__end_lane();
    };
    auto emit_value_lane = [&]() {
        vxui_menu__begin_lane( scope, row_id, 3u, false );
        Clay__OpenElementWithId( vxui__clay_id_from_hash( vxui_menu__mix_id( row_id, 31u ) ) );
        Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                .childGap = vxui_menu__u16( scope->style.prompt_gap ),
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } );

        if ( focused && interactive && ( scope->style.flags & VXUI_MENU_STYLE_VALUE_ARROWS ) != 0u ) {
            vxui_menu__emit_text_leaf( ctx, row_id, scope->ctx->rtl ? ">" : "<", font_id, font_size, scope->style.arrow_color );
        }

        Clay__OpenElementWithId( vxui__clay_id_from_hash( vxui_menu__mix_id( row_id, 32u ) ) );
        Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
            .layout = {
                .sizing = { scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED ? CLAY_SIZING_FIXED( 180.0f ) : CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIXED( 12.0f ) },
            },
            .backgroundColor = vxui_menu__to_clay_color( scope->style.slider_track_color ),
            .cornerRadius = CLAY_CORNER_RADIUS( 6.0f ),
        } );
        CLAY_AUTO_ID( {
            .layout = {
                .sizing = { CLAY_SIZING_PERCENT( fill ), CLAY_SIZING_GROW( 0 ) },
            },
            .backgroundColor = vxui_menu__to_clay_color( scope->style.slider_fill_color ),
            .cornerRadius = CLAY_CORNER_RADIUS( 6.0f ),
        } ) {}
        Clay__CloseElement();

        vxui_menu__emit_text_leaf( ctx, row_id, formatted, font_id, font_size, vxui_menu__row_text_color( &scope->style, focused, !interactive, false ) );
        if ( row_cfg.badge_text_key ) {
            vxui_menu__emit_badge_inline( scope, row_id, row_cfg.badge_text_key, &row_cfg.badge );
        }
        if ( focused && interactive && ( scope->style.flags & VXUI_MENU_STYLE_VALUE_ARROWS ) != 0u ) {
            vxui_menu__emit_text_leaf( ctx, row_id, scope->ctx->rtl ? "<" : ">", font_id, font_size, scope->style.arrow_color );
        }
        Clay__CloseElement();
        vxui_menu__end_lane();
    };

    if ( scope->ctx->rtl ) {
        emit_value_lane();
        emit_label_lane();
    } else {
        emit_label_lane();
        emit_value_lane();
    }
    vxui_menu__end_top_lane();

    if ( row_cfg.secondary_key && ( scope->style.flags & VXUI_MENU_STYLE_SECONDARY_TEXT ) != 0u ) {
        vxui_menu__emit_text_leaf( ctx, row_id, row_cfg.secondary_key, font_id, scope->style.secondary_font_size, vxui_menu__row_text_color( &scope->style, focused, !interactive, true ) );
    }

    vxui_menu__end_common_row();
    vxui_menu__track_row( scope, row_id );
}

void vxui_menu_badge( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const char* text_key, vxui_menu_badge_cfg cfg )
{
    ( void ) state;
    vxui_menu__scope* scope = vxui_menu__current_scope();
    if ( !scope || scope->ctx != ctx || cfg.hidden ) {
        return;
    }

    uint32_t row_id = vxui_menu__row_id( scope, id );
    vxui_menu__register_non_focusable( ctx, VXUI_DECL_LABEL, row_id );
    vxui_menu__begin_common_row( scope, row_id, false, false, scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED );
    vxui_menu__emit_badge_inline( scope, row_id, text_key, &cfg );
    vxui_menu__end_common_row();
    vxui_menu__track_row( scope, row_id );
}

void vxui_menu_prompt_bar( vxui_ctx* ctx, vxui_menu_state* state, const char* id, const vxui_menu_prompt_bar_cfg* cfg )
{
    ( void ) state;
    vxui_menu__scope* scope = vxui_menu__current_scope();
    if ( !scope || scope->ctx != ctx || !cfg || cfg->hidden || cfg->count <= 0 ) {
        return;
    }

    uint32_t row_id = vxui_menu__row_id( scope, id );
    vxui_menu__register_non_focusable( ctx, VXUI_DECL_PROMPT, row_id );
    vxui_menu__begin_common_row( scope, row_id, false, false, scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED );

    Clay__OpenElementWithId( vxui__clay_id_from_hash( vxui_menu__mix_id( row_id, 51u ) ) );
    Clay__ConfigureOpenElement( ( Clay_ElementDeclaration ) {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .childGap = vxui_menu__u16( cfg->gap > 0.0f ? cfg->gap : scope->style.prompt_gap ),
            .childAlignment = {
                .x = scope->style.layout_mode == VXUI_MENU_LAYOUT_CENTERED ? CLAY_ALIGN_X_CENTER : CLAY_ALIGN_X_LEFT,
                .y = CLAY_ALIGN_Y_CENTER,
            },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } );

    for ( int i = 0; i < cfg->count; ++i ) {
        const char* action_name = cfg->action_names ? cfg->action_names[ i ] : nullptr;
        const char* label_key = cfg->label_keys ? cfg->label_keys[ i ] : nullptr;
        if ( !action_name ) {
            continue;
        }

        CLAY_AUTO_ID( {
            .layout = {
                .sizing = { CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
                .childGap = vxui_menu__u16( scope->style.prompt_gap * 0.5f ),
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
            .backgroundColor = vxui_menu__to_clay_color( scope->style.prompt_fill_color ),
            .cornerRadius = CLAY_CORNER_RADIUS( scope->style.corner_radius ),
        } ) {
            VXUI_PROMPT( ctx, action_name );
            if ( label_key ) {
                vxui_menu__emit_text_leaf( ctx, row_id, label_key, scope->style.body_font_id, scope->style.secondary_font_size, scope->style.prompt_text_color );
            }
        }
    }

    Clay__CloseElement();
    vxui_menu__end_common_row();
    vxui_menu__track_row( scope, row_id );
}

#endif
