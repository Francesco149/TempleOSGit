/*
This uses [C:/Demo/Lectures/FixedPoint.CPP,1] fixed-point arithmetic
because it used to be faster than floating
point.	See [C:/Demo/Lectures/FixedPoint.CPP,1].

The decimal place is between
bits 31 and 32.
*/


#define NUM_BALLS	64
#define BALL_RADIUS	5

I64 ball_x[NUM_BALLS],ball_y[NUM_BALLS],
   ball_velocity_x[NUM_BALLS],ball_velocity_y[NUM_BALLS];

U0 DrawIt(CTask *,CDC *dc)
{
  I64 i;
  dc->color=RED;
  for (i=0;i<NUM_BALLS;i++)
    GrCircle(dc,ball_x[i].i32[1],ball_y[i].i32[1],BALL_RADIUS);
}




/****



/*  <1> (image) */






Initial and final velocity vects
with normal and tangential components.
All masses are identical, so they
have been dropped from the equations.


Conservation of Momentum:

V1it+V2it=V1ft+V2ft

V1in+V2in=V1fn+V2fn



Conservation of Energy:

|V1i|2+|V2i|2=|V1f|2+|V2f|2


****/

U0 AnimateTask(I64)
{
  CTask *task=Fs->parent_task;
  I64 i,j,h,v,distdist,
    dia=(2*BALL_RADIUS)<<32,
    diadia=SqrI64(2*BALL_RADIUS)<<32,
    delta_x,delta_y,v_t1,v_n1,v_t2,v_n2;
  F64 dist;
  while (TRUE) {
    h=task->win_pixel_width;
    v=task->win_pixel_height;
    for (i=0;i<NUM_BALLS;i++) {
      ball_x[i]+=ball_velocity_x[i];
      ball_y[i]+=ball_velocity_y[i];
      if (ball_x[i]<BALL_RADIUS<<32) {
	ball_velocity_x[i]*=-1;
	ball_x[i]=BALL_RADIUS<<32;
      }
      if (ball_x[i]>=(h-BALL_RADIUS)<<32) {
	ball_velocity_x[i]*=-1;
	ball_x[i]=(h-BALL_RADIUS)<<32;
      }
      if (ball_y[i]<BALL_RADIUS<<32) {
	ball_velocity_y[i]*=-1;
	ball_y[i]=BALL_RADIUS<<32;
      }
      if (ball_y[i]>=(v-BALL_RADIUS)<<32) {
	ball_velocity_y[i]*=-1;
	ball_y[i]=(v-BALL_RADIUS)<<32;
      }
    }
    for (i=0;i<NUM_BALLS;i++) {
      for (j=i+1;j<NUM_BALLS;j++) {
	 delta_x=ball_x[i]-ball_x[j];
	 delta_y=ball_y[i]-ball_y[j];

	 //We shift 16 because multiplying
	 //two 32 shifted would yield 64 shifted
	 //and we want a 32 shifted result.
	 distdist=SqrI64(delta_x>>16)+SqrI64(delta_y>>16);

	 //We work with square instead of sqrt
	 //to avoid unnecessarily calculating
	 //square roots (They are slow.)
	 if (distdist && distdist<=diadia) {
	   dist=Sqrt(distdist); //shifted 16 bits
	   delta_x/=dist; //shifted 16
	   delta_y/=dist;

	   v_t1=(ball_velocity_x[i]>>16*delta_y-
		 ball_velocity_y[i]>>16*delta_x)>>16;
	   v_n1=(ball_velocity_x[i]>>16*delta_x+
		 ball_velocity_y[i]>>16*delta_y)>>16;
	   v_t2=(ball_velocity_x[j]>>16*delta_y-
		 ball_velocity_y[j]>>16*delta_x)>>16;
	   v_n2=(ball_velocity_x[j]>>16*delta_x+
		 ball_velocity_y[j]>>16*delta_y)>>16;

	   if (ball_velocity_x[i]>>16*ball_velocity_x[j]>>16+
	       ball_velocity_y[i]>>16*ball_velocity_y[j]>>16<=0) {
	     ball_velocity_x[i]= v_t1*delta_y-v_n1*delta_x;
	     ball_velocity_y[i]=-v_t1*delta_x-v_n1*delta_y;
	     ball_velocity_x[j]= v_t2*delta_y-v_n2*delta_x;
	     ball_velocity_y[j]=-v_t2*delta_x-v_n2*delta_y;
	   } else {
	     ball_velocity_x[i]= v_t1*delta_y+v_n2*delta_x;
	     ball_velocity_y[i]=-v_t1*delta_x+v_n2*delta_y;
	     ball_velocity_x[j]= v_t2*delta_y+v_n1*delta_x;
	     ball_velocity_y[j]=-v_t2*delta_x+v_n1*delta_y;
	   }

	   //Correct for overlap
	   dist=0x10000+(dia/0x10000-dist)/2;
	   ball_x[i]+=dist*delta_x;
	   ball_y[i]+=dist*delta_y;
	   ball_x[j]-=dist*delta_x;
	   ball_y[j]-=dist*delta_y;
	 }
      }
    }
    Sleep(1);
  }
}

U0 Init()
{
  I64 i;
  for (i=0;i<NUM_BALLS;i++) {
    ball_x[i]=(RandU16%(Fs->win_pixel_width-BALL_RADIUS*2)+BALL_RADIUS)<<32;
    ball_y[i]=(RandU16%(Fs->win_pixel_height-BALL_RADIUS*2)+BALL_RADIUS)<<32;
    ball_velocity_x[i]=RandI32/4;
    ball_velocity_y[i]=RandI32/4;
  }
}

U0 Collision()
{
  SettingsPush; //See [C:/TempleOS/Adam/TaskSettings.CPP.Z,3] SettingsPush
  Init;
  Fs->animate_task=Spawn(&AnimateTask,NULL,"Animate",,Fs);
  DocCursor;
  DocClear;
  Fs->draw_it=&DrawIt;
  GetChar;
  SettingsPop;
}

Collision;
