# Untitled Projects
These project are more of PoC that I created when I was learning a specific topic in Linux. Some are useful for redteaming, others are just trollling tools.

## troll.c
This program is meant to simulate(emulate?) a shell like bash. Except it does nothing. Literally nothing. But if the redteam happens to be on the shell, if they enter the magic word, "bingus", it should spawn a root bash shell. In order to prevent some clever escape like using signals, I have a signal handler that watches for ctrl + c and ctrl + z.

### How to use 
Place the binary under /bin/ or /usr/bin to avoid suspicious. Then replace your target user's shell by using `chsh` or editing the `/etc/passwd` to have the path of the binary rather than /bin/bash. 
Have fun :)

## Gatekeeper.c
This program watches specific directory for file creation, modified deletion and any form of alteration. The goal is to monitor any changes that might occur on redteam tools and for instance, if deleted, it would download it back. But there's current a bug/issues(stated in the program description) and it doesn't fully work.
