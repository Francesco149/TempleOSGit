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

I64 x,y,dx,dy,finish_line;
F64 theta;
F64 t0,tf,snd_timeout;

#define PHASES_GROUPS		8
#define MAX_ENEMY	(PHASES_GROUPS*64)
#define MAX_FRIENDLY	(PHASES_GROUPS*16)
#define MAX_MAN		(MAX_FRIENDLY+MAX_ENEMY)
#define HACK_DIST	5
class Man
{
  I64 x,y,best_dd;
  F64 theta,phase,corpse_timeout;
  Bool friendly,dead,pad[6];
} m[MAX_MAN];

I64 total_score,friendly_fire,enemy_by_friendly,friendly_left,enemy_left,
    main_loop_pass;
Bool game_over;



	/* <1> <1> (image) */







	/* <2> <2> (image) */



	/* <3> <3> (image) */



	/* <4> <4> (image) */



CSprite *friendly_imgs[4]={__BIN_3,__BIN_2,__BIN_3,__BIN_4};




	/* <5> <5> (image) */




	/* <6> <6> (image) */




	/* <7> <7> (image) */




CSprite *friendly_hacking_imgs[4]={__BIN_6,__BIN_5,__BIN_6,__BIN_7};




	/* <8> <8> (image) */



	/* <9> <9> (image) */



	/* <10> <10> (image) */



CSprite *enemy_imgs[4]={__BIN_9,__BIN_8,__BIN_9,__BIN_10};




	/* <11> <11> (image) */




	/* <12> <12> (image) */




	/* <13> <13> (image) */




CSprite *enemy_hacking_imgs[4]={__BIN_12,__BIN_11,__BIN_12,__BIN_13};



#define LANDSCAPE_TYPES	4






	/* <14> <14> (image) */



	/* <15> <15> (image) */







	/* <16> <16> (image) */















	/* <17> <17> (image) */



CSprite *landscape_imgs[LANDSCAPE_TYPES]=
  {__BIN_14,__BIN_15,__BIN_16,__BIN_17};

#define MAX_LANDSCAPE 64
class LandScapeItem
{
  I64 x,y;
  U8 *img;
} ls[MAX_LANDSCAPE];

I64 mp_not_done_flags;
U0 MPDrawMen(CDC *dc2)
{
  CTask *task=dc2->win_task;
  CDC *dc=DCAlias(dc2,task);
  I64 i,r[16],lo=Gs->num*MAX_MAN/mp_cnt,hi=(Gs->num+1)*MAX_MAN/mp_cnt,
	yy,phase,scroll_y=MAP_HEIGHT-100-100*(tS-t0);
  Man *tempm;
  CSprite *temps,**_temps;
  F64 tt,ts=tS;

  for (i=lo;i<hi;i++) {
    tempm=&m[i];
    yy=(tempm->y-scroll_y)&(MAP_HEIGHT-1);
    if (-32<=yy<=task->pix_bottom+32 && !tempm->dead) {
      Mat4x4IdentEqu(r);
      Mat4x4RotY(r,tempm->theta+pi/2);
      Mat4x4RotX(r,pi/6);
      Mat4x4Scale(r,0.3);
      if (tempm->best_dd<(2*HACK_DIST)*(2*HACK_DIST)) {//It's neat so times 2.
	if (tempm->friendly)
	  _temps=friendly_hacking_imgs;
	else
	  _temps=enemy_hacking_imgs;
	tt=4*Wrap(tempm->phase+20*ts,0)/(2*pi);
      } else {
	if (tempm->friendly)
	  _temps=friendly_imgs;
	else
	  _temps=enemy_imgs;
	tt=4*Wrap(tempm->phase+5*ts,0)/(2*pi);
      }
      phase=tt; tt%=1.0;
      temps=SpriteInterpolate(tt,_temps[phase&3],_temps[(phase+1)&3]);
      SpriteMat3B(dc,tempm->x,yy,GR_Z_ALL,temps,r);
      Free(temps);
    }
  }

  dc->depth_buf=NULL;
  DCDel(dc);
  LBtr(&mp_not_done_flags,Gs->num);
}

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,yy,scroll_y=MAP_HEIGHT-100-100*(tS-t0);
  F64 tt,ts=tS;
  Man *tempm;
  Bullet *tempb;
  CSprite *temps;

  for (i=0;i<MAX_LANDSCAPE;i++) {
    yy=(ls[i].y-scroll_y)&(MAP_HEIGHT-1);
    if (-32<=yy<=task->pix_bottom+32)
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

  DCDepthBufAlloc(dc);
  mp_not_done_flags=1<<mp_cnt-1;
  for (i=0;i<mp_cnt;i++)
    JobQue(&MPDrawMen,dc,i);
  while (mp_not_done_flags)
    Yield;
  Free(dc->depth_buf);
  dc->depth_buf=NULL;

  SpriteZ3B(dc,x,y,0,__BIN_1,theta);

  if (tf) {
    tt=tf;
    dc->color=RED;
    if (game_over && Blink)
      GrPrint(dc,task->pix_width/2-9*FONT_WIDTH/2,task->pix_height/2,
	    "Game Over");
  } else {
    tt=ts;
    if (!enemy_left || !friendly_left)
      game_over=TRUE;
  }
  dc->color=BROWN;
  GrPrint(dc,0,0,"Enemy:%d Friends:%d Friendly Fire:%d Time:%6.2f Bullets:%d",
	enemy_left,friendly_left,friendly_fire,tt-t0,bullets_fired);
  GrPrint(dc,0,8,"Total Score:%,d High Score:%,d",
	total_score,best_score);
}

U0 CheckCollisions()
{
  I64 i,x,y,scroll_y=MAP_HEIGHT-100-100*(tS-t0);
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
	GrRect(dc2,x-3,y-9,6,8);
	if (dc2->collision_cnt) {
	  tempm->dead=TRUE;
	  tempm->corpse_timeout=tS+30.0;
	  if (tempm->friendly) {
	    friendly_fire++;
	    friendly_left--;
	  } else
	    enemy_left--;
	  if (!snd_timeout) {
	    snd_timeout=tS+0.01;
	    if (tempm->friendly)
	      Snd(200);
	    else
	      Snd(50);
	  }
	}
      }
    }
  }
  DCDel(dc2);
}

U0 Init()
{
  I64 i,xx,yy,scroll_y=MAP_HEIGHT-100;
  Man *tempm;

  snd_timeout=0;
  Snd(0);

  total_score=0;
  game_over=FALSE;
  main_loop_pass=0;

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

  MemSet(m,0,sizeof(m));
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
    tempm->best_dd=MAX_I64;
    tempm->theta=0;
    tempm->phase=2*pi*Rand;
    tempm->corpse_timeout=0;
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
  if (!snd_timeout) {
    snd_timeout=tS+0.0005;
    Snd(1000);
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
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;

  "\n\nYou're sent back through time to help fight some bad guys.  "
	"($LTPURPLE$Purple$FG$)\n"
	"Perhaps, you are an angel helping the Jews in the Bible?\n\n"
	"Scoring:\n"
	"\tEnemy Killed\t\t+5\n"
	"\tEnemy by Friendly\t+10\n"
	"\tFriendly Fire\t\t-200\n"
	"\tBullets Fired\t\t-1\n"
	"\tGame Time\t\t-10 per second\n"
	"\tSurviving Friendlies\t+500\n\n"
	"Sweep side-to-side while shooting, "
	"holding down $GREEN$<SPACE>$FG$.\n\n";
  PressAKey;

  Fs->text_attr=YELLOW<<4+CYAN;
  Init;
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
      switch [main_loop_pass&7] {
	case 0:
	  switch [main_loop_pass>>3&7] {
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

      if (snd_timeout && tS>snd_timeout) {
	snd_timeout=0;
	Snd(0);
      }
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
