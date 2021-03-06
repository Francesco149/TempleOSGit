Bool chged;

#define DAWG_SETTINGS_SPACING 16
#define DAWG_SETTINGS_RANGE   60
#define DAWG_SETTINGS_BORDER  2

class DawgSettingsState
{
  I64 cnt,n2_amp,color_scale;
} glbl_state;

U0 DrawDawgSettingsCtrl(CDC *dc,CCtrl *c)
{
  DawgSettingsState *s=c->state;

  dc->color=LTRED;
  GrRect(dc, c->left,c->top,DAWG_SETTINGS_SPACING*4+3,
	DAWG_SETTINGS_SPACING*2+DAWG_SETTINGS_RANGE);
  dc->color=BLUE;
  GrRect(dc, c->left+DAWG_SETTINGS_BORDER,c->top+DAWG_SETTINGS_BORDER,
	DAWG_SETTINGS_SPACING*4+3-2*DAWG_SETTINGS_BORDER,
	DAWG_SETTINGS_SPACING*2+DAWG_SETTINGS_RANGE-2*DAWG_SETTINGS_BORDER);
  dc->color=BLACK;
  GrLine(dc,c->left+DAWG_SETTINGS_SPACING,c->top+DAWG_SETTINGS_SPACING,
	c->left+DAWG_SETTINGS_SPACING,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1);
  GrLine(dc,c->left+2*DAWG_SETTINGS_SPACING+1,c->top+DAWG_SETTINGS_SPACING,
	c->left+2*DAWG_SETTINGS_SPACING+1,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1);
  GrLine(dc,c->left+3*DAWG_SETTINGS_SPACING+1,c->top+DAWG_SETTINGS_SPACING,
	c->left+3*DAWG_SETTINGS_SPACING+1,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1);

  dc->color=LTRED;
  GrPrint(dc,c->left+DAWG_SETTINGS_SPACING-FONT_WIDTH/2,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE+3,
	"%d",s->cnt*10/DAWG_SETTINGS_RANGE);
  GrPrint(dc,c->left+2*DAWG_SETTINGS_SPACING+1-FONT_WIDTH/2,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE+3,
	"%d",s->n2_amp*10/DAWG_SETTINGS_RANGE);
  GrPrint(dc,c->left+3*DAWG_SETTINGS_SPACING+2-FONT_WIDTH/2,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE+3,
	"%d",s->color_scale*10/DAWG_SETTINGS_RANGE);
  GrRect(dc,c->left+DAWG_SETTINGS_SPACING-3,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1-s->cnt-2 ,7,5);
  GrRect(dc,c->left+2*DAWG_SETTINGS_SPACING+1-3,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1-s->n2_amp-2,7,5);
  GrRect(dc,c->left+3*DAWG_SETTINGS_SPACING+2-3,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1-s->color_scale-2,
	7,5);
  dc->color=YELLOW;
  GrRect(dc,c->left+DAWG_SETTINGS_SPACING-2,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1-s->cnt-1 ,5,3);
  GrRect(dc,c->left+2*DAWG_SETTINGS_SPACING+1-2,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1-s->n2_amp-1,5,3);
  GrRect(dc,c->left+3*DAWG_SETTINGS_SPACING+2-2,
	c->top+DAWG_SETTINGS_SPACING+DAWG_SETTINGS_RANGE-1-s->color_scale-1,
	5,3);
}

U0 UpdateDerivedDawgSettingsCtrl(CCtrl *c)
{
  DawgSettingsState *s=c->state;
  c->left=c->win_task->pix_width-(DAWG_SETTINGS_SPACING*4+3);
  c->right=c->left+DAWG_SETTINGS_SPACING*4+3;
  c->top=c->win_task->pix_height-(DAWG_SETTINGS_SPACING*2+DAWG_SETTINGS_RANGE);
  c->bottom=c->top+DAWG_SETTINGS_SPACING*2+DAWG_SETTINGS_RANGE;
  s->cnt =ClampI64(s->cnt,0,DAWG_SETTINGS_RANGE-1);
  s->n2_amp=ClampI64(s->n2_amp,0,DAWG_SETTINGS_RANGE-1);
  s->color_scale=ClampI64(s->color_scale,0,DAWG_SETTINGS_RANGE-1);
}

U0 LeftClickDawgSettings(CCtrl *c,I64 x,I64 y,Bool down)
{
  DawgSettingsState *s=c->state;
  if (x<c->left+(c->right-c->left)/3)
    s->cnt=DAWG_SETTINGS_RANGE-1-(y-(c->top+DAWG_SETTINGS_SPACING));
  else if (x<c->left+2*(c->right-c->left)/3)
    s->n2_amp=DAWG_SETTINGS_RANGE-1-(y-(c->top+DAWG_SETTINGS_SPACING));
  else
    s->color_scale=DAWG_SETTINGS_RANGE-1-(y-(c->top+DAWG_SETTINGS_SPACING));
  if (c->update_derived_vals)
    (*c->update_derived_vals)(c);
  if (!down) chged=TRUE;
}

CCtrl *DawgSettingsNew()
{
  CCtrl *c=CAlloc(sizeof(CCtrl));
  c->win_task=Fs;
  c->flags=CTRLF_SHOW|CTRLF_CAPTURE_LEFT_MS;
  c->type=CTRLT_GENERIC;
//  c->state=CAlloc(sizeof(DawgSettingsState));
  c->state=&glbl_state;
  glbl_state.cnt=DAWG_SETTINGS_RANGE/2;
  glbl_state.n2_amp=DAWG_SETTINGS_RANGE/2;
  glbl_state.color_scale=DAWG_SETTINGS_RANGE/2;
  c->draw_it=&DrawDawgSettingsCtrl;
  c->left_click=&LeftClickDawgSettings;
  c->update_derived_vals=&UpdateDerivedDawgSettingsCtrl;
  QueIns(c,Fs->last_ctrl);
  TaskDerivedValsUpdate;
  return c;
}

U0 DawgSettingsDel(CCtrl *c)
{
  QueRem(c);
//  Free(c->state);
  Free(c);
}

//This program does a fractel-like thing with
//int arithmetic.
class Simplex //This is something I made-up.  It's not standard math.
{
  I64 x,y;
};

I64 Norm(Simplex *n1)
{
  return AbsI64(n1->x)+AbsI64(n1->y);
}

Simplex *CopySimplex(Simplex *dst, Simplex *src)
{
  MemCpy(dst,src,sizeof(Simplex));
  return dst;
}

Simplex *Mix(Simplex *res,Simplex *n1,Simplex *n2)
{ //This is something I made-up.  It's not standard math.
  res->x=n1->x*n2->x;
  res->y=n1->y*n2->y;
  return res;
}

Simplex *Mangle(Simplex *res,Simplex *n1,Simplex *n2)
{ //This is something I made-up.  It's not standard math.
  res->x=n1->x*n2->y;
  res->y=n1->y*n2->x;
  return res;
}

Simplex *Add(Simplex *res,Simplex *n1,Simplex *n2)
{
  res->x=n1->x+n2->x;
  res->y=n1->y+n2->y;
  return res;
}

Simplex *Sub(Simplex *res,Simplex *n1,Simplex *n2)
{
  res->x=n1->x-n2->x;
  res->y=n1->y-n2->y;
  return res;
}

U0 Bling()
{
  I64 ch;
  I64 x,y,cnt;
  I64 d;
  F64 color,color_scale;
  Simplex n1,n2,n3,n4,n5,n6,n7,n8;
  I64 timeout_jiffy;
  CCtrl *c=DawgSettingsNew;
  CDC *dc=DCAlias;
  F64 e=Exp(1);

  SettingsPush; //See [C:/Adam/TaskSettings.HC.Z,3] SettingsPush
  Fs->win_inhibit=WIG_TASK_DFT-WIF_SELF_FOCUS-WIF_SELF_BORDER-WIF_SELF_CTRLS;

  chged=FALSE;
  AutoComplete;
  WinBorder;
  WinMax;
  DocCursor;
  DocClear;
  while (TRUE) {
next_dotel:
    ch=0;
    cnt =2e9*(RandU32+0x1000)/U32_MAX
	  *((Exp(ToF64(glbl_state.cnt+10)/DAWG_SETTINGS_RANGE)-1)/e);
    n2.x=2e9*Sign(RandI32)*(RandU32+0x1000)/U32_MAX
	  *((Exp(ToF64(glbl_state.n2_amp)/DAWG_SETTINGS_RANGE)-1)/e);
    n2.y=2e9*Sign(RandI32)*(RandU32+0x1000)/U32_MAX
	  *((Exp(ToF64(glbl_state.n2_amp)/DAWG_SETTINGS_RANGE)-1)/e);
    n8.x=RandI32;
    n8.y=RandI32;
    color_scale=4.0*(glbl_state.color_scale+1.0)/DAWG_SETTINGS_RANGE+0.5;
    timeout_jiffy=cnts.jiffies+JIFFY_FREQ/10;
    for (y=0;y<GR_HEIGHT;y++) {
      if (y<20 && cnts.jiffies>timeout_jiffy)
	goto next_dotel;
      if (ScanKey(&ch))
	goto next_dotel;
      for (x=0;x<GR_WIDTH;x++) {
	if (y<20 && cnts.jiffies>timeout_jiffy)
	  goto next_dotel;

	n1.x=x;	n1.y=y;
	CopySimplex(&n3,&n1);

	color=0;
	d=Norm(&n3);

	//This doesn't make mathematical sense, but it makes nice ress.
	while (d.u32[0]<cnt && color<65536) {
	  if (y<20 && cnts.jiffies>timeout_jiffy)
	    goto next_dotel;
	  Mangle(&n7,Mix(&n4,&n3,&n1),Add(&n5,&n3,&n2));
	  color++;
	  d=Norm(Add(&n3,&n7,Mangle(&n6,&n3,&n8)));
	}
	color-=(cnt-d.u32[0])/cnt;
	dc->color=ROP_EQU+color=
	      gr_rainbow_10[ClampI64(color_scale*(Ln(color)),0,9)];
	GrPlot(dc,x,y);
      }
    }
    dc->color=TRANSPARENT;
    GrRect(dc,c->left,c->top,c->right-c->left,c->bottom-c->top);
    while (TRUE) {
      if (chged) {
	chged=FALSE;
	break;
      }
      if (ch=ScanChar) {
	if (ch==CH_SPACE||ch=='\n')
	  break;
	else
	  goto bling_done;
      }
      Refresh;
    }
  }
bling_done:
  SettingsPop;
  DCDel(dc);
  DCFill;
  DawgSettingsDel(c);
}

Bling;	//Start when included

U0 DoBling()
{
  CTask *task=User;
  XTalkWait(task,"#include \"::/TempleOS/Demo/Graphics/Bling\";\n");
  Sleep(500);
  PostMsgWait(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  PostMsgWait(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  DeathWait(&task,TRUE);
}

