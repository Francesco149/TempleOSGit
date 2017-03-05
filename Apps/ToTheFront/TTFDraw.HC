
/* <1> <1> (image) */

U0 UnitDraw(CDC *dc,Unit *tmpu,I64 x,I64 y,F64 f)
{
  x+=x0; y+=y0;
  if (tmpu->infantry)
    Sprite3(dc,x,y,0,tmpu->img);
  else
    Sprite3ZB(dc,x,y,0,tmpu->img,f);
  if (phase&~1==PHASE_INDIRECT && tmpu->indirect_fire &&
	!tmpu->fired && tmpu->player==cur_player) {
    dc->pen_width=2;
    if (cur_player)
      dc->color=PURPLE;
    else
      dc->color=CYAN;
    GrCircle3(dc,x,y,0,20*Saw(4*tS,2.0));
    GrCircle3(dc,x,y,0,20*Saw(4*tS+1.0,2.0));
    dc->pen_width=1;
  }
}

U0 UnitsDraw(CDC *dc)
{
  I64 i,j;
  F64 x,y;
  Unit *tmpu;
  for (j=0;j<2;j++) {
    for (i=0;i<UNITS_NUM;i++) {
      tmpu=&units[j][i];
      if (tmpu==target_unit) {
	if (target_hit)
	  dc->color=RED;
	else
	  dc->color=GREEN;
      } else {
	if (j)
	  dc->color=LTPURPLE;
	else
	  dc->color=LTCYAN;
      }
      if (tmpu->life>0 && Bt(&tmpu->vis[view_player],0) &&
	    tmpu!=moving_unit) {
	RowCol2XY(&x,&y,tmpu->row,tmpu->col);
	if (phase&~1==PHASE_MOVE && tmpu->remaining_movement ||
	      (phase&~1==PHASE_INDIRECT&& tmpu->indirect_fire||
	      phase&~1==PHASE_DIRECT&&!tmpu->indirect_fire) && !tmpu->fired ||
	      Blink(5))
	  UnitDraw(dc,tmpu,x,y,tmpu->facing*60.0*pi/180.0);
      }
    }
  }
}

U0 DrawIt(CTask *task,CDC *dc)
{
  F64 x,y;
  I64 i,j,r,c;
  U8 buf[STR_LEN];
  IndirectOrders *tmpi;

  //Erase text beneath.
  dc->color=WHITE;
  GrRect(dc,-task->scroll_x,-task->scroll_y,dc->width,dc->height);

  map_dc->flags|=DCF_NO_TRANSPARENTS;
  GrBlot(dc,x0,y0,map_dc);

  i=ms.pos.x-task->pix_left-task->scroll_x;
  j=ms.pos.y-task->pix_top -task->scroll_y;
  if (CursorInWin(task,i,j)) {
    i-=x0; j-=y0;
    CursorUpdate(task,i,j);
  }
  RowCol2XY(&x,&y,cursor_row,cursor_col);

  //Roads require multiple cursor fills
  dc->color=YELLOW;
  c=terrain[cursor_row][cursor_col];
  for (i=-(HEX_SIDE+DCOS)/2;i<=(HEX_SIDE+DCOS)/2;i++) {
    if (GrPeek(dc,x+i+x0,y+y0)==c)
      GrFloodFill(dc,x+i+x0,y+y0);
    for (j=-HEX_SIDE/2;j<=HEX_SIDE/2;j++)
      if (GrPeek(dc,x+j+x0,y+i+y0)==c)
	GrFloodFill(dc,x+j+x0,y+i+y0);
  }

  UnitsDraw(dc);
  if (firing) {
    dc->color=BLACK;
    GrCircle(dc,fire_x+x0,fire_y+y0,2);
  }
  if (indirect_explosion) {
    for (i=0;i<7;i++) {
      if (indirect_row&1)
	c=indirect_col+col_offsets_odd[i];
      else
	c=indirect_col+col_offsets_even[i];
      r=indirect_row+row_offsets[i];
      if (0<=r<map_rows && 0<=c<map_cols) {
	RowCol2XY(&x,&y,r,c);
	for (j=0;j<10;j++)  {
	  if (j&1)
	    dc->color=LTRED;
	  else
	    dc->color=YELLOW;
	  GrCircle(dc,x+RandU16%HEX_SIDE-HEX_SIDE/2+x0,
		y+RandU16%HEX_SIDE-HEX_SIDE/2+y0,2);
	}
      }
    }
  }
  if (moving_unit && moving_unit->vis[view_player]) {
    dc->color=YELLOW;
    UnitDraw(dc,moving_unit,move_x,move_y,move_facing);
  }
  ProgressBarsRst;
  if (moving_unit) {
    if (ms.pos.y<GR_HEIGHT/2) {
      progress4_max=moving_unit->movement;
      progress4=moving_unit->remaining_movement;
    } else {
      progress1_max=moving_unit->movement;
      progress1=moving_unit->remaining_movement;
    }
  }
  if (fire_radius) {
    dc->color=YELLOW;
    GrCircle(dc,fire_radius_x+x0,fire_radius_y+y0,fire_radius-1);
    GrCircle(dc,fire_radius_x+x0,fire_radius_y+y0,fire_radius+1);
    dc->color=RED;
    GrCircle(dc,fire_radius_x+x0,fire_radius_y+y0,fire_radius);
  }
  if (Blink(10)) {
    tmpi=indirect_head.next;
    while (tmpi!=&indirect_head) {
      if (tmpi->attacker->player==view_player) {
	RowCol2XY(&x,&y,tmpi->row,tmpi->col);
	Sprite3(dc,x+x0,y+y0,0,__BIN_1);
      }
      tmpi=tmpi->next;
    }
  }
  if (Bt(kbd.down_bitmap,SC_SHIFT)) {
    if (show_vis_row!=cursor_row || show_vis_col!=cursor_col) {
      show_vis_row=cursor_row;
      show_vis_col=cursor_col;
      VisRecalcMap(show_vis_row,show_vis_col);
    }

    dc->color=LTGRAY;
    for (j=0;j<map_rows;j++)
      for (i=0;i<map_cols;i++)
	if (!vis_map[j][i]) {
	  RowCol2XY(&x,&y,j,i);
	  GrLine(dc,x-6+x0,y-6+y0,x+6+x0,y+6+y0);
	  GrLine(dc,x+6+x0,y-6+y0,x-6+x0,y+6+y0);
	  GrLine(dc,x+x0,y-6+y0,x+x0,y+6+y0);
	  GrLine(dc,x+6+x0,y+y0,x-6+x0,y+y0);
	}
  }
  if (i=StrLen(msg_buf)*FONT_WIDTH) {
    dc->color=BLACK;
    GrRect(dc,(task->pix_width-i)>>1-10-task->scroll_x,
	  (task->pix_height-FONT_HEIGHT)>>1-10-task->scroll_y,
	  i+20,FONT_HEIGHT+20);

    dc->color=YELLOW;
    GrRect(dc,(task->pix_width-i)>>1-7-task->scroll_x,
	  (task->pix_height-FONT_HEIGHT)>>1-7-task->scroll_y,
	  i+14,FONT_HEIGHT+14);

    dc->color=RED;
    GrPrint(dc,(task->pix_width-i)>>1-task->scroll_x,
	  (task->pix_height-FONT_HEIGHT)>>1-task->scroll_y,
	  msg_buf);
    if (msg_off_timeout) {
      if (msg_off_timeout-cnts.jiffies<3*JIFFY_FREQ/2*animation_delay)
	Snd;
      if (cnts.jiffies>msg_off_timeout)
	*msg_buf=0;
    }
  }

  dc->color=WHITE;
  GrRect(dc,-task->scroll_x,-task->scroll_y,
	(13+7+10+10)*FONT_WIDTH,FONT_HEIGHT);

  if (phase&~1==PHASE_INDIRECT)
    StrPrint(buf,"Turn:%2d Artillery",turn);
  else if (phase&~1==PHASE_MOVE)
    StrPrint(buf,"Turn:%2d Move",turn);
  else
    StrPrint(buf,"Turn:%2d Fire",turn);
  dc->color=BLACK;
  GrPrint(dc,-task->scroll_x,-task->scroll_y,buf);

  StrPrint(buf,"%3d Units",alive_cnt[0]);
  if (cur_player || Blink)
    dc->color=CYAN;
  else
    dc->color=LTCYAN;
  GrPrint(dc,-task->scroll_x+(13+7)*FONT_WIDTH,-task->scroll_y,buf);

  StrPrint(buf,"%3d Units",alive_cnt[1]);
  if (!cur_player || Blink)
    dc->color=PURPLE;
  else
    dc->color=LTPURPLE;
  GrPrint(dc,-task->scroll_x+(13+7+10)*FONT_WIDTH,-task->scroll_y,buf);
}
