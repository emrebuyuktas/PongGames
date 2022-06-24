/*
Copyright (C) 2015-2019 The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#include "console.h"
#include "page.h"
#include "process.h"
#include "keyboard.h"
#include "mouse.h"
#include "interrupt.h"
#include "clock.h"
#include "ata.h"
#include "device.h"
#include "cdromfs.h"
#include "string.h"
#include "graphics.h"
#include "kernel/ascii.h"
#include "kernel/syscall.h"
#include "rtc.h"
#include "kernelcore.h"
#include "kmalloc.h"
#include "memorylayout.h"
#include "kshell.h"
#include "cdromfs.h"
#include "diskfs.h"
#include "serial.h"

/*
This is the C initialization point of the kernel.
By the time we reach this point, we are in protected mode,
with interrupts disabled, a valid C stack, but no malloc heap.
Now we initialize each subsystem in the proper order:
*/
static int border_one_x=0;
static int border_one_y=200;
static int border_one_w=1020;
static int border_one_h=0;

static int border_two_x=0;
static int border_two_y=700;
static int border_two_w=1020;
static int border_two_h=0;


static struct graphics_color color_white = { 255, 255, 255, 0 };
static struct graphics_color color_black = { 0, 0, 0, 0 };


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

int kernel_main()
{

	
	
	int board_one_y_one=((border_two_y+border_one_y)/2)-45;
	int board_one_y_two=90;
	int board_one_x=40;
	
	int board_two_y_one=((border_two_y+border_one_y)/2)-45;
	int board_two_y_two=90;
	int board_two_x=980;
	
	
	int ball_x=(border_one_w/2)-10;
    	int ball_y=((border_two_y+border_one_y)/2)-10;
    	int ball_w=20;
    	int ball_h=20;
    	int ball_x_direction=1;
    	int ball_y_direction=-1;
	
	
	int scoreone=0;
	int scoretwo=0;
	
	
	struct console *console = console_create_root();
	struct graphics *g=graphics_create_root();
    	graphics_create(g);
    	
    	graphics_line(g, border_one_x, border_one_y, border_one_w, border_one_h);
    	graphics_line(g, border_two_x, border_two_y, border_two_w, border_two_h);
    	
    	
    	graphics_line(g, board_one_x, board_one_y_one, 0, board_one_y_two);
    	graphics_line(g, board_two_x, board_two_y_one, 0, board_two_y_two);
    	
    	graphics_rect(g,ball_x,ball_y,ball_w, ball_h);
    	
    	//graphics_line(g, border_two_x, border_two_y, border_two_w, border_two_h);
    	
    	keyboard_init();
	console_addref(g);
	page_init();
	kmalloc_init((char *) KMALLOC_START, KMALLOC_LENGTH);
	interrupt_init();
	mouse_init();
	keyboard_init();
	rtc_init();
	clock_init();
	process_init();
	ata_init();
	cdrom_init();
	diskfs_init();

	
	while(1) {
		graphics_char(g, 500, 720,scoreone+'0');
		graphics_char(g, 515, 720,'-');
		graphics_char(g, 525, 720,scoretwo+'0');
		int key=console_getchar(console);
		if(key=='w')
		{
		graphics_fgcolor(g, color_black);
		graphics_line(g, board_one_x, board_one_y_one, 0, board_one_y_two);
		graphics_fgcolor(g, color_white);
		board_one_y_one-= board_one_y_one==border_one_y?0:5;
		graphics_line(g, board_one_x, board_one_y_one, 0, board_one_y_two);
		}
		else if(key=='s')
		{
		graphics_fgcolor(g, color_black);
		graphics_line(g, board_one_x, board_one_y_one, 0, board_one_y_two);
		graphics_fgcolor(g, color_white);
		board_one_y_one+= board_one_y_one+board_one_y_two==border_two_y?0:5;
		graphics_line(g, board_one_x, board_one_y_one, 0, board_one_y_two);
		}
		else if(key=='k')
		{
		graphics_fgcolor(g, color_black);
		graphics_line(g, board_two_x, board_two_y_one, 0, board_two_y_two);
		graphics_fgcolor(g, color_white);
		board_two_y_one-= board_two_y_one==border_one_y?0:5;
		graphics_line(g, board_two_x, board_two_y_one, 0, board_two_y_two);
		}
		else if(key=='m')
		{
		graphics_fgcolor(g, color_black);
		graphics_line(g, board_two_x, board_two_y_one, 0, board_two_y_two);
		graphics_fgcolor(g, color_white);
		board_two_y_one+= board_two_y_one+board_two_y_two==border_two_y?0:10;
		graphics_line(g, board_two_x, board_two_y_one, 0, board_two_y_two);
		}
		

		if(board_one_y_one<=(ball_y+ball_h) && (board_one_y_one+board_one_y_two)>=ball_y && ball_x==board_one_x+1)
		{
		ball_x_direction*=-1;
		}
		else if(board_two_y_one<=ball_y+ball_h && board_two_y_one+board_one_y_two>=ball_y && ball_x+ball_w==board_two_x-1)
		{
		ball_x_direction*=-1;
		
		}
		else if(ball_y==border_one_y+2 || ball_y+ball_w==border_two_y)
		{
		ball_y_direction*=-1;
		}
		else if(ball_x<0 || ball_x+ball_w>1020)
		{
		
		scoreone+=ball_x>=1000?1:0;
		scoretwo+=ball_x<=0?1:0;
		
		graphics_fgcolor(g, color_black);
		graphics_rect(g,ball_x,ball_y,ball_w, ball_h);
		ball_x=(border_one_w/2)-10;
    		ball_y=((border_two_y+border_one_y)/2)-10;
    		ball_w=20;
    		ball_h=20;
    		ball_x_direction*=-1;
    		ball_y_direction*=1;
    		graphics_fgcolor(g, color_white);
		graphics_rect(g,ball_x,ball_y,ball_w, ball_h);
		}
		graphics_fgcolor(g, color_black);
		graphics_rect(g,ball_x,ball_y,ball_w, ball_h);
		ball_x+=ball_x_direction;
		ball_y+=ball_y_direction;
		graphics_fgcolor(g, color_white);
		graphics_rect(g,ball_x,ball_y,ball_w, ball_h);
		sleep(500000);
	}

	return 0;
}
