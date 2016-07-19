


		/* <1> <1> (image) */

		/* <2> <2> (image) */

F64 tNoise;

#define NUM_BOMBS	256
I64 bomb_in,bomb_out;
class Obj
{
  F64 x,y,dx,dy,theta;
} plane,bombs[NUM_BOMBS];

U0 DrawIt(CTask *task, CDC *dc)
{
  I64 i=bomb_out,j,h=task->pix_height;
  while (i!=bomb_in) {
    SpriteZ3B(dc,bombs[i].x,bombs[i].y,0,__BIN_2,bombs[i].theta);
    if (bombs[i].y>h-30)
      for (j=1;j<20;j++) {
	if (j&1)
	  dc->color=RED;
	else
	  dc->color=YELLOW;
	GrCircle(dc,bombs[i].x,bombs[i].y,j);
      }
    i=(i+1)&(NUM_BOMBS-1);
  }
  SpriteZ3B(dc,plane.x,plane.y,0,__BIN_1,plane.theta);
  if (tS<tNoise)
    Snd(RandU16&64+10);
  else
    Snd(0);
}

U0 AnimateTask(CTask *task)
{
  I64 min_x,max_x,min_y,max_y,i,w,h;
  SpriteExtents(__BIN_1,&min_x,&max_x,&min_y,&max_y);
  while (TRUE) {
    w=task->pix_width; h=task->pix_height;
    plane.dx=0.2*Cos(plane.theta);
    plane.dy=0.2*Sin(plane.theta);
    plane.x=(plane.x+plane.dx)%w;
    if (plane.theta>0)
      plane.y=Clamp(plane.y+plane.dy,
	    -min_y*Cos(plane.theta)-min_x*Sin(plane.theta),
	    h-(max_y*Cos(plane.theta)+max_x*Sin(plane.theta)));
    else
      plane.y=Clamp(plane.y+plane.dy,
	    -min_y*Cos(plane.theta)-max_x*Sin(plane.theta),
	    h-(max_y*Cos(plane.theta)+min_x*Sin(plane.theta)));

    i=bomb_out;
    while (i!=bomb_in) {
      bombs[i].dy+=0.0001;
      bombs[i].dx-=0.0002*bombs[i].dx;
      bombs[i].x=(bombs[i].x+bombs[i].dx)%w;
      bombs[i].y+=bombs[i].dy;
      bombs[i].theta=0.3*Sin(0.1*bombs[i].x);
      if (bombs[i].y>=h) {
	bomb_out=(bomb_out+1)&(NUM_BOMBS-1);
	tNoise=tS+0.3;
      }
      i=(i+1)&(NUM_BOMBS-1);
    }
    Sleep(1);
  }
}

U0 DropBomb(CTask *)
{
  I64 i=bomb_in;
  bomb_in=(bomb_in+1)&(NUM_BOMBS-1);
  bombs[i].x=plane.x;
  bombs[i].y=plane.y;
  bombs[i].dx=plane.dx;
  bombs[i].dy=plane.dy;
  bombs[i].theta=0.3;
}

U0 Init()
{
  tNoise=0;
  bomb_in=bomb_out=0;
  plane.theta=0;
  plane.x=0;
  plane.y=100;
}

U0 B17()
{
  I64 sc;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  WinMax;
  WinBorder;
  DocClear;
  DocCursor;
  AutoComplete;
  Init;
  Fs->animate_task=Spawn(&AnimateTask,Fs,"Animate Task");
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      switch (GetKey(&sc)) {
	case 0:
	  switch (sc.u8[0]) {
	    case SC_CURSOR_RIGHT:
	      plane.theta=Clamp(plane.theta-0.1,-pi/4,pi/4);
	      break;
	    case SC_CURSOR_LEFT:
	      plane.theta=Clamp(plane.theta+0.1,-pi/4,pi/4);
	      break;
	    case SC_CURSOR_UP:
	      break;
	    case SC_CURSOR_DOWN:
	      break;
	  }
	  break;
	case CH_SPACE:
	  DropBomb(Fs);
	  break;
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto b_done;
      }
    }
b_done:
  } catch
    PutExcept;
  SettingsPop;
}


B17;

