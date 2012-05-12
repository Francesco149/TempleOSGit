/*This is an alternative window manager.  It augments the existing one found 
in [::/LT/Adam/Window.APZ] ::/LT/Adam/Window.APZ.  It uses the real-time differential equation solver 
to repos windows as though they were repulsively charged particles.  I find 
this window manager to be a novelty, not desirable for actual use.  You're 
welcome to add it, or some variation to your start-up scripts if you like it.  
It's not fully debugged.

I had to cheat to make this work.  I added [C:/LT/OSMain/Adam1a.HPZ,3093] DISPLAYf_HAS_BEEN_RESIZED2 and 
modified [::/LT/Adam/Window.APZ] ::/LT/Adam/Window.APZ.

The differential equation stuff was overkill and made it overly complicated.  
Oh, well.
*/

U0 MyDerivative(Ode *ode,F64 t,Order2D3 *state,Order2D3 *DstateDt)
{
  U64 i,j,task_cnt=ode->n/6;
  F64 f,d,dd,dx,dy;
  nounusedwarn t;
  if (!task_cnt) return;

  MemSet(DstateDt,0,sizeof(Order2D3)*task_cnt);
  for (i=0;i<task_cnt;i++) {
    DstateDt[i].x=state[i].DxDt;
    DstateDt[i].y=state[i].DyDt;
    for (j=0;j<task_cnt;j++) {
      if (i!=j) {
	dx=state[i].x-state[j].x;
	dy=state[i].y-state[j].y;
	dd=Sqr(dx)+Sqr(dy)+1.0;
	d=Sqrt(dd);

	dx/=d;
	dy/=d;

	f=1000000.0/dd;

	DstateDt[i].DxDt+=f*dx;
	DstateDt[i].DyDt+=f*dy;
      }
    }
  }
  for (i=0;i<task_cnt;i++) {
    DstateDt[i].DxDt-=state[i].DxDt;
    DstateDt[i].DyDt-=state[i].DyDt;
  }
}

U0 WinMngTask(U64 dummy=0)
{ //Preempt() is off after Spawn()
  nounusedwarn dummy;
  BoolI8 same=FALSE;
  I64 i,j,w,h,x,y,cnt,task_cnt=0;
  Order2D3 *tempo;
  TaskStruct *task,**task_map=NULL;
  Ode *ode=NULL;

  while (TRUE) {
restart:
    WinSync;
    while (Bt(&sys_semas[SYS_SEMA_REFRESH_IN_PROGRESS],0))
      Yield;
    cnt=0;
    task=Fs->next_task;
    while (task!=Fs) {
      if (Bt(&task->display_flags,DISPLAYf_SHOW))
	cnt++;
      task=task->next_task;
    }
    if (!cnt) {
      same=FALSE;
      goto restart;
    }

    if (same && cnt==task_cnt) {
      i=0;
      task=Fs->next_task;
      while (task!=Fs) {
	if (Bt(&task->display_flags,DISPLAYf_SHOW))
	  if (task_map[i++]!=task)
	    same=FALSE;
	task=task->next_task;
      }
    } else
      same=FALSE;
    if (!same) {
      task_cnt=cnt;
      if (task_map) {
	Free(task_map);
	RemQue(ode);
	OdeDel(ode);
      }
      task_map=MAlloc(sizeof(U0 *)*task_cnt);
      ode=OdeNew(task_cnt*6,0.1);
      ode->derivative=&MyDerivative;
      InsQue(ode,Fs->last_ode);

      i=0;
      task=Fs->next_task;
      while (task!=Fs) {
	if (Bt(&task->display_flags,DISPLAYf_SHOW)) {
	  tempo=ode->state(U8 *)+sizeof(Order2D3)*i;
	  task_map[i++]=task;
	  w=task->win_width-1;
	  h=task->win_height-1;
	  tempo->x=task->win_left+w>>1;
	  tempo->y=task->win_top+h>>1;
	  tempo->DxDt=0;
	  tempo->DyDt=0;
	}
	task=task->next_task;
      }
      same=TRUE;
    } else {
      for (i=0;i<task_cnt;i++) {
	tempo=ode->state(U8 *)+sizeof(Order2D3)*i;
	task=task_map[i];
	w=task->win_width-1;
	h=task->win_height-1;
	if (LBtr(&task->display_flags,DISPLAYf_HAS_BEEN_RESIZED2)) {
	  tempo->x=task->win_left+w>>1;
	  tempo->y=task->win_top+h>>1;
	  tempo->DxDt=0;
	  tempo->DyDt=0;
	} else {
	  x=task->win_left+w>>1;
	  y=task->win_top+h>>1;
	  if (tempo->x>x+2) tempo->x=x+2;
	  if (tempo->y>y+2) tempo->y=y+2;
	  if (tempo->x<x-2) tempo->x=x-2;
	  if (tempo->y<y-2) tempo->y=y-2;

	  j=Round(tempo->x-w>>1);
	  if (Bt(&task->display_flags,DISPLAYf_NO_BORDER))
	    j=LimitI64(j,0,TEXT_COLS-1-w);
	  else
	    j=LimitI64(j,1,TEXT_COLS-2-w);
	  SetWinHorz(j,j+w,task);

	  j=Round(tempo->y-h>>1);
	  if (Bt(&task->display_flags,DISPLAYf_NO_BORDER))
	    j=LimitI64(j,0,TEXT_ROWS-1-h);
	  else
	    j=LimitI64(j,1,TEXT_ROWS-2-h);
	  SetWinVert(j,j+h,task);

	  x=task->win_left+(task->win_width-1)>>1;
	  y=task->win_top+(task->win_height-1)>>1;
	  if (LBtr(&task->display_flags,DISPLAYf_HAS_BEEN_RESIZED2)) {
	    tempo->x=x;
	    tempo->y=y;
	  }
	  if (tempo->DxDt>8.0)	tempo->DxDt=8.0;
	  if (tempo->DyDt>8.0)	tempo->DyDt=8.0;
	  if (tempo->DxDt<-8.0) tempo->DxDt=-8.0;
	  if (tempo->DyDt<-8.0) tempo->DyDt=-8.0;
	}
      }
    }
  }
}

Spawn(&WinMngTask,NULL,"Window Manager 2");
