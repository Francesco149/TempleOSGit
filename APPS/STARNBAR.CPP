
#define p1_color	    0x0107
#define p2_color	    0x0104
#define jn1_color	    0x010A
#define jn2_color	    0x0109
#define enemy_fire_color    0x0109
#define friendly_fire_color 0x010F

class trooper
{
  int x,y,dx,dy,fx,fy,att,def,rng,player;
  DWORD fire_end_time;
};

#define num_troopers 100

trooper tr[num_troopers*2];
int count1,count2;
int jn;
int text_area_size=(screen_height/font_height)*(screen_width/font_width);


#define mm_none 	      1
#define mm_friendly_firepower 2
#define mm_laps_time	      3
int map_mode=mm_none;
WORD *map_values;


#define ai_nothing 1
#define ai_target  2
#define ai_random  3
int ai;

void update_map()
{
  int i,j;
  for (i=fs->window_top;i<=fs->window_bottom;i++) {
    memcpy(*SYS_TEXT_BASE+i*fs->crt_width+fs->window_left,
	   map_values+i*fs->crt_width+fs->window_left,
	   fs->window_right+2-fs->window_left);
  }
}

void clear_map_values()
{
  memset(map_values,0,text_area_size*sizeof(WORD));
}

void display_friendly_firepower()
{
  int i,d,x,y,dx,dy;
  int  lt=fs->window_left/2,rt=fs->window_right/2,
       tp=fs->window_top,   bm=fs->window_bottom;
  WORD *ptr;
  DWORD v;
  for (y=0;y<=bm-tp;y++) {
    ptr=map_values+(y+tp)*fs->crt_width+fs->window_left;
    for (x=0;x<=rt-lt;x++) {
      v=0;
      for (i=0;i<num_troopers;i++)
	if (tr[i].def>0 && tr[i].fire_end_time<*JIFFIES) {
	  dx=(tr[i].x>>16)-(x*font_width+font_width/2);
	  dy=(tr[i].y>>16)-(y*font_height+font_height/2);
	  d=dx*dx+dy*dy;
	  if (d<tr[i].rng)
	    v=v+tr[i].att;
	}
      ptr[x]=(v*128 & 0xF000)+0x20;
    }
  }
}

void draw_trooper(trooper *t,DWORD color)
{
  int x,y,s,fire_col;

  if (map_mode==mm_laps_time) {
    x=(t->x+500*t->dx)>>16;
    y=(t->y+500*t->dy)>>16;
  } else {
    x=t->x>>16;
    y=t->y>>16;
  }
  s=t->def;

  if (s>0) {
    s=s/3;
    if (x>s && y>s && x<=screen_width-s && y<=screen_height-s) {
      gr_line(x-s,y,x,y-s,color);
      gr_line(x,y-s,x+s,y,color);
      gr_line(x+s,y,x,y+s,color);
      gr_line(x,y+s,x-s,y,color);
    }
    if (t->player==0)
      fire_col=friendly_fire_color;
    else
      fire_col=enemy_fire_color;
    if (*JIFFIES<t->fire_end_time)
      gr_line(x,y,t->fx>>16,t->fy>>16,fire_col);
  }
}

int cursor_col;
void output_dec(int i)
{
  WORD *ptr=*SYS_TEXT_BASE+cursor_col*2;
  ptr[3]=0x0F00+i%10+'0'; i=i/10;
  ptr[2]=0x0F00+i%10+'0'; i=i/10;
  ptr[1]=0x0F00+i%10+'0'; i=i/10;
  ptr[0]=0x0F00+i%10+'0'; i=i/10;
  cursor_col=cursor_col+5;
}

void draw_all_troopers()
{
  int i,j,c,x=joycx-fs->window_pixel_left,y=joycy-fs->window_pixel_top;
  DWORD old_jiffies=0;
  if (*JIFFIES==old_jiffies)
    return;
  old_jiffies=*JIFFIES;
  gr_clear();
  if (!(*JIFFIES & 7)) {
    if (map_mode==mm_friendly_firepower)
      display_friendly_firepower;
    else {
      clear_crt();
      clear_map_values;
    }
  }
  update_map;

  count1=0;
  count2=0;
  for (i=0;i<num_troopers;i++)
    if (tr[i].def>0) {
      draw_trooper(&tr[i],p1_color);
      count1++;
    }
  for (i=num_troopers;i<num_troopers*2;i++)
    if (tr[i].def>0) {
      draw_trooper(&tr[i],p2_color);
      count2++;
    }

  if (joymax[2]-joymin[2]>joymax[3]-joymin[3])
    jn=150*(joyv[2]-joyrest[2])/(joymax[2]-joymin[2]+1);
  else
    jn=150*(joyv[3]-joyrest[3])/(joymax[3]-joymin[3]+1);
  if (jn>0)
    c=jn1_color;
  else
    c=jn2_color;
  j=abs(jn);
  if (x-j>0 && y-j>0 && x+j<screen_width && y+j<screen_height) {
    gr_line(x-jn,y-jn,x+jn,y-jn,c);
    gr_line(x+jn,y-jn,x+jn,y+jn,c);
    gr_line(x+jn,y+jn,x-jn,y+jn,c);
    gr_line(x-jn,y+jn,x-jn,y-jn,c);
  }
  cursor_col=(fs->window_top-1)*fs->crt_width+fs->window_left+10;
  output_dec(count1);
  output_dec(count2);
}

int targets[10];
void init_troopers()
{
  int i,j,dx,dy;
  trooper *t;
  for (i=0;i<10;i++) {
    targets[i]=rand*num_troopers/MAX_RAND;
    dx=(rand-MAX_RAND/2);
    dy=(rand-MAX_RAND/2);
    for (j=0;j<10;j++) {
      t=&tr[i*10+j];
      t->x=(screen_width-100-i*10)*65536;
      t->y=(screen_height/2-50+j*10)*65536;
      t->dx=0;
      t->dy=0;
      t->fire_end_time=*JIFFIES;
      t->att=3;
      t->def=10;
      t->rng=50*50;
      t->player=0;
      t=&tr[i*10+j+num_troopers];
      t->x=(100+i*10)*65536;
      t->y=(screen_height/2-50+j*10)*65536;
      t->dx=0;
      t->dy=0;
      if (ai==ai_random) {
	t->dx=dx;
	t->dy=dy;
      }
      t->fire_end_time=*JIFFIES;
      t->att=3;
      t->def=10;
      t->rng=50*50;
      t->player=1;
    }
  }
}


void do_ai_target()
{
  int i,j;
  trooper *t,*t2;
  for (i=0;i<10;i++) {
    t2=&tr[targets[i]];
    for (j=0;j<10;j++) {
      t=&tr[i*10+j+num_troopers];
      t->dx=(t2->x-t->x)/2048;
      t->dy=(t2->y-t->y)/2048;
    }
  }
}


void update_positions()
{
  int i,j;
  trooper *t;
  for (i=0;i<num_troopers*2;i++) {
    t=&tr[i];
    t->x=t->x+t->dx;
    if (t->x>=screen_width<<16) t->x=t->x-screen_width<<16;
    if (t->x<0) t->x=t->x+screen_width<<16;
    t->y=t->y+t->dy;
    if (t->y>=screen_height<<16) t->y=t->y-screen_height<<16;
    if (t->y<0) t->y=t->y+screen_height<<16;
  }
}

void resolve_firing()
{
  int i,j,d,dx,dy;
  for (i=0;i<num_troopers;i++)
    for (j=num_troopers;j<num_troopers*2;j++) {
      if (tr[i].def>0 && tr[j].def>0) {
	dx=(tr[i].x-tr[j].x)>>16;
	dy=(tr[i].y-tr[j].y)>>16;
	d=dx*dx+dy*dy;
	if (d<tr[i].rng && tr[i].fire_end_time<*JIFFIES) {
	  tr[i].fire_end_time=*JIFFIES+10;
	  tr[i].fx=tr[j].x;
	  tr[i].fy=tr[j].y;
	  tr[j].def=tr[j].def-tr[i].att;
	}
	if (d<tr[j].rng && tr[j].fire_end_time<*JIFFIES) {
	  tr[j].fire_end_time=*JIFFIES+10;
	  tr[j].fx=tr[i].x;
	  tr[j].fy=tr[i].y;
	  tr[i].def=tr[i].def-tr[j].att;
	}
      }
    }
}

void update_velocities()
{
  int i,j,x=joycx-fs->window_pixel_left,y=joycy-fs->window_pixel_top,dx,dy,d;
  trooper *t;
  if (joyb[0]) {
    for (i=0;i<num_troopers;i++) {
      t=&tr[i];
      dx=(x-(t->x>>16));
      dy=(y-(t->y>>16));
      d=dx*dx+dy*dy+1;
      dx=jn*1000*dx/d;
      dy=jn*1000*dy/d;
      t->dx=t->dx-dx;
      t->dy=t->dy-dy;
    }
  }
}


void stars_n_bars()
{
  bool cont1=true,cont2;
  char ch=0,*st;
  DWORD sc=0,jif=0,ms=0,old_ip=input_pointer,
	old_update_window=fs->update_window,
	old_text_attr=fs->text_attribute;
  linked_text_file *m;
  bool old_menu=assign_bit(&fs->crt_flags,CRTF_HAS_MENU,1);

  input_pointer=ip_joystick0;
  map_values=malloc(text_area_size*sizeof(DWORD));

  cursor_off;
  m=new_linked_text_file;
  m->line_start_col=5;
  m->cur_data_col=5;
  st=load_text_file("/HOPPY/APPS/STARNBAR.MEN");
  to_linked_text_file(st,m);
  free(st);

  clear_crt();
  clear_map_values;
  srand(*JIFFIES);
  fs->update_window=&draw_all_troopers;
  while (cont1) {
    ai=rand*3/MAX_RAND+1;
    init_troopers();
    cont2=true;
    while (cont2) {
      rand();
      sc=0;ms=0;ch=0;
      if (scanchar(&sc,&ch)) {
	if ((sc & 0x17F)==SC_L_GUI || (sc & 0x17F)==SC_R_GUI) {
	  gr_clear;
	  ms=do_menu(m);
        }
        if (ms==1 || ch=='1') 
	  map_mode=mm_none;
        else if (ms==2 || ch=='2') 
	  map_mode=mm_friendly_firepower;
        else if (ms==3 || ch=='3')
  	  map_mode=mm_laps_time;
      }
      jif=*JIFFIES;
      while (*JIFFIES==jif)
	swap_in_next;
      if (ai==ai_target)
	do_ai_target;
      update_velocities;
      update_positions;
      resolve_firing;
      if (ch==27 || ch==13 || ch==32 || ms==101 || ms==100)
        cont2=false;
    }
    if (ms==101 || ch==27)
      cont1=false;
  }
  gr_clear;
  cursor_on;

  delete_ltf(m);
  fs->update_window=old_update_window;
  free(map_values);
  input_pointer=old_ip;
  assign_bit(&fs->crt_flags,CRTF_HAS_MENU,old_menu);
}

stars_n_bars;;
