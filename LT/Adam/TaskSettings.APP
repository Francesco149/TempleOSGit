#help_index "Task/Settings"

public TaskSettings *SettingsPush(TaskStruct *task=NULL,U64 flags=0)
{ //This is typically used at the start of an application.
  //It saves many settings so they can be restored
  //at the end of the application with [C:/LT/Adam/TaskSettings.APZ,64] SettingsPop().

  TaskSettings *tempse;
  Ltf *l;
  if (!task) task=Fs;
  if (!TaskValidate(task)) return NULL;
  tempse=CAlloc(sizeof(TaskSettings),task);
  tempse->draw_it=task->draw_it;
  GetVGAPalette4(tempse->palette4);
  tempse->task_end_cb=task->task_end_cb;

  if (!(flags&TSF_SAME_SONG)) {
    if (tempse->song_task=task->song_task) {
      LBts(&task->song_task->task_flags,TASKf_SUSPENDED);
      Snd(0);
    }
    task->song_task=NULL;
  }

  if (tempse->animate_task=task->animate_task)
    LBts(&task->animate_task->task_flags,TASKf_SUSPENDED);
  task->animate_task=NULL;

  if (l=LtfCur(task)) {
    tempse->cursor=!Bt(&l->flags,LTFf_HIDE_CURSOR);
    tempse->scroll=!Bt(&l->flags,LTFf_NO_SCROLL_BARS);
    tempse->ltf_cur_attr=l->start_text_attr;
  }
  if (l=LtfAux(task))
    tempse->ltf_aux_attr=l->start_text_attr;

  tempse->left=task->win_left;
  tempse->right=task->win_right;
  tempse->top=task->win_top;
  tempse->bottom=task->win_bottom;

  tempse->scroll_x=task->win_scroll_x;
  tempse->scroll_y=task->win_scroll_y;
  tempse->scroll_z=task->win_scroll_z;

  tempse->win_inhibit=task->win_inhibit;
  MemCpy(&tempse->snap,&task->snap,sizeof(SnapStruct));

  tempse->win_max_refresh=task->win_max_refresh;
  tempse->text_attr=task->text_attr;
  tempse->border_attr=task->border_attr;
  tempse->border=!Bt(&task->display_flags,DISPLAYf_NO_BORDER);
  tempse->preempt=Bt(&task->task_flags,TASKf_PREEMPT);
  if (TaskValidate(ws_task))
    tempse->wordstat=TRUE;
  else
    tempse->wordstat=FALSE;

  tempse->next=task->next_settings;
  task->next_settings=tempse;
  return tempse;
}

public U0 SettingsPop(TaskStruct *task=NULL,U64 flags=0)
{
  TaskSettings *tempse;
  Ltf *l;
  BoolI8 old_preempt=Preempt(OFF);
  if (!task) task=Fs;
  if (!TaskValidate(task)) {
    Preempt(old_preempt);
    return;
  }
  if (tempse=task->next_settings) {
    task->next_settings=tempse->next;
    task->draw_it=tempse->draw_it;
    SetVGAPalette4(tempse->palette4);
    task->task_end_cb=tempse->task_end_cb;

    if (task->animate_task)
      Kill(task->animate_task);
    if (task->animate_task=tempse->animate_task)
      LBtr(&task->animate_task->task_flags,TASKf_SUSPENDED);

    if (!(flags&TSF_SAME_SONG)) {
      if (task->song_task)
	Kill(task->song_task);
      if (task->song_task=tempse->song_task)
	LBtr(&task->song_task->task_flags,TASKf_SUSPENDED);
    }

    if (l=LtfCur(task)) {
      LBEqu(&l->flags,LTFf_HIDE_CURSOR,!tempse->cursor);
      LBEqu(&l->flags,LTFf_NO_SCROLL_BARS,!tempse->scroll);
      l->start_text_attr=tempse->ltf_cur_attr;
    }
    if (l=LtfAux(task))
      l->start_text_attr=tempse->ltf_aux_attr;

    WinBorder(tempse->border,task);
    SetWinHorz(tempse->left,tempse->right,task);
    SetWinVert(tempse->top,tempse->bottom,task);
    task->win_scroll_x=tempse->scroll_x;
    task->win_scroll_y=tempse->scroll_y;
    task->win_scroll_z=tempse->scroll_z;
    MemCpy(&task->snap,&tempse->snap,sizeof(SnapStruct));
    task->win_inhibit=tempse->win_inhibit;
    task->win_max_refresh=tempse->win_max_refresh;
    task->text_attr=tempse->text_attr;
    task->border_attr=tempse->border_attr;
    WordStat(tempse->wordstat);
    Snd(0);
    Free(tempse);
    WinSync;
    Preempt(old_preempt);
    LBEqu(&task->task_flags,TASKf_PREEMPT,tempse->preempt);
  } else
    Preempt(old_preempt);
}

#help_index ""
