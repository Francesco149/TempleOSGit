;CHARIO.ASM
        EXTRN   GET_CHAR                :NEAR
        EXTRN   GET_STRING              :NEAR
        EXTRN   PUT_CHAR                :NEAR
        EXTRN   PUT_DEC                 :NEAR
        EXTRN   PUT_HEX                 :NEAR
        EXTRN   PUT_HEX_WORD            :NEAR
        EXTRN   PUT_HEX_BYTE            :NEAR
        EXTRN   FIXED_PUT_HEX           :NEAR
        EXTRN   PUT_STRING              :NEAR
        EXTRN   FIXED_PUT_STRING        :NEAR
;CMD.ASM
        EXTRN   EXECUTE_COMMAND_LINE    :NEAR
        EXTRN   COMMAND_LINE            :NEAR
;CRT.ASM
        EXTRN   CURSOR_ON               :NEAR
        EXTRN   CURSOR_OFF              :NEAR
        EXTRN   INIT_CRT                :NEAR
        EXTRN   CURSOR_HOME             :NEAR
        EXTRN   CRT_OUT                 :NEAR
        EXTRN   CRT_LINE_OUT            :NEAR
        EXTRN   CLEAR_CRT               :NEAR
;GRMODE.ASM
        EXTRN   CMD_GR_CLEAR            :NEAR
        EXTRN   CMD_GR_PLOT             :NEAR
        EXTRN   CMD_GR_LINE             :NEAR
        EXTRN   GR_UPDATE_SCREEN        :NEAR
;KEYBD.ASM
        EXTRN   GET_KEY                 :NEAR
;LEX.ASM 
        EXTRN   LEX_INCLUDE             :NEAR   
        EXTRN   NEW_LEX                 :NEAR
        EXTRN   DELETE_LEX              :NEAR
        EXTRN   LEX                     :NEAR
        EXTRN   LEX_GET_CHAR            :NEAR
        EXTRN   PUT_TOKEN               :NEAR
        EXTRN   LEX_PUSH                :NEAR
        EXTRN   LEX_POP                 :NEAR
        EXTRN   LEX_SUB_STACK_PTR       :NEAR
        EXTRN   LEX_CB_PUSH             :NEAR
        EXTRN   LEX_CB_POP              :NEAR
        EXTRN   LEX_CB_POP_NO_FREE      :NEAR
        EXTRN   LEX_CB_APPEND           :NEAR

;LTF.ASM
        EXTRN   TO_LINKED_TEXT_FILE     :NEAR
;MEMORY.ASM
        EXTRN   ALLOCATE_MEMORY_BLOCKS  :NEAR
        EXTRN   FREE_BLOCK_LIST         :NEAR
        EXTRN   MALLOC                  :NEAR
        EXTRN   FREE                    :NEAR
        EXTRN   RMALLOC                 :NEAR
        EXTRN   RFREE                   :NEAR
        EXTRN   RNEW_STRING             :NEAR
        EXTRN   SYSTEM_MEMORY_LOW       :DWORD
        EXTRN   SYSTEM_MEMORY_HIGH      :DWORD

;PARSE.ASM
        EXTRN   PARSE_ERROR             :NEAR
        EXTRN   CONT_PARSE_EXPRESSION   :NEAR
        EXTRN   COMPILE_EXPRESSION      :NEAR
        EXTRN   PUT_EXPRESSION          :NEAR
        EXTRN   EVAL_EXPRESSION         :NEAR
        EXTRN   RUN_STATEMENT           :NEAR
        EXTRN   DELETE_EXPRESSION       :NEAR
        EXTRN   GET_EXPRESSION          :NEAR
;SCHED.ASM
        EXTRN   ADD_TASK_TO_QUEUE       :NEAR
        EXTRN   REMQUE_TASK             :NEAR
        EXTRN   SWAP_IN_NEXT            :NEAR
        EXTRN   KILL_TASK               :NEAR
        EXTRN   END_TASK                :NEAR
        EXTRN   SAVE_CONTEXT            :NEAR
        EXTRN   RESTORE_CONTEXT         :NEAR
        EXTRN   DEACTIVATE_USER         :NEAR
        EXTRN   ACTIVATE_NEXT_USER      :NEAR
        EXTRN   CMD_ACTIVATE_NEXT_USER  :NEAR
        EXTRN   SPAWN_USER_AND_ACTIVATE :NEAR
        EXTRN   KILL_ACTIVE_USER        :NEAR
;STRING.ASM
        EXTRN   LOAD_TEXT_FILE          :NEAR
        EXTRN   NEW_STRING              :NEAR
        EXTRN   ADDS                    :NEAR
        EXTRN   STRLEN                  :NEAR
        EXTRN   HASH_STRING             :NEAR
        EXTRN   NEW_HASH_TABLE          :NEAR
        EXTRN   FIND_HASH_ENTRY         :NEAR
        EXTRN   FIND_HASH_BUCKET        :NEAR
;SYSTEXT.ASM
        EXTRN   PUT_SYSTEXT             :NEAR
        EXTRN   CRLF                    :NEAR
        EXTRN   SPACE                   :NEAR
        EXTRN   SYSTEM_EXTERN_TABLE     :DWORD
;SYSUTILS.ASM
        EXTRN   DELETE_LINKED_LIST      :NEAR
        EXTRN   INSQUE                  :NEAR
        EXTRN   INSQUE_REV              :NEAR
        EXTRN   REMQUE                  :NEAR
        EXTRN   HEX_DUMP                :NEAR
        EXTRN   DEBUG_SPIN_WAIT         :NEAR
        EXTRN   BINARY_TO_HEX           :NEAR
        EXTRN   BINARY_TO_FIXED_HEX     :NEAR
;TRAP.ASM
        EXTRN   REGISTER_TRAP           :NEAR
        EXTRN   END_TRAP                :NEAR
        EXTRN   TRAP                    :NEAR


