report_raw="$1"
report="$2"

sed -e 's/.*file://g' -e 's/ Assertion.*failed://g' $report_raw |\
awk '{if (NF==7) printf "\t%03d:  %s = %s %s = %s\n",$1,$2,$4,$5,$7;
      else if (NF == 3) {print;getline;print;} else print;}' > $report
# cat $report
