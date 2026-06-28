#include <stdio.h>

#include "memory.h"
#include "config.h"
#include "page_table.h"
#include "tlb.h"

static signed char physical_memory[NUM_FRAMES][FRAME_SIZE];

/*
 * Indica qual página está carregada em cada quadro.
 * Valor -1 indica quadro livre.
 */
static int frame_to_page[NUM_FRAMES];

static FILE *backing = NULL;

void memory_init(FILE *backing_store)
{
    backing = backing_store;

    for (int i = 0; i < NUM_FRAMES; i++) {
        frame_to_page[i] = -1;

        for (int j = 0; j < FRAME_SIZE; j++) {
            physical_memory[i][j] = 0;
        }
    }
}

static int find_free_frame(void)
{
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frame_to_page[i] == -1) {
            return i;
        }
    }

    return -1;
}

int handle_page_fault(int page)
{
    int frame = find_free_frame();

    if (frame == -1) {
        int victim_page = select_victim_page();

        if (victim_page == -1) {
            fprintf(stderr, "Erro: nao foi possivel selecionar pagina vitima.\n");
            return -1;
        }

        frame = page_table_get_frame(victim_page);

        if (frame == -1) {
            fprintf(stderr, "Erro: pagina vitima invalida.\n");
            return -1;
        }

        page_table_invalidate(victim_page);
        tlb_remove(victim_page);
    }

    if (backing == NULL) {
        fprintf(stderr, "Erro interno: BACKING_STORE nao inicializado.\n");
        return -1;
    }

    if (fseek(backing, (long)page * PAGE_SIZE, SEEK_SET) != 0) {
        fprintf(stderr, "Erro: falha ao posicionar BACKING_STORE.\n");
        return -1;
    }

    size_t bytes_read = fread(physical_memory[frame],
                              sizeof(signed char),
                              FRAME_SIZE,
                              backing);

    if (bytes_read != FRAME_SIZE) {
        fprintf(stderr, "Erro: falha ao ler pagina do BACKING_STORE.\n");
        return -1;
    }

    frame_to_page[frame] = page;
    page_table_update(page, frame);

    return frame;
}

int select_victim_page(void)
{
    int victim_page = -1;
    unsigned char smallest_age = 0xFF;

    for (int page = 0; page < PAGE_TABLE_SIZE; page++) {
        if (page_table_get_frame(page) == -1) {
            continue;
        }

        unsigned char age = page_table_get_aging_counter(page);

        if (age < smallest_age) {
            smallest_age = age;
            victim_page = page;
        }
    }

    return victim_page;
}

signed char read_memory(int frame, int offset)
{
    if (frame < 0 || frame >= NUM_FRAMES || offset < 0 || offset >= FRAME_SIZE) {
        return 0;
    }

    return physical_memory[frame][offset];
}

int get_page_loaded_in_frame(int frame)
{
    if (frame < 0 || frame >= NUM_FRAMES) {
        return -1;
    }

    return frame_to_page[frame];
}
