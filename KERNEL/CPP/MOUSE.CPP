#define KEYB_PORT (0x60)
#define KEYB_CTRL (0x64)

int mxx = screen_width/2, 
    myy = screen_height/2, mzz = 0;
int mouse_buttons[5];
bool mouse_has_wheel=false;
bool mouse_has_ext_buttons=false;
bool mouse_evt=false;
BYTE mouse_buf[4];

void kbd_command_send(DWORD port, BYTE val)
{
  DWORD timeout;
  for (timeout=500000; 
       (inp(KEYB_CTRL)&2) && timeout;timeout--);
  if (timeout) 
    outp(port, val);
  else
    ?"CMD TIMEOUT\r\n";
}

int kbd_command_read()
{
  DWORD timeout;
  for (timeout=20000; timeout; timeout--) {
    if (inp(KEYB_CTRL) & 0x01) {
      port_nop;
      return inp(KEYB_PORT);
    }
  }
  return -1;
}

bool kbd_mouse_cmd_ack(BYTE val)
{
  BYTE b;
  kbd_command_send(KEYB_CTRL,0xD4);
  kbd_command_send(KEYB_PORT,val);
  b=kbd_command_read;
  if (b==0xFA)
    return true;
  ? "MOUSE CMD FAILED\r\n";
  return false;
}

void mouse_irq_handler()
{
  int i,dx,dy,dz;

  kbd_command_send(KEYB_CTRL, 0xAD); // disable keyb
  for (i=0;i<3;i++)
    mouse_buf[i]=kbd_command_read;
/*
  if (mouse_has_wheel || mouse_has_ext_buttons)
    mouse_buf[3]=kbd_command_read;
*/


  mouse_buttons[0] = mouse_buf[0] & 1;
  mouse_buttons[1] = (mouse_buf[0] & 2) >> 1;
  mouse_buttons[2] = (mouse_buf[0] & 4) >> 2;
  mouse_buttons[3] = (mouse_buf[3] & 0x10) >> 4;
  mouse_buttons[4] = (mouse_buf[3] & 0x20) >> 5;
  if (mouse_buf[0] & 0x10) dx=mouse_buf[1]-256; 
                else dx=mouse_buf[1];
  if (mouse_buf[0] & 0x20) dy=256-mouse_buf[2]; 
                else dy=0-mouse_buf[2];
  if (mouse_buf[3] & 0x08) dz=(mouse_buf[3]&7)-8; 
                else dz=mouse_buf[3]&7;

  if (dx<8 && dx>-8 && dy<8 && dy>-8) {
    dx=sign(dx);
    dy=sign(dy);
  }

  mxx = mxx+dx;
  myy = myy+dy;
  mzz = mzz+dz;
  if (mxx < 0) mxx=0;
  if (mxx>=screen_width) mxx=screen_width-1;
  if (myy < 0) myy=0;
  if (myy>=screen_height) myy=screen_height-1;

  kbd_command_send(KEYB_CTRL, 0xAE); // enable keyb
  outp(0xa0, 0x20); // eoi slave
  outp(0x20, 0x20); // eoi master
  mouse_evt=true;
}



void install_mouse_driver()
{
  BYTE b;
  int i;
  
  for(i=0;i<4;i++)
    mouse_buf[i]=0;
  for(i=0;i<5;i++)
    mouse_buttons[i]=0;

  cli;
  kbd_command_send(KEYB_CTRL, 0xA8); // enable aux
  for (i=0;i<10000;i++);

  kbd_command_send(KEYB_CTRL,0xA9); //check for mouse
  b=kbd_command_read;
  ? "Status:",b,"\r\n";

  kbd_mouse_cmd_ack(0xF3);
  kbd_mouse_cmd_ack(0xC8);
  kbd_mouse_cmd_ack(0xF3); // enable Mouse
  kbd_mouse_cmd_ack(0x64); // Set Rate
  kbd_mouse_cmd_ack(0xF3);
  kbd_mouse_cmd_ack(0x50); //Resolution

  kbd_mouse_cmd_ack(0xF2);
  b=kbd_command_read;
  if (b==3) {
    mouse_has_wheel=true;
    ? "Wheel Mouse\r\n";
  } else
    mouse_has_wheel=false;

  kbd_mouse_cmd_ack(0xF3);
  kbd_mouse_cmd_ack(0xC8);
  kbd_mouse_cmd_ack(0xF3); // enable Mouse
  kbd_mouse_cmd_ack(0xC8); // Set Rate
  kbd_mouse_cmd_ack(0xF3);
  kbd_mouse_cmd_ack(0x50); //Resolution

  kbd_mouse_cmd_ack(0xF2);
  b=kbd_command_read;
  if (b==4) {
    mouse_has_ext_buttons=true;
    ? "Ext Mouse Buttons\r\n";
  } else
    mouse_has_ext_buttons=false;

  kbd_mouse_cmd_ack(0xF6);
  kbd_mouse_cmd_ack(0xE6);
  kbd_mouse_cmd_ack(0xF4); // enable Mouse
  kbd_mouse_cmd_ack(0xF3); // Set Rate
  kbd_mouse_cmd_ack(0x64);
  kbd_mouse_cmd_ack(0xE8); //Resolution
  kbd_mouse_cmd_ack(0x03);
  ? "Success\r\n";
  sti;
}

defext(ext_irq0C,&mouse_irq_handler);
install_mouse_driver;
