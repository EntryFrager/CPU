DEF_CMD (HLT,
    {
        int cmd = HLT;
        fprintf (data->fp_print_txt, "%d", HLT);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (OUT,
    {
        int cmd = OUT;
        fprintf (data->fp_print_txt, "%d\n", OUT);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (PUSH,
    {
        int err_reg = 0;
        int value = 0;
        char buf[256] = "";

        if (sscanf ((data->cmd[id].command + cmd_len_PUSH + 1), "%d", &value) == 1)
        {
            sprintf (buf, "%d", value);

            if (*(data->cmd[id].command + cmd_len_PUSH + 1 + strlen (buf) + 1) == '\0' && (*(data->cmd[id].command + cmd_len_PUSH) == ' '))
            {
                fprintf (data->fp_print_txt, "%d %d\n", PUSH + (1 << 4), value);

                int push = PUSH + (1 << 4);
                fwrite (&push, sizeof (int), 1, data->fp_print_bin);
                fwrite (&value, sizeof (int), 1, data->fp_print_bin);
            }
            else
            {
                return ERR_COMMAND;
            }
        }
        else
        {
            for (size_t n_reg = 0; n_reg < REG_CNT; n_reg++)
            {
                size_t reg_len = strlen (REG[n_reg]);

                if (strncmp ((data->cmd[id].command + cmd_len_PUSH + 1), REG[n_reg], reg_len) == 0)
                {
                    err_reg++;

                    if ((*(data->cmd[id].command + cmd_len_PUSH) != ' ') || (*(data->cmd[id].command + cmd_len_PUSH + 1 + reg_len + 1) != '\0'))
                    {
                        return ERR_REG_PUSH;
                    }
                    else
                    {
                        fprintf (data->fp_print_txt, "%d %d\n", PUSH + (1 << 5), n_reg + 1);

                        int push_reg = PUSH + (1 << 5);
                        fwrite (&push_reg, sizeof (int), 1, data->fp_print_bin);
                        int reg = n_reg + 1;
                        fwrite (&reg, sizeof (int), 1, data->fp_print_bin);
                    }
                }
            }

            if (err_reg == 0)
            {
                return ERR_REG_PUSH;
            }
        }
    })

DEF_CMD (POP,
    {
        int err_reg = 0;

        if (*(data->cmd[id].command + cmd_len_POP) == '\0')
        {
            int pop = POP;

            fwrite (&pop, sizeof (int), 1, data->fp_print_bin);
            fprintf (data->fp_print_txt, "%d\n", POP);
        }
        else
        {
            for (size_t n_reg = 0; n_reg < REG_CNT; n_reg++)
            {
                size_t reg_len = strlen (REG[n_reg]);

                if (strncmp ((data->cmd[id].command + cmd_len_POP + 1), REG[n_reg], reg_len) == 0)
                {
                    err_reg++;
                    if ((*(data->cmd[id].command + cmd_len_POP) != ' ') || (*(data->cmd[id].command + cmd_len_POP + 1 + reg_len + 1) != '\0'))
                    {
                        return ERR_REG_POP;
                    }
                    else
                    {
                        fprintf (data->fp_print_txt, "%d %d\n", POP + (1 << 5), n_reg + 1);

                        int cmd_pop = POP + (1 << 5);
                        int reg_pop = n_reg + 1;

                        fwrite (&cmd_pop, sizeof (int), 1, data->fp_print_bin);
                        fwrite (&reg_pop, sizeof (int), 1, data->fp_print_bin);
                    }
                }
            }

            if (err_reg == 0)
            {
                return ERR_REG_POP;
            }
        }
    })

DEF_CMD (ADD,
    {
        int cmd = ADD;
        fprintf (data->fp_print_txt, "%d\n", ADD);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (SUB,
    {
        int cmd = SUB;
        fprintf (data->fp_print_txt, "%d\n", SUB);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (MUL,
    {
        int cmd = MUL;
        fprintf (data->fp_print_txt, "%d\n", MUL);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (DIV,
    {
        int cmd = DIV;
        fprintf (data->fp_print_txt, "%d\n", DIV);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (SQRT,
    {
        int cmd = SQRT;
        fprintf (data->fp_print_txt, "%d\n", SQRT);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (SIN,
    {
        int cmd = SIN;
        fprintf (data->fp_print_txt, "%d\n", SIN);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (COS,
    {
        int cmd = COS;
        fprintf (data->fp_print_txt, "%d\n", COS);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })

DEF_CMD (IN,
    {
        int cmd = IN;
        fprintf (data->fp_print_txt, "%d\n", IN);
        fwrite (&cmd, sizeof (int), 1, data->fp_print_bin);
    })