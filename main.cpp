#include <cassert>
#include <string>
#include <vector>

#define CLAY_IMPLEMENTATION
#include "clay/clay.h"

#define VE_FONTCACHE_IMPL
#include "vefc/ve_fontcache.h"

#define VXUI_IMPL
#include "vxui.h"

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

    vxui_begin( &ctx, 1.0f / 60.0f );
    VXUI( &ctx, "root", {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 0 ) },
            .padding = CLAY_PADDING_ALL( 16 ),
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = { 8, 12, 24, 255 },
    } ) {
        CLAY_TEXT(
            CLAY_STRING( "VXUI phase 2" ),
            CLAY_TEXT_CONFIG( {
                .textColor = { 255, 255, 255, 255 },
                .fontId = 0,
                .fontSize = 24,
            } ) );
    }

    vxui_draw_list list = vxui_end( &ctx );
    assert( list.length > 0 );

    ve_fontcache_shutdown( &cache );
    return 0;
}
