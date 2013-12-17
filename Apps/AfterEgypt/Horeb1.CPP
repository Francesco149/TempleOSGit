




	/* <1> <1> (image) */







	/* <2> <2> (image) */





	/* <3> <3> (image) */




















	/* <4> <4> (image) */



























	/* <5> <5> (image) */

//See [C:/Doc/Acknowledgements.TXT.Z,1].









	/*  <6> (image) */










	/*  <7> (image) */











	/*  <8> (image) */

#define OT_BUSH1	0
#define OT_BUSH2	1
#define OT_LOG		2
#define OT_TREE1	3
#define OT_TREE2	4
#define OT_SHEEP	5
#define OT_GOAT1	6
#define OT_GOAT2	7
#define OT_NUM_TYPES	8

U8 *type_imgs	[OT_NUM_TYPES]={__BIN_1,__BIN_2,__BIN_3,__BIN_4,__BIN_5,__BIN_6,__BIN_7,__BIN_8};

F64 theta,vx,vz;
Bool horeb_done;

#define O_BURNING_BUSH	0
#define O_NUM_OBJS	256

class Obj
{
  F64 x,y,z,dx,dy,dz,theta;
  I64 x1,y1,z1;
  U16 type;
  Bool sym,pad[5];
} objs[O_NUM_OBJS];
