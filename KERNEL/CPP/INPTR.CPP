#define ip_joystick0 0
#define ip_joystick1 1
#define ip_mouse     2

int ipx=-100,ipy=-100;
int input_pointer=ip_mouse;
int iptx=ipx/font_width,ipty=ipy/font_height;
bool ip_on=true,  //Show or hide cursor
     ip_lb_last=false,ip_rb_last=false,
     ip_lb=false,ip_cb=false,ip_rb=false; //Left/center/right buttons
