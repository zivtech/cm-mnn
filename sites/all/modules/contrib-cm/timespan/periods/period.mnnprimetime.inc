<?php

define ('MNN_PRIMETIME_SUNDAY', 7);

function timespan_mnnprimetime_info() {
  return array('id' => 'mnnprimetime',
               'name' => 'MNN prime time',
               );
}

function timespan_current_mnnprimetime($datetime = NULL) {
  if ($datetime) {
    $ts = strtotime($datetime);
  }
  else {
    $ts = mktime();
  }

  list($ts_start, $ts_end) = _timespan_mnnprimetime($ts);

  return array('start' => date('Y-m-d H:i:s', $ts_start),
               'end'   => date('Y-m-d H:i:s', $ts_end),
              );
}

function timespan_next_mnnprimetime($datetime = NULL) {
  if ($datetime) {
    $ts = strtotime($datetime);
  }
  else {
    $ts = mktime();
  }
  
  $ts += 60 * 60 * 24; 

  list($ts_start, $ts_end) = _timespan_mnnprimetime($ts);

  return array('start' => date('Y-m-d H:i:s', $ts_start),
               'end'   => date('Y-m-d H:i:s', $ts_end),
              );

}

function _timespan_mnnprimetime($ts = NULL) {
  if ($ts) {
    $month = date("n", $ts);
    $day   = date("j", $ts);
    $year  = date("Y", $ts);
    $dow   = date("N", $ts);
  }
  else {
    $ts = mktime();
    $month = date("n");
    $day   = date("j");
    $year  = date("Y");
    $dow   = date("N");
  }

  if (MNN_PRIMETIME_SUNDAY == $dow) {
    $ts_start = mktime(18, 0, 0, $month, $day + 1, $year);
    $ts_end   = mktime(22, 0, 0, $month, $day + 1, $year);
  }
  else {
    $ts_start = mktime(18, 0, 0, $month, $day, $year);
    $ts_end   = mktime(22, 0, 0, $month, $day, $year);
  }
  
  return array($ts_start, $ts_end);
  
}

