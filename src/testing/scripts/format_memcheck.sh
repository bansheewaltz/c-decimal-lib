grep -v '/bin/bash:' |
sed '/.*ERROR SUMMARY/a \ ' |
tee -a $TST_REPORT_R |
grep -e "Invalid read" -e "Conditional jump" -e "Use of uninitialised" \
     -e "SIGSEGV" -e "in use at exit" -e "ERROR SUMMARY" |
sed "s/==[0-9]*== */\t/g"