



	/* <1> <1> (image) */






	/* <2> <2> (image) */




	/* <3> <3> (image) */



















	/* <4> <4> (image) */


























	/* <5> <5> (image) */

//See [C:/Doc/Credits.DD.Z,1].









	/*  <6> (image) */








	/*  <7> (image) */









	/*  <8> (image) */
#define OT_PEBBLE	0
#define OT_BUSH1	1
#define OT_BUSH2	2
#define OT_LOG		3
#define OT_TREE1	4
#define OT_TREE2	5
#define OT_SHEEP	6
#define OT_GOAT1	7
#define OT_GOAT2	8
#define OT_TYPES_NUM	9

U8 *imgs[OT_TYPES_NUM]={NULL,__BIN_1,__BIN_2,__BIN_3,__BIN_4,__BIN_5,__BIN_6,__BIN_7,__BIN_8};

F64 theta,vx,vz;
Bool horeb_done;

#define O_BURNING_BUSH	0
#define O_OBJS_NUM	256
#define O_PEBBLES_NUM	4096

class Obj
{
  F64 x,y,z,dx,dy,dz,theta;
  I64 x1,y1,z1;
  U32 num;
  U16 type;
  Bool sym,pad;
} objs[O_OBJS_NUM+O_PEBBLES_NUM];

U8 pebble_colors[4]={BLACK,DKGRAY,DKGRAY,LTGRAY};
