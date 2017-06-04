RegDft("TempleOS/Titanium","I64 best_score=0;\n");
RegExe("TempleOS/Titanium");

#define MAP_HEIGHT	4096

#define B_LEN	10
#define B_SPEED	5
#define B_NUM	128
class Bullet
{
  I64 x,y,dx,dy,dx2,dy2;
  Bool dead,missile,pad[6];
} b[B_NUM];
I64 bullets_fired,missile_bmp;

I64 x,y,dx,dy,finish_line;
F64 theta,t0,tf,snd_timeout;

#define PHASES_GROUPS	8
#define HACK_DIST	5

#define U_ENEMY_NUM	(PHASES_GROUPS*64)
#define U_FRIENDLY_NUM	(PHASES_GROUPS*16)
#define U_NUM	(U_FRIENDLY_NUM+U_ENEMY_NUM)
class Unit
{
  I64 x,y,best_dd;
  F64 theta,phase;
  Bool friendly,dead,tank,pad[5];
} u[U_NUM];

#define ET_MAN		0
#define ET_TANK		1
#define ET_MISSILE	2
#define ET_BUILDING	3
#define E_NUM		512
class Explosion
{
  I64 x,y,num,type;
  F64 t0,tf;
  Bool dead,pad[7];
} e[E_NUM];

I64 total_score,friendly_fire,enemy_by_friendly,friendly_left,enemy_left,
    main_loop_pass;
Bool game_over;





	/* <1> <1> (image) */





	/* <2> <2> (image) */

	/* <3> <3> (image) */




	/* <4> <4> (image) */



	/* <5> <5> (image) */



	/* <6> <6> (image) */



U8 *friendly_imgs[4]={__BIN_5,__BIN_4,__BIN_5,__BIN_6};




	/* <7> <7> (image) */




	/* <8> <8> (image) */




	/* <9> <9> (image) */




U8 *friendly_hacking_imgs[4]={__BIN_7,__BIN_8,__BIN_7,__BIN_9};




	/* <10> <10> (image) */



	/* <11> <11> (image) */



	/* <12> <12> (image) */



U8 *enemy_imgs[4]={__BIN_11,__BIN_10,__BIN_11,__BIN_12};




	/* <13> <13> (image) */




	/* <14> <14> (image) */




	/* <15> <15> (image) */











	/* <16> <16> (image) */









U8 *enemy_hacking_imgs[4]={__BIN_14,__BIN_13,__BIN_14,__BIN_15};







	/* <17> <17> (image) */




	/* <18> <18> (image) */








	/* <19> <19> (image) */




	/* <20> <20> (image) */





#define LS_APTS_NUM		20
#define LS_MOUNTAINS_NUM	3
#define LS_NUM			128
class LandScapeItem
{
  I64 x,y;
  U8 *img;
} ls[LS_NUM];

#define LS_TYPES	4
U8 *landscape_imgs[LS_TYPES]=
  {__BIN_17,__BIN_18,__BIN_19,__BIN_20};

U0 ExplosionDraw(CDC *dc,Explosion *tmpe,I64 y)
{
  I64 i,n1,n2,n3,n4,n5;
  F64 t=(tS-tmpe->t0)/(tmpe->tf-tmpe->t0);
  Seed(tmpe->num+1);

  switch (tmpe->type) {
    case ET_MAN:	n1=8;	n2=0;  n3=0;  n4=18;  n5=28;	break;
    case ET_TANK:	n1=64;	n2=14; n3=24; n4=60;  n5=90;	break;
    case ET_MISSILE:	n1=128; n2=30; n3=60; n4=100; n5=200;	break;
    case ET_BUILDING:	n1=128; n2=80; n3=80; n4=200; n5=300;	break;
  }
  for (i=0;i<n1;i++) {
    if (i&2)
      switch (tmpe->type) {
	case ET_MAN:
	  if (i&1)
	    dc->color=WHITE;
	  else
	    dc->color=LTGRAY;
	  break;
	case ET_MISSILE:
	  if (i&1) {
	    if (i&4)
	      dc->color=RED;
	    else
	      dc->color=DKGRAY;
	  } else
	    dc->color=YELLOW;
	  break;
	case ET_TANK:
	  if (i&1) {
	    if (i&4)
	      dc->color=RED;
	    else
	      dc->color=DKGRAY;
	  } else
	    dc->color=LTRED;
	  break;
	case ET_BUILDING:
	  if (i&1) {
	    if (i&4)
	      dc->color=BLACK;
	    else
	      dc->color=DKGRAY;
	  } else
	    dc->color=LTGRAY;
	  break;
      }
    else if (i&1)
      dc->color=WHITE;
    else
      dc->color=LTGRAY;
    GrLine(dc,tmpe->x+n2*(Rand-.5),y+n3/2*Rand,
	  tmpe->x+n4*t*(Rand-.5),y-n5/2*t*Rand);
  }
}

I64 mp_not_done_flags;
U0 MPMenDraw(CDC *dc2)
{
  CTask *task=dc2->win_task;
  CDC *dc=DCAlias(dc2,task);
  I64 i,r[16],lo=Gs->num*U_NUM/mp_cnt,hi=(Gs->num+1)*U_NUM/mp_cnt,
	yy,phase,scroll_y=MAP_HEIGHT-100-100*(tS-t0);
  Unit *tmpu;
  Explosion *tmpe;
  U8 *tmps,**_tmps;
  F64 tt,ts=tS;

  for (i=Gs->num;i<E_NUM;i+=mp_cnt) {
    tmpe=&e[i];
    yy=(tmpe->y-scroll_y)&(MAP_HEIGHT-1);
    if (-32<=yy<=task->pix_bottom+32 && !tmpe->dead) {
      ExplosionDraw(dc,tmpe,yy);
      if (tS>tmpe->tf)
	tmpe->dead=TRUE;
    }
  }
  for (i=lo;i<hi;i++) {
    tmpu=&u[i];
    yy=(tmpu->y-scroll_y)&(MAP_HEIGHT-1);
    if (-32<=yy<=task->pix_bottom+32) {
      if (!tmpu->dead) {
	Mat4x4IdentEqu(r);
	Mat4x4RotY(r,tmpu->theta);
	Mat4x4RotX(r,pi/6);
	Mat4x4Scale(r,0.3);
	if (tmpu->tank)
	  Sprite3Mat4x4B(dc,tmpu->x,yy,GR_Z_ALL,__BIN_16,r);
	else {
	  if (tmpu->best_dd<(2*HACK_DIST)*(2*HACK_DIST)) {//It's neat so times 2
	    if (tmpu->friendly)
	      _tmps=friendly_hacking_imgs;
	    else
	      _tmps=enemy_hacking_imgs;
	    tt=4*Wrap(tmpu->phase+20*ts,0)/(2*pi);
	  } else {
	    if (tmpu->friendly)
	      _tmps=friendly_imgs;
	    else
	      _tmps=enemy_imgs;
	    tt=4*Wrap(tmpu->phase+5*ts,0)/(2*pi);
	  }
	  phase=tt; tt%=1.0;
	  tmps=SpriteInterpolate(tt,_tmps[phase&3],_tmps[(phase+1)&3]);
	  Sprite3Mat4x4B(dc,tmpu->x,yy,GR_Z_ALL,tmps,r);
	  Free(tmps);
	}
      }
    }
  }

  dc->depth_buf=NULL;
  DCDel(dc);
  LBtr(&mp_not_done_flags,Gs->num);
  Seed; //Return Seth task to timer-based.
}

U0 MissilePos(I64 m,F64 theta,I64 *_x,I64 *_y)
{
  I64 n;
  if (m<2)
    n=-1;
  else
    n=1;
  *_x=x+3.0*Cos(theta)-(15.0-(m&1)<<3)*Cos(theta-n*pi/2);
  *_y=y+3.0*Sin(theta)-(15.0-(m&1)<<3)*Sin(theta-n*pi/2);
}

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 i,m,xx,yy,scroll_y=MAP_HEIGHT-100-100*(tS-t0);
  F64 tt,ts=tS;
  Bullet *tmpb;

  dc->color=ROPF_DITHER|BROWN<<16|YELLOW;
  GrRect3(dc,0,0,0,dc->width,dc->height);

  for (i=0;i<LS_NUM;i++) {
    yy=(ls[i].y-scroll_y)&(MAP_HEIGHT-1);
    if (-32<=yy<=task->pix_bottom+32)
      Sprite3(dc,ls[i].x,yy,0,ls[i].img);
  }

  dc->thick=3;
  dc->color=BROWN;
  GrLine3(dc,0,  (finish_line-scroll_y)&(MAP_HEIGHT-1),0,
	GR_WIDTH,(finish_line-scroll_y)&(MAP_HEIGHT-1),0);

  dc->color=LTGRAY;
  for (i=0,tmpb=b;i<B_NUM;i++,tmpb++)
    if (!tmpb->dead) {
      if (tmpb->missile)
	Sprite3ZB(dc,tmpb->x>>32,tmpb->y>>32,0,__BIN_2,pi/2+Arg(tmpb->dx,tmpb->dy));
      else
	GrLine(dc,tmpb->x>>32,tmpb->y>>32,
	      (tmpb->x+tmpb->dy*B_LEN)>>32,
	      (tmpb->y+tmpb->dx*B_LEN)>>32);
    }

  DCDepthBufAlloc(dc);
  mp_not_done_flags=1<<mp_cnt-1;
  for (i=0;i<mp_cnt;i++)
    JobQue(&MPMenDraw,dc,i);
  while (mp_not_done_flags)
    Yield;
  Free(dc->depth_buf);
  dc->depth_buf=NULL;

  for (m=0;m<4;m++)
    if (Bt(&missile_bmp,m)) {
      MissilePos(m,theta,&xx,&yy);
      Sprite3ZB(dc,xx,yy,0,__BIN_3,theta);
    }
  Sprite3ZB(dc,x,y,0,__BIN_1,theta);

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
  dc->color=BLACK;
  GrPrint(dc,0,0,"Enemy:%d Friends:%d Friendly Fire:%d Time:%6.2f Bullets:%d",
	enemy_left,friendly_left,friendly_fire,tt-t0,bullets_fired);
  GrPrint(dc,0,8,"Total Score:%,d High Score:%,d",
	total_score,best_score);
}

Explosion *ExplosionNew(I64 x,I64 y)
{
  I64 i;
  for (i=0;i<E_NUM;i++)
    if (e[i].dead) {
      e[i].x=x;
      e[i].y=y;
      e[i].dead=FALSE;
      return &e[i];
    }
  return NULL;
}

U0 ManDie(Unit *tmpu,Bool by_human)
{
  Explosion *tmpe;

  tmpu->dead=TRUE;
  if (by_human) {
    if (tmpe=ExplosionNew(tmpu->x,tmpu->y)) {
      tmpe->t0=tS;
      if (tmpu->tank) {
	tmpe->type=ET_TANK;
	tmpe->tf=tmpe->t0+0.40;
      } else {
	tmpe->type=ET_MAN;
	tmpe->tf=tmpe->t0+0.20;
      }
    }
  }

  if (tmpu->friendly) {
    friendly_left--;
    if (by_human)
      friendly_fire++;
  } else {
    enemy_left--;
    if (!by_human)
      enemy_by_friendly++;
  }
  if (by_human && !snd_timeout) {
    snd_timeout=tS+0.01;
    if (tmpu->friendly)
      Snd(46);
    else
      Snd(22);
  }
}

U0 ExplosionDo(I64 x,I64 y,I64 scroll_y)
{
  I64 i,x2,y2;
  Explosion *tmpe;
  Unit *tmpu;
  if (tmpe=ExplosionNew(x,y+scroll_y)) {
    tmpe->t0=tS;
    tmpe->type=ET_MISSILE;
    tmpe->tf=tmpe->t0+1.0;
  }
  for (i=0,tmpu=u;i<U_NUM;i++,tmpu++) {
    if (!tmpu->dead) {
      x2=tmpu->x;
      y2=(tmpu->y-scroll_y)&(MAP_HEIGHT-1);
      if (SqrI64(x-x2)+SqrI64(y-y2)<100*100)
	ManDie(tmpu,TRUE);
    }
  }
  for (i=0;i<LS_APTS_NUM;i++) {
    x2=ls[i].x;
    y2=(ls[i].y-scroll_y)&(MAP_HEIGHT-1);
    if (SqrI64(x-x2)+SqrI64(y-y2)<100*100) {
      if (tmpe=ExplosionNew(x2,y2+scroll_y)) {
	tmpe->t0=tS;
	tmpe->type=ET_BUILDING;
	tmpe->tf=tmpe->t0+2.0;
      }
    }
  }
}

U0 CheckCollisions()
{
  I64 i,n1,x,y,scroll_y=MAP_HEIGHT-100-100*(tS-t0);
  Unit *tmpu;
  Bullet *tmpb;
  CDC	*dc2=DCNew(GR_WIDTH,GR_HEIGHT);
  dc2->color=LTRED;
  for (i=0,tmpb=b;i<B_NUM;i++,tmpb++)
    if (!tmpb->dead && !tmpb->missile)	//Bullets not missiles
      GrLine(dc2,tmpb->x>>32,tmpb->y>>32,
	    (tmpb->x+tmpb->dx*B_LEN)>>32,
	    (tmpb->y+tmpb->dy*B_LEN)>>32);

  dc2->color  =ROP_COLLISION;
  dc2->bkcolor=BLACK;
  for (i=0,tmpu=u;i<U_NUM;i++,tmpu++) {
    if (!tmpu->dead) {
      x=tmpu->x;
      y=(tmpu->y-scroll_y)&(MAP_HEIGHT-1);
      if (0<=x<GR_WIDTH && 0<=y<GR_HEIGHT) {
	dc2->collision_cnt=0;
	GrRect(dc2,x-3,y-9,6,8);
	if (dc2->collision_cnt)
	  ManDie(tmpu,TRUE);
      }
    }
  }
  DCDel(dc2);

  for (i=0,tmpb=b;i<B_NUM;i++,tmpb++)
    if (!tmpb->dead && tmpb->missile)  { //Missiles not bullets
      x=tmpb->x>>32;
      y=tmpb->y>>32;
      for (i=0,tmpu=u;i<U_NUM;i++,tmpu++) {
	if (tmpu->tank)
	  n1=16;
	else
	  n1=6;
	if (!tmpu->dead && AbsI64(x-tmpu->x-n1)+
	      AbsI64(y-(tmpu->y-scroll_y)&(MAP_HEIGHT-1)+n1)<n1<<1) {
	  tmpb->dead=TRUE;
	  ExplosionDo(x,y,scroll_y);
	}
      }
      if (!tmpb->dead)
	for (i=0;i<LS_APTS_NUM;i++)
	  if (2*SqrI64(x-ls[i].x)+
		3*SqrI64(y+35-(ls[i].y-scroll_y)&(MAP_HEIGHT-1))<2*60*60) {
	    tmpb->dead=TRUE;
	    ExplosionDo(x,y,scroll_y);
	  }
    }
}

U0 Init()
{
  I64 i,xx,yy,scroll_y=MAP_HEIGHT-100;
  Unit *tmpu;

  snd_timeout=0;
  Snd;

  total_score=0;
  game_over=FALSE;
  main_loop_pass=0;

  x=Fs->pix_width>>1;
  y=0.9*Fs->pix_height;
  finish_line=scroll_y+y;
  dx=0;
  dy=0;
  theta=-pi/2;

  for (i=0;i<LS_NUM;i++) {
    ls[i].x=(Fs->pix_width-100)*RandU32/U32_MAX+50;
    ls[i].y=(MAP_HEIGHT-100)*RandU32/U32_MAX+50;
    ls[i].img=landscape_imgs[RandU16%(LS_TYPES-2)];
  }

  for (i=0;i<LS_APTS_NUM;i++)
    ls[i].img=landscape_imgs[LS_TYPES-1]; //Apartment

  for (;i<LS_APTS_NUM+LS_MOUNTAINS_NUM;i++)
    ls[i].img=landscape_imgs[LS_TYPES-2]; //mountain

  MemSet(u,0,sizeof(u));
  for (i=0,tmpu=u;i<U_NUM;i++,tmpu++) {
    if (i<U_FRIENDLY_NUM) {
      tmpu->friendly=TRUE;
      if (!(i&7)) {
	xx=(Fs->pix_width-200)*RandU32/U32_MAX;
	yy=(MAP_HEIGHT-200)*RandU32/U32_MAX;
      }
    } else {
      tmpu->friendly=FALSE;
      if (!(i&31)) {
	xx=(Fs->pix_width-200)*RandU32/U32_MAX;
	yy=(MAP_HEIGHT-200)*RandU32/U32_MAX;
      }
      if (!(i&15))
	tmpu->tank=TRUE;
    }
    tmpu->dead=FALSE;
    tmpu->x=xx+64*RandI32/I32_MAX+100;
    tmpu->y=yy+64*RandU32/I32_MAX-64+100;
    tmpu->best_dd=I64_MAX;
    tmpu->theta=pi/2;
    tmpu->phase=2*pi*Rand;
  }
  for (i=0;i<B_NUM;i++)
    b[i].dead=TRUE;
  for (i=0;i<E_NUM;i++) {
    e[i].dead=TRUE;
    e[i].num=i;
  }
  friendly_left=U_FRIENDLY_NUM;
  enemy_left   =U_ENEMY_NUM;
  enemy_by_friendly=0;
  bullets_fired=0;
  missile_bmp =15;
  friendly_fire=0;
  t0=tS;
  tf=0;
}

U0 FireBullet()
{
  I64 i,j;
  F64 a;
  Bullet *tmpb;
  for (i=0;i<B_NUM-1;i++)
    if (b[i].dead)
      break;
  tmpb=&b[i];

  j=x+28.0*Cos(theta);
  tmpb->x=j<<32;
  j=y+28.0*Sin(theta);
  tmpb->y=j<<32;

  tmpb->dx2=(B_SPEED*Cos(theta)+dx)*0x100000000;
  tmpb->dy2= B_SPEED*Sin(theta)*0x100000000;
  a=Arg(tmpb->dx2,tmpb->dy2);
  tmpb->dx=Sin(a)*0x100000000;
  tmpb->dy=Cos(a)*0x100000000;
  bullets_fired++;
  tmpb->dead=FALSE;
  tmpb->missile=FALSE;
  if (!snd_timeout) {
    snd_timeout=tS+0.0005;
    Snd(74);
  }
}

Bool FireMissile(I64 n)
{
  I64 i,m;
  F64 a;
  Bullet *tmpb;
  Bool res=FALSE;

  m=n;
  if (Btr(&missile_bmp,++m) || Btr(&missile_bmp,++m))
    res=TRUE;

  if (res) {
    for (i=0;i<B_NUM-1;i++)
      if (b[i].dead)
	break;
    tmpb=&b[i];

    MissilePos(m,theta,&tmpb->x,&tmpb->y);
    tmpb->x<<=32;
    tmpb->y<<=32;

    tmpb->dx2=(B_SPEED*Cos(theta)+dx)*0x100000000;
    tmpb->dy2= B_SPEED*Sin(theta)*0x100000000;
    a=Arg(tmpb->dx2,tmpb->dy2);
    tmpb->dx=Sin(a)*0x100000000;
    tmpb->dy=Cos(a)*0x100000000;
    tmpb->dead=FALSE;
    tmpb->missile=TRUE;
    if (!snd_timeout) {
      snd_timeout=tS+0.0005;
      Snd(74);
    }
  }
}

U0 MenMove(I64 phase_group)
{
  I64 i,j,dd,best,best_dd;
  for (i=phase_group;i<U_FRIENDLY_NUM;i+=PHASES_GROUPS) {
    if (!u[i].dead) {
      best=U_FRIENDLY_NUM;
      best_dd=I64_MAX;
      for (j=U_FRIENDLY_NUM;j<U_NUM;j++) {
	if (!u[j].dead) {
	  dd=SqrI64(u[i].x-u[j].x)+SqrI64(u[i].y-u[j].y);
	  if (dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      u[i].best_dd=best_dd;
      if (best_dd!=I64_MAX) {
	u[i].x+=4*SignI64(u[best].x-u[i].x);
	u[i].y+=4*SignI64(u[best].y-u[i].y);
	u[i].theta=Arg(u[best].x-u[i].x,u[best].y-u[i].y);
      }
    }
  }
  for (i=U_FRIENDLY_NUM+phase_group;i<U_NUM;i+=PHASES_GROUPS) {
    if (!u[i].dead) {
      best=0;
      best_dd=I64_MAX;
      for (j=0;j<U_FRIENDLY_NUM;j++) {
	if (!u[j].dead) {
	  dd=SqrI64(u[i].x-u[j].x)+SqrI64(u[i].y-u[j].y);
	  if (dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      u[i].best_dd=best_dd;
      if (best_dd!=I64_MAX) {
	u[i].x+=4*SignI64(u[best].x-u[i].x);
	u[i].y+=4*SignI64(u[best].y-u[i].y);
	u[i].theta=Arg(u[best].x-u[i].x,u[best].y-u[i].y);
      }
    }
  }
}

U0 MenFight(I64 phase_group)
{
  I64 i,j,dd,best,best_dd;
  for (i=phase_group;i<U_FRIENDLY_NUM;i+=PHASES_GROUPS) {
    if (!u[i].dead) {
      best=U_FRIENDLY_NUM;
      best_dd=I64_MAX;
      for (j=U_FRIENDLY_NUM;j<U_NUM;j++) {
	if (!u[j].dead && u[i].y-u[j].y<8) {
	  dd=SqrI64(u[i].x-u[j].x)+SqrI64(u[i].y-u[j].y);
	  if (dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      u[i].best_dd=best_dd;
      if (best_dd<HACK_DIST*HACK_DIST && !(RandU16&1))
	ManDie(&u[best],FALSE);
    }
  }
  for (i=U_FRIENDLY_NUM+phase_group;i<U_NUM;i+=PHASES_GROUPS) {
    if (!u[i].dead) {
      best=0;
      best_dd=I64_MAX;
      for (j=0;j<U_FRIENDLY_NUM;j++) {
	if (!u[j].dead&& u[i].y-u[j].y<8) {
	  dd=SqrI64(u[i].x-u[j].x)+SqrI64(u[i].y-u[j].y);
	  if (dd<best_dd) {
	    best_dd=dd;
	    best=j;
	  }
	}
      }
      u[i].best_dd=best_dd;
      if (best_dd<HACK_DIST*HACK_DIST && !(RandU16&1))
	ManDie(&u[best],FALSE);
    }
  }
}

U0 Titanium()
{
  I64 i,msg_code,ch,sc;
  Bool gun_on;

  I64 next_update_jiffy;
  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush

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
	"  LeftMissile(,,SC_CURSOR_LEFT|SCF_CTRL);"
	"  RightMissile(,,SC_CURSOR_RIGHT|SCF_CTRL);"
	"}"
	);
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;

  "\nScoring:\n"
	"\tEnemy Killed\t\t+3\n"
	"\tEnemy by Friendly\t+5\n"
	"\tFriendly Fire\t\t-100\n"
	"\tBullets Fired\t\t-1\n"
	"\tGame Time\t\t-10 per second\n"
	"\tSurviving Friendlies\t+250\n\n"
	"Sweep side-to-side while shooting, "
	"holding down $GREEN$<SPACE>$FG$.\n\n";
  PressAKey;

  Init;
  DocClear;
  Fs->draw_it=&DrawIt;

  gun_on=FALSE;

  try {
    while (TRUE) {
      next_update_jiffy=cnts.jiffies+JIFFY_FREQ/100;
      while (msg_code=ScanMsg(&ch,&sc,1<<MSG_KEY_DOWN+1<<MSG_KEY_UP)) {
	switch (msg_code) {
	  case MSG_KEY_DOWN:
	    switch (ch) {
	      case 0:
		switch (sc.u8[0]) {
		  case SC_CURSOR_RIGHT:
		    if (sc&SCF_CTRL)
		      FireMissile(1);
		    break;
		  case SC_CURSOR_LEFT:
		    if (sc&SCF_CTRL)
		      FireMissile(-1);
		    break;
		}
		break;
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
	    if (ch==CH_SPACE)
	      gun_on=FALSE;
	    else if (sc.u8[0]==SC_CURSOR_RIGHT||sc.u8[0]==SC_CURSOR_LEFT)
	      theta=-pi/2;
	    break;
	}
      }

      for (i=0;i<B_NUM;i++) {
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
      if (!Bt(kbd.down_bitmap,SC_CTRL)) {
	if (Bt(kbd.down_bitmap,SC_CURSOR_LEFT)) {
	  theta=-pi/2-15.0*pi/180.0;
	  dx=-2;
	} else if (Bt(kbd.down_bitmap,SC_CURSOR_RIGHT)) {
	  theta=-pi/2+15.0*pi/180.0;
	  dx=2;
	}
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
	Snd;
      }
      SleepUntil(next_update_jiffy);

      total_score=3*(U_ENEMY_NUM-enemy_left)+5*enemy_by_friendly
	    -100*friendly_fire-10*(tS-t0)-bullets_fired;
      if (game_over) {
	tf=tS;
	Sleep(750);
	FlushMsgs;
	while (!ScanKey(&ch)&&friendly_left||tS-tf<1.5) {
	  total_score+=250;
	  Snd(86); Sleep(150);
	  Snd;	  Sleep(50);
	  friendly_left--;
	}
	total_score+=250*friendly_left;
	if (total_score>best_score)
	  best_score=total_score;
	if (!ch) ch=GetChar(,FALSE);
	if (ch==CH_ESC||ch==CH_SHIFT_ESC)
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
  RegWrite("TempleOS/Titanium","I64 best_score=%d;\n",best_score);
}
