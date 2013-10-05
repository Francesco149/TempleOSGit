#define BORDER		32
#define NUM_UNITS	64
#define MOVE_DISTANCE	100
#define BREAK_DISTANCE	6

class Unit
{
  I64 num,x,y,player,link,bandwidth;
  U8 *img;
  Bool alive,king;
} u[NUM_UNITS];

I64 cur_player,start_x,start_y,end_x,end_y;
Bool show_start;




/* <1> <1> (image) */  /* <2> <2> (image) */


/* <3> <3> (image) */  /* <4> <4> (image) */



U8 *pawn_imgs[2]={__BIN_1,__BIN_2};
U8 *king_imgs[2]={__BIN_3,__BIN_4};
I64 num_alive[2];

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i;
  for (i=0;i<NUM_UNITS;i++)
    if (u[i].alive && !u[i].king) {
      dc->color=YELLOW;
      dc->pen_width=u[i].bandwidth;
      GrLine3(dc,u[i].x,u[i].y,0,u[u[i].link].x,u[u[i].link].y,0);
      if (Blink) {
	dc->color=BLUE;
	dc->pen_width=2;
	GrPlot3(dc,(u[i].x+u[u[i].link].x)>>1,(u[i].y+u[u[i].link].y)>>1,0);
      }
    }
  for (i=0;i<NUM_UNITS;i++)
    if (u[i].alive)
      SpritePlot3(dc,u[i].x,u[i].y,0,u[i].img);
  if (show_start) {
    dc->color=LTRED;
    GrLine(dc,start_x-4,start_y-4,start_x+4,start_y+4);
    GrLine(dc,start_x-4,start_y+4,start_x+4,start_y-4);
    GrLine(dc,start_x,start_y,end_x,end_y);
  }
  if (cur_player==0)
    dc->color=LTCYAN;
  else
    dc->color=LTPURPLE;
  GrPrint(dc,0,0,"Player %d",cur_player+1);
  if ((num_alive[0]==1 || num_alive[1]==1) && Blink(4)) {
    dc->color=BLACK;
    GrPrint(dc,(task->win_pixel_width-FONT_WIDTH*9)>>1,
		  (task->win_pixel_height-FONT_HEIGHT)>>1,"Game Over");
  }
}

Unit *FindUnit(I64 x,I64 y,I64 player)
{
  I64 i,dd,best_dd=MAX_I64;
  Unit *result=NULL;
  for (i=0;i<NUM_UNITS;i++) {
    if (u[i].player==player && u[i].alive) {
      dd=SqrI64(u[i].x-x)+SqrI64(u[i].y-y);
      if (dd<best_dd) {
	best_dd=dd;
	result=&u[i];
      }
    }
  }
  return result;
}

U0 CheckForKills(I64 x1,I64 y1,I64 player)
{
  I64 i,j,x2,y2,dd;
  Bool found;
  for (i=0;i<NUM_UNITS;i++) {
    if (u[i].player!=player && u[i].alive && !u[i].king) {
      x2=(u[i].x+u[u[i].link].x)>>1;
      y2=(u[i].y+u[u[i].link].y)>>1;
      dd=SqrI64(x2-x1)+SqrI64(y2-y1);
      if (dd<=BREAK_DISTANCE*BREAK_DISTANCE) {
	u[i].alive=FALSE;
	Snd(500);Sleep(100);Snd(0);Sleep(25);
	num_alive[u[i].player]--;
	do {
	  found=FALSE;
	  for (j=0;j<NUM_UNITS;j++)
	    if (u[j].alive && u[j].player!=player && !u[j].king &&
	      !u[u[j].link].alive) {
	      found=TRUE;
	      u[j].alive=FALSE;
	      Snd(500);Sleep(100);Snd(0);Sleep(25);
	      num_alive[u[j].player]--;
	    }
	} while (found);
      }
    }
  }
}

U0 MoveUnit(Unit *tempu,I64 x,I64 y)
{
  F64 d,dx,dy;
  dx=x-tempu->x; dy=y-tempu->y;
  if (!(d=Sqrt(dx*dx+dy*dy))) return;
  dx/=d; dy/=d;
  if (d>MOVE_DISTANCE) d=MOVE_DISTANCE;
  tempu->x+=d*dx;
  tempu->y+=d*dy;
  end_x=tempu->x;
  end_y=tempu->y;
}

U0 Init()
{
  I64 i,j,dd,best_dd,best;
  best=0;
  best_dd=MAX_I64;
  for (i=0;i<NUM_UNITS/2;i++) {
    u[i].num=i;
    u[i].x=RandU32%((GR_WIDTH-4*BORDER)/2)+BORDER;
    u[i].y=RandU32%(GR_HEIGHT-2*BORDER)+BORDER;
    u[i].player=0;
    u[i].alive=TRUE;
    if (u[i].x<=best_dd) {
      best_dd=u[i].x;
      best=i;
    }
    u[i+NUM_UNITS/2].num=i+NUM_UNITS/2;
    u[i+NUM_UNITS/2].x=GR_WIDTH-u[i].x;
    u[i+NUM_UNITS/2].y=GR_HEIGHT-FONT_HEIGHT-u[i].y;
    u[i+NUM_UNITS/2].player=1;
    u[i+NUM_UNITS/2].alive=TRUE;
  }
  for (i=0;i<NUM_UNITS/2;i++)
    if (i==best) {
      u[i].img=king_imgs[0];
      u[i].king=TRUE;
      u[i+NUM_UNITS/2].img=king_imgs[1];
      u[i+NUM_UNITS/2].king=TRUE;
    } else {
      u[i].img=pawn_imgs[0];
      u[i].king=FALSE;
      u[i+NUM_UNITS/2].img=pawn_imgs[1];
      u[i+NUM_UNITS/2].king=FALSE;
    }

  for (i=0;i<NUM_UNITS/2;i++) {
    if (!u[i].king) {
      best_dd=MAX_I64;
      for (j=0;j<NUM_UNITS/2;j++) {
	if (i!=j) {
	  dd=SqrI64(u[i].x-u[j].x)+SqrI64(u[i].y-u[j].y);
	  if (u[j].x<u[i].x && dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      u[i].link=best;
      u[i].bandwidth=5-u[i].x/((GR_WIDTH-4*BORDER)/8);
      u[i+NUM_UNITS/2].link=u[i].link+NUM_UNITS/2;
      u[i+NUM_UNITS/2].bandwidth=u[i].bandwidth;
    }
  }
  num_alive[0]=NUM_UNITS/2;
  num_alive[1]=NUM_UNITS/2;
}



U0 ChessNet()
{
  I64 msg_code,p1,p2,ch,sc;
  Unit *tempu,saved_u;

  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=GREEN<<4+WHITE;
  Preempt(OFF);
  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,'\n');"
  "}"
  );
  WinMax;
  WinBorder;
  WordStat;

  Init;
  PopUpOk("Step on the link mid points.\n"
	  "Make a computer player or\n"
	  "modify the rules.\n");
  Fs->win_inhibit=WIF_ALL-WIF_BORDER-WIF_MENU;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  try {
    do {
      ch=0;
      Init;
      tempu=NULL;
      cur_player=0;
      show_start=FALSE;
      do {
	msg_code=GetMsg(&p1,&p2,1<<MSG_KEY_DOWN+1<<MSG_IP_L_DOWN+1<<MSG_IP_L_UP+1<<MSG_IP_MOVE);
	switch (msg_code) {
	  case MSG_KEY_DOWN:
	    ch=p1;
	    sc=p2;
	    break;
	  case MSG_IP_L_DOWN:
	    if (num_alive[0]>1 && num_alive[1]>1) {
	      tempu=FindUnit(p1,p2,cur_player);
	      MemCpy(&saved_u,tempu,sizeof(Unit));
	      end_x=start_x=tempu->x;
	      end_y=start_y=tempu->y;
	      show_start=TRUE;
	    }
	    break;
	  case MSG_IP_MOVE:
	    if (tempu) {
	      MemCpy(tempu,&saved_u,sizeof(Unit));
	      MoveUnit(tempu,p1,p2);
	    }
	    break;
	  case MSG_IP_L_UP:
	    MemCpy(tempu,&saved_u,sizeof(Unit));
	    MoveUnit(tempu,p1,p2);
	    CheckForKills(tempu->x,tempu->y,cur_player);
	    show_start=FALSE;
	    tempu=NULL;
	    cur_player=1-cur_player;
	    break;
	}
      } while (ch!=CH_ESC && ch!='\n' && ch!=CH_SHIFT_ESC);
    } while (ch!=CH_ESC && ch!=CH_SHIFT_ESC);
    GetMsg(,,1<<MSG_KEY_UP);
  } catch
    CatchAll;
  SettingsPop;
  MenuPop;
}

ChessNet;
