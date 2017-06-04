RegDft("TempleOS/Wenceslas","F64 best_score=9999;\n");
RegExe("TempleOS/Wenceslas");

#define BORDER		5
#define KING_STEP	6





	/* <1> <1> (image) */	//See ::/Apps/GrModels
`Cd("::/Apps/GrModels");Dir;View;
`
 for making 3D men.




	/* <2> <2> (image) */



	/* <3> <3> (image) */



	/* <4> <4> (image) */



	/* <5> <5> (image) */



	/* <6> <6> (image) */









 


	/* <7> <7> (image) */




	/* <8> <8> (image) */






	/* <9> <9> (image) */


















	/* <10> <10> (image) */






	/* <11> <11> (image) */


U8 *king_imgs[4]={__BIN_2,__BIN_1,__BIN_2,__BIN_3};
U8 *peasant_imgs[4]={__BIN_5,__BIN_4,__BIN_5,__BIN_6};


#define TREES_NUM	8
class Tree
{
  U16 x,y;
  I64 fire_frame_idx;
} trees[TREES_NUM];

U64 snow_x,snow_y,new_snow_mS;
#define SNOW_TILE_SIZE	128
CDC *snow_tile;

I64 king_x,king_y,king_mS,king_phase,
    not_stopped_cnt;
F64 king_timeout,king_theta,t0,tf,door_open_t0;
I64 animate_mS,animate_phase;

#define PEASANTS_NUM	10
class Peasant
{
  Peasant *next,*last;
  F64 x,y,theta,door_opened_t0;
  Bool stopped;
} peasant_head;

class Step
{
  Step *next,*last;
  U16 x,y;
  F64 t0;
} step_head,king_step_head;

U0 DrawIt(CTask *task,CDC *dc)
{
  I64 r[16],i,w=task->pix_width,h=task->pix_height,x,y;
  U8 *img,*tmps;
  Step *tmpst;
  Peasant *tmpp;
  F64 tt;

  dc->thick=1;
  for (i=0;i<TREES_NUM;i++) {
    if (trees[i].y<=h/2) {//Draw trees behind house
      if (trees[i].fire_frame_idx) {
	if (trees[i].fire_frame_idx++&1)
	  img=__BIN_8;
	else
	  img=__BIN_9;
      } else
	img=__BIN_7;
      Sprite3(dc,trees[i].x,trees[i].y,0,img);
    }
  }

  Sprite3(dc,w/2,h/2,0,__BIN_10);
  if (tS<door_open_t0+1.0)
    Sprite3(dc,w/2,h/2,0,__BIN_11);

  for (i=0;i<TREES_NUM;i++) {
    if (trees[i].y>h/2) {//Draw trees in front of house
      if (trees[i].fire_frame_idx) {
	if (trees[i].fire_frame_idx++&1)
	  img=__BIN_8;
	else
	  img=__BIN_9;
      } else
	img=__BIN_7;
      Sprite3(dc,trees[i].x,trees[i].y,0,img);
    }
  }

  tmpst=step_head.next;
  while (tmpst!=&step_head) {
    if (tS-tmpst->t0<2.0)
      dc->color=DKGRAY;
    else
      dc->color=LTGRAY;
    dc->thick=4;
    GrPlot3(dc,tmpst->x,tmpst->y,0);
    tmpst=tmpst->next;
  }

  DCDepthBufAlloc(dc);
  dc->flags|=DCF_TRANSFORMATION;
  dc->thick=1;
  tmpp=peasant_head.next;
  while (tmpp!=&peasant_head) {
    Mat4x4IdentEqu(r);
    Mat4x4RotY(r,tmpp->theta+pi/2);
    Mat4x4RotX(r,pi/6);
    if (tmpp->stopped)
      Sprite3Mat4x4B(dc,tmpp->x,tmpp->y,GR_Z_ALL,peasant_imgs[0],r);
    else {
      tmps=SpriteInterpolate(Saw(animate_mS/250.0,1.0),
	    peasant_imgs[ animate_phase   &3],
	    peasant_imgs[(animate_phase+1)&3]);
      Sprite3Mat4x4B(dc,tmpp->x,tmpp->y,GR_Z_ALL,tmps,r);
      Free(tmps);
    }
    tmpp=tmpp->next;
  }

  dc->thick=1;
  Mat4x4IdentEqu(r);
  Mat4x4RotY(r,king_theta+pi/2);
  Mat4x4RotX(r,pi/6);
  if (tS>king_timeout)
    Sprite3Mat4x4B(dc,king_x,king_y,GR_Z_ALL,king_imgs[0],r);
  else {
    tmps=SpriteInterpolate(Saw(king_mS/250.0,1.0),
	  king_imgs[ king_phase   &3],
	  king_imgs[(king_phase+1)&3]);
    Sprite3Mat4x4B(dc,king_x,king_y,GR_Z_ALL,tmps,r);
    Free(tmps);
  }

  dc->thick=1;
  dc->color=ROP_MONO|WHITE;
  for (y=snow_y%SNOW_TILE_SIZE-SNOW_TILE_SIZE;y<=h;y+=SNOW_TILE_SIZE)
    for (x=snow_x%SNOW_TILE_SIZE-SNOW_TILE_SIZE;x<=w;x+=SNOW_TILE_SIZE)
      GrBlot(dc,x,y,snow_tile);

  if (tf) {
    dc->color=LTRED;
    if (Blink)
      GrPrint(dc,w/2-(FONT_WIDTH*14)/2,h/2+FONT_HEIGHT,"Game Completed");
    tt=tf;
  } else {
    tt=tS;
  }
  dc->color=LTBLUE;
  GrPrint(dc,0,0,"Freezing Peasants:%d Time:%3.2f Best:%3.2f",
	not_stopped_cnt,tt-t0,best_score);
}

U0 StepNew(CTask *task,I64 x,I64 y,F64 theta,Bool left_right,Bool king)
{
  Step *tmps;

  if (king) {
    tmps=MAlloc(sizeof(Step),task);
    tmps->x=x;
    tmps->y=y;
    tmps->t0=tS;
    QueIns(tmps,king_step_head.last);
  }

  if (left_right) {
    tmps=MAlloc(sizeof(Step),task);
    tmps->x=x-3.5*Sin(theta)+2.0*Cos(theta);
    tmps->y=y+3.5*Cos(theta)+2.0*Sin(theta);
    tmps->t0=tS;
    QueIns(tmps,step_head.last);
    tmps=MAlloc(sizeof(Step),task);
    tmps->x=x-3.5*Sin(theta)+5.0*Cos(theta);
    tmps->y=y+3.5*Cos(theta)+5.0*Sin(theta);
    tmps->t0=tS;
    QueIns(tmps,step_head.last);
  } else {
    tmps=MAlloc(sizeof(Step),task);
    tmps->x=x+3.5*Sin(theta)+0.0*Cos(theta);
    tmps->y=y-3.5*Cos(theta)+0.0*Sin(theta);
    tmps->t0=tS;
    QueIns(tmps,step_head.last);
    tmps=MAlloc(sizeof(Step),task);
    tmps->x=x+3.5*Sin(theta)+3.0*Cos(theta);
    tmps->y=y-3.5*Cos(theta)+3.0*Sin(theta);
    tmps->t0=tS;
    QueIns(tmps,step_head.last);
  }
}

U0 Init()
{
  I64 i,min_x,max_x,min_y,max_y,
	w=Fs->pix_width,h=Fs->pix_height;
  Peasant *tmpp;

  snow_x=snow_y=0;
  new_snow_mS=0;
  snow_tile=DCNew(SNOW_TILE_SIZE,SNOW_TILE_SIZE);

  SpriteExtents(__BIN_7,&min_x,&max_x,&min_y,&max_y);
  for (i=0;i<TREES_NUM;i++) {
    trees[i].x=RandU16%(w-2*BORDER-(max_x-min_x+1))+BORDER-min_x;
    trees[i].y=RandU16%(h-2*BORDER-(max_y-min_y+1))+BORDER-min_y;
    trees[i].fire_frame_idx=0;
  }

  QueInit(&step_head);
  QueInit(&king_step_head);

  SpriteExtents(__BIN_12,&min_x,&max_x,&min_y,&max_y);
  QueInit(&peasant_head);
  for (i=0;i<PEASANTS_NUM;i++) {
    tmpp=MAlloc(sizeof(Peasant));
    tmpp->x=RandU16%(w-2*BORDER-(max_x-min_x+1))+BORDER-min_x;
    tmpp->y=RandU16%(h-2*BORDER-(max_y-min_y+1))+BORDER-min_y;
    tmpp->theta=pi*2*Rand;
    tmpp->door_opened_t0=0;
    tmpp->stopped=FALSE;
    QueIns(tmpp,peasant_head.last);
  }
  animate_phase=0;
  animate_mS=0;
  king_phase=0;
  king_mS=0;
  king_timeout=0;
  king_x=w/2;
  king_y=h/2;
  king_theta=-pi/2;
  door_open_t0=0;
  t0=tS;
  tf=0;
}

U0 CleanUp()
{
  DCDel(snow_tile);
  QueDel(&peasant_head,TRUE);
  QueDel(&step_head,TRUE);
  QueDel(&king_step_head,TRUE);
}

U0 Follow(CTask *,Peasant *tmpp)
{
  Step *tmps=king_step_head.next;
  F64 d,best_d=F64_MAX;
  while (tmps!=&king_step_head) {
    if ((d=Sqr(tmps->x-tmpp->x)+Sqr(tmps->y-tmpp->y)) &&
	  d<15*15) {
      d+=1000*Sqr(tS-tmps->t0);
      if (d<best_d) {
	best_d=d;
	tmpp->theta=Arg(tmps->x-tmpp->x,tmps->y-tmpp->y);
      }
    }
    tmps=tmps->next;
  }
}

U0 AnimateTask(I64)
{
  Step *tmps,*tmps1;
  Peasant *tmpp;
  I64 i,w,h;
  while (TRUE) {
    w=Fs->parent_task->pix_width;
    h=Fs->parent_task->pix_height;

    tmps=step_head.next;
    while (tmps!=&step_head) {
      tmps1=tmps->next;
      if (tS-tmps->t0>5.0) {
	QueRem(tmps);
	Free(tmps);
      }
      tmps=tmps1;
    }

    tmps=king_step_head.next;
    while (tmps!=&king_step_head) {
      tmps1=tmps->next;
      if (tS-tmps->t0>3.0) {
	QueRem(tmps);
	Free(tmps);
      }
      tmps=tmps1;
    }

    not_stopped_cnt=0;
    tmpp=peasant_head.next;
    while (tmpp!=&peasant_head) {
      if (tmpp->stopped) {
	if (tmpp->door_opened_t0 && tS>tmpp->door_opened_t0+1.0) {
	  tmpp->door_opened_t0=0;
	  tmpp->y=tmpp->x=-9999;
	}
      } else {
	if (Sqr(tmpp->x-w/2)+Sqr(tmpp->y-h/2)<20*20) {
	  tmpp->stopped=TRUE;
	  tmpp->door_opened_t0=door_open_t0=tS;
	}
	for (i=0;i<TREES_NUM;i++) //Hang-out by fire
	  if (trees[i].fire_frame_idx &&
		Sqr(tmpp->x-trees[i].x)+Sqr(tmpp->y-trees[i].y)<20*20) {
	    tmpp->stopped=TRUE;
	    break;
	  }
	if (!tmpp->stopped) {
	  Follow(Fs->parent_task,tmpp);
	  tmpp->x+=Cos(tmpp->theta)/100;
	  tmpp->y+=Sin(tmpp->theta)/100;
	  if (!(BORDER/2<=tmpp->x<w-BORDER/2) ||
		!(BORDER/2<=tmpp->y<h-BORDER/2)) {
	    tmpp->theta+=pi;
	    tmpp->x+=3*Cos(tmpp->theta)/100;
	    tmpp->y+=3*Sin(tmpp->theta)/100;
	  }
	  if (!animate_mS && animate_phase&1)
	    StepNew(Fs->parent_task,tmpp->x,tmpp->y,tmpp->theta,
		  animate_phase&2,FALSE);
	  not_stopped_cnt++;
	}
      }
      tmpp=tmpp->next;
    }
    if (!not_stopped_cnt && !tf) {
      tf=tS;
      music.mute=TRUE;
      Snd(86);Sleep(200);Snd;Sleep(100);
      if (tf-t0<best_score) {
	best_score=tf-t0;
	Snd(86);Sleep(200);Snd;Sleep(100);
      }
      music.mute=FALSE;
    }

    snow_x+=RandU16%3-1;
    snow_y+=1-SignI64(RandU16&3);
    if (new_snow_mS++>8) {
      new_snow_mS=0;
      snow_tile->color=WHITE;
      GrPlot(snow_tile,RandU16&(SNOW_TILE_SIZE-1),RandU16&(SNOW_TILE_SIZE-1));
    }

    Sleep(1);
    if (animate_mS++>=250) {
      animate_mS=0;
      animate_phase=(animate_phase+1)&3;
    }
    if (tS<king_timeout) {
      if (king_mS++>=250) {
	king_mS=0;
	king_phase|=1;
      }
    }
  }
}

U0 BurnTrees()
{
  I64 i;
  for (i=0;i<TREES_NUM;i++)
    if (Sqr(king_x-trees[i].x)+Sqr(king_y-trees[i].y)<10*10)
      trees[i].fire_frame_idx=1;
}

U0 SongTask(I64)
{
  Fs->task_end_cb=&SndTaskEndCB;
  MusicSettingsRst;
  music.tempo= 2.480;
  music.stacatto_factor= 0.902;
  while (TRUE) {
    Play("5eCCCDCC4qGeAGAB5qCC");
    Play("5eCCCDCC4qGeAGAB5qCC");
    Play("5eGFEDEDqC4eAGAB5qCC");
  }
}
 
U0 Wenceslas()
{
  I64 sc;

  MenuPush(
	"File {"
	"  Abort(,CH_SHIFT_ESC);"
	"  Exit(,CH_ESC);"
	"}"
	"Play {"
	"  Restart(,'\n');"
	"  BurnTree(,CH_SPACE);"
	"  Up(,,SC_CURSOR_UP);"
	"  Down(,,SC_CURSOR_DOWN);"
	"  Left(,,SC_CURSOR_LEFT);"
	"  Right(,,SC_CURSOR_RIGHT);"
	"}"
	);

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  Fs->song_task=Spawn(&SongTask,NULL,"Song",,Fs);

  PopUpOk("
Good King Wenceslas looked out
On the feast of Stephen
When the snow lay round about
Deep and crisp and even
Brightly shone the moon that night
Though the frost was cruel
When a poor man came in sight
Gath'ring winter fuel

\"Hither, page, and stand by me
If thou know'st it, telling
Yonder peasant, who is he?
Where and what his dwelling?\"
\"Sire, he lives a good league hence
Underneath the mountain
Right against the forest fence
By Saint Agnes' fountain.\"

\"Bring me flesh and bring me wine
Bring me pine logs hither
Thou and I will see him dine
When we bear him thither.\"
Page and monarch forth they went
Forth they went together
Through the rude wind's wild lament
And the bitter weather

");
  PopUpOk("
\"Sire, the night is darker now
And the wind blows stronger
Fails my heart, I know not how,
I can go no longer.\"
\"Mark my footsteps, my good page
Tread thou in them boldly
Thou shalt find the winter's rage
Freeze thy blood less coldly.\"

In his master's steps he trod
Where the snow lay dinted
Heat was in the very sod
Which the Saint had printed
Therefore, Christian men, be sure
Wealth or rank possessing
Ye who now will bless the poor
Shall yourselves find blessing

");
  PopUpOk("
$PURPLE$$TX+CX,\"Winceslas Game\"$$FG$

Start fires by pressing $GREEN$<SPACE>$FG$
on trees. (Yule logs)

Lead peasants to fires.
");

  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  Fs->draw_it=&DrawIt;
  try {
    while (TRUE) {
      switch (GetKey(&sc)) {
	case '\n':
	  CleanUp;
	  Init;
	  break;
	case CH_ESC:
	case CH_SHIFT_ESC:
	  goto ws_done;
	case CH_SPACE:
	  BurnTrees;
	  break;
	case 0:
	  switch (sc.u8[0]) {
	    start:
	      case SC_CURSOR_RIGHT:
		if (king_x+KING_STEP<Fs->pix_width-BORDER)
		  king_x+=KING_STEP;
		king_theta=0;
		break;
	      case SC_CURSOR_LEFT:
		if (king_x-KING_STEP>=BORDER)
		  king_x-=KING_STEP;
		king_theta=pi;
		break;
	      case SC_CURSOR_DOWN:
		if (king_y+KING_STEP<Fs->pix_height-BORDER)
		  king_y+=KING_STEP;
		king_theta=-pi/2;
		break;
	      case SC_CURSOR_UP:
		if (king_y-KING_STEP>=BORDER)
		  king_y-=KING_STEP;
		king_theta=pi/2;
		break;
	    end:
	      king_mS=0;
	      king_phase=(king_phase+2)&2;
	      StepNew(Fs,king_x,king_y,king_theta,king_phase&2,TRUE);
	      king_timeout=tS+0.5;
	      break;
	  }
      }
    }
ws_done:
  } catch
    PutExcept;
  SettingsPop;
  CleanUp;
  MenuPop;
  RegWrite("TempleOS/Wenceslas","F64 best_score=%5.4f;\n",best_score);
}

Wenceslas;
