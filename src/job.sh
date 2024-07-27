#!/bin/sh

g++ -O2 search_truePareto.cpp

echo -n "offset_value_lower = "
read offset_value_lower

echo -n "offset_value_upper = "
read offset_value_upper

echo -n "max_jobn = "
read max_jobn

wait_until_jobn_lt () {
  local max_jobn=$1
  while [[ "$(jobs | wc -l)" -ge "$max_jobn" ]]
  do
    sleep 1
  done
}

exec_job () {
  local title="job #$1"
  echo "$(date) starting $title"
  ./a.out $1
  echo "$(date) finished $title"
}

for i in `seq $offset_value_lower $offset_value_upper`
do
  wait_until_jobn_lt $max_jobn
  exec_job $i &
done

wait
echo "all finished"
