#define ip_joystick0 0
#define ip_joystick1 1  //Not implemented
#define ip_mouse     2  //Not implemented

int ipx=screen_width/2,ipy=screen_height/2;
int input_pointer=ip_joystick0;
int iptx=ipx/font_width,ipty=ipy/font_height;
bool ip_on=true,  //Show or hide cursor
     ip_lb=false,ip_cb=false,ip_rb=false; //Left/center/right buttons
