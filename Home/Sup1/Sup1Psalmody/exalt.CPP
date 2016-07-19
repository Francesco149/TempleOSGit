//5 has graphics

F64 t0=Beat;

class Obj
{
  F64 x,y,dx,dy;
};

U0 ObjAnimate(Obj *tempo,F64 dt)
{
  tempo->x+=dt*tempo->dx;
  tempo->y+=dt*tempo->dy;
  tempo->dy+=2*dt;
}

#define NUM_SPARKS	128
class FireWork
{
  FireWork *next,*last;
  F64 t0,t_burst,t_death;
  CColorROPU32	color,pad;
  Obj rocket,sparks[NUM_SPARKS]
} fireworks;

CColorROPU32 colors[5]={BLUE,RED,PURPLE,GREEN,YELLOW};

U0 FireWorkNew(F64 t)
{
  F64 theta,d;
  FireWork *res=CAlloc(sizeof(FireWork));
  res->t0=t;
  res->t_burst=res->t0+4;
  res->t_death=res->t_burst+4;
  res->color=colors[Rand*5];
  res->rocket.x=344;
  res->rocket.y=70;
  theta=0.5*(Rand-0.5);
  d=2*Rand+17.0;
  res->rocket.dx=d*Sin(theta);
  res->rocket.dy=-d*Cos(theta);
  QueIns(res,fireworks.last);
}

U0 FireWorkDel(FireWork *tempf)
{
  QueRem(tempf);
  Free(tempf);
}

U0 FireWorkBurst(FireWork *tempf)
{
  I64 i;
  F64 theta,d;
  for (i=0,theta=0;i<NUM_SPARKS;i++,theta+=2*pi/NUM_SPARKS) {
    tempf->sparks[i].x=tempf->rocket.x;
    tempf->sparks[i].y=tempf->rocket.y;
    d=Rand+1.5;
    d=d*d/4;
    tempf->sparks[i].dx=4*Cos(theta)*d;
    tempf->sparks[i].dy=4*Sin(theta)*d;
  }
}

U0 FireWorkAnimate(FireWork *tempf,F64 t,F64 dt)
{
  I64 i;
  if (t<tempf->t_burst)
    ObjAnimate(&tempf->rocket,dt);
  else if (t-dt<=tempf->t_burst) {
    ObjAnimate(&tempf->rocket,dt);
    FireWorkBurst(tempf);
  } else if (t<tempf->t_death)
    for (i=0;i<NUM_SPARKS;i++)
      ObjAnimate(&tempf->sparks[i],dt);
  else
    FireWorkDel(tempf);
}

U0 FireWorkDraw(CDC *dc,F64 t,FireWork *tempf)
{
  I64 i;
  if (t<tempf->t_burst) {
    dc->color=YELLOW;
    GrPlot(dc,tempf->rocket.x,tempf->rocket.y);
  } else {
    dc->color=tempf->color;
    for (i=0;i<NUM_SPARKS;i++)
      GrPlot(dc,tempf->sparks[i].x,tempf->sparks[i].y);
  }
}

U0 DrawIt(CTask *,CDC *dc)
{
  static F64 last_t=Beat-t0;
  F64 t=Beat-t0,d;
  FireWork *tempf;
  for (d=20;d<32;d+=1)
    if (t>d && last_t<=d)
      FireWorkNew(t);
  tempf=fireworks.next;
  while (tempf!=&fireworks) {
    FireWorkDraw(dc,t,tempf);
    FireWorkAnimate(tempf,t,t-last_t);
    tempf=tempf->next;
  }
  last_t=t;
}

U0 Song()
{
  SettingsPush; //See [C:/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Fs->text_attr=BLACK<<4+WHITE;
  Fs->draw_it=&DrawIt;
  Fs->task_end_cb=&SndTaskEndCB;

  MusicSettingsRst;
  music.tempo=2.5;
  try {
    while (!ScanKey) {
      t0=Beat;
      QueInit(&fireworks);
      DocClear;
      "$BG,BLACK$$WHITE$";
      Play("3eEDsBABA2qG3etGECFDBECFCBReBE",
	    "Come \0and \0sing \0 \0a\0 \0long.  \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0\n\0 \0 \0 \0");
      Play("EDsBABA2qG3etGECFDBECFCBReBE",
	    "Of\0fer \0praise \0 \0to \0 \0God.  \0 \0 \0"
	    " \0 \0 \0 \0 \0 \0 \0 \0\n\0 \0 \0 \0");
      Play("qBeDF2qG3eFRqBetFFGeACDF",
	    "Up \0in \0His \0hea\0ven \0 \0He \0is \0 \0"
	    " \0glor\0i\0ous.\n\0 \0");
      Play("qBeDF2qG3eFRqBetFFGeACDF",
	    "Break \0in\0to \0song.  \0 \0 \0Strike-\0up \0 \0 \0"
	    "the \0chor\0us.\n\0 \0");
      QueDel(&fireworks);
    }
  } catch
    PutExcept;
  SettingsPop;
}

Song;
