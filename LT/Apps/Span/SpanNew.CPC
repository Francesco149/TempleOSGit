Ode *SpanNew()
{

  ode=OdeNew(0,1e-4,ODEF_HAS_MASSES|ODEF_PAUSED);
  ode->derivative=&MyDerivative;
  ode->drag_v2=0.002;
  ode->drag_v3=0.00001;
  ode->acceleration_limit=5e3;

  InsQue(ode,Fs->last_ode);
  return ode;
}

U0 SpanDel(Ode *ode)
{
  MyMass   *tempm,*tempm1;
  MySpring *temps,*temps1;

  RemQue(ode);

  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    tempm1=tempm->next;
    RemQue(tempm);
    Free(tempm);
    tempm=tempm1;
  }

  temps=ode->next_spring;
  while (temps!=&ode->next_spring) {
    temps1=temps->next;
    RemQue(temps);
    Free(temps);
    temps=temps1;
  }

  OdeDel(ode);
}


#define M_SIZE (sizeof(MyMass)-offset(MassStruct.start_saved_area))
#define S_SIZE (sizeof(MySpring)-offset(SpringStruct.start_saved_area))

U0 *SpanSave(Ode *ode,U64 *size=NULL)
{
  I64 cnt;
  U8 *result,*ptr;
  MyMass   *tempm;
  MySpring *temps;
  SpanHeader h;

  OdeRenum(ode);
  h.version=SPAN_VERSION;
  if (ode->next_mass!=&ode->next_mass)
    h.num_masses=ode->last_mass->num+1;
  else
    h.num_masses=0;
  if (ode->next_spring!=&ode->next_spring)
    h.num_springs=ode->last_spring->num+1;
  else
    h.num_springs=0;

  cnt=sizeof(SpanHeader)+h.num_masses*M_SIZE+h.num_springs*S_SIZE;

  ptr=result=MAlloc(cnt);
  MemCpy(ptr,&h,sizeof(SpanHeader));
  ptr+=sizeof(SpanHeader);

  tempm=ode->next_mass;
  while (tempm!=&ode->next_mass) {
    MemCpy(ptr,&tempm->start_saved_area,M_SIZE);
    ptr+=M_SIZE;
    tempm=tempm->next;
  }

  temps=ode->next_spring;
  while (temps!=&ode->next_spring) {
    MemCpy(ptr,&temps->start_saved_area,S_SIZE);
    ptr+=S_SIZE;
    temps=temps->next;
  }

  if (size) *size=cnt;
  return result;
}

BoolI64 SpanWrite(Ode *ode)
{
  U8 *name,*buf;
  U64 size;
  BoolI8 result=FALSE,
	 old_silent=Silent(ON);
  MkDir("HOME/Span");
  Silent(old_silent);
  if (name=PopUpFileName("HOME/Span/Game.DTZ")) {
    if (buf=SpanSave(ode,&size)) {
      FileWrite(name,buf,size);
      Free(buf);
      result=TRUE;
    }
    Free(name);
  }
  return result;
}

U0 SpanLoad(Ode *ode,U8 *src)
{
  I64 i;
  MyMass   *tempm;
  MySpring *temps;
  SpanHeader h;

  if (!src) return;

  MemCpy(&h,src,sizeof(SpanHeader));
  src+=sizeof(SpanHeader);

  for (i=0;i<h.num_masses;i++) {
    tempm=CAlloc(sizeof(MyMass));
    MemCpy(&tempm->start_saved_area,src,M_SIZE);
    src+=M_SIZE;
    InsQue(tempm,ode->last_mass);
  }

  for (i=0;i<h.num_springs;i++) {
    temps=CAlloc(sizeof(MySpring));
    MemCpy(&temps->start_saved_area,src,S_SIZE);
    src+=S_SIZE;
    InsQue(temps,ode->last_spring);
    temps->end1=MassFindNum(ode,temps->end1_num);
    temps->end2=MassFindNum(ode,temps->end2_num);
  }
}

U8 *SpanRead()
{
  U8 *src=NULL,*name;
  BoolI8 old_silent=Silent(ON);
  MkDir("HOME/Span");
  Silent(old_silent);
  if (name=PopUpPickFile("HOME/Span")) {
    src=FileRead(name);
    Free(name);
  }
  return src;
}

