#include "page_table.h"

page_table_entry_t page_table[NUM_PAGES];

void page_table_init(void)
{
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i].frame = -1;
        page_table[i].valid = 0;
        page_table[i].reference_bit = 0;
        page_table[i].aging_counter = 0;
    }
}

int page_table_lookup(int page)
{
    if (page < 0 || page >= NUM_PAGES) {
        return -1;
    }

    if (!page_table[page].valid) {
        return -1;
    }

    return page_table[page].frame;
}

void page_table_update(int page, int frame)
{
    if (page < 0 || page >= NUM_PAGES) {
        return;
    }

    page_table[page].frame = frame;
    page_table[page].valid = 1;
    page_table[page].reference_bit = 1;
    page_table[page].aging_counter = 0;
}

void page_table_invalidate(int page)
{
    if (page < 0 || page >= NUM_PAGES) {
        return;
    }

    page_table[page].valid = 0;
    page_table[page].frame = -1;
    page_table[page].reference_bit = 0;
    page_table[page].aging_counter = 0;
}

void page_table_set_reference(int page)
{
    if (page < 0 || page >= NUM_PAGES) {
        return;
    }

    page_table[page].reference_bit = 1;
}

void page_table_update_aging(void)
{
    for (int page = 0; page < NUM_PAGES; page++) {
        if (!page_table[page].valid) {
            continue;
        }

        page_table[page].aging_counter >>= 1;

        if (page_table[page].reference_bit) {
            page_table[page].aging_counter |= 0x80u;
        }

        page_table[page].reference_bit = 0;
    }
}

int page_table_get_frame(int page)
{
    if (page < 0 || page >= NUM_PAGES) {
        return -1;
    }

    return page_table[page].frame;
}

int page_table_is_valid(int page)
{
    if (page < 0 || page >= NUM_PAGES) {
        return 0;
    }

    return page_table[page].valid;
}

unsigned char page_table_get_aging_counter(int page)
{
    if (page < 0 || page >= NUM_PAGES) {
        return 0;
    }

    return page_table[page].aging_counter;
}
