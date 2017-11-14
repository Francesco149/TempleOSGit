+] Adam
  +] SysMsgFlags
      sys_msg_flags[0]=0xFF;
      +] SysRegVer
      registry_version=5.030;
      +] InsReg
      #help_index "Registry/Install Registration"
    MemSet(&ins_reg,0,sizeof(CInsReg));
    ins_reg.registered=1;
    ins_reg.host_os=2;
    ins_reg.host_vm=2;
    
    #define INS_REG_PERSONAL_INITIALS "TAD"
    StrCpy(ins_reg.initials,"TAD");
    HashPublic("INS_REG_PERSONAL_INITIALS",HTT_DEFINE_STR);
    
    #define INS_REG_MACHINE_NUM 0
    ins_reg.machine_num=0;
    HashPublic("INS_REG_MACHINE_NUM",HTT_DEFINE_STR);
    
    "InsReg:%s:%d\n",INS_REG_PERSONAL_INITIALS,INS_REG_MACHINE_NUM;
    
    #help_index ""
      +] TempleOS
  +] EagleDive
      F64 best_score=147.6932;
      +] DiningStars
      F64 best_score=9999.0000;
      +] CircleTrace
      F64 best_score=4.9425;
      +] BlackDiamond
      I64 best_score=2;
      +] DunGen
      F64 best_score=20.5295;
      +] CastleFrankenstein
      F64 best_score=33.8405;
      +] ZoneOut
      F64 best_score=8.1373;
      +] Varoom
      F64 best_score=46.2643;
      +] FlapBat
      F64 best_score=16.3077;
      +] RawHide
      F64 best_score=96.1279;
      +] KeepAway
      I64 best_score0=68,best_score1=30;
      +] Wenceslas
      F64 best_score=43.9944;
      +] OSTestSuite
      progress1_tf=0.000;progress2_tf=0.000;
    progress3_tf=0.000;progress4_tf=287.834;
      +] XCaliber
      I64 best_score=90;
    I64 msg_flags=2;
      +] Talons
      F64 best_score=9999.0000;
      +] BomberGolf
      I64 best_score=175;
      +] Titanium
      I64 best_score=16469;
      +] TOSRegen
      progress1_tf=0.000;progress2_tf=0.000;
    progress3_tf=0.000;progress4_tf=92.868;
      +] TempleTetris
      I64 hiscore=0;
      +] Once
  +] Adam
        +] User
        +] DemoCompany
  +] Game1
      F64 best_score=434.94752;
    +] SimpleVal
          1246;
            +] WalnutZone
  +] Cherub
      #define rom_path M:/SnailNet
    I64 sound_mode=2;
    I64 sound_enable=1;
    I64 use_keyboard=1;
    I64 use_gamepad=0;
    I64 lcd_scale=2;
      