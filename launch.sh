#!/bin/bash
clear
sudo rm ~/repositories.repo*; sudo zypper lr -e ~/repositories.repo && sudo chown kLeZ-hAcK:users ~/repositories.repo
~/projects/repo-converter/build/repo-converter -c 11.3 -n 11.4 -f ~/repositories.repo $@
ls -lh ~/repositories*
