#help_index "Sprites/Binary"

class CMeshTri
{
  I32 color;	//Give one of 0-7 colors.
//Colors 8-15 are 0-7 with intensity bit set.
  I32 nums[3];	//Vertex number
};

class CQueMeshTri
{
  CQueMeshTri *next,*last;
  U0 start;
  I32 color;
  I32 nums[3];
};

public class CSpriteBase
{
  U8 type;
};

public class CSpriteColor : CSpriteBase
{
  U8 color;
};

public class CSpriteDitherColor : CSpriteBase
{
  U16 dither_color;
};

public class CSpriteW : CSpriteBase
{
  I32 width;
};

public class CSpritePt : CSpriteBase
{
  I32 x1;
  I32 y1;
};

public class CSpritePtRad : CSpritePt
{
  I32 radius;
};

public class CSpritePtPt : CSpritePt
{
  I32 x2;
  I32 y2;
};

public class CSpritePtPtAng : CSpritePtPt
{
  F64 angle;
};

public class CSpritePtWH : CSpritePt
{
  I32 width;
  I32 height;
};

public class CSpritePtWHU8s : CSpritePtWH
{
  U8 u[0];
};

public class CSpritePtWHAng : CSpritePtWH
{
  F64 angle;
};

public class CSpritePtWHAngSides : CSpritePtWHAng
{
  I32 sides;
};

public class CSpriteNumU8s : CSpriteBase
{
  I32 num;
  U8  u[0];
};

public class CSpriteNumPtU8s : CSpriteBase
{
  I32 num;
  I32 x;
  I32 y;
  U8  u[0];
};

public class CSpritePtStr : CSpritePt
{
  U8  st[0];
};

public class CSpriteMeshU8s : CSpriteBase
{
  I32 vertex_cnt;
  I32 tri_cnt;
  U8  u[0];
};

public class CSpritePtMeshU8s : CSpriteBase
{
  I32 x;
  I32 y;
  I32 z;
  I32 vertex_cnt;
  I32 tri_cnt;
  U8  u[0];
};

#define SPT_END			0	//[C:/Adam/Gr/Gr.HPP.Z,18] CSpriteBase
#define SPT_COLOR		1	//[C:/Adam/Gr/Gr.HPP.Z,23] CSpriteColor
#define SPT_DITHER_COLOR	2	//[C:/Adam/Gr/Gr.HPP.Z,28] CSpriteDitherColor
#define SPT_WIDTH		3	//[C:/Adam/Gr/Gr.HPP.Z,33] CSpriteW
#define SPT_PLANAR_SYMMETRY	4	//[C:/Adam/Gr/Gr.HPP.Z,49] CSpritePtPt
#define SPT_TRANSFORM_ON	5	//[C:/Adam/Gr/Gr.HPP.Z,18] CSpriteBase
#define SPT_TRANSFORM_OFF	6	//[C:/Adam/Gr/Gr.HPP.Z,18] CSpriteBase
#define SPT_SHIFT		7	//[C:/Adam/Gr/Gr.HPP.Z,38] CSpritePt
#define SPT_PT			8	//[C:/Adam/Gr/Gr.HPP.Z,38] CSpritePt
#define SPT_POLYPT		9	//[C:/Adam/Gr/Gr.HPP.Z,87] CSpriteNumPtU8s
#define SPT_LINE		10	//[C:/Adam/Gr/Gr.HPP.Z,49] CSpritePtPt
#define SPT_POLYLINE		11	//[C:/Adam/Gr/Gr.HPP.Z,81] CSpriteNumU8s
#define SPT_RECT		12	//[C:/Adam/Gr/Gr.HPP.Z,49] CSpritePtPt
#define SPT_ROTATED_RECT	13	//[C:/Adam/Gr/Gr.HPP.Z,55] CSpritePtPtAng
#define SPT_CIRCLE		14	//[C:/Adam/Gr/Gr.HPP.Z,44] CSpritePtRad
#define SPT_ELLIPSE		15	//[C:/Adam/Gr/Gr.HPP.Z,71] CSpritePtWHAng
#define SPT_POLYGON		16	//[C:/Adam/Gr/Gr.HPP.Z,76] CSpritePtWHAngSides
#define SPT_BSPLINE2		17	//[C:/Adam/Gr/Gr.HPP.Z,81] CSpriteNumU8s
#define SPT_BSPLINE2_CLOSED	18	//[C:/Adam/Gr/Gr.HPP.Z,81] CSpriteNumU8s
#define SPT_BSPLINE3		19	//[C:/Adam/Gr/Gr.HPP.Z,81] CSpriteNumU8s
#define SPT_BSPLINE3_CLOSED	20	//[C:/Adam/Gr/Gr.HPP.Z,81] CSpriteNumU8s
#define SPT_FLOOD_FILL		21	//[C:/Adam/Gr/Gr.HPP.Z,38] CSpritePt
#define SPT_FLOOD_FILL_NOT	22	//[C:/Adam/Gr/Gr.HPP.Z,38] CSpritePt
#define SPT_BITMAP		23	//[C:/Adam/Gr/Gr.HPP.Z,66] CSpritePtWHU8s
#define SPT_MESH		24	//[C:/Adam/Gr/Gr.HPP.Z,100] CSpriteMeshU8s See ::/Apps/GrModels
`Cd("::/Apps/GrModels");Dir;View;
`
.
#define SPT_SHIFTABLE_MESH	25	//[C:/Adam/Gr/Gr.HPP.Z,107] CSpritePtMeshU8s See ::/Apps/GrModels
`Cd("::/Apps/GrModels");Dir;View;
`
.
#define SPT_ARROW		26	//[C:/Adam/Gr/Gr.HPP.Z,49] CSpritePtPt
#define SPT_TEXT		27	//[C:/Adam/Gr/Gr.HPP.Z,95] CSpritePtStr
#define SPT_TEXT_BOX		28	//[C:/Adam/Gr/Gr.HPP.Z,95] CSpritePtStr
#define SPT_TEXT_DIAMOND	29	//[C:/Adam/Gr/Gr.HPP.Z,95] CSpritePtStr
#define SPT_NUM_TYPES		30

#help_index "Graphics/Sprite;Sprites;Sprites/Binary"

//See [Sprites,1] Sprites.  See [C:/Adam/Gr/GrSpritePlot.CPP.Z,1] ::/Adam/Gr/GrSpritePlot.CPP and
//[C:/Demo/Graphics/SpriteRaw.CPP,1] for how CSprite are stored.
public class CSprite
{
  CSprite *next,*last;
  Bool selected;
  U0 start;
  union {
    U8 type;
    CSpriteBase		b;
    CSpriteColor	c;
    CSpriteDitherColor	d;
    CSpriteW	w;
    CSpritePt		p;
    CSpritePtPt		pp;
    CSpritePtPtAng	ppa;
    CSpritePtRad	pr;
    CSpritePtWHU8s	pwhu;
    CSpritePtWHAng	pwha;
    CSpritePtWHAngSides pwhas;
    CSpriteNumU8s	nu;
    CSpriteNumPtU8s	npu;
    CSpriteMeshU8s	mu;
    CSpritePtMeshU8s	pmu;
    CSpritePtStr	ps;
  }
};
