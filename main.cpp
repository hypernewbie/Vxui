#include <cassert>
#include <string>
#include <vector>

#define CLAY_IMPLEMENTATION
#include "clay/clay.h"

#define VE_FONTCACHE_IMPL
#include "vefc/ve_fontcache.h"

#define VXUI_IMPL
#include "vxui.h"

static const char* vxui_demo_text( const char* key, void* userdata )
{
    ( void ) userdata;

    if ( std::strcmp( key, "menu.title" ) == 0 ) {
        return "VXUI Demo";
    }
    if ( std::strcmp( key, "menu.volume" ) == 0 ) {
        return "Volume";
    }
    if ( std::strcmp( key, "menu.start" ) == 0 ) {
        return "Start";
    }
    if ( std::strcmp( key, "menu.quit" ) == 0 ) {
        return "Quit";
    }
    return key;
}

static void vxui_demo_action( vxui_ctx* ctx, uint32_t id, void* userdata )
{
    ( void ) ctx;
    ( void ) id;
    ( void ) userdata;
}

int main( void )
{
    std::vector< uint8_t > memory( ( size_t ) vxui_min_memory_size() );
    vxui_ctx ctx = {};
    vxui_init(
        &ctx,
        vxui_create_arena( ( uint64_t ) memory.size(), memory.data() ),
        ( vxui_config ) {
            .screen_width = 640,
            .screen_height = 360,
            .max_elements = 64,
        } );

    ve_fontcache cache = {};
    ve_fontcache_init( &cache, false );
    vxui_set_fontcache( &ctx, &cache );
    vxui_set_text_fn( &ctx, vxui_demo_text, nullptr );

    vxui_begin( &ctx, 1.0f / 60.0f );
    VXUI( &ctx, "root", {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 0 ) },
            .padding = CLAY_PADDING_ALL( 16 ),
            .childGap = 12,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = { 8, 12, 24, 255 },
    } ) {
        VXUI_LABEL( &ctx, "menu.title", ( vxui_label_cfg ) {
            .font_size = 48.0f,
            .color = { 255, 255, 255, 255 },
        } );

        VXUI_VALUE( &ctx, "menu.volume", 0.85f, ( vxui_value_cfg ) {
            .font_size = 24.0f,
            .color = { 180, 220, 255, 255 },
            .format = "%.2f",
        } );

        VXUI_ACTION( &ctx, "start", "menu.start", vxui_demo_action, ( vxui_action_cfg ) { 0 } );
        VXUI_ACTION( &ctx, "quit", "menu.quit", vxui_demo_action, ( vxui_action_cfg ) { 0 } );
    }

    vxui_draw_list list = vxui_end( &ctx );
    assert( list.length > 0 );
    assert( ctx.decl_count == 4 );

    for ( int i = 0; i < list.length; ++i ) {
        if ( list.commands[ i ].type == VXUI_CMD_TEXT ) {
            vxui_flush_text( &ctx );
        }
    }

    ve_fontcache_shutdown( &cache );
    return 0;
}
