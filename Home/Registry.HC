+] Adam
  +] SysMsgFlags
      sys_msg_flags[0]=0;
      +] SysRegVer
      registry_version=5.030;
      +] InsReg
      #help_index "Registry/Install Registration"
    MemSet(&ins_reg,0,sizeof(CInsReg));
    ins_reg.registered=0;
    ins_reg.host_os=0;
    ins_reg.host_vm=0;
    
    #define INS_REG_PERSONAL_INITIALS ""
    StrCpy(ins_reg.initials,"");
    HashPublic("INS_REG_PERSONAL_INITIALS",HTT_DEFINE_STR);
    
    #define INS_REG_MACHINE_NUM 0
    ins_reg.machine_num=0;
    HashPublic("INS_REG_MACHINE_NUM",HTT_DEFINE_STR);
    
    "InsReg:%s:%d\n",INS_REG_PERSONAL_INITIALS,INS_REG_MACHINE_NUM;
    
    #help_index ""
      +] Once
  +] Adam
        +] User
        +] TempleOS
  +] Talons
      F64 best_score=9999.0000;
      +] XCaliber
      I64 best_score=2;
    I64 msg_flags=2;
      +] CastleFrankenstein
      F64 best_score=35.4044;
      +] Varoom
      F64 best_score=9999.0000;
      +] Wenceslas
      F64 best_score=9999.0000;
      +] BomberGolf
      I64 best_score=99999;
      +] RawHide
      F64 best_score=285.8427;
      +] BlackDiamond
      I64 best_score=9999;
      +] FlapBat
      F64 best_score=9999.0000;
      +] Titanium
      I64 best_score=0;
      +] KeepAway
      I64 best_score0=0,best_score1=9999;
      