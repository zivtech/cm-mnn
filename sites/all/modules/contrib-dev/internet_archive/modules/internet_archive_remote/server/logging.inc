<?php

/**
 * @file
 * Logging class for remote debugging.
 */

/**
 * Logging class, credit to:
 * Darko Bunić @ http://www.redips.net/php/write-to-log-file/
 * - contains lopen and lwrite methods
 * - lwrite will write message to the log file
 * - first call of the lwrite will open log file implicitly
 * - message is written with the following format: 
 *   hh:mm:ss (script name) message
 */
class Logging {
  // define log file
  private $log_file = 'logs/log.txt';
  // define file pointer
  private $fp = NULL;
  // write message to the log file
  public function lwrite($message) {
    // if file pointer doesn't exist, then open log file
    if (!$this->fp) {
      $this->lopen();
    }
    // define script name
    $script_name = pathinfo($_SERVER['PHP_SELF'], PATHINFO_FILENAME);
    // define current time
    $time = date('H:i:s');
    // write current time, script name and message to the log file
    fwrite($this->fp, "$time ($script_name) $message\n");
  }
  // open log file
  private function lopen() {
    if (!is_dir('logs')) {
      mkdir('logs');
    }
    // define log file path and name
    $lfile = $this->log_file;
    // define the current date (it will be appended to the log file name)
    $today = date('Y-m-d');
    // open log file for writing only; place the file pointer at the end of 
    // the file if the file does not exist, attempt to create it
    $this->fp = fopen($lfile . '_' . $today, 'a') or ("Can't open $lfile!");
  }

  public function lclose() {
    if ($this->fp)
     fclose($this->fp);
     $this->fp = NULL;
  }
}
