RegSetDftEntry("TempleOS/TimeOut","I64 best_score=0;\n");
RegExeBranch("TempleOS/TimeOut");

#define MAP_HEIGHT	4096
#define BULLET_LEN	10
#define BULLET_SPEED	5

#define MAX_BULLETS 128
class Bullet
{
  I64 x,y,dx,dy,dx2,dy2;
  Bool dead,pad[7];
} b[MAX_BULLETS];
I64 bullets_fired;

I64 x,y,dx,dy,scroll_y,finish_line;
F64 theta;
F64 t0,tf,snd_bullet_timeout,snd_death_timeout;

#define PHASES_GROUPS		10
#define MAX_ENEMY	(PHASES_GROUPS*100)
#define MAX_FRIENDLY	(PHASES_GROUPS*20)
#define MAX_MAN		(MAX_FRIENDLY+MAX_ENEMY)
#define HACK_DIST	5
class Man
{
  I64 x,y,best_dd;
  F64 theta,hack_phase,corpse_timeout;
  Bool friendly,dead,pad[6];
} m[MAX_MAN];

I64 total_score,friendly_fire,enemy_by_friendly,friendly_left,enemy_left,
    main_loop_pass;
Bool game_over;



	/* <1> <1> (image) */





	/* <2> <2> (image) */

	/* <3> <3> (image) */

	/* <4> <4> (image) */

	/* <5> <5> (image) */

CSprite *friendly_imgs[4]={__BIN_3,__BIN_2,__BIN_3,__BIN_4};

	/* <6> <6> (image) */

	/* <7> <7> (image) */

	/* <8> <8> (image) */

	/* <9> <9> (image) */

CSprite *enemy_imgs[4]={__BIN_7,__BIN_6,__BIN_7,__BIN_8};

#define LANDSCAPE_TYPES	4






	/* <10> <10> (image) */



	/* <11> <11> (image) */







	/* <12> <12> (image) */















	/* <13> <13> (image) */



CSprite *landscape_imgs[LANDSCAPE_TYPES]=
  {__BIN_10,__BIN_11,__BIN_12,__BIN_13};

#define MAX_LANDSCAPE 64
class LandScapeItem
{
  I64 x,y;
  U8 *img;
} ls[MAX_LANDSCAPE];

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,yy;
  F64 t1;
  Man *tempm;
  Bullet *tempb;
  CSprite *temps,**_temps;

  for (i=0;i<MAX_LANDSCAPE;i++) {
    yy=(ls[i].y-scroll_y)&(MAP_HEIGHT-1);
    if (-32<=yy<=Fs->pix_bottom+32)
      Sprite3(dc,ls[i].x,yy,0,ls[i].img);
  }

  dc->pen_width=3;
  dc->color=BROWN;
  GrLine3(dc,0,  (finish_line-scroll_y)&(MAP_HEIGHT-1),0,
	  GR_WIDTH,(finish_line-scroll_y)&(MAP_HEIGHT-1),0);

  dc->color=LTRED;
  for (i=0,tempb=b;i<MAX_BULLETS;i++,tempb++)
    if (!tempb->dead)
      GrLine(dc,tempb->x>>32,tempb->y>>32,
	(tempb->x+tempb->dx*BULLET_LEN)>>32,
	(tempb->y+tempb->dy*BULLET_LEN)>>32);
  for (i=0,tempm=m;i<MAX_MAN;i++,tempm++) {
    yy=(tempm->y-scroll_y)&(MAP_HEIGHT-1);
    if (-32<=yy<=Fs->pix_bottom+32) {
      if (tempm->dead &&
	  tS<tempm->corpse_timeout)
	if (!(RandU16&15)) {
	  if (tempm->friendly)
	    temps=__BIN_5;
	  else
	    temps=__BIN_9;
	  SpriteZ3B(dc,tempm->x,yy,0,temps,tempm->theta);
	}
   }
  }
  for (i=0,tempm=m;i<MAX_MAN;i++,tempm++) {
    yy=(tempm->y-scroll_y)&(MAP_HEIGHT-1);
    if (-32<=yy<=Fs->pix_bottom+32) {
      if (!tempm->dead) {
	if (tempm->friendly)
	  _temps=friendly_imgs;
	else
	  _temps=enemy_imgs;
	if (tempm->best_dd<HACK_DIST*HACK_DIST)
	  temps=_temps[4*Wrap(tempm->hack_phase+20*tS,0)/(2*pi)];
	else
	  temps=_temps[0];
	SpriteZ3B(dc,tempm->x,yy,0,temps,tempm->theta);
      }
    }
  }
  SpriteZ3B(dc,x,y,0,__BIN_1,theta);

  if (tf) {
    t1=tf;
    dc->color=RED;
    if (game_over && Blink)
      GrPrint(dc,task->pix_width/2-9*FONT_WIDTH/2,task->pix_height/2,"Game Over");
  } else {
    t1=tS;
    if (!enemy_left || !friendly_left)
     game_over=TRUE;
  }
  dc->color=BROWN;
  GrPrint(dc,0,0,"Enemy:%d Friends:%d Friendly Fire:%d Time:%6.2f Bullets:%d",
    enemy_left,friendly_left,friendly_fire,t1-t0,bullets_fired);
  GrPrint(dc,0,8,"Total Score:%,d High Score:%,d",
      total_score,best_score);
}

U0 CheckCollisions()
{
  I64 i,x,y;
  Man *tempm;
  Bullet *tempb;
  CDC	*dc2=DCNew(GR_WIDTH,GR_HEIGHT);
  dc2->color=LTRED;
  for (i=0,tempb=b;i<MAX_BULLETS;i++,tempb++)
    if (!tempb->dead)
      GrLine(dc2,tempb->x>>32,tempb->y>>32,
	(tempb->x+tempb->dx*BULLET_LEN)>>32,
	(tempb->y+tempb->dy*BULLET_LEN)>>32);

  dc2->color  =ROP_COLLISION;
  dc2->bkcolor=BLACK;
  for (i=0,tempm=m;i<MAX_MAN;i++,tempm++) {
    if (!tempm->dead) {
      x=tempm->x;
      y=(tempm->y-scroll_y)&(MAP_HEIGHT-1);
      if (0<=x<GR_WIDTH && 0<=y<GR_HEIGHT) {
	dc2->collision_cnt=0;
	if (tempm->friendly)
	  Sprite3(dc2,x,y,0,__BIN_2);
	else
	  Sprite3(dc2,x,y,0,__BIN_6);
	if (dc2->collision_cnt) {
	  tempm->dead=TRUE;
	  tempm->corpse_timeout=tS+30.0;
	  if (tempm->friendly) {
	    friendly_fire++;
	    friendly_left--;
	    Snd(200);
	  } else {
	    enemy_left--;
	    Snd(50);
	  }
	  snd_death_timeout=tS+0.01;
	}
      }
    }
  }
  DCDel(dc2);
}

U0 Init()
{
  I64 i,xx,yy;
  Man *tempm;

  snd_bullet_timeout=snd_death_timeout=0;
  Snd(0);

  total_score=0;
  game_over=FALSE;
  main_loop_pass=0;

  scroll_y=MAP_HEIGHT-100;
  x=Fs->pix_width>>1;
  y=0.9*Fs->pix_height;
  finish_line=scroll_y+y;
  dx=0;
  dy=0;
  theta=0;
 
  for (i=0;i<MAX_LANDSCAPE;i++) {
    ls[i].x=(Fs->pix_width-100)*RandU32/MAX_U32+50;
    ls[i].y=(MAP_HEIGHT-100)*RandU32/MAX_U32+50;
    ls[i].img=landscape_imgs[RandU16%(LANDSCAPE_TYPES-2)];
  }
  ls[0].img=landscape_imgs[LANDSCAPE_TYPES-2]; //mountain
  ls[1].img=landscape_imgs[LANDSCAPE_TYPES-2]; //mountain
  ls[2].img=landscape_imgs[LANDSCAPE_TYPES-1]; //castle

  for (i=0,tempm=m;i<MAX_MAN;i++,tempm++) {
    if (i<MAX_FRIENDLY) {
      tempm->friendly=TRUE;
      if (!(i&7)) {
	xx=(Fs->pix_width-200)*RandU32/MAX_U32;
	yy=(MAP_HEIGHT-200)*RandU32/MAX_U32;
      }
    } else {
      tempm->friendly=FALSE;
      if (!(i&31)) {
	xx=(Fs->pix_width-200)*RandU32/MAX_U32;
	yy=(MAP_HEIGHT-200)*RandU32/MAX_U32;
      }
    }
    tempm->dead=FALSE;
    tempm->x=xx+64*RandI32/MAX_I32+100;
    tempm->y=yy+64*RandU32/MAX_I32-64+100;
    tempm->corpse_timeout=0;
    tempm->hack_phase=2*pi*Rand;
    tempm->best_dd=MAX_I64;
  }
  for (i=0;i<MAX_BULLETS;i++)
    b[i].dead=TRUE;
  friendly_left=MAX_FRIENDLY;
  enemy_left   =MAX_ENEMY;
  enemy_by_friendly=0;
  bullets_fired=0;
  friendly_fire=0;
  t0=tS;
  tf=0;
}

U0 FireBullet()
{
  I64 i,j;
  F64 a;
  Bullet *tempb;
  for (i=0;i<MAX_BULLETS-1;i++)
    if (b[i].dead)
      break;
  tempb=&b[i];

  j=x+24.0*Sin(theta);
  tempb->x=j<<32;

  j=y-24.0*Cos(theta);
  tempb->y=j<<32;

  tempb->dx2=(BULLET_SPEED*Sin(theta)+dx)*0x100000000;
  tempb->dy2=-BULLET_SPEED*Cos(theta)*0x100000000;
  a=Arg(tempb->dx2,tempb->dy2);
  tempb->dx=Cos(a)*0x100000000;
  tempb->dy=Sin(a)*0x100000000;
  bullets_fired++;
  tempb->dead=FALSE;
  if (!snd_death_timeout) {
    Snd(1000);
    snd_bullet_timeout=tS+0.0005;
  }
}

U0 MenMove(I64 phase_group)
{
  I64 i,j,dd,best,best_dd;
  for (i=phase_group;i<MAX_FRIENDLY;i+=PHASES_GROUPS) {
    if (!m[i].dead) {
      best=MAX_FRIENDLY;
      best_dd=MAX_I64;
      for (j=MAX_FRIENDLY;j<MAX_MAN;j++) {
	if (!m[j].dead) {
	  dd=SqrI64(m[i].x-m[j].x)+SqrI64(m[i].y-m[j].y);
	  if (dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      m[i].best_dd=best_dd;
      if (best_dd!=MAX_I64) {
	m[i].x+=4*SignI64(m[best].x-m[i].x);
	m[i].y+=4*SignI64(m[best].y-m[i].y);
	m[i].theta=Arg(m[best].x-m[i].x,m[best].y-m[i].y);
      }
    }
  }
  for (i=MAX_FRIENDLY+phase_group;i<MAX_MAN;i+=PHASES_GROUPS) {
    if (!m[i].dead) {
      best=0;
      best_dd=MAX_I64;
      for (j=0;j<MAX_FRIENDLY;j++) {
	if (!m[j].dead) {
	  dd=SqrI64(m[i].x-m[j].x)+SqrI64(m[i].y-m[j].y);
	  if (dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      m[i].best_dd=best_dd;
      if (best_dd!=MAX_I64) {
	m[i].x+=4*SignI64(m[best].x-m[i].x);
	m[i].y+=4*SignI64(m[best].y-m[i].y);
	m[i].theta=Arg(m[best].x-m[i].x,m[best].y-m[i].y);
      }
    }
  }
}

U0 MenFight(I64 phase_group)
{
  I64 i,j,dd,best,best_dd;
  for (i=phase_group;i<MAX_FRIENDLY;i+=PHASES_GROUPS) {
    if (!m[i].dead) {
      best=MAX_FRIENDLY;
      best_dd=MAX_I64;
      for (j=MAX_FRIENDLY;j<MAX_MAN;j++) {
	if (!m[j].dead && m[i].y-m[j].y<8) {
	  dd=SqrI64(m[i].x-m[j].x)+SqrI64(m[i].y-m[j].y);
	  if (dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      m[i].best_dd=best_dd;
      if (best_dd<HACK_DIST*HACK_DIST)
	if (!(RandU16&1)) {
	  enemy_left--;
	  enemy_by_friendly++;
	  m[best].dead=TRUE;
	  m[best].corpse_timeout=tS+30.0;
	}
    }
  }
  for (i=MAX_FRIENDLY+phase_group;i<MAX_MAN;i+=PHASES_GROUPS) {
    if (!m[i].dead) {
      best=0;
      best_dd=MAX_I64;
      for (j=0;j<MAX_FRIENDLY;j++) {
	if (!m[j].dead&& m[i].y-m[j].y<8) {
	  dd=SqrI64(m[i].x-m[j].x)+SqrI64(m[i].y-m[j].y);
	  if (dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      m[i].best_dd=best_dd;
      if (best_dd<HACK_DIST*HACK_DIST)
	if (!(RandU16&1)) {
	  friendly_left--;
	  m[best].dead=TRUE;
	  m[best].corpse_timeout=tS+30.0;
	}
    }
  }
}

U0 TimeOut()
{
  I64 i,msg_code,a1,a2;
  Bool gun_on;

  I64 next_update_jiffy;
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Preempt;
  WinBorder;
  AutoComplete;

  MenuPush(
  "File {"
  "  Abort(,CH_SHIFT_ESC);"
  "  Exit(,CH_ESC);"
  "}"
  "Play {"
  "  Restart(,'\n');"
  "  Fire(,CH_SPACE);"
  "  Left(,,SC_CURSOR_LEFT);"
  "  Right(,,SC_CURSOR_RIGHT);"
  "}"
  );
  WinMax;
  DocClear;
  "\n\nYou're sent back through time "
  "to help fight some bad guys.  ($LTPURPLE$Purple$FG$)\n\n"
  "Scoring:\n"
  "  Enemy Killed\t\t+5\n"
  "  Enemy by Friendly\t+10\n"
  "  Friendly Fire\t\t-200\n"
  "  Bullets Fired\t\t-1\n"
  "  Game Time\t\t-10 per second\n"
  "  Surviving Friendlies\t+500\n\n"
  "Sweep side-to-side while shooting.\n\n";

  PressAKey;
  Fs->text_attr=YELLOW<<4+CYAN;
  Init;
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;

  gun_on=FALSE;

  try {
    while (TRUE) {
      next_update_jiffy=cnts.jiffies+JIFFY_FREQ/100;
      while (msg_code=ScanMsg(&a1,&a2,
      1<<MSG_KEY_DOWN+1<<MSG_KEY_UP)) {
	switch (msg_code) {
	  case MSG_KEY_DOWN:
	    switch (a1) {
	      case CH_SHIFT_ESC:
	      case CH_ESC:
		goto to_done;
	      case '\n':
		Init;
		break;
	      case CH_SPACE:
		gun_on=TRUE;
		break;
	    }
	    break;
	  case MSG_KEY_UP:
	    if (a1==CH_SPACE)
	      gun_on=FALSE;
	    else if (a2.u8[0]==SC_CURSOR_RIGHT||a2.u8[0]==SC_CURSOR_LEFT)
	      theta=0.0;
	    break;
	}
      }
      if (snd_death_timeout) {
	if (tS>snd_death_timeout) {
	  snd_death_timeout=0;
	  Snd(0);
	}
      } else if (snd_bullet_timeout && tS>snd_bullet_timeout) {
	snd_bullet_timeout=0;
	Snd(0);
      }

      for (i=0;i<MAX_BULLETS;i++) {
	if (!b[i].dead) {
	  b[i].x+=b[i].dx2;
	  b[i].y+=b[i].dy2;
	  if (b[i].y<0||b[i].x<0||
	  b[i].x>>32>=Fs->pix_width||
	  b[i].y>>32>=Fs->pix_height)
	    b[i].dead=TRUE;
	}
      }

      dx=0;
      if (Bt(kbd.down_bitmap,SC_CURSOR_LEFT)) {
	theta=-15.0*pi/180.0;
	dx=-2;
      } else if (Bt(kbd.down_bitmap,SC_CURSOR_RIGHT)) {
	theta=15.0*pi/180.0;
	dx=2;
      }
      x+=dx;
      while (x>=Fs->pix_width)
	x-=Fs->pix_width;
      while (x<0)
	x+=Fs->pix_width;

      //It takes too much CPU do do all these all the time.
      nobound_switch (main_loop_pass&7) {
	case 0:
	  nobound_switch (main_loop_pass>>3&7) {
	    case 0:
	      if (--y<20)
		game_over=TRUE;
	    case 2:
	    case 4:
	    case 6:
	      MenFight(main_loop_pass>>4%PHASES_GROUPS);
	      break;
	    case 1:
	    case 3:
	    case 5:
	    case 7:
	      MenMove (main_loop_pass>>4%PHASES_GROUPS);
	      break;
	  }
	case 4:
	  break;
	case 2:
	case 6:
	  if (gun_on)
	    FireBullet;
	  break;
	case 1:
	case 3:
	case 5:
	case 7:
	  CheckCollisions;
	  break;
      }
      main_loop_pass++;

      scroll_y--;
      SleepUntil(next_update_jiffy);

      total_score=5*(MAX_ENEMY-enemy_left)
	+10*enemy_by_friendly
	-200*friendly_fire
	-10*(tS-t0)
	-bullets_fired;
      if (game_over) {
	tf=tS;
	Sleep(750);
	FlushMsgs;
	while (!ScanKey(&a1)&&friendly_left) {
	  total_score+=500;
	  Snd(2000); Sleep(150);
	  Snd(0);    Sleep(50);
	  friendly_left--;
	}
	total_score+=500*friendly_left;
	if (total_score>best_score)
	  best_score=total_score;
	if (!a1) a1=GetChar(,FALSE);
	if (a1==CH_ESC||a1==CH_SHIFT_ESC)
	  goto to_done;
	gun_on=FALSE;
	Init;
      }
    }
to_done:
    GetMsg(,,1<<MSG_KEY_UP);
  }
    catch
      PutExcept;
  SettingsPop;
  MenuPop;
  RegWriteBranch("TempleOS/TimeOut","I64 best_score=%d;\n",best_score);
}
