class CtrlButtonState
{
  U8 *state_texts;
  I32 *state_colors;
  U8 state,num_states;
} snap_button,run_button,mode_button;


#define CTRL_BUTTON_WIDTH	80
#define CTRL_BUTTON_HEIGHT	(3*FONT_HEIGHT)

U0 DrawCtrlButton(GrDC *dc,Ctrl *c)
{
  CtrlButtonState *s=c->state;
  I64 l;
  U8 *st;

  dc->color=BLACK;
  GrRect(dc,c->left,c->top,c->right-c->left+1,c->bottom-c->top+1);
  st=ListEntryPoint(s->state,s->state_texts);
  dc->color=s->state_colors[s->state];
  l=StrLen(st);
  GrRect(dc,c->left+2,c->top+2,c->right-c->left+1-4,c->bottom-c->top+1-4);
  dc->color=WHITE;
  GrPutS(dc,c->left+(c->left+c->right+1-l*FONT_WIDTH)>>1,c->top+FONT_HEIGHT,st);
}

U0 LeftClickCtrlButton(Ctrl *c,I64 x,I64 y,BoolI8 down)
{
  nounusedwarn x,y;
  CtrlButtonState *s=c->state;
  if (down) {
    s->state++;
    if (s->state==s->num_states)
      s->state=0;
  }
}

Ctrl *CtrlButtonNew(I64 x,I64 y,I64 num_states,U8 *state_texts,I32 state_colors[],CtrlButtonState *s)
{
  BoolI8 old_preempt;
  Ctrl *c=CAlloc(sizeof(Ctrl));
  MemSet(s,0,sizeof(CtrlButtonState));
  s->num_states=num_states;
  s->state_texts=state_texts;
  s->state_colors=state_colors;
  c->win_task=Fs;
  c->flags=CTRLF_SHOW;
  c->type=CTRLT_GENERIC;
  c->state=s;
  c->draw_it=&DrawCtrlButton;
  c->left_click=&LeftClickCtrlButton;
  c->left=x;
  c->top=y;
  c->right=c->left+CTRL_BUTTON_WIDTH-1;
  c->bottom=c->top+CTRL_BUTTON_HEIGHT-1;
  old_preempt=Preempt(OFF);
  InsQue(c,Fs->last_ctrl);
  Preempt(old_preempt);
  return c;
}

U0 CtrlButtonDel(Ctrl *c)
{
  BoolI8 old_preempt=Preempt(OFF);
  RemQue(c);
  Preempt(old_preempt);
  Free(c);
}

