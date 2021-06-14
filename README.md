## Builtins implemented:
* cd 
* pwd
* ls
* echo
* pinfo
* nightswatch
* history
* kjob 
* jobs
* overkill
* bg 
* fg
* setenv
* unsetenv


### Rest of the commands implemented through execvp (Background+Foreground processes)
### Background processe's exit status displayed when they get terminated.
### piping and redirection inculded.
### Execution:
* Run make command: make.
* Run ./shell command to launch the shell.

### Features:
* Shell prompt is displayed as <username@hostname:PWD>. PWD is displayed relative to HOME directory, which is set as the directory in which the shell is initially started.
* Semi-colon separated list of commands.
* Input/Output Redirection & Piping.
* Background/Foreground processes.
* Signal Handling(Ctrl-C, Ctrl-z, Ctrl-D).
* If the ‘UP’ key is pressed ‘K’ times consecutively, the K​th ​previous command should be executed.
* If the ‘DOWN’ key is pressed ‘K’ times consecutively, the K​th ​future command should be executed.
### Commands implemented:
* `pwd` : returns present working directory.
* `cd [PATH_NAME]` : changes current working directory to the specified directory, if it exists. Otherwise, error is shown.
* echo : Displays a string on the console.
* `ls [OPTIONS] [PATH]` : displays information about the directory specified in PATH, if it exists, or about the PWD.
  Options: 
    1) `-l` : displays detailed information.
    2) `-a` : displays hidden files.
* `pinfo [PID]` : displays process information about the shell's process, or PID if specified.
* `setenv [VAR] [value]` : Sets value of environment variable VAR to value. If variable does not exist, it is created and initialised.
* `unsetenv [VAR]` : destroys the environment variable VAR.
* `jobs` : prints a list of all currently running jobs.
* `kjob [jobNumber] [signalNumber]` : takes the job id of a running job and sends a signal value to that process.
* `fg [jobNumber]` : brings a running or a stopped background job with given job number to foreground.
* `bg [jobNumber]` : changes a stopped background job to a running background job.
* `overkill` : kills all background process at once.
* `nightswatch [​options​] <​Command​>`: 
    `[Command]` is either interrupt or dirty. 
    
    Command:
      1) `interrupt`:  Prints the number of times the CPU(s) has(ve) been interrupted by the ​keyboard controller (i8042 with ​IRQ 1​) every 'n' seconds specified by -n flag, until key 'q' is pressed.
      2) `dirty`:  Prints the size of the part of the memory which is ​dirty​.
* `history [num]`: Ouputs the most recent 'num' commands executed on the shell.
