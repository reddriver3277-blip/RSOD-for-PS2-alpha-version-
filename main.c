/*
 * PS2 RSOD (Red Screen of Death) Demo
 * Работи през USB с uLaunchELF / wLaunchELF
 * Компилира се с PS2DEV SDK (ps2sdk)
 */

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <stdio.h>
#include <graph.h>
#include <draw.h>
#include <draw3d.h>
#include <packet.h>

// GS регистри (директен достъп)
#define GS_BGCOLOUR *((volatile unsigned long*)0x120000E0)

// RSOD съобщение (опционално, ако имаш font renderer)
// За простота — само червен екран

static framebuffer_t fb;
static zbuffer_t     zb;

void init_graphics(void)
{
    // Инициализирай GS
    graph_set_mode(GRAPH_MODE_AUTO, GRAPH_MODE_INTERLACE,
                   GRAPH_MODE_FIELD, GRAPH_DISABLE);

    fb.width   = 640;
    fb.height  = 448;
    fb.mask    = 0;
    fb.psm     = GS_PSM_CT32;
    fb.address = graph_vram_allocate(fb.width, fb.height, fb.psm, GRAPH_ALIGN_PAGE);

    zb.enable  = DRAW_DISABLE;
    zb.mask    = 0;
    zb.method  = ZBUFFER_METHOD_GREATER;
    zb.zsm     = GS_ZBUF_32;
    zb.address = 0;

    graph_initialize(fb.address, fb.width, fb.height, fb.psm, 0, 0);
}

void fill_red_screen(void)
{
    packet_t *packet = packet_init(50, PACKET_NORMAL);
    qword_t  *q      = packet->data;

    // Настрой GS за clear
    q = draw_setup_environment(q, 0, &fb, &zb);
    q = draw_primitive_xyoffset(q, 0,
            (2048 - fb.width  / 2),
            (2048 - fb.height / 2));

    // Изчисти с червен цвят (R=0xFF, G=0x00, B=0x00, A=0x80)
    q = draw_clear(q, 0,
            0.0f, 0.0f,
            (float)fb.width, (float)fb.height,
            0xFF, 0x00, 0x00);   // R, G, B

    q = draw_finish(q);

    dma_channel_wait(DMA_CHANNEL_GIF, 0);
    dma_wait_fast();

    packet_send(packet, DMA_CHANNEL_GIF, DMA_FLAG_TRANSFERTAG);
    draw_wait_finish();

    graph_wait_vsync();
    packet_free(packet);
}

int main(int argc, char *argv[])
{
    // Инициализирай SIF RPC (нужно за повечето homebrew)
    SifInitRpc(0);

    // Инициализирай графиката
    init_graphics();

    // Покажи червения екран
    fill_red_screen();

    // Безкраен цикъл — стой на червения екран
    while (1) {
        // vsync за да не натоварваме процесора
        graph_wait_vsync();
    }

    return 0;
}
