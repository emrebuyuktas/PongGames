/*
* Copyright (C) 2014  Arjun Sreedharan
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/
#include "keyboard_map.h"

/* there are 25 lines each of 80 columns; each element takes 2 bytes */
#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define ENTER_KEY_CODE 0x1C

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

/* current cursor location */
unsigned int current_loc = 0;
/* video memory begins at address 0xb8000 */
char *vidptr = (char*)0xb8000;

/**********  MY DEFINITIONS ***********/
unsigned int r1_xpos = 1;
unsigned int r1_ypos = 11;
unsigned int r2_xpos = 78;
unsigned int r2_ypos = 11;
unsigned int move = 1;

unsigned int ball_xpos = 40;
unsigned int ball_ypos = 12;
unsigned int ball_xdirection=1;
unsigned int ball_ydirection=1;
void gotoxy(unsigned int x, unsigned int y);
void draw_strxy(const char *str,unsigned int x, unsigned int y);
void draw_borders();
void draw_rkt(void);
void clear_rkt(char keycode);
void move_rkt_right(char keycode);

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];


void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

void kb_init(void)
{
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	write_port(0x21 , 0xFD);
}
void sleep(int i)
{
	int j=0;
	int k=0;
	for(k;k<=i;k++)
	{
		for(j;j<=i;j++){j+=1;}	
		k+=1;
	}	
}

void kprint(const char *str)
{
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++];
		vidptr[current_loc++] = 0x07;
	}
}

void kprint_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}

void clear_screen(void)
{
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}
}

void gotoxy(unsigned int x, unsigned int y)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = BYTES_FOR_EACH_ELEMENT * (y * COLUMNS_IN_LINE + x);
}

void draw_strxy(const char *str,unsigned int x, unsigned int y)
{
	gotoxy(x,y);
	kprint(str);
}


void ball_draw()
{
    const char *ball ="O";	
    if(ball_xpos==0 || ball_xpos==79)
    {
	draw_strxy(" ",ball_xpos,ball_ypos);
        ball_xpos = 40;
        ball_ypos = 12;
    }
    else if(ball_ypos==LINES-3 || ball_ypos==2)
    {
            ball_ydirection*=-1;
    }
    else if(r2_ypos<= ball_ypos && ball_ypos<= r2_ypos+2 && ball_xpos==r2_xpos){
            ball_xdirection*=-1;
    }
    else if(r1_ypos<= ball_ypos && ball_ypos<= r1_ypos+2 && ball_xpos==r1_xpos){
            ball_xdirection*=-1;
        }
	draw_strxy(" ",ball_xpos,ball_ypos);
        ball_xpos+=ball_xdirection;
        ball_ypos+=ball_ydirection;
        draw_strxy(ball,ball_xpos,ball_ypos);
	sleep(5000000);
}

void draw_rkt(void)
{
	const char *rkt_s = "|";
	int i=0;
	for(i;i<3;i++)
	{
		int b=r1_ypos+i;
		draw_strxy(rkt_s,r1_xpos,b);
	}
	i=0;
	for(i;i<3;i++)
	{
		int b=r2_ypos+i;
		draw_strxy(rkt_s,r2_xpos,b);
	}
	
}

void clear_rkt(char keycode)
{
	const char *rkt_s = " ";
	if(keycode=='w' || keycode=='s'){

	int i=0;
	for(i;i<3;i++)
	{
		int b=r1_ypos+i;
		draw_strxy(rkt_s,r1_xpos,b);
	}
	}else
	{
	int i=0;
	for(i;i<3;i++)
	{
		int b=r2_ypos+i;
		draw_strxy(rkt_s,r2_xpos,b);
	}
	}
}

void move_rkt(char keycode)
{

 	switch(keycode)
    	{
    		case 'w':
			clear_rkt(keycode);
			r1_ypos-=r1_ypos-move==1?0:move;
			draw_rkt();
			break;
		case 's':
			clear_rkt(keycode);
			r1_ypos+=r1_ypos+move==LINES-4?0:move;
			draw_rkt();
			break;
		case 'k':
			clear_rkt(keycode);
			r2_ypos-=r2_ypos-move==1?0:move;
			draw_rkt();
			break;
		case 'm':
			clear_rkt(keycode);
			r2_ypos+=r2_ypos+move==LINES-4?0:move;
			draw_rkt();
			break;
    	}
}

void draw_borders()
{
	const char *upper_border = "=";
	int i=0;
	for(i;i<80;i++)
	{
		draw_strxy(upper_border,i,1);
	}
	int j=0;
	for(j;j<80;j++)
	{
		draw_strxy(upper_border,j,LINES-2);
	}

}
void keyboard_handler_main(void)
{
	unsigned char status;
	char keycode;

	/* write EOI */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;

		if(keycode == ENTER_KEY_CODE) {
			return;
		}
	}

	keycode = keyboard_map[(unsigned char) keycode];
	move_rkt(keycode);
}

void kmain(void)
{
	clear_screen();

	idt_init();
	kb_init();

	
	draw_borders();
	const char *ball = "O";
	draw_strxy(ball,ball_xpos,ball_ypos);
	while(1)
	{
		ball_draw();
		draw_rkt();
	}
}
