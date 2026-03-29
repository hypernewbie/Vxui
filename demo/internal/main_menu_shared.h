#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>

#include "layout_contract.h"
#include "theme.h"
#include "vxui.h"
#include "vxui_menu.h"

struct vxui_demo_main_menu_preview
{
    const char* row_id;
    const char* title;
    const char* subtitle;
    const char* body;
    const char* badge_key;
};

struct vxui_demo_controls_block_copy
{
    const char* title;
    const char* lines[ 4 ];
};

struct vxui_demo_main_menu_visuals
{
    uint32_t body_font_id;
    uint32_t title_font_id;
    uint32_t section_font_id;
};

struct vxui_demo_main_menu_shell_copy
{
    const char* hero_title;
    const char* hero_banner;
    const char* preview_label_text;
    const char* locale_label;
    const char* locale_value;
    const char* prompts_label;
    const char* prompts_value;
    const char* screens_label;
    int screen_count;
    const char* top_label;
    const char* top_value;
    const char* confirm_label;
    const char* cancel_label;
};

struct vxui_demo_controls_block_visuals
{
    uint16_t padding;
    uint16_t row_gap;
    float title_font_size;
    float line_font_size;
    float line_gap_min;
    float min_height;
    bool compact_copy;
    uint8_t visible_line_count;
};

inline Clay_String vxui_demo_shared_clay_string( const char* text )
{
    return ( Clay_String ) {
        .isStaticallyAllocated = false,
        .length = text ? ( int32_t ) std::strlen( text ) : 0,
        .chars = text ? text : "",
    };
}

inline constexpr vxui_demo_main_menu_preview VXUI_DEMO_MAIN_MENU_PREVIEWS[] = {
    { "sortie", "Sortie Command", "Deploy a live mission stub.", "Choose a mission rail, set difficulty, and run a full launch-to-results loop without leaving the front end.", "badge.recommended" },
    { "loadout", "Loadout Deck", "Tune the frame before launch.", "Swap the chassis, primary, support, and system packages while watching the command-deck stat card react in real time.", "badge.demo" },
    { "archives", "Archives Vault", "Badge, lock, and detail patterns.", "Inspect intel, frame logs, and signal captures with locked entries still visible to show list/detail states.", "badge.demo" },
    { "settings", "Settings Grid", "Carry-forward locale and prompt controls.", "Locale switching, prompt-table switching, scroll-safe settings layout, and visual toggles all live in one bounded surface.", "badge.recommended" },
    { "records", "Records Relay", "Scoreboard and debrief split view.", "Browse demo-owned run summaries, board filters, and result notes in a wider desktop list/detail surface.", "badge.demo" },
    { "credits", "Credits Channel", "Acknowledgements and stack callouts.", "A lightweight text-heavy screen that still lives inside the same command-deck shell and navigation model.", "badge.demo" },
    { "quit", "Quit Demo", "Exit the command deck.", "Close the demo executable after proving the front-end loop and shell flow.", nullptr },
};

inline std::string vxui_demo_controls_block_title_id( const char* id )
{
    return std::string( id ) + ".title";
}

inline std::string vxui_demo_controls_block_line_id( const char* id, int index )
{
    return std::string( id ) + ".line." + std::to_string( index );
}

inline bool vxui_demo_shared_locale_matches( const char* locale, const char* prefix )
{
    if ( !locale || !prefix ) {
        return false;
    }
    size_t prefix_length = std::strlen( prefix );
    if ( std::strncmp( locale, prefix, prefix_length ) != 0 ) {
        return false;
    }
    return locale[ prefix_length ] == '\0' || locale[ prefix_length ] == '-' || locale[ prefix_length ] == '_';
}

inline const char* vxui_demo_main_menu_preview_label( const char* locale )
{
    if ( vxui_demo_shared_locale_matches( locale, "ja" ) ) {
        return "注目セクター";
    }
    if ( vxui_demo_shared_locale_matches( locale, "ar" ) ) {
        return "القطاع المحدد";
    }
    return "Focused Sector";
}

inline const char* vxui_demo_badge_text( const char* locale, const char* badge_key )
{
    if ( !badge_key ) {
        return nullptr;
    }
    if ( std::strcmp( badge_key, "badge.recommended" ) == 0 ) {
        if ( vxui_demo_shared_locale_matches( locale, "ja" ) ) {
            return "推奨";
        }
        if ( vxui_demo_shared_locale_matches( locale, "ar" ) ) {
            return "موصى به";
        }
        return "Recommended";
    }
    if ( std::strcmp( badge_key, "badge.demo" ) == 0 ) {
        if ( vxui_demo_shared_locale_matches( locale, "ja" ) ) {
            return "デモ";
        }
        if ( vxui_demo_shared_locale_matches( locale, "ar" ) ) {
            return "عرض";
        }
        return "Demo";
    }
    if ( std::strcmp( badge_key, "badge.selected" ) == 0 ) {
        if ( vxui_demo_shared_locale_matches( locale, "ja" ) ) {
            return "選択中";
        }
        if ( vxui_demo_shared_locale_matches( locale, "ar" ) ) {
            return "محدد";
        }
        return "Selected";
    }
    if ( std::strcmp( badge_key, "badge.locked" ) == 0 ) {
        if ( vxui_demo_shared_locale_matches( locale, "ja" ) ) {
            return "ロック";
        }
        if ( vxui_demo_shared_locale_matches( locale, "ar" ) ) {
            return "مغلق";
        }
        return "Locked";
    }
    return badge_key;
}

inline vxui_demo_controls_block_copy vxui_demo_controls_block_copy_for_locale( const char* locale, bool compact_copy = false )
{
    if ( vxui_demo_shared_locale_matches( locale, "ja" ) ) {
        if ( compact_copy ) {
            return {
                "操作",
                {
                    "矢印 / Dパッドで移動。",
                    "Enter / Space / Aで決定。",
                    "Esc / Backspace / Bで戻る。",
                    "[ / ]で列切替。1-5で設定。",
                },
            };
        }
        return {
            "操作",
            {
                "矢印 / Dパッドで移動、Enter / Space / A で決定。",
                "Escape / Backspace / B で戻る、[ と ] でタブ切替。",
                "1 2 3 で言語、4 5 でプロンプト表を切替。",
#ifdef VXUI_DEBUG
                "F1 でデバッガー、R でホットリロード確認、C で書き出しをコピー。",
#else
                "デバッグビルドでシーケンスデバッガーを有効にできます。",
#endif
            },
        };
    }
    if ( vxui_demo_shared_locale_matches( locale, "ar" ) ) {
        if ( compact_copy ) {
            return {
                "التحكم",
                {
                    "الأسهم / الاتجاهات للحركة.",
                    "Enter / Space / A للتأكيد.",
                    "Esc / Backspace / B للرجوع.",
                    "[ / ] للأعمدة. 1-5 للإعداد.",
                },
            };
        }
        return {
            "التحكم",
            {
                "الأسهم / لوحة الاتجاهات للتنقل، و Enter / Space / A للتأكيد.",
                "Escape / Backspace / B للرجوع، و [ و ] للتبديل بين الأعمدة.",
                "1 2 3 لتبديل اللغة، و 4 5 لتبديل جدول الأزرار.",
#ifdef VXUI_DEBUG
                "F1 لفتح المصحح، و R للتحقق من إعادة التحميل، و C لنسخ المخرجات.",
#else
                "شغّل بناء التصحيح لتفعيل مصحح التسلسلات.",
#endif
            },
        };
    }
    if ( compact_copy ) {
        return {
            "Controls",
            {
                "Arrows / D-pad move.",
                "Enter / Space / A confirm.",
                "Esc / Backspace / B back.",
                "[ / ] lanes. 1-5 setup.",
            },
        };
    }
    return {
        "Controls",
        {
            "Arrows / D-pad navigate, Enter / Space / A confirm.",
            "Escape / Backspace / B cancels, [ and ] tab between lanes.",
            "1 2 3 switch locale, 4 5 switch prompt table.",
#ifdef VXUI_DEBUG
            "F1 toggles the debugger, R polls hot reload, C copies or prints exports.",
#else
            "Build a Debug configuration to enable the sequence debugger overlay.",
#endif
        },
    };
}

inline vxui_menu_style vxui_demo_make_title_deck_menu_style( uint32_t body_font_id, uint32_t title_font_id )
{
    vxui_menu_style style = vxui_menu_style_br_title();
    style.option_wrap_by_default = true;
    style.body_font_id = body_font_id;
    style.title_font_id = title_font_id;
    style.badge_font_id = body_font_id;
    style.body_font_size = 24.0f;
    style.title_font_size = 42.0f;
    style.badge_font_size = 15.0f;
    style.row_height = 58.0f;
    style.row_gap = 10.0f;
    style.section_gap = 14.0f;
    style.padding_x = 20.0f;
    style.padding_y = 18.0f;
    vxui_demo_apply_title_menu_theme( style );
    return style;
}

inline const vxui_demo_main_menu_preview* vxui_demo_main_menu_preview_from_focused_row( uint32_t focused_row_id )
{
    for ( const vxui_demo_main_menu_preview& preview : VXUI_DEMO_MAIN_MENU_PREVIEWS ) {
        if ( focused_row_id == vxui_idi( "main.command_menu", ( int ) vxui_id( preview.row_id ) ) ) {
            return &preview;
        }
    }
    return &VXUI_DEMO_MAIN_MENU_PREVIEWS[ 0 ];
}

inline void vxui_demo_emit_compact_meta_row_text(
    vxui_ctx* ctx,
    const char* id,
    const char* label_text,
    const char* value_text,
    bool rtl,
    const vxui_label_cfg* text_cfg = nullptr )
{
    CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
            .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_INLINE_GAP,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } ) {
        if ( rtl ) {
            VXUI_LABEL( ctx, value_text, text_cfg ? *text_cfg : ( vxui_label_cfg ) { 0 } );
            VXUI_LABEL( ctx, label_text, text_cfg ? *text_cfg : ( vxui_label_cfg ) { 0 } );
        } else {
            VXUI_LABEL( ctx, label_text, text_cfg ? *text_cfg : ( vxui_label_cfg ) { 0 } );
            VXUI_LABEL( ctx, value_text, text_cfg ? *text_cfg : ( vxui_label_cfg ) { 0 } );
        }
    }
}

inline void vxui_demo_emit_prompt_pair_text(
    vxui_ctx* ctx,
    const char* id,
    const char* action_name,
    const char* label_text,
    const vxui_label_cfg* label_cfg = nullptr )
{
    CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
            .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_INLINE_GAP,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } ) {
        VXUI_PROMPT( ctx, action_name );
        VXUI_LABEL( ctx, label_text, label_cfg ? *label_cfg : ( vxui_label_cfg ) { 0 } );
    }
}

inline void vxui_demo_emit_main_menu_footer(
    vxui_ctx* ctx,
    const char* locale_label,
    const char* locale_value,
    const char* prompts_label,
    const char* prompts_value,
    const char* screens_label,
    int screen_count,
    const char* top_label,
    const char* top_value,
    const char* confirm_label,
    const char* cancel_label,
    bool rtl,
    bool compact_vertical )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const uint16_t footer_padding = ( uint16_t ) ( compact_vertical ? 4 : 12 );
    const uint16_t footer_gap = ( uint16_t ) ( compact_vertical ? 4 : 8 );
    const uint16_t row_padding = ( uint16_t ) ( compact_vertical ? 4 : 8 );
    const vxui_label_cfg prompt_footer_text_cfg = vxui_demo_text_style( 0, compact_vertical ? 15.0f : 17.0f, theme.utility_text );
    const vxui_label_cfg compact_footer_text_cfg = vxui_demo_text_style( 0, compact_vertical ? 14.0f : 15.0f, theme.muted_text );
    const vxui_value_cfg compact_footer_value_cfg = vxui_demo_value_style( 0, compact_vertical ? 14.0f : 15.0f, theme.muted_text, "%.0f" );
    CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.footer" ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .padding = CLAY_PADDING_ALL( footer_padding ),
            .childGap = footer_gap,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_demo_clay_color( theme.utility_fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( compact_vertical ? 10.0f : 12.0f ),
        .border = vxui_demo_panel_border( theme.utility_border, 1 ),
    } ) {
        if ( compact_vertical ) {
            CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.footer.prompts" ) ), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                    .padding = CLAY_PADDING_ALL( row_padding ),
                    .childGap = footer_gap,
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                },
                .backgroundColor = vxui_demo_clay_color( theme.action_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 10 ),
                .border = vxui_demo_panel_border( theme.action_border, 1 ),
            } ) {
                vxui_demo_emit_prompt_pair_text( ctx, "main.prompt.confirm", "action.confirm", confirm_label, &prompt_footer_text_cfg );
                vxui_demo_emit_prompt_pair_text( ctx, "main.prompt.cancel", "action.cancel", cancel_label, &prompt_footer_text_cfg );
            }
            CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.footer.status" ) ), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                    .padding = CLAY_PADDING_ALL( row_padding ),
                    .childGap = footer_gap,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 10 ),
                .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
            } ) {
                CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.footer.status.row.compact" ) ), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                        .childGap = footer_gap,
                        .childAlignment = { .x = rtl ? CLAY_ALIGN_X_RIGHT : CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER },
                        .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    },
                } ) {
                    vxui_demo_emit_compact_meta_row_text( ctx, "main.footer.status.locale", locale_label, locale_value, rtl, &compact_footer_text_cfg );
                    vxui_demo_emit_compact_meta_row_text( ctx, "main.footer.status.prompts", prompts_label, prompts_value, rtl, &compact_footer_text_cfg );
                    VXUI_VALUE( ctx, screens_label, ( float ) screen_count, compact_footer_value_cfg );
                    vxui_demo_emit_compact_meta_row_text( ctx, "main.footer.status.top", top_label, top_value, rtl, &compact_footer_text_cfg );
                }
            }
        } else {
            CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.footer.prompts" ) ), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                    .padding = CLAY_PADDING_ALL( row_padding ),
                    .childGap = footer_gap,
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                },
                .backgroundColor = vxui_demo_clay_color( theme.action_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 10 ),
                .border = vxui_demo_panel_border( theme.action_border, 1 ),
            } ) {
                vxui_demo_emit_prompt_pair_text( ctx, "main.prompt.confirm", "action.confirm", confirm_label, &prompt_footer_text_cfg );
                vxui_demo_emit_prompt_pair_text( ctx, "main.prompt.cancel", "action.cancel", cancel_label, &prompt_footer_text_cfg );
            }
            CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.footer.status" ) ), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                    .padding = CLAY_PADDING_ALL( row_padding ),
                    .childGap = footer_gap,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 10 ),
                .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
            } ) {
                CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.footer.status.row.primary" ) ), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                        .childGap = footer_gap,
                        .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    },
                } ) {
                    vxui_demo_emit_compact_meta_row_text( ctx, "main.footer.status.locale", locale_label, locale_value, rtl, &compact_footer_text_cfg );
                    vxui_demo_emit_compact_meta_row_text( ctx, "main.footer.status.prompts", prompts_label, prompts_value, rtl, &compact_footer_text_cfg );
                }
                CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.footer.status.row.secondary" ) ), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                        .childGap = footer_gap,
                        .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    },
                } ) {
                    VXUI_VALUE( ctx, screens_label, ( float ) screen_count, compact_footer_value_cfg );
                    vxui_demo_emit_compact_meta_row_text( ctx, "main.footer.status.top", top_label, top_value, rtl, &compact_footer_text_cfg );
                }
            }
        }
    }
}

inline vxui_demo_controls_block_visuals vxui_demo_controls_block_visuals_for_height( float surface_max_height, float owner_width )
{
    const vxui_demo_controls_block_contract contract = vxui_demo_get_controls_block_contract( surface_max_height, owner_width );
    return {
        contract.padding,
        contract.row_gap,
        contract.title_font_size,
        contract.line_font_size,
        contract.line_gap_min,
        contract.min_height,
        contract.compact_copy,
        contract.visible_line_count,
    };
}

inline void vxui_demo_emit_controls_block(
    vxui_ctx* ctx,
    const char* id,
    const char* locale,
    uint32_t body_font_id,
    const vxui_demo_controls_block_visuals& visuals,
    const vxui_label_cfg* section_cfg )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const vxui_demo_controls_block_copy copy = vxui_demo_controls_block_copy_for_locale( locale, visuals.compact_copy );
    const float line_gap = std::max( visuals.line_gap_min, ( float ) visuals.row_gap );
    const float title_font_size = visuals.compact_copy ? std::max( 15.0f, visuals.title_font_size - 1.0f ) : std::max( 20.0f, visuals.title_font_size - 2.0f );
    const float line_font_size = visuals.compact_copy ? std::max( 13.0f, visuals.line_font_size - 1.0f ) : std::max( 18.0f, visuals.line_font_size - 2.0f );
    CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( visuals.min_height ) },
            .padding = CLAY_PADDING_ALL( visuals.padding ),
            .childGap = ( uint16_t ) line_gap,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_demo_clay_color( theme.utility_fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
        .border = vxui_demo_panel_border( theme.utility_border, 1 ),
    } ) {
        const std::string title_id = vxui_demo_controls_block_title_id( id );
        CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( title_id.c_str() ) ), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        } ) {
            VXUI_LABEL( ctx, copy.title, section_cfg ? *section_cfg : ( vxui_label_cfg ) {
                .font_id = body_font_id,
                .font_size = title_font_size,
                .color = theme.section_text,
            } );
        }
        for ( int line_index = 0; line_index < visuals.visible_line_count; ++line_index ) {
            const std::string line_id = vxui_demo_controls_block_line_id( id, line_index );
            CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( line_id.c_str() ) ), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
            } ) {
                VXUI_LABEL( ctx, copy.lines[ line_index ], ( vxui_label_cfg ) {
                    .font_id = body_font_id,
                    .font_size = line_font_size,
                    .color = theme.utility_text,
                } );
            }
        }
    }
}

inline void vxui_demo_emit_main_menu_preview_panel(
    vxui_ctx* ctx,
    const char* locale,
    const vxui_demo_main_menu_preview& preview,
    const vxui_demo_main_menu_layout_spec& layout,
    const vxui_demo_main_menu_visuals& visuals,
    const char* preview_label_text )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const bool compact = layout.surface_max_height <= 650.0f;
    const float panel_padding = compact ? 8.0f : layout.preview_panel_padding;
    const float header_min_height = compact ? 76.0f : layout.preview_header_min_height;
    const float header_label_size = compact ? 18.0f : 26.0f;
    const float title_size = compact ? 30.0f : 40.0f;
    const float subtitle_size = compact ? 14.0f : 21.0f;
    const float badge_size = compact ? 12.0f : 15.0f;
    const float body_size = compact ? 14.0f : 18.0f;
    const float body_gap = compact ? 6.0f : 9.0f;
    const float preview_to_help_gap = 8.0f;
    const float body_bottom_guard = compact ? 4.0f : layout.preview_viewport_bottom_guard;
    const float help_owner_width = std::max( 0.0f, layout.preview_panel_width - panel_padding * 2.0f );
    const vxui_demo_controls_block_visuals controls_visuals =
        vxui_demo_controls_block_visuals_for_height( layout.surface_max_height, help_owner_width );

    VXUI( ctx, "main.preview_panel", {
        .layout = {
            .sizing = { CLAY_SIZING_FIXED( layout.preview_panel_width ), CLAY_SIZING_GROW( 0 ) },
            .padding = CLAY_PADDING_ALL( ( uint16_t ) panel_padding ),
            .childGap = ( uint16_t ) layout.preview_header_gap,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
        .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
    } ) {
        VXUI( ctx, "main.preview_header", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( header_min_height ) },
                .childGap = 8,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        } ) {
            VXUI_LABEL( ctx, preview_label_text, ( vxui_label_cfg ) {
                .font_id = visuals.section_font_id,
                .font_size = header_label_size,
                .color = theme.section_text,
            } );
            VXUI_LABEL( ctx, preview.title, ( vxui_label_cfg ) {
                .font_id = visuals.title_font_id,
                .font_size = title_size,
                .color = theme.title_text,
            } );
        }

        VXUI( ctx, "main.preview_body_region", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 0 ) },
                .childGap = ( uint16_t ) preview_to_help_gap,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        } ) {
            CLAY( Clay_GetElementId( vxui_demo_shared_clay_string( "main.preview_body_viewport" ) ), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 0 ) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .clip = {
                    .horizontal = true,
                    .vertical = true,
                },
            } ) {
                VXUI( ctx, "main.preview_body_content", {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                        .padding = { 0, 0, 0, ( uint16_t ) body_bottom_guard },
                        .childGap = ( uint16_t ) body_gap,
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    },
                    } ) {
                        VXUI_LABEL( ctx, preview.subtitle, vxui_demo_text_style( visuals.section_font_id, subtitle_size, theme.accent_cool ) );
                        const char* badge_text = vxui_demo_badge_text( locale, preview.badge_key );
                        if ( badge_text && !compact ) {
                            VXUI( ctx, "main.preview.badge", {
                                .layout = {
                                    .sizing = { CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
                                .padding = { 10, 10, 5, 5 },
                            },
                            .backgroundColor = vxui_demo_clay_color( theme.badge_fill ),
                            .cornerRadius = CLAY_CORNER_RADIUS( 8 ),
                            .border = vxui_demo_panel_border( theme.action_border, 1 ),
                        } ) {
                            VXUI_LABEL( ctx, badge_text, vxui_demo_text_style( visuals.body_font_id, badge_size, theme.badge_text ) );
                        }
                    }
                    VXUI( ctx, "main.preview.body_text", {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        },
                    } ) {
                        VXUI_LABEL( ctx, preview.body, vxui_demo_text_style( visuals.body_font_id, body_size, theme.body_text ) );
                    }
                }
            }
            vxui_demo_emit_controls_block( ctx, "main.help", locale, visuals.body_font_id, controls_visuals, nullptr );
        }
    }
}

template <typename TEmitCommandPanel>
inline void vxui_demo_emit_main_menu_shell(
    vxui_ctx* ctx,
    bool rtl,
    const char* locale,
    const vxui_demo_main_menu_layout_spec& layout,
    const vxui_demo_main_menu_visuals& visuals,
    const vxui_demo_main_menu_shell_copy& copy,
    const vxui_demo_main_menu_preview& preview,
    TEmitCommandPanel&& emit_command_panel )
{
    const bool compact_vertical = layout.surface_max_height <= 650.0f;
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const uint16_t hero_padding = ( uint16_t ) ( compact_vertical ? 10 : 18 );

    VXUI( ctx, "main.hero", {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .padding = CLAY_PADDING_ALL( hero_padding ),
            .childGap = 8,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_demo_clay_color( theme.hero_surface_fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( 14 ),
        .border = vxui_demo_panel_border( theme.hero_surface_border, 1 ),
    } ) {
        VXUI_LABEL( ctx, copy.hero_title, ( vxui_label_cfg ) {
            .font_id = visuals.title_font_id,
            .font_size = compact_vertical ? 32.0f : 50.0f,
            .color = theme.title_text,
        } );
        if ( !compact_vertical && copy.hero_banner && copy.hero_banner[ 0 ] ) {
            VXUI_LABEL( ctx, copy.hero_banner, ( vxui_label_cfg ) {
                .font_id = visuals.section_font_id,
                .font_size = 24.0f,
                .color = theme.accent_cool,
            } );
        }
    }

    VXUI( ctx, "main.deck", {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 180.0f ) },
            .childGap = ( uint16_t ) layout.deck_gap,
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } ) {
        VXUI( ctx, "main.command_panel", {
            .layout = {
                .sizing = {
                    CLAY_SIZING_FIXED( layout.command_panel_width ),
                    CLAY_SIZING_GROW( 0 ),
                },
            },
            .backgroundColor = vxui_demo_clay_color( theme.primary_panel_fill ),
            .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
            .border = vxui_demo_panel_border( theme.primary_panel_border, 1 ),
        } ) {
            emit_command_panel( layout.command_menu_viewport_height );
        }

        vxui_demo_emit_main_menu_preview_panel(
            ctx,
            locale,
            preview,
            layout,
            visuals,
            copy.preview_label_text );
    }

    vxui_demo_emit_main_menu_footer(
        ctx,
        copy.locale_label,
        copy.locale_value,
        copy.prompts_label,
        copy.prompts_value,
        copy.screens_label,
        copy.screen_count,
        copy.top_label,
        copy.top_value,
        copy.confirm_label,
        copy.cancel_label,
        rtl,
        compact_vertical );
}
