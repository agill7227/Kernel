#include<stddef.h>
#include<stdint.h>

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA =5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED =12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,   
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGTH = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

static void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY,VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;

    for(size_t y = 0; y < VGA_HEIGTH; y++){
        for(size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

static void terminal_setcolor(uint8_t color){
    terminal_color = color;
}

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y){
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

static void terminal_putchar(char c){
    if(c == '\n'){
        terminal_column = 0;
        if(++terminal_row == VGA_HEIGTH){
            terminal_row = 0;
            return;
        }
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if(++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if(++terminal_row == VGA_HEIGTH){
            terminal_row =0;
        }
    }
}

static void terminal_writestring(const char* data){
    for(size_t i = 0; data[i] != '\0'; i++){
        terminal_putchar(data[i]);
    }
}

void kernel_main(void){
    terminal_initialize();
    terminal_writestring("Welcomn to my kernel!\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
    terminal_writestring("Successfully booted into protected mode.\n");
}



























