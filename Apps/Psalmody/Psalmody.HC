/*
This uses four types of ctrls

  1) The pull-down menu.

  2) The [C:/Kernel/KernelA.HH.Z,3065] CCtrl type for the tempo/stacatto
sliders.

  3) The active [C:/Kernel/KernelA.HH.Z,1367] CDoc sprite bttns with macros
for left/record/play/right.

  4) Hand-made ctrls for the kbdgraphic, notes and staff.

This uses four types of output

  1) The [C:/Kernel/KernelA.HH.Z,1367] CDoc for the text and bttns and
kbdgraphic.

  2) The Fs->draw_it() for the staff region, drawn every refresh.

  3) The gr.dc persistent layer for the note chooser, meter chooser.  The 
persistent layer is used during drag-and-drop.

  4) The Fs->next_ctrl for the tempo/stacatto sliders.

See [C:/Adam/Gr/GrScrn.HC.Z,18] GrUpdateTaskWin(),
[C:/Adam/Gr/GrScrn.HC.Z,58] GrUpdateTasks() and [C:/Adam/Gr/GrScrn.HC.Z,398]
GrUpdateScrn().
*/

#define PSMT_HEAD	0
#define PSMT_NOTE	1
#define PSMT_METER	2

#define PSMf_SEL	0
#define PSMF_SEL	1
#define PSMf_SHARP	1
#define PSMf_FLAT	2
#define PSMf_TIE	3

class PsmNote
{
  PsmNote *next,*last;
  I64 x,y;
  U8 *word;
  I64 type,flags;
  I64 ona,meter_top,meter_bottom;
  I64 duration,width;
  U8 ascii[32];
};

//Tool types
#define PSMTT_PTR_TOOL	0
#define PSMTT_BOX_TOOL	1

class PsmCtrl
{
  PsmNote head;
  PsmNote clip;
  CMenuEntry *incomplete_entry,*record_entry;
  I64 scrn_x,tool;
  PsmNote *cur_note;
  CDC *dc2;
  Bool playing;
} psm;

U8 *psm_note_lst="A\0A#\0B\0C\0C#\0D\0D#\0E\0F\0F#\0G\0G#\0";
U8 psm_note_map[12]={6,6,5,4,4,3,3,2,1,1,0,0};
U8 psm_note_inverse_map[7]={10,8,7,5,3,2,0};
