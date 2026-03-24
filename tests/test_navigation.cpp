#include <cstdint>
#include <cstdlib>

#include "../third_party/utest.h"
#include "../vxui.h"

typedef struct navigation_fixture
{
    uint8_t* memory;
    uint64_t memory_size;
    vxui_ctx ctx;
    int confirm_count;
} navigation_fixture;

static const char* vxui__navigation_text_fn( const char* key, void* userdata )
{
    ( void ) userdata;
    if ( std::strcmp( key, "nav.start" ) == 0 ) {
        return "Start";
    }
    if ( std::strcmp( key, "nav.options" ) == 0 ) {
        return "Options";
    }
    if ( std::strcmp( key, "nav.quit" ) == 0 ) {
        return "Quit";
    }
    if ( std::strcmp( key, "nav.extra" ) == 0 ) {
        return "Extra";
    }
    return key;
}

static void vxui__navigation_confirm( vxui_ctx* ctx, uint32_t id, void* userdata )
{
    ( void ) ctx;
    ( void ) id;
    int* count = ( int* ) userdata;
    ( *count ) += 1;
}

static int vxui__find_last_border( const vxui_draw_list* list )
{
    for ( int i = list->length - 1; i >= 0; --i ) {
        if ( list->commands[ i ].type == VXUI_CMD_BORDER ) {
            return i;
        }
    }
    return -1;
}

UTEST_F_SETUP( navigation_fixture )
{
    utest_fixture->memory_size = vxui_min_memory_size();
    utest_fixture->memory = ( uint8_t* ) std::malloc( ( size_t ) utest_fixture->memory_size );
    ASSERT_TRUE( utest_fixture->memory != nullptr );
    utest_fixture->confirm_count = 0;

    vxui_init(
        &utest_fixture->ctx,
        vxui_create_arena( utest_fixture->memory_size, utest_fixture->memory ),
        ( vxui_config ) {
            .screen_width = 640,
            .screen_height = 360,
            .max_elements = 128,
            .max_anim_states = 128,
        } );
    vxui_set_text_fn( &utest_fixture->ctx, vxui__navigation_text_fn, nullptr );
}

UTEST_F_TEARDOWN( navigation_fixture )
{
    vxui_shutdown( &utest_fixture->ctx );
    std::free( utest_fixture->memory );
    utest_fixture->memory = nullptr;
    utest_fixture->memory_size = 0;
}

UTEST_F( navigation_fixture, first_focusable_declaration_becomes_default_focus )
{
    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        VXUI_ACTION( &utest_fixture->ctx, "quit", "nav.quit", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( vxui_focused_id( &utest_fixture->ctx ), utest_fixture->ctx.decls[ 0 ].id );
}

UTEST_F( navigation_fixture, set_focus_overrides_default_focus )
{
    uint32_t quit_id = vxui_id( "quit" );
    vxui_set_focus( &utest_fixture->ctx, quit_id );

    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        VXUI_ACTION( &utest_fixture->ctx, "quit", "nav.quit", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( vxui_focused_id( &utest_fixture->ctx ), quit_id );
}

UTEST_F( navigation_fixture, confirm_dispatches_exactly_once )
{
    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
    }
    vxui_end( &utest_fixture->ctx );

    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    vxui_input_confirm( &utest_fixture->ctx );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( utest_fixture->confirm_count, 1 );
}

UTEST_F( navigation_fixture, disabled_action_ignores_confirm )
{
    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    vxui_input_confirm( &utest_fixture->ctx );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        VXUI_ACTION( &utest_fixture->ctx, "quit", "nav.quit", vxui__navigation_confirm, ( vxui_action_cfg ) {
            .disabled = true,
            .userdata = &utest_fixture->confirm_count,
        } );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( utest_fixture->confirm_count, 0 );
}

UTEST_F( navigation_fixture, spatial_neighbor_search_prefers_requested_half_plane )
{
    uint32_t start_id = 0;
    uint32_t options_id = 0;

    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    VXUI( &utest_fixture->ctx, "root", {
        .layout = {
            .childGap = 40,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } ) {
        VXUI( &utest_fixture->ctx, "row.top", {
            .layout = {
                .childGap = 120,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
            VXUI_ACTION( &utest_fixture->ctx, "options", "nav.options", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        }

        VXUI( &utest_fixture->ctx, "row.bottom", {} ) {
            VXUI_ACTION( &utest_fixture->ctx, "quit", "nav.quit", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        }
    }
    vxui_end( &utest_fixture->ctx );
    start_id = utest_fixture->ctx.decls[ 0 ].id;
    options_id = utest_fixture->ctx.decls[ 1 ].id;

    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_RIGHT );
    vxui_set_focus( &utest_fixture->ctx, start_id );
    VXUI( &utest_fixture->ctx, "root", {
        .layout = {
            .childGap = 40,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } ) {
        VXUI( &utest_fixture->ctx, "row.top", {
            .layout = {
                .childGap = 120,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
            VXUI_ACTION( &utest_fixture->ctx, "options", "nav.options", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        }

        VXUI( &utest_fixture->ctx, "row.bottom", {} ) {
            VXUI_ACTION( &utest_fixture->ctx, "quit", "nav.quit", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        }
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( vxui_focused_id( &utest_fixture->ctx ), options_id );
}

UTEST_F( navigation_fixture, explicit_override_beats_spatial_for_that_direction )
{
    uint32_t quit_id = vxui_id( "quit" );

    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    vxui_set_focus( &utest_fixture->ctx, vxui_id( "start" ) );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_RIGHT );
    VXUI( &utest_fixture->ctx, "root", {
        .layout = {
            .childGap = 32,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } ) {
        VXUI( &utest_fixture->ctx, "row.top", {
            .layout = {
                .childGap = 100,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) {
                .nav_right = quit_id,
                .userdata = &utest_fixture->confirm_count,
            } );
            VXUI_ACTION( &utest_fixture->ctx, "options", "nav.options", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        }
        VXUI_ACTION( &utest_fixture->ctx, "quit", "nav.quit", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( vxui_focused_id( &utest_fixture->ctx ), quit_id );
}

UTEST_F( navigation_fixture, no_valid_neighbor_preserves_focus )
{
    uint32_t start_id = vxui_id( "start" );

    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    vxui_set_focus( &utest_fixture->ctx, start_id );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_UP );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
    }
    vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( vxui_focused_id( &utest_fixture->ctx ), start_id );
}

UTEST_F( navigation_fixture, focus_ring_snaps_first_then_animates )
{
    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    VXUI( &utest_fixture->ctx, "root", {
        .layout = {
            .childGap = 60,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } ) {
        VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        VXUI_ACTION( &utest_fixture->ctx, "quit", "nav.quit", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
    }
    vxui_draw_list first = vxui_end( &utest_fixture->ctx );

    int first_border = vxui__find_last_border( &first );
    ASSERT_TRUE( first_border >= 0 );
    float first_y = first.commands[ first_border ].border.bounds.y;

    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    vxui_input_nav( &utest_fixture->ctx, VXUI_DIR_DOWN );
    VXUI( &utest_fixture->ctx, "root", {
        .layout = {
            .childGap = 60,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } ) {
        VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
        VXUI_ACTION( &utest_fixture->ctx, "quit", "nav.quit", vxui__navigation_confirm, ( vxui_action_cfg ) { .userdata = &utest_fixture->confirm_count } );
    }
    vxui_draw_list second = vxui_end( &utest_fixture->ctx );

    int second_border = vxui__find_last_border( &second );
    ASSERT_TRUE( second_border >= 0 );
    EXPECT_GT( second.commands[ second_border ].border.bounds.y, first_y );
}

UTEST_F( navigation_fixture, focus_ring_suppressed_by_no_focus_ring )
{
    vxui_begin( &utest_fixture->ctx, 1.0f / 60.0f );
    VXUI( &utest_fixture->ctx, "root", {} ) {
        VXUI_ACTION( &utest_fixture->ctx, "start", "nav.start", vxui__navigation_confirm, ( vxui_action_cfg ) {
            .no_focus_ring = true,
            .userdata = &utest_fixture->confirm_count,
        } );
    }
    vxui_draw_list list = vxui_end( &utest_fixture->ctx );

    EXPECT_EQ( vxui__find_last_border( &list ), -1 );
}
