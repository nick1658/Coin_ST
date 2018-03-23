
#include "main.h"
#include  "user_app.h"

cmd_analyze_struct cmd_analyze; 




s_system_env sys_env;

//接收数据                                                                         
static uint32_t rec_count=0;  


int do_help (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
void system_env_init (void)
{
	int i;
	i = (unsigned int)do_help;
	int8_t *p = (int8_t *) &sys_env;
	for (i = 0; i < sizeof(s_system_env); i++){
		*(p++) = 0;
	}
//	sys_env.country_index = coinchoose;
	sys_env.save_ng_data = 1;
	sys_env.save_good_data = 1;
	sys_env.uart0_cmd_flag = 0xA5;//console 未激活
	sys_env.password = 1573;
}


unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

long simple_strtol(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);
}


                                                                        
	
	/*命令行分析任务*/  
void vTaskCmdAnalyze( void )  
{  
		
}

              
                    
/*提供给串口中断服务程序，保存串口接收到的单个字符*/   
void fill_rec_buf(uint8_t data)                                                           
{
	if (data == CTRL_C)
	{
		cy_println ("Op Cancel!");
		sys_env.sys_break = 1;
		return;
	}
	if (sys_env.uart0_cmd_flag == 0xA5){
		if (0x0D != data){
			return;
		}else{
			sys_env.uart0_cmd_flag = 0;
		}
	}
	if (sys_env.tty_mode == 0x55){ //程序下载
//		sys_env.tty_online_ms = TTY_ONLINE_TIME;
//		iap_code_buf[rec_count++] = data;
//		if (rec_count == 1){
//			cy_print ("\n");
//		}
//		if (rec_count % 1024 == 0){
//			cy_print (".");
//		}
//		if (rec_count % (1024*64) == 0){
//			cy_print ("\n");
//		}
//		if (rec_count >= CODE_BUF_SIZE){
//			rec_count=0; 
//			sys_env.tty_mode = 0;
//			cy_println("ERROR");//数组越界，表示接受失败
//		}
	}else if (sys_env.tty_mode == 0xaa){
		sys_env.tty_online_ms = TTY_ONLINE_TIME;
		cmd_analyze.rec_buf[rec_count++] = data; 
	}else if (sys_env.uart0_cmd_flag == 0){
		if (data == '\b'){
			if (rec_count > 0){
				putchar('\b');
				putchar(' ');
				putchar('\b');
				rec_count--;
				cmd_analyze.rec_buf[rec_count] = ' ';
			}
			return;
		}else if (data == ':'){
			sys_env.tty_mode = 0xaa;
			sys_env.update_flag = UART_COMMAND;
			cmd_analyze.rec_buf[rec_count++] = data; 
			sys_env.tty_online_ms = TTY_ONLINE_TIME;
		}else{
			putchar(data);                                              
			if(0x0D == data){// && 0x0D==cmd_analyze.rec_buf[rec_count-1]) 
				if (rec_count > 0){				
					cmd_analyze.rec_buf[rec_count] = '\0';  
					rec_count=0;    
				}	  
				putchar('\n');
				//sys_env.uart0_cmd_flag = 1;	  
				run_command (cmd_analyze.rec_buf, 0);
				cmd();
			}else{                                    
				cmd_analyze.rec_buf[rec_count] = data;                                                 
				rec_count++;                                                                   																   
			   /*防御性代码，防止数组越界*/                                                    
			   if(rec_count>=CMD_BUF_LEN) {                                                                               
				   rec_count=0;                                                                
			   }                                                                               
			} 
		}		
	}else{
		rec_count = 0;
	}
}                                                                                      
 


int do_task_manager (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int test1, test2, test3;
	switch (argc) {
	case 1 :
		cy_println ("no arg");
		break;
	case 2 :
		test1 = simple_strtol (argv[1], NULL, 16);
		cy_println ("test1 = 0x%x", test1);
		break;
	case 3 :
		test1 = simple_strtol (argv[1], NULL, 16);
		test2 = simple_strtol (argv[2], NULL, 10);
		cy_println ("test1 = 0x%x, test2 = %d", test1, test2);
		break;
	case 4 :
		test1 = simple_strtol (argv[1], NULL, 16);
		test2 = simple_strtol (argv[2], NULL, 10);
		test3 = simple_strtol (argv[3], NULL, 10);
		cy_println ("test1 = 0x%x, test2 = %d, test3 = %d", test1, test2, test3);
		break;
	default:
		cy_print ("Usage:%s", cmdtp->usage);
		return 1;
	}
	return 0;
}

MY_CMD(
	task_manager,	4,	0,	do_task_manager,
	"task_manager - task_manager from USB device\r\n",
	"task_manager dev:part\r\n"
);


int do_print_version  (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{	
	cy_println("\r\n----------------------------------------------------");
	PRINT_VERSION()
	cy_println("----------------------------------------------------");   
	return 0;
}
MY_CMD(
	version,	4,	1,	do_print_version,
	"version - print version information\r\n",
	"version\r\n"
);



int do_help (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);

MY_CMD(
	help,	CFG_MAXARGS,	0,	do_help,
 	"help    - print online help\r\n",
 	"[command ...]\r\n"
 	"    - show help information (for 'command')\r\n"
 	"'help' prints online help for the monitor commands.\r\n\n"
 	"Without arguments, it prints a short usage message for all commands.\r\n\n"
 	"To get detailed help information for specific commands you can type\r\n"
  "'help' with one or more command names as arguments.\r\n"
);

/* This do not ust the U_BOOT_CMD macro as ? can't be used in symbol names */
#ifdef  CFG_LONGHELP
cmd_tbl_t __u_my__cmd_question_mark Struct_Section = {
	"?",	CFG_MAXARGS,	0,	do_help,
 	"?       - alias for 'help'\r\n",
	NULL
};
#else
cmd_tbl_t __u_my__cmd_question_mark Struct_Section = {
	"?",	CFG_MAXARGS,	0,	do_my_help,
 	"?       - alias for 'help'\r\n"
};
#endif /* CFG_LONGHELP */
int do_help (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int i;
	int rcode = 0;
	cmdtp = &__my_cmd_help; //加上这一句防止my_cmd_sec这个Section 被编译器优化掉
	if (argc == 1)
	{	/*show list of commands */

		int cmd_items = (unsigned int)MY_CMD_RO_SEC_LENGTH / sizeof (cmd_tbl_t);	/* pointer arith! */
		cmd_tbl_t *cmd_array[cmd_items];
		int i, j, swaps;

		/* Make array of commands from .uboot_cmd section */
		cmdtp = (cmd_tbl_t *)MY_CMD_RO_SEC_START;
		for (i = 0; i < cmd_items; i++) {
			cmd_array[i] = cmdtp++;
		}

		/* Sort command list (trivial bubble sort) */
		for (i = cmd_items - 1; i > 0; --i) {
			swaps = 0;
			for (j = 0; j < i; ++j) {
				if (strcmp (cmd_array[j]->name,
					    cmd_array[j + 1]->name) > 0) {
					cmd_tbl_t *tmp;
					tmp = cmd_array[j];
					cmd_array[j] = cmd_array[j + 1];
					cmd_array[j + 1] = tmp;
					++swaps;
				}
			}
			if (!swaps)
				break;
		}

		/* print short help (usage) */
		for (i = 0; i < cmd_items; i++) {
			const char *usage = cmd_array[i]->usage;

			/* allow user abort */
			if (had_ctrlc ())
				return 1;
			if (usage == NULL)
				continue;
			cy_puts (usage);
		}
		return 0;
	}
	/*
	 * command help (long version)
	 */
	for (i = 1; i < argc; ++i) {
		if ((cmdtp = find_cmd (argv[i])) != NULL) {
#ifdef	CFG_LONGHELP
			/* found - print (long) help info */
			cy_puts (cmdtp->name);
			cy_putc (' ');
			if (cmdtp->help) {
				cy_puts (cmdtp->help);
			} else {
				cy_puts ("- No help available.\r\n");
				rcode = 1;
			}
			cy_putc ('\n');
#else	/* no long help available */
			if (cmdtp->usage)
				cy_puts (cmdtp->usage);
#endif	/* CFG_LONGHELP */
		} else {
			cy_print ("Unknown command '%s' - try 'help'"
				" without arguments for list of all"
				" known commands\r\n", argv[i]
					);
			rcode = 1;
		}
	}
	return rcode;
}


/***************************************************************************
 * find command table entry for a command
 */
cmd_tbl_t *find_cmd (const char *cmd)
{
	cmd_tbl_t *cmdtp;
	cmd_tbl_t *cmdtp_temp = (cmd_tbl_t *)MY_CMD_RO_SEC_START;	/*Init value */
	const char *p;
	int len;
	int n_found = 0;

	/*
	 * Some commands allow length modifiers (like "cp.b");
	 * compare command name only until first dot.
	 */
	len = ((p = strchr(cmd, '.')) == NULL) ? strlen (cmd) : (p - cmd);

	for (cmdtp = (cmd_tbl_t *)MY_CMD_RO_SEC_START;
	     cmdtp != (cmd_tbl_t *)MY_CMD_RO_SEC_END;
	     cmdtp++) {
		if (strncmp (cmd, cmdtp->name, len) == 0) {
			if (len == strlen (cmdtp->name))
				return cmdtp;	/* full match */

			cmdtp_temp = cmdtp;	/* abbreviated command ? */
			n_found++;
		}
	}
	if (n_found == 1) {			/* exactly one match */
		return cmdtp_temp;
	}

	return NULL;	/* not found or ambiguous command */
}

int had_ctrlc (void)
{
	return sys_env.sys_break;
}

void clear_ctrlc (void)
{
	sys_env.sys_break = 0;
}
/****************************************************************************/

int parse_line (char *line, char *argv[])
{
	int nargs = 0;

#ifdef DEBUG_PARSER
	cy_print ("parse_line: \"%s\"\n", line);
#endif
	while (nargs < CFG_MAXARGS) {

		/* skip any white space */
		while ((*line == ' ') || (*line == '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		cy_print ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
		cy_print ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	cy_print ("** Too many args (max. %d) **\n", CFG_MAXARGS);

#ifdef DEBUG_PARSER
	cy_print ("parse_line: nargs=%d\n", nargs);
#endif
	return (nargs);
}

/****************************************************************************/

static void process_macros (const char *input, char *output)
{
	char c, prev;
	const char *varname_start = NULL;
	int inputcnt = strlen (input);
	int outputcnt = CFG_CBSIZE;
	int state = 0;		/* 0 = waiting for '$'  */

	/* 1 = waiting for '(' or '{' */
	/* 2 = waiting for ')' or '}' */
	/* 3 = waiting for '''  */
#ifdef DEBUG_PARSER
	char *output_start = output;

	cy_print ("[PROCESS_MACROS] INPUT len %d: \"%s\"\n", strlen (input),
		input);
#endif

	prev = '\0';		/* previous character   */

	while (inputcnt && outputcnt) {
		c = *input++;
		inputcnt--;

		if (state != 3) {
			/* remove one level of escape characters */
			if ((c == '\\') && (prev != '\\')) {
				if (inputcnt-- == 0)
					break;
				prev = c;
				c = *input++;
			}
		}

		switch (state) {
		case 0:	/* Waiting for (unescaped) $    */
			if ((c == '\'') && (prev != '\\')) {
				state = 3;
				break;
			}
			if ((c == '$') && (prev != '\\')) {
				state++;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		case 1:	/* Waiting for (        */
			if (c == '(' || c == '{') {
				state++;
				varname_start = input;
			} else {
				state = 0;
				*(output++) = '$';
				outputcnt--;

				if (outputcnt) {
					*(output++) = c;
					outputcnt--;
				}
			}
			break;
		case 2:	/* Waiting for )        */
			if (c == ')' || c == '}') {
				int i;
				char envname[CFG_CBSIZE], *envval;
				int envcnt = input - varname_start - 1;	/* Varname # of chars */

				/* Get the varname */
				for (i = 0; i < envcnt; i++) {
					envname[i] = varname_start[i];
				}
				envname[i] = 0;

				/* Get its value */
				envval = getenv (envname);

				/* Copy into the line if it exists */
				if (envval != NULL)
					while ((*envval) && outputcnt) {
						*(output++) = *(envval++);
						outputcnt--;
					}
				/* Look for another '$' */
				state = 0;
			}
			break;
		case 3:	/* Waiting for '        */
			if ((c == '\'') && (prev != '\\')) {
				state = 0;
			} else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		}
		prev = c;
	}

	if (outputcnt)
		*output = 0;

#ifdef DEBUG_PARSER
	cy_print ("[PROCESS_MACROS] OUTPUT len %d: \"%s\"\n",
		strlen (output_start), output_start);
#endif
}

/****************************************************************************
 * returns:
 *	1  - command executed, repeatable
 *	0  - command executed but not repeatable, interrupted commands are
 *	     always considered not repeatable
 *	-1 - not executed (unrecognized, bootd recursion or too many args)
 *           (If cmd is NULL or "" or longer than CFG_CBSIZE-1 it is
 *           considered unrecognized)
 *
 * WARNING:
 *
 * We must create a temporary copy of the command since the command we get
 * may be the result from getenv(), which returns a pointer directly to
 * the environment data, which may change magicly when the command we run
 * creates or modifies environment variables (like "bootp" does).
 */

int run_command (const char *cmd, int flag)
{
	cmd_tbl_t *cmdtp;
	char cmdbuf[CFG_CBSIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char finaltoken[CFG_CBSIZE];
	char *str = cmdbuf;
	char *argv[CFG_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int repeatable = 1;
	int rc = 0;

#ifdef DEBUG_PARSER
	cy_print ("[RUN_COMMAND] cmd[%p]=\"", cmd);
	cy_puts (cmd ? cmd : "NULL");	/* use cy_puts - string may be loooong */
	cy_puts ("\"\r\n");
#endif

	clear_ctrlc();		/* forget any previous Control C */

	if (!cmd || !*cmd) {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CFG_CBSIZE) {
		cy_puts ("## Command too long!\r\n");
		return -1;
	}

	strcpy (cmdbuf, cmd);

	/* Process separators and check for invalid
	 * repeatable commands
	 */

#ifdef DEBUG_PARSER
	cy_print ("[PROCESS_SEPARATORS] %s\r\n", cmd);
#endif
	while (*str) {

		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep=='\'') &&
			    (*(sep-1) != '\\'))
				inquotes=!inquotes;

			if (!inquotes &&
			    (*sep == ';') &&	/* separator		*/
			    ( sep != str) &&	/* past string start	*/
			    (*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
			str = sep;	/* no more commands for next pass */
#ifdef DEBUG_PARSER
		cy_print ("token: \"%s\"\r\n", token);
#endif

		/* find macros in this token and replace them */
		process_macros (token, finaltoken);

		/* Extract arguments */
		if ((argc = parse_line (finaltoken, argv)) == 0) {
			rc = -1;	/* no command at all */
			continue;
		}

		/* Look up command in command table */
		if ((cmdtp = find_cmd(argv[0])) == NULL) {
			cy_print ("Unknown my_cmd command '%s'\r\n", argv[0]);
			rc = -1;	/* give up after bad command */
			continue;
		}

		/* found - check max args */
		if (argc > cmdtp->maxargs) {
			cy_print ("Usage:\n%s\n", cmdtp->usage);
			rc = -1;
			continue;
		}


		/* OK - call function to do the command */
		if ((cmdtp->cmdhandle) (cmdtp, flag, argc, argv) != 0) {
			rc = -1;
		}

		repeatable &= cmdtp->repeatable;

		/* Did the user stop this? */
		if (had_ctrlc ())
			return 0;	/* if stopped then not repeatable */
	}

	return rc ? rc : repeatable;
}


                                                                












