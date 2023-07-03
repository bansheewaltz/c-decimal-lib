sed 's/.*[FE]:.\{1,8\}:/\t/g' | \
awk '{
    if (NF>7) {
      printf "\t%s res = %s exp = %s\n", $1,$9,$12
    } else {
      print
    }
    }'
