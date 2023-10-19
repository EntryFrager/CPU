DEF_CMD (HLT,
    {
        fprintf (data->fp_print, "%s", COMMAND[data->cmd[id].command]);
        break;
    })
    
DEF_CMD (PUSH + (1 << 4),
    {
        fprintf (data->fp_print, "%s %d\n", COMMAND[PUSH], (data->cmd[id].argc));
        break;
    })

DEF_CMD (PUSH + (1 << 5),
    {
        fprintf (data->fp_print, "%s %s\n", COMMAND[PUSH], REG[data->cmd[id].reg - 1]);
        break;
    })

DEF_CMD (POP + (1 << 5),
    {
        fprintf (data->fp_print, "%s %s\n", COMMAND[POP], REG[data->cmd[id].reg - 1]);
        break;
    })