<?php

function timespan_mnnquarter_info() {
  return array('id' => 'mnnquarter',
               'name' => 'MNN quarters',
               );
}

function timespan_current_mnnquarter($datetime = NULL) {
  if ($datetime) {
    $ts = strtotime($datetime);
  }
  else {
    $ts = mktime();
  }
  
  $ts_start = _timespan_current_mnnquarter($ts);
  $ts_end   = _timespan_next_mnnquarter($ts_start) - 1;  // 1 sec before 00:00 on day quarter starts
  
  return array('start' => date('Y-m-d H:i:s', $ts_start),
               'end'   => date('Y-m-d H:i:s', $ts_end),
              );
  
}

function timespan_next_mnnquarter($datetime = NULL) {
  if ($datetime) {
    $ts = strtotime($datetime);
  }
  else {
    $ts = mktime();
  }
  
  $ts_start = _timespan_next_mnnquarter($ts);
  $ts_end   = _timespan_next_mnnquarter($ts_start) - 1;  // 1 sec before 00:00 on day quarter starts
  
  return array('start' => date('Y-m-d H:i:s', $ts_start),
               'end'   => date('Y-m-d H:i:s', $ts_end),
              );
}

function _timespan_current_mnnquarter($targ = NULL) {
  if ($targ) {
    $now_month = date("n", $targ);
    $now_day = date("j", $targ);
    $now_year = date("Y", $targ);
  }
  else {
    $now_month = date("n");
    $now_day = date("j");
    $now_year = date("Y");
  }
  if ($now_month % 3 == 0) {
    $this_month_quarter = timespan_get_third_sunday($now_month, $now_year);
    if ($this_month_quarter <= $now_day) {
      return mktime(0,0,0, $now_month, timespan_get_third_sunday($now_month, $now_year), $now_year);
    }
    else {
      $now_month -= 3;
    }
  }
  else {
    $now_month -= ($now_month % 3);
  }
  if ($now_month < 1) {
    $now_year--;
    $now_month += 12;
  }
  return mktime(0,0,0, $now_month, timespan_get_third_sunday($now_month, $now_year), $now_year);
}

function _timespan_next_mnnquarter($targ = NULL) {

  //TODO:  we should add these as params to this function:
         //  $periods:  quarters? semesters?  set this to n where 1/n is the length in years of your period
         //  $modulo:  how many months into the year does the new period begin? for periods beginning in December, choose 0
  if ($targ) {
    $now_month = date("n", $targ);
    $now_day = date("j", $targ);
    $now_year = date("Y", $targ);
  }
  else {
    $now_month = date("n");
    $now_day = date("j");
    $now_year = date("Y");
  }

  //TODO change this to use the period and modulo from the TODO above
  if ($now_month % 3 == 0) {  // this month is a quarter change
    $this_month_quarter = timespan_get_third_sunday($now_month, $now_year);
    
    if ($this_month_quarter > $now_day) {
      return mktime(0,0,0, $now_month, timespan_get_third_sunday($now_month, $now_year), $now_year);
    }
    else {
      $now_month += 3;
    }
  }
  else {
    $now_month += (3 - $now_month % 3);
  }
  
  if ($now_month > 12) {
    $now_year++;
    $now_month -= 12;
  }
  
  return mktime(0,0,0, $now_month, timespan_get_third_sunday($now_month, $now_year), $now_year);
}

/** 
 * WARNING: does not actually return the third sunday, but actually returns
 * the second from the last sunday, which is usually the third sunday, but not
 * always.
 *
 * TODO: change the name of this function
 *
 * Params: $month: month as int 1 through 12
 *         $year:  4 digit year
 *         $day_of_week: 0 for sunday, through 6 for saturday
 *         $offset:  0 for 1st, positive #s for 2nd, 3rd, etc.
 *                   negative for last, 2nd from last, etc.
 *
 * Returns:  the day of month of the desired day and week offset (1 through 31)
 *
 * FIXME: change the default offset to 1
 *
 * The offset is used to determine
 */
function timespan_get_third_sunday($month, $year, $day_of_week = 0, $offset = -2) {

  // get the first of the month as seconds from the epoch
  $first = mktime(0, 0, 0, $month, 1, $year);

  // get an empty list of this month's sundays (or whatever day quarter begins)
  $l_suns = array();

  // How many days are in this month?
  $nod = date('t', $first);

  $i = NULL;
  for ($i = 0; $i < $nod; $i++) {
    if (date('w', $first) == $day_of_week) {
      //store this date in our list
      $l_suns[] = $first;
    }
    $first += 24*60*60;
  }
  $o = count($l_suns) + $offset;
  if ($offset < 0) {
    $o = count($l_suns) + $offset;
  }
  else {
    $o = $offset;
  }
  // return day of month (1 through 31)
  return date('j', $l_suns[$o]); 
}
