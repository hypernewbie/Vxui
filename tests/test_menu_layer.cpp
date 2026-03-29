#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <type_traits>
#include <vector>

#include "../third_party/utest.h"
#include "../vxui.h"
#include "../vxui_menu.h"
#include "test_layout_helpers.h"

static_assert( std::is_standard_layout_v< vxui_menu_state > );
static_assert( std::is_trivially_copyable_v< vxui_menu_state > );
static_assert( std::is_enum_v< vxui_menu_wrap_mode > );
static_assert( std::is_enum_v< vxui_menu_focus_decor > );
static_assert( VXUI_MENU_WRAP_DEFAULT == 0 );
static_assert( VXUI_MENU_WRAP_FORCE_OFF != VXUI_MENU_WRAP_FORCE_ON );
static_assert( VXUI_MENU_FOCUS_DECOR_NONE == 0 );
static_assert( VXUI_MENU_FOCUS_DECOR_GLOW != VXUI_MENU_FOCUS_DECOR_PULSE );

typedef struct menu_fixture
{
    uint8_t* memory;
    uint64_t memory_size;
    vxui_ctx ctx;
    vxui_menu_state state;
    int action_count;
    int option_change_count;
    float slider_change_value;
} menu_fixture;

static const char* vxui__menu_text( const char* key, void* userdata )
{
    menu_fixture* fixture = ( menu_fixture* ) userdata;
    if ( std::strcmp( key, "menu.start" ) == 0 ) return fixture && fixture->ctx.rtl ? "ابدأ" : "Start";
    if ( std::strcmp( key, "menu.options" ) == 0 ) return fixture && fixture->ctx.rtl ? "خيارات" : "Options";
    if ( std::strcmp( key, "menu.quit" ) == 0 ) return fixture && fixture->ctx.rtl ? "خروج" : "Quit";
    if ( std::strcmp( key, "menu.settings" ) == 0 ) return fixture && fixture->ctx.rtl ? "الإعدادات" : "Settings";
    if ( std::strcmp( key, "menu.volume" ) == 0 ) return fixture && fixture->ctx.rtl ? "الصوت" : "Volume";
    if ( std::strcmp( key, "menu.mode" ) == 0 ) return fixture && fixture->ctx.rtl ? "الوضع" : "Mode";
    if ( std::strcmp( key, "menu.subtitle" ) == 0 ) return fixture && fixture->ctx.rtl ? "قائمة" : "Main Menu";
    if ( std::strcmp( key, "menu.section.audio" ) == 0 ) return fixture && fixture->ctx.rtl ? "الصوت" : "Audio";
    if ( std::strcmp( key, "menu.help" ) == 0 ) return fixture && fixture->ctx.rtl ? "يسار ويمين" : "Use left and right";
    if ( std::strcmp( key, "menu.badge.new" ) == 0 ) return "NEW";
    if ( std::strcmp( key, "difficulty.easy" ) == 0 ) return fixture && fixture->ctx.rtl ? "سهل" : "Easy";
    if ( std::strcmp( key, "difficulty.normal" ) == 0 ) return fixture && fixture->ctx.rtl ? "عادي" : "Normal";
    if ( std::strcmp( key, "difficulty.hard" ) == 0 ) return fixture && fixture->ctx.rtl ? "صعب" : "Hard";
    if ( std::strcmp( key, "value.high" ) == 0 ) return fixture && fixture->ctx.rtl ? "مرتفع" : "High";
    if ( std::strcmp( key, "prompt.confirm" ) == 0 ) return fixture && fixture->ctx.rtl ? "تأكيد" : "Confirm";
    if ( std::strcmp( key, "prompt.cancel" ) == 0 ) return fixture && fixture->ctx.rtl ? "إلغاء" : "Cancel";
    return key;
}

static void vxui__menu_action( vxui_ctx* ctx, uint32_t id, void* userdata )
{
    ( void ) ctx;
    ( void ) id;
    int* count = ( int* ) userdata;
    *count += 1;
}

static void vxui__menu_option_changed( vxui_ctx* ctx, int value, void* userdata )
{
    ( void ) ctx;
    ( void ) value;
    int* count = ( int* ) userdata;
    *count += 1;
}

static void vxui__menu_slider_changed( vxui_ctx* ctx, float value, void* userdata )
{
    ( void ) ctx;
    float* out_value = ( float* ) userdata;
    *out_value = value;
}

static uint32_t vxui__menu_row_id( const char* menu_id, const char* row_id )
{
    return vxui_idi( menu_id, ( int ) vxui_id( row_id ) );
}

static const vxui_anim_state* vxui__menu_find_anim_state( const vxui_ctx* ctx, uint32_t id )
{
    if ( !ctx || !ctx->anim_slots || ctx->anim_capacity <= 0 ) {
        return nullptr;
    }

    for ( int i = 0; i < ctx->anim_capacity; ++i ) {
        if ( ctx->anim_slots[ i ].occupied && ctx->anim_slots[ i ].state.id == id ) {
            return &ctx->anim_slots[ i ].state;
        }
    }
    return nullptr;
}

static int vxui__menu_count_focus_glow_rects( const vxui_draw_list* list )
{
    if ( !list ) {
        return 0;
    }

    int count = 0;
    for ( int i = 0; i < list->length; ++i ) {
        const vxui_cmd* cmd = &list->commands[ i ];
        if ( cmd->type != VXUI_CMD_RECT_ROUNDED ) {
            continue;
        }
        if ( cmd->rect_rounded.color.r == 80 && cmd->rect_rounded.color.g == 200 && cmd->rect_rounded.color.b == 255 && cmd->rect_rounded.color.a > 0 ) {
            count += 1;
        }
    }
    return count;
}

UTEST_F_SETUP( menu_fixture )
{
    utest_fixture->memory_size = vxui_min_memory_size();
    utest_fixture->memory = ( uint8_t* ) std::malloc( ( size_t ) utest_fixture->memory_size );
    ASSERT_TRUE( utest_fixture->memory != nullptr );

    utest_fixture->state = {};
    utest_fixture->action_count = 0;
    utest_fixture->option_change_count = 0;
    utest_fixture->slider_change_value = 0.0f;

    vxui_init(
        &utest_fixture->ctx,
        vxui_create_arena( utest_fixture->memory_size, utest_fixture->memory ),
        ( vxui_config ) {
            .screen_width = 640,
            .screen_height = 360,
            .max_elements = 256,
            .max_anim_states = 256,
        } );

    vxui_set_text_fn( &utest_fixture->ctx, vxui__menu_text, utest_fixture );
    vxui_set_input_table( &utest_fixture->ctx, nullptr );
}

UTEST_F_TEARDOWN( menu_fixture )
{
    vxui_shutdown( &utest_fixture->ctx );
    std::free( utest_fixture->memory );
    utest_fixture->memory = nullptr;
}

UTEST_F( menu_fixture, action_rows_register_focus_and_confirm )
{
    vxui_menu_style style = vxui_menu_style_br_panel();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "start", "menu.start", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "quit", "menu.quit", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( vxui_focused_id( &utest_fixture->ctx ), vxui__menu_row_id( "menu", "start" ) );

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_input_confirm( &utest_fixture->ctx );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "start", "menu.start", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( utest_fixture->action_count, 1 );
}

UTEST_F( menu_fixture, option_rows_change_once_per_actual_mutation )
{
    const char* options[] = { "difficulty.easy", "difficulty.normal", "difficulty.hard" };
    int index = 1;
    uint32_t row_id = vxui__menu_row_id( "menu", "difficulty" );
    vxui_menu_style style = vxui_menu_style_form();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, row_id );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_RIGHT );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_option(
            &utest_fixture->ctx,
            &utest_fixture->state,
            "difficulty",
            "menu.mode",
            &index,
            options,
            3,
            ( vxui_menu_row_cfg ) { 0 },
            ( vxui_option_cfg ) {
                .on_change = vxui__menu_option_changed,
                .userdata = &utest_fixture->option_change_count,
            } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( index, 2 );
    EXPECT_EQ( utest_fixture->option_change_count, 1 );
}

UTEST_F( menu_fixture, option_wrap_default_is_stable_with_callbacks )
{
    const char* options[] = { "difficulty.easy", "difficulty.normal", "difficulty.hard" };
    int index = 2;
    uint32_t row_id = vxui__menu_row_id( "menu", "difficulty" );
    vxui_menu_style style = vxui_menu_style_form();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, row_id );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_RIGHT );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_option(
            &utest_fixture->ctx,
            &utest_fixture->state,
            "difficulty",
            "menu.mode",
            &index,
            options,
            3,
            ( vxui_menu_row_cfg ) { 0 },
            ( vxui_option_cfg ) {
                .nav_up = vxui_id( "nav.up" ),
                .nav_down = vxui_id( "nav.down" ),
                .nav_left = vxui_id( "nav.left" ),
                .nav_right = vxui_id( "nav.right" ),
                .on_change = vxui__menu_option_changed,
                .userdata = &utest_fixture->option_change_count,
            } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_TRUE( style.option_wrap_by_default );
    EXPECT_EQ( index, 0 );
    EXPECT_EQ( utest_fixture->option_change_count, 1 );
}

UTEST_F( menu_fixture, option_wrap_can_be_forced_off )
{
    const char* options[] = { "difficulty.easy", "difficulty.normal", "difficulty.hard" };
    int index = 2;
    uint32_t row_id = vxui__menu_row_id( "menu", "difficulty" );
    vxui_menu_style style = vxui_menu_style_form();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, row_id );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_RIGHT );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_option(
            &utest_fixture->ctx,
            &utest_fixture->state,
            "difficulty",
            "menu.mode",
            &index,
            options,
            3,
            ( vxui_menu_row_cfg ) {
                .wrap_mode = VXUI_MENU_WRAP_FORCE_OFF,
            },
            ( vxui_option_cfg ) {
                .on_change = vxui__menu_option_changed,
                .userdata = &utest_fixture->option_change_count,
            } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( index, 2 );
    EXPECT_EQ( utest_fixture->option_change_count, 0 );
}

UTEST_F( menu_fixture, option_wrap_can_be_forced_on )
{
    const char* options[] = { "difficulty.easy", "difficulty.normal", "difficulty.hard" };
    int index = 2;
    uint32_t row_id = vxui__menu_row_id( "menu", "difficulty" );
    vxui_menu_style style = vxui_menu_style_form();
    style.option_wrap_by_default = false;

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, row_id );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_RIGHT );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_option(
            &utest_fixture->ctx,
            &utest_fixture->state,
            "difficulty",
            "menu.mode",
            &index,
            options,
            3,
            ( vxui_menu_row_cfg ) {
                .wrap_mode = VXUI_MENU_WRAP_FORCE_ON,
            },
            ( vxui_option_cfg ) {
                .on_change = vxui__menu_option_changed,
                .userdata = &utest_fixture->option_change_count,
            } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_FALSE( style.option_wrap_by_default );
    EXPECT_EQ( index, 0 );
    EXPECT_EQ( utest_fixture->option_change_count, 1 );
}

UTEST_F( menu_fixture, slider_rows_clamp_step_and_render_value )
{
    float value = 0.95f;
    uint32_t row_id = vxui__menu_row_id( "menu", "volume" );
    vxui_menu_style style = vxui_menu_style_form();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, row_id );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_RIGHT );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_slider(
            &utest_fixture->ctx,
            &utest_fixture->state,
            "volume",
            "menu.volume",
            &value,
            0.0f,
            1.0f,
            ( vxui_menu_row_cfg ) { 0 },
            ( vxui_slider_cfg ) {
                .step = 0.5f,
                .format = "%.2f",
                .on_change = vxui__menu_slider_changed,
                .userdata = &utest_fixture->slider_change_value,
            } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list list = vxui_end( &utest_fixture->ctx );

    EXPECT_TRUE( std::fabs( value - 1.0f ) < 0.0001f );
    EXPECT_TRUE( std::fabs( utest_fixture->slider_change_value - 1.0f ) < 0.0001f );
    EXPECT_TRUE( vxui_layout_helpers::find_text_pos( &list, "1.00", nullptr ) );
}

UTEST_F( menu_fixture, hidden_rows_do_not_emit_and_disabled_rows_do_not_take_focus )
{
    const char* options[] = { "difficulty.easy", "difficulty.normal", "difficulty.hard" };
    int index = 1;
    float value = 0.75f;
    vxui_menu_style style = vxui_menu_style_form();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "hidden", "menu.start", vxui__menu_action, ( vxui_menu_row_cfg ) {
            .hidden = true,
        }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_option( &utest_fixture->ctx, &utest_fixture->state, "difficulty", "menu.mode", &index, options, 3, ( vxui_menu_row_cfg ) {
            .disabled = true,
        }, ( vxui_option_cfg ) { 0 } );
        vxui_menu_slider( &utest_fixture->ctx, &utest_fixture->state, "volume", "menu.volume", &value, 0.0f, 1.0f, ( vxui_menu_row_cfg ) {
            .disabled = true,
        }, ( vxui_slider_cfg ) {
            .format = "%.2f",
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "quit", "menu.quit", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list list = vxui_end( &utest_fixture->ctx );

    EXPECT_FALSE( vxui_layout_helpers::find_text_pos( &list, "Start", nullptr ) );
    EXPECT_TRUE( vxui_layout_helpers::find_text_pos( &list, "Normal", nullptr ) );
    EXPECT_TRUE( vxui_layout_helpers::find_text_pos( &list, "0.75", nullptr ) );
    EXPECT_EQ( vxui_focused_id( &utest_fixture->ctx ), vxui__menu_row_id( "menu", "quit" ) );
}

UTEST_F( menu_fixture, section_rows_stay_non_focusable_and_prompt_bar_emits_labels )
{
    static const char* action_names[] = { "action.confirm", "action.cancel" };
    static const char* label_keys[] = { "prompt.confirm", "prompt.cancel" };
    vxui_menu_style style = vxui_menu_style_br_panel();
    vxui_menu_prompt_bar_cfg prompt_cfg = {
        .action_names = action_names,
        .label_keys = label_keys,
        .count = 2,
    };

    vxui_begin( &utest_fixture->ctx, 0.016f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 220.0f,
        } );
        vxui_menu_section( &utest_fixture->ctx, &utest_fixture->state, "audio", "menu.section.audio", ( vxui_menu_section_cfg ) { 0 } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "options", "menu.options", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_prompt_bar( &utest_fixture->ctx, &utest_fixture->state, "prompts", &prompt_cfg );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list list = vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( vxui_focused_id( &utest_fixture->ctx ), vxui__menu_row_id( "menu", "options" ) );
    EXPECT_TRUE( vxui_layout_helpers::find_text_pos( &list, "E", nullptr ) );
    EXPECT_TRUE( vxui_layout_helpers::find_text_pos( &list, "Confirm", nullptr ) );
    EXPECT_TRUE( vxui_layout_helpers::find_text_pos( &list, "Cancel", nullptr ) );
}

UTEST_F( menu_fixture, br_title_and_panel_presets_emit_distinct_layouts )
{
    float title_x = 0.0f;
    float panel_x = 0.0f;
    vxui_menu_style title_style = vxui_menu_style_br_title();
    vxui_menu_style panel_style = vxui_menu_style_br_panel();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    VXUI( &utest_fixture->ctx, "root.title", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "title_menu", ( vxui_menu_cfg ) {
            .style = &title_style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "start", "menu.start", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list title_list = vxui_end( &utest_fixture->ctx );
    ASSERT_TRUE( vxui_layout_helpers::find_text_x( &title_list, "Start", &title_x ) );

    vxui_begin( &utest_fixture->ctx, 0.016f );
    VXUI( &utest_fixture->ctx, "root.panel", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "panel_menu", ( vxui_menu_cfg ) {
            .style = &panel_style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "start", "menu.start", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list panel_list = vxui_end( &utest_fixture->ctx );
    ASSERT_TRUE( vxui_layout_helpers::find_text_x( &panel_list, "Start", &panel_x ) );

    EXPECT_GT( title_x, panel_x );
}

UTEST_F( menu_fixture, auto_scroll_tracks_focus_inside_current_menu )
{
    const vxui_menu_style style = vxui_menu_style_compact();
    uint32_t target_row_id = vxui__menu_row_id( "menu", "row.7" );

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, target_row_id );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 96.0f,
        } );
        for ( int i = 0; i < 8; ++i ) {
            char row_id[ 16 ] = {};
            std::snprintf( row_id, sizeof( row_id ), "row.%d", i );
            vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, row_id, "menu.start", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
                .userdata = &utest_fixture->action_count,
            } );
        }
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_GT( utest_fixture->state.scroll_target, 0.0f );
    EXPECT_EQ( utest_fixture->state.last_focused_row_id, target_row_id );
}

UTEST_F( menu_fixture, auto_scroll_ignores_focus_outside_current_menu )
{
    const vxui_menu_style style = vxui_menu_style_compact();
    vxui_menu_state other_state = {};
    uint32_t other_menu_row_id = vxui__menu_row_id( "menu_b", "row.7" );

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, other_menu_row_id );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu_a", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 96.0f,
        } );
        for ( int i = 0; i < 8; ++i ) {
            char row_id[ 16 ] = {};
            std::snprintf( row_id, sizeof( row_id ), "row.%d", i );
            vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, row_id, "menu.start", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
                .userdata = &utest_fixture->action_count,
            } );
        }
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );

        vxui_menu_begin( &utest_fixture->ctx, &other_state, "menu_b", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 96.0f,
        } );
        for ( int i = 0; i < 8; ++i ) {
            char row_id[ 16 ] = {};
            std::snprintf( row_id, sizeof( row_id ), "row.%d", i );
            vxui_menu_action( &utest_fixture->ctx, &other_state, row_id, "menu.start", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
                .userdata = &utest_fixture->action_count,
            } );
        }
        vxui_menu_end( &utest_fixture->ctx, &other_state );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( utest_fixture->state.scroll_target, 0.0f );
    EXPECT_EQ( utest_fixture->state.last_focused_row_id, 0u );
    EXPECT_GT( other_state.scroll_target, 0.0f );
    EXPECT_EQ( other_state.last_focused_row_id, other_menu_row_id );
}

UTEST_F( menu_fixture, rtl_swaps_label_and_value_lanes )
{
    vxui_set_locale( &utest_fixture->ctx, "ar" );
    vxui_menu_style style = vxui_menu_style_form();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_label( &utest_fixture->ctx, &utest_fixture->state, "volume_label", "menu.volume", ( vxui_menu_row_cfg ) {
            .value_key = "value.high",
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list list = vxui_end( &utest_fixture->ctx );

    float label_x = 0.0f;
    float value_x = 0.0f;
    ASSERT_TRUE( vxui_layout_helpers::find_text_x( &list, "الصوت", &label_x ) );
    ASSERT_TRUE( vxui_layout_helpers::find_text_x( &list, "مرتفع", &value_x ) );
    EXPECT_LT( value_x, label_x );
}

UTEST_F( menu_fixture, menu_rows_publish_anim_bounds_and_badges )
{
    uint32_t row_id = vxui__menu_row_id( "menu", "settings" );
    vxui_menu_style style = vxui_menu_style_br_panel();

    vxui_begin( &utest_fixture->ctx, 0.016f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "menu", ( vxui_menu_cfg ) {
            .style = &style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "settings", "menu.settings", vxui__menu_action, ( vxui_menu_row_cfg ) {
            .badge_text_key = "menu.badge.new",
        }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list list = vxui_end( &utest_fixture->ctx );

    vxui_rect bounds = {};
    EXPECT_TRUE( vxui_find_anim_bounds( &utest_fixture->ctx, row_id, &bounds ) );
    EXPECT_GT( bounds.w, 0.0f );
    EXPECT_GT( bounds.h, 0.0f );
    EXPECT_TRUE( vxui_layout_helpers::find_text_pos( &list, "NEW", nullptr ) );
}

UTEST_F( menu_fixture, focus_decor_public_api_matches_supported_modes )
{
    uint32_t none_row_id = vxui__menu_row_id( "none_menu", "settings" );
    uint32_t pulse_row_id = vxui__menu_row_id( "pulse_menu", "settings" );
    vxui_menu_style none_style = vxui_menu_style_form();
    vxui_menu_style glow_style = vxui_menu_style_br_panel();
    vxui_menu_style pulse_style = vxui_menu_style_br_title();
    none_style.focus_decor = VXUI_MENU_FOCUS_DECOR_NONE;

    EXPECT_EQ( none_style.focus_decor, VXUI_MENU_FOCUS_DECOR_NONE );
    EXPECT_EQ( glow_style.focus_decor, VXUI_MENU_FOCUS_DECOR_GLOW );
    EXPECT_EQ( pulse_style.focus_decor, VXUI_MENU_FOCUS_DECOR_PULSE );

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, none_row_id );
    VXUI( &utest_fixture->ctx, "root.none", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "none_menu", ( vxui_menu_cfg ) {
            .style = &none_style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "settings", "menu.settings", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list none_list = vxui_end( &utest_fixture->ctx );
    EXPECT_EQ( vxui__menu_count_focus_glow_rects( &none_list ), 0 );

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, vxui__menu_row_id( "glow_menu", "settings" ) );
    VXUI( &utest_fixture->ctx, "root.glow", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "glow_menu", ( vxui_menu_cfg ) {
            .style = &glow_style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "settings", "menu.settings", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_draw_list glow_list = vxui_end( &utest_fixture->ctx );
    EXPECT_GT( vxui__menu_count_focus_glow_rects( &glow_list ), 0 );

    vxui_begin( &utest_fixture->ctx, 0.016f );
    vxui_set_focus( &utest_fixture->ctx, pulse_row_id );
    VXUI( &utest_fixture->ctx, "root.pulse", {} ) {
        vxui_menu_begin( &utest_fixture->ctx, &utest_fixture->state, "pulse_menu", ( vxui_menu_cfg ) {
            .style = &pulse_style,
            .viewport_height = 180.0f,
        } );
        vxui_menu_action( &utest_fixture->ctx, &utest_fixture->state, "settings", "menu.settings", vxui__menu_action, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) {
            .userdata = &utest_fixture->action_count,
        } );
        vxui_menu_end( &utest_fixture->ctx, &utest_fixture->state );
    }
    vxui_end( &utest_fixture->ctx );

    const vxui_anim_state* pulse_state = vxui__menu_find_anim_state( &utest_fixture->ctx, pulse_row_id );
    ASSERT_TRUE( pulse_state != nullptr );
    EXPECT_GT( pulse_state->scale_current, 1.0f );
    EXPECT_LT( pulse_state->opacity_current, 1.0f );
}
