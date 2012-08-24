I64 ts_num;
U0 TSStart()
{
  ts_num=0;
}
U0 TS()
{
  progress4=ts_num++;
  progress4_max=152;
}
U0 TSEnd()
{
  if (ts_num!=progress4_max)
    "$FG,RED$Progress4:%d$FG$\n",progress4;
  else
    "$FG,GREEN$Test Suite Completed$FG$\n";
  ProgressBarsRst;
}

U0 DoGrSprite1()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/GrSprite1.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoGrSprite2()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/GrSprite2.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoGrSprite3()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/GrSprite3.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoGrSprite4()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/GrSprite4.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoEdGrSprite()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/EdGrSprite.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoExtents()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Extents.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}


U0 DoDotNet()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/DotNet.CPZ\";\n");
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoLowPassFilter()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/LowPassFilter.CPZ\";\n");
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoMathAudioDemo()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/MathAudioDemo.CPZ\";\n");
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);

  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);

  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoLines()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Lines.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoBounce()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/Graphics/Bounce.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoMsgLoop()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MsgLoop.CPZ\";\n");
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoASCIIOrgan()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Snd/ASCIIOrgan.CPZ\";\n");
  Sleep(100);
  SendMsg(task,MSG_KEY_DOWN_UP,'A',0);
  Sleep(200);
  SendMsg(task,MSG_KEY_DOWN_UP,'B',0);
  Sleep(200);
  SendMsg(task,MSG_KEY_DOWN_UP,'C',0);
  Sleep(200);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoDoodle()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Doodle.CPZ\";\n");
  SendMsg(task,MSG_IP_L_DOWN,10,10);
  SendMsg(task,MSG_IP_L_UP,100,200);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoSpeedLine()
{
  I64 i=PURPLE;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Speedline.CPZ\";\n");
  SendMsg(task,MSG_IP_L_DOWN,10,10);
  SendMsg(task,MSG_IP_L_UP,100,200);
  PostMsg(task,MSG_IP_R_DOWN_UP,0,0);
  while (!TaskValidate(task->popup_task))
    Yield;
  while (i--)
    SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  Sleep(100);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_ESC,0);
  TaskWaitIdle(task);
  SendMsg(task,MSG_IP_L_DOWN,50,10);
  SendMsg(task,MSG_IP_L_UP,150,200);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoKeyBitMap()
{
  I64 i_sc=A2ScanCode('i');
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/KeyBitMap.CPZ\" Exit;\n");
  Sleep(20);
  LBts(key_down_bitmap,i_sc);
  Sleep(500);
  LBtr(key_down_bitmap,i_sc);
}

U0 DoBlink()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Blink.CPZ\";\n");
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoGolf()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Golf.CPZ\";\n");
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoMountain()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/Graphics/Mountain.CPZ\";\n");
  Sleep(1500);
  Kill(task);
}

U0 DoCoach()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Coach.CPZ\";\n");
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoDigits()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Digits.CPZ\";\n");
  Sleep(100);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(100);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(250);
  SendMsg(task,MSG_KEY_DOWN_UP,'1',0);
  Sleep(250);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoCPUPlot()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/CPUPlot.CPZ\";\n");
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoDither()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Dither.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoSymmetry()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Symmetry.CPZ\";\n");
  SendMsg(task,MSG_IP_R_DOWN,100,100);
  SendMsg(task,MSG_IP_R_UP,200,200);
  Sleep(100);
  SendMsg(task,MSG_IP_L_DOWN,10,10);
  SendMsg(task,MSG_IP_L_UP,100,200);
  Sleep(250);
  SendMsg(task,MSG_IP_L_DOWN,100,200);
  SendMsg(task,MSG_IP_L_UP,400,400);
  Sleep(250);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoBSpline()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/BSpline.CPZ\";\n");
  SendMsg(task,MSG_IP_L_DOWN_UP,50,50);
  SendMsg(task,MSG_IP_L_DOWN_UP,300,100);
  SendMsg(task,MSG_IP_L_DOWN_UP,150,300);
  SendMsg(task,MSG_IP_R_DOWN_UP,0,0);
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoBlot()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Blot.CPZ\";\n");
  Sleep(1800);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoCaptureScreen()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/CaptureScreen.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(2000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
  Del("HOME/SnapShot.LGZ");
}

/*
U0 DoPullDownMenu()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/PullDownMenu.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoStadium()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Stadium/Stadium\";\n");
  Sleep(50);
  SendMsg(task,MSG_IP_L_DOWN_UP,100,10);
  Sleep(300);
  SendMsg(task,MSG_IP_L_DOWN_UP,320,20);
  Sleep(300);
  SendMsg(task,MSG_IP_L_DOWN_UP,520,10);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoPalette()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Palette.CPZ\";\n");
  Sleep(400);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(400);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoHalogen()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Halogen.CPZ\";\n");
  SendMsg(task,MSG_KEY_DOWN,0,SC_CURSOR_UP);
  Sleep(1000);
  SendMsg(task,MSG_KEY_UP,0,SC_CURSOR_UP);
  SendMsg(task,MSG_KEY_DOWN,0,SC_CURSOR_RIGHT);
  Sleep(333);
  SendMsg(task,MSG_KEY_UP,0,SC_CURSOR_RIGHT);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoTheDead()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/TheDead.CPZ\";\n");
  for (i=0;i<15;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
    SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
    Sleep(50);
  }
  for (i=0;i<15;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
    SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
    Sleep(50);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoBomber()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Bomber.CPZ\";\n");
  for (i=0;i<7;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(100);
  }
  for (i=0;i<7;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
    Sleep(200);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoZoneOut()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/ZoneOut.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  for (i=0;i<15;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
    SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
    Sleep(100);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoVaroom()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/Varoom.CPZ\";\n");
  for (i=0;i<15;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
    Sleep(50);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_LEFT);
    Sleep(50);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoRocks()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/Rocks.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  SendMsg(task,MSG_KEY_DOWN,0,SC_CURSOR_RIGHT);
  Sleep(300);
  for (i=0;i<15;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
    Sleep(100);
  }
  SendMsg(task,MSG_KEY_UP,0,SC_CURSOR_RIGHT);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoWenceslas()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/Wenceslas.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(100);
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(100);
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(100);
  for (i=0;i<25;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(25);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoChess()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/Chess.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(100);
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(100);
  for (i=0;i<8;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(25);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
  Sleep(250);
  SendMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
  Sleep(250);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
  SendMsg(task,MSG_KEY_UP,CH_SPACE,0);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoChessNet()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/ChessNet.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_NEW_LINE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoRainDrops()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/RainDrops.CPZ\";\n");
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoMorseCode()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Snd/MorseCode.CPZ\";\n");
  Sleep(50);

  PostMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
  Sleep(200);
  PostMsg(task,MSG_KEY_UP,CH_SPACE,0);
  Sleep(350);

  PostMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
  Sleep(50);
  PostMsg(task,MSG_KEY_UP,CH_SPACE,0);
  Sleep(350);

  PostMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
  Sleep(50);
  PostMsg(task,MSG_KEY_UP,CH_SPACE,0);
  Sleep(25);
  PostMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
  Sleep(200);
  PostMsg(task,MSG_KEY_UP,CH_SPACE,0);
  Sleep(25);
  PostMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
  Sleep(50);
  PostMsg(task,MSG_KEY_UP,CH_SPACE,0);
  Sleep(1350);

  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

/*
U0 DoPixelCollision()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/PixelCollision.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoBlackDiamond()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/BlackDiamond.CPZ\";\n");
  for (i=0;i<15;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(75);
  }
  for (i=0;i<12;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
    Sleep(75);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoCtrlR1()
{
  I64 i;
  TaskStruct *task=SpawnUser,*task1;
  XTalkWithWait(task,"//");
  PostMsg(task,MSG_KEY_DOWN_UP,CH_CTRLR,0);
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;

//Tag text
  PostMsg(task,MSG_KEY_DOWN_UP,CH_ESC,0);

//Color
  while (!TaskValidate(task->popup_task))
    Yield;
  task1=task->popup_task;
  TaskWaitIdle(task->popup_task);
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (task->popup_task==task1 || !TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  for (i=0;i<2;i++)
    SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  task1=task->popup_task;
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);

//Width
  while (task->popup_task==task1 || !TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  task1=task->popup_task;
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (task->popup_task==task1 || !TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  for (i=0;i<4;i++)
    SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  task1=task->popup_task;
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);

//Line
  while (task->popup_task==task1 || !TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  for (i=0;i<6;i++)
    SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  SendMsg(task,MSG_IP_L_DOWN,150,150);
  SendMsg(task,MSG_IP_L_UP,150,95);
  Sleep(250);
  SendMsg(task,MSG_IP_L_DOWN,150,150);
  SendMsg(task,MSG_IP_L_UP,190,190);
  Sleep(250);
  SendMsg(task,MSG_IP_L_DOWN,150,150);
  SendMsg(task,MSG_IP_L_UP,110,190);
  Sleep(250);
  PostMsg(task,MSG_IP_R_DOWN_UP,100,100);

//Color
  while (!TaskValidate(task->popup_task))
    Yield;
  task1=task->popup_task;
  TaskWaitIdle(task->popup_task);
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (task->popup_task==task1 || !TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  for (i=0;i<3;i++)
    SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  task1=task->popup_task;
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);

//Circle
  while (task->popup_task==task1 || !TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  for (i=0;i<9;i++)
    SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  SendMsg(task,MSG_IP_L_DOWN,150,150);
  SendMsg(task,MSG_IP_L_UP,190,190);
  Sleep(250);
  PostMsg(task,MSG_IP_R_DOWN_UP,100,100);

//Exit
  while (!TaskValidate(task->popup_task))
    Yield;
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN+SCF_CTRL);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
  PostMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  Sleep(1000);
  XTalk(task,"\nExit;\n");
}

U0 DoCircuit()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/Graphics/Circuit.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  SetIP(300,350);
  SendMsg(task,MSG_KEY_DOWN_UP,'r',0);
  SetIP(400,350);
  SendMsg(task,MSG_KEY_DOWN_UP,'l',0);
  SetIP(350,400);
  SendMsg(task,MSG_KEY_DOWN_UP,'c',0);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  SendMsg(task,MSG_IP_L_DOWN,350,385);
  SendMsg(task,MSG_IP_L_UP,390,350);
  Sleep(500);
  SendMsg(task,MSG_IP_L_DOWN,350,385);
  SendMsg(task,MSG_IP_L_UP,310,350);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoSpaceShip()
{
  TaskStruct *task=SpawnUser;
  SetIP(300,300);
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/SpaceShip.CPZ\";\n");
  Sleep(150);
  AFSetIP(3,125,125);
  SetIP(,,,TRUE);
  Sleep(50);
  SetIP(,,,FALSE);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoBox()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Box.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoShadow()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Shadow.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoPredatorPrey()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/Graphics/PredatorPrey.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do3DPoly()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/3DPoly.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoLogo()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Logo.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoRotateTank()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/RotateTank.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

/*
U0 DoPic2ASCII()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Pic2ASCII.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoScreenCodes()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/ScreenCodes.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoExtendedChars()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/ExtendedChars.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoNeatTrick()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/NeatTrick.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoCharGraphics()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/CharGraphics.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoMaze()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Maze.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoFontEd()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/FontEd.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoTimeIns()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/TimeIns.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoWaterFowl()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Snd/WaterFowl.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/MusicOrgan/Examples\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

*/

U0 DoMPPrintF()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/MPPrintF.CPZ\";\n");
  Sleep(750);
  XTalk(task,"Exit;\n");
}

U0 DoLayers()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Layers.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoCartesian()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Cartesian.CPZ\";\n");
  XTalkWithWait(task,"0.2*x`1.5\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoHanoi()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Hanoi.CPZ\";\n");
  Sleep(3000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

/*
U0 DoLife()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Life.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoZing()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Zing.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

*/

U0 DoFlowers()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Flowers.CPZ\";\n");
  Sleep(3000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoPalindrome()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Palindrome.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoMagicPairs()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MagicPairs.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoAdd()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Add.CPZ\";\n");
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoLock()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Lock.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoLayers2()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Layers2.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoClouds()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Clouds.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoNoise()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Noise.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/SliderCtrl.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoBling()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Bling.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/ScrollBars.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/RegistryDemo.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoBirdLand()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/BirdLand.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  for (i=0;i<4;i++) {
    SendMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
    Sleep(100);
    SendMsg(task,MSG_KEY_UP,CH_SPACE,0);
    Sleep(100);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}


U0 DoMissileDefense()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/MissileDefense.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  task->popup_task->user_data0=Fs;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (!TaskValidate(task->popup_task)||task->popup_task->user_data0==Fs)
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  for (i=0;i<10;i++) {
    SetIP(RandU16%task->win_pixel_width,RandU16%task->win_pixel_height);
    SendMsg(task,MSG_KEY_DOWN_UP,RandU16%3+'1',0);
    Sleep(200);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoWarGame()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/WarGame.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  SetIP(430,300,,TRUE);
  AFSetIP(10,530,400);
  SetIP(,,,FALSE);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoBigGuns()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/BigGuns.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  task->popup_task->user_data0=Fs;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (!TaskValidate(task->popup_task)||task->popup_task->user_data0==Fs)
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  for (i=0;i<5;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_LEFT);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_LEFT);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_LEFT);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_LEFT);
    Sleep(200);
    SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoDunGen()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/DunGen.CPZ\";\n");
  for (i=0;i<10;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(100);
  }
  for (i=0;i<12;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
    Sleep(100);
  }
  for (i=0;i<6;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(100);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoTimeOut()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/Timeout/Run\";\n");
  Sleep(200);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  for (i=0;i<3;i++) {
    SendMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
    SendMsg(task,MSG_KEY_DOWN,0,SC_CURSOR_RIGHT,0);
    Sleep(200);
    SendMsg(task,MSG_KEY_UP,CH_SPACE,0);
    SendMsg(task,MSG_KEY_UP,0,SC_CURSOR_RIGHT,0);
    Sleep(200);
    SendMsg(task,MSG_KEY_DOWN,CH_SPACE,0);
    SendMsg(task,MSG_KEY_DOWN,0,SC_CURSOR_LEFT,0);
    Sleep(200);
    SendMsg(task,MSG_KEY_UP,CH_SPACE,0);
    SendMsg(task,MSG_KEY_UP,0,SC_CURSOR_LEFT,0);
    Sleep(200);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoCollision()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Collision.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoLoadTest()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/LoadTest.CPZ\";\n");
  Sleep(2000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
  while (TaskValidate(task))
    Yield;
}

U0 DoPrimes()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Primes.CPZ\";\n");
  Sleep(2500);
  XTalk(task,"Exit;\n");
}

U0 DoSort()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Sort.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoRadixSort()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/RadixSort.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoPageTableEntries2()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Lectures/PageTableEntries2.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoInterrupts()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/MultiCore/Interrupts.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoStkGrow()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/StkGrow.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoPilgrims()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Apps/Pilgrims/Run\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(2500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoSpan()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Apps/Span/Run\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoRawHide()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/RawHide.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(2500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoAfterEgypt()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Apps/AfterEgypt/Run\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  Sleep(600); //It flushes messages!
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  while (TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0); //Camp
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0); //Map
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0); //Map
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0); //Camp
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0); //Main Menu
  XTalk(task,"Exit;\n");
}

U0 DoShading()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Shading.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoPoleZeros()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/Graphics/PoleZeros.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(750);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoWhap()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Whap.CPZ\";\n");
  SetIP(300,200);
  Sleep(500);
  AFSetIP(3,350,300);
  Sleep(500);
  AFSetIP(3,450,200);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoPick()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Pick.CPZ\";\n");
  SetIP(200,200);
  Sleep(500);
  SetIP(,,,TRUE);
  AFSetIP(4,400,400);
  SetIP(,,,FALSE);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoPick2()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Pick2.CPZ\";\n");
  SetIP(200,200);
  Sleep(500);
  SetIP(,,,TRUE);
  AFSetIP(4,400,400);
  SetIP(,,,FALSE);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Transform.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/Tanks\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoGrModels()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/GrModels\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/


U0 DoKeepAway()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Apps/KeepAway/Run\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  SetIP(100,100);
  Sleep(1500);
  SendMsg(task,MSG_IP_R_DOWN_UP,ipx,ipy);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoCamp()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Graphics/Camp.CPZ\";\n");
  Sleep(1250);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoRocket()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Rocket.CPZ\";\n");
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
  Bts(key_down_bitmap,SC_CURSOR_UP);
  Sleep(1000);
  Btr(key_down_bitmap,SC_CURSOR_UP);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
  Bts(key_down_bitmap,SC_CURSOR_RIGHT);
  Sleep(1000);
  Btr(key_down_bitmap,SC_CURSOR_RIGHT);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoRocketScience()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/RocketScience.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(750);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(2000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoMassSpring()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/MassSpring.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  SendMsg(task,MSG_IP_L_DOWN_UP,50,50);  //#1
  SendMsg(task,MSG_IP_L_DOWN_UP,50,200); //#2
  SendMsg(task,MSG_IP_L_DOWN_UP,210,50); //#3
  SendMsg(task,MSG_IP_L_DOWN_UP,310,200);//#4
  Sleep(500);
  SendMsg(task,MSG_IP_R_DOWN,50,50); //1-2
  SendMsg(task,MSG_IP_R_UP,50,200);
  Sleep(500);
  SendMsg(task,MSG_IP_R_DOWN,210,50);//3-4
  SendMsg(task,MSG_IP_R_UP,310,200);
  Sleep(500);
  SendMsg(task,MSG_IP_R_DOWN,50,200);//2-4
  SendMsg(task,MSG_IP_R_UP,310,200);
  Sleep(500);
  SendMsg(task,MSG_IP_R_DOWN,50,50); //1-3
  SendMsg(task,MSG_IP_R_UP,210,50);
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Whap.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoSquirt()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GameStarters/Squirt.CPZ\";\n");
  for (i=0;i<5;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
    Sleep(100);
  }
  for (i=0;i<5;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_LEFT);
    Sleep(100);
  }
  for (i=0;i<5;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
    Sleep(100);
  }
  for (i=0;i<5;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(100);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoXCaliber()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/X-Caliber/Run\";\n");
  for (i=0;i<5;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SCF_SHIFT|SC_CURSOR_UP);
    SendMsg(task,MSG_KEY_DOWN_UP,0,SCF_SHIFT|SC_CURSOR_RIGHT);
  }
  for (i=0;i<10;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
    SendMsg(task,MSG_KEY_DOWN,0,SC_CURSOR_RIGHT,0);
    Sleep(100);
    SendMsg(task,MSG_KEY_UP,0,SC_CURSOR_RIGHT,0);
    Sleep(50);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoFPS()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/GameStarters/FPS.CPZ\";\n");
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  for (i=0;i<15;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
    Sleep(50);
  }
  for (i=0;i<8;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(50);
  }
  for (i=0;i<9;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
    Sleep(50);
  }
  for (i=0;i<6;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
    Sleep(50);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoEagleDive()
{
  I64 i;
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"Cd(\"::/LT/Demo/GameStarters\");Del(\"EagleDive.BIZ\");\n");
  XTalkWithWait(task,"Cmp(\"EagleDive.CPZ\");\n");
  Sleep(500);
  XTalkWithWait(task,"Load(\"EagleDive\");\n");
  for (i=0;i<10;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_UP);
    Sleep(50);
  }
  for (i=0;i<10;i++) {
    SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
    Sleep(50);
  }
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalkWithWait(task,"Del(\"::/LT/Demo/GameStarters/EagleDive.BIZ\");\n");
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/Span\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/MusicOrgan\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoPhoneNumWords()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/PhoneNumWords.CPZ\";\n");
  XTalkWithWait(task,"702-254-4223\n\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoSuggestedSpelling()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/SuggestSpelling.CPZ\";\n");
  XTalkWithWait(task,"effecient\n\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoWordSearch()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/WordSearch.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Dsk/DataBase.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoOrderedIO()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Dsk/OrderedIO.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

/*
U0 DoFPrintF()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Dsk/FPrintF.CPZ\";\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoRaw()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Dsk/Raw.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoUnusedSpaceRep()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Dsk/UnusedSpaceRep.CPZ\";\n");
  Sleep(500);
  XTalk(task,"Exit;\n");
}

U0 DoMiniCompiler()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Lectures/MiniCompiler.CPZ\";\n");
  XTalkWithWait(task,"1+2*(3+4)\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoMiniGrLib()
{
  TaskStruct *task=SpawnUser;
  XTalk(task,"#include \"::/LT/Demo/Lectures/MiniGrLib.CPZ\";Exit;\n");
  while (TaskValidate(task))
    Yield;
}

U0 DoWebLog()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/WebLogExample/WebLogRep.CPZ\";\n");
  SendMsg(task,MSG_KEY_DOWN_UP,CH_NEW_LINE,0);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_NEW_LINE,0);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_ESC,0);
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/TimeClock\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoLogic()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Apps/Logic/Run\";\n");
  XTalkWithWait(task,"NAND\nNOR\n\n0x100\n0xF0\n0xCC\n0xAA\n\n0x12\n\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Lectures\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/OldSchool\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/AutoFile\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoPrintF()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/PrintF.CPZ\";\n");
  Sleep(2000);
  XTalk(task,"Exit;\n");
}

U0 DoDateTime()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/DateTime.CPZ\";\n");
  Sleep(2500);
  XTalk(task,"Exit;\n");
}

U0 DoSubIntAccess()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/SubIntAccess.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoParenWarn()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/ParenWarn.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoCompileDemo()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/CompileDemo.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoPrompt()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Prompt.CPZ\";\n");
  XTalkWithWait(task,"1+2*3<<4\n");
  XTalkWithWait(task,"1.0+2*3`2\n");
  XTalkWithWait(task,"Terry Davis\n");
  XTalkWithWait(task,"*-1\n");
  Sleep(2000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoClassMeta()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/ClassMeta.CPZ\";\n");
  Sleep(2000);
  XTalk(task,"Exit;\n");
}

U0 DoForm()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/Form.CPZ\";\n");
  Sleep(250);
  XTalk(task,"54321");
  Sleep(250);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  Sleep(250);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SCF_CTRL|SC_CURSOR_LEFT);
  XTalk(task,"77777");
  Sleep(250);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN);
  Sleep(250);
  XTalk(task,"MyName");
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_ESC,0);
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoMenu()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/Menu.CPZ\";\n");
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN,0);
  Sleep(500);
  PostMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (!TaskValidate(task->popup_task))
    Yield;
  TaskWaitIdle(task->popup_task);
  Sleep(500);
  SendMsg(sys_focus_task,MSG_KEY_DOWN_UP,0,SC_CURSOR_RIGHT);
  Sleep(500);
  SendMsg(task->popup_task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  while (TaskValidate(task->popup_task))
    Yield;
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoMenu2()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/Menu2.CPZ\";\n");
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN,0);
  Sleep(50);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN,0);
  Sleep(50);
  SendMsg(task,MSG_KEY_DOWN_UP,0,SC_CURSOR_DOWN,0);
  Sleep(50);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(300);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

U0 DoTreeDemo()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/TreeDemo.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoExceptions()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Exceptions.CPZ\";\n");
  XTalkWithWait(task,"yyn");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoTextDemo()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/TextDemo.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoCursorMove()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/CursorMove.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoDemoDoc()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"Ed(\"::/LT/Demo/Ltf/DemoDoc.TXZ\");\n");
  SendMsg(task,MSG_KEY_DOWN_UP,CH_CTRLG,0);
  XTalkWithWait(task,"100");
  SendMsg(task,MSG_KEY_DOWN_UP,CH_ESC,0);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SHIFT_ESC,0);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Spy.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoFileRead()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/FileRead.CPZ\";\n");
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  Sleep(1000);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 DoData()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/Data.CPZ\";\n");
  Sleep(500);
  XTalk(task,"Exit;\n");
}

U0 DoDefineStr()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/DefineStr.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoCallBack()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/CallBack.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoCallBack2()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/CallBack2.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

U0 DoDefine()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Define.CPZ\";\n");
  XTalkWithWait(task,"YS\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Ltf/UnusedDefine.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/

U0 DoAsmHelloWorld()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Asm/AsmHelloWorld.CPZ\";\n");
  Sleep(750);
  XTalk(task,"Exit;\n");
}

U0 DoAsmAndC()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Asm/AsmAndC.CPZ\";\n2\n");
  XTalk(task,"Exit;\n");
  while (TaskValidate(task))
    Yield;
}

U0 DoAsmAndC2()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Asm/AsmAndC2.CPZ\";\n2\n");
  XTalk(task,"Exit;\n");
  while (TaskValidate(task))
    Yield;
}

U0 DoMulByHand()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Asm/MulByHand.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoDivByHand()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Asm/DivByHand.CPZ\";\n");
  Sleep(1000);
  XTalk(task,"Exit;\n");
}

U0 DoGlblVars()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/GlblVars.CPZ\";\n");
  Sleep(1500);
  XTalk(task,"Exit;\n");
}

/*
U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/WinMgr2.APZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Snd/PolyPhonic.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Snd/SndScope.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Snd/Synth.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Snd/Record.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}

U0 Do()
{
  TaskStruct *task=SpawnUser;
  XTalkWithWait(task,"#include \"::/LT/Demo/Snd/Echo.CPZ\";\n");
  Sleep(500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalk(task,"Exit;\n");
}
*/


U0 DoReps()
{
  TaskStruct *task=SpawnUser;
TS; XTalkWithWait(task,"LtfMax;ProfAll;HeapLog(ON);\n");
TS; XTalkWithWait(task,"PCIRep;Sleep(1000);\n");
TS; XTalkWithWait(task,"MemBIOSRep;Sleep(1000);\n");
TS; XTalkWithWait(task,"MemRep;Sleep(1000);\n");
TS; XTalkWithWait(task,"ProfRep;Sleep(1000);\n");
TS; XTalkWithWait(task,"HeapLogRep; HeapLog(OFF);Sleep(1000);\n");
TS; XTalkWithWait(task,"BlkDevRep;Sleep(1000);\n");
TS; XTalkWithWait(task,"ChkDsk;Sleep(1000);\n");
TS; XTalkWithWait(task,"DrvView;\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
  XTalkWithWait(task,"DskView;\n");
  Sleep(1500);
  SendMsg(task,MSG_KEY_DOWN_UP,CH_SPACE,0);
TS; XTalkWithWait(task,"ATARep;\npSleep(1000);\n");
TS; XTalkWithWait(task,"HashDepthRep(adam_task->hash_table);Sleep(1000);\n");
TS; XTalkWithWait(task,"HAllRep;Sleep(1000);\n");
TS; XTalkWithWait(task,"PrtRep;Sleep(1000);\n");
TS; XTalkWithWait(task,"TaskRep;Sleep(1000);\n");
  XTalk(task,"Exit;\n");
}

U0 LTTestSuite()
{
TSStart;

TS; DoSpaceShip;
TS; DoBlackDiamond;
TS; if (IsDrvWritable(':')) DoEagleDive;
TS; DoRocks;
TS; DoDunGen;
TS; DoFPS;
TS; DoZoneOut;
TS; DoVaroom;
TS; DoRocket;
TS; DoRocketScience;
TS; DoMissileDefense;
TS; DoWarGame;
TS; DoBigGuns;
TS; DoBirdLand;
TS; DoTimeOut;
TS; DoPilgrims;
TS; DoSpan;
TS; DoRawHide;
TS; DoKeepAway;
TS; DoAfterEgypt;
TS; DoXCaliber;
TS; DoWenceslas;
TS; DoBomber;
TS; DoTheDead;
TS; DoChess;
TS; DoChessNet;

TS; DoAdd;
TS; DoPrimes;
TS; DoPalindrome;
TS; DoClouds;
TS; DoNoise;
TS; if (mp_cnt>1) DoRadixSort;
TS; if (mp_cnt>1) DoLoadTest;
TS; if (mp_cnt>1) DoMPPrintF;
TS; if (mp_cnt>1) DoLayers;
TS; if (mp_cnt>1) DoLock;
TS; if (mp_cnt>1) DoLayers2;
TS; if (mp_cnt>1) DoInterrupts;
TS; if (mp_cnt>1) DoSort;

TS; DoGrSprite1;
TS; DoGrSprite2;
TS; DoGrSprite3;
TS; DoGrSprite4;
TS; DoEdGrSprite;
TS; DoExtents;
TS; DoDotNet;
TS; DoLowPassFilter;
TS; DoMathAudioDemo;
TS; DoLines;
TS; DoBounce;
TS; DoMsgLoop;
TS; DoASCIIOrgan;
TS; DoMorseCode;
TS; DoDoodle;
TS; DoMassSpring;
TS; DoSpeedLine;
TS; DoKeyBitMap;
TS; DoBlink;
TS; DoGolf;
TS; DoMountain;
TS; DoCoach;
TS; DoCPUPlot;
TS; DoDither;
TS; DoBSpline;
TS; DoBlot;
TS; if (IsDrvWritable(':')) DoCaptureScreen;
TS; DoPick;
TS; DoPick2;
TS; DoWhap;
TS; DoPalette;
TS; DoPrintF;
TS; DoCamp;
TS; DoShading;
TS; DoRotateTank;
TS; Do3DPoly;
TS; DoBox;
TS; DoSymmetry;
TS; DoShadow;
TS; DoPredatorPrey;
TS; DoPoleZeros;
TS; DoDateTime;
TS; DoDigits;
TS; DoStadium;
TS; DoHalogen;
TS; DoMaze;
TS; DoFontEd;
TS; DoCtrlR1;
TS; DoLogo;
TS; DoSubIntAccess;
TS; DoDemoDoc;
TS; DoTreeDemo;
TS; DoTextDemo;
TS; DoCursorMove;
TS; DoMiniCompiler;
TS; DoMiniGrLib;
TS; DoPhoneNumWords;
TS; DoUnusedSpaceRep;
TS; DoExceptions;
TS; DoScreenCodes;
TS; DoExtendedChars;
TS; DoNeatTrick;
TS; DoCharGraphics;
TS; DoMagicPairs;
TS; DoHanoi;
TS; DoSquirt;
TS; DoFlowers;
TS; DoCartesian;
TS; DoBling;
TS; DoRainDrops;
TS; DoCollision;
TS; DoOrderedIO;
TS; DoLogic;
TS; DoCircuit;
TS; DoCompileDemo;
TS; DoPrompt;
TS; if (IsDrvWritable(':')) DoWebLog;
TS; DoDefine;
TS; DoGlblVars;
TS; DoFileRead;
TS; DoParenWarn;
TS; DoDefineStr;
TS; DoData;
TS; DoCallBack;
TS; DoCallBack2;
TS; DoClassMeta;
TS; DoForm;
TS; DoMenu;
TS; DoMenu2;
TS; DoSuggestedSpelling;
TS; DoWordSearch;
TS; DoStkGrow;
TS; DoWaterFowl;
TS; DoAsmHelloWorld;
TS; DoAsmAndC;
TS; DoAsmAndC2;
TS; DoMulByHand;
TS; DoDivByHand;
TS; DoPageTableEntries2;
DoReps;

//TicTacToe?
TSEnd;
}

LTTestSuite;
