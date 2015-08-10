This directory contains definition files for time periods that can be used by
the timespan module.

A timespan definition file is a PHP include file, so it needs the <?php at the beginning

To create your own definition file:

 1. Create a new file in this directory named period.{myperiod}.inc, where {myperiod}
    is the type of time period you're defining.

 2. Start the file with  <?php
 
 3. Your file must define 3 external functions:
    - timespan_{myperiod}_info - provides info about your period to other functions
    - timespan_current_{myperiod} - returns the current time period
    - timespan_next_{myperiod} - returns the next time period, both taking a string datetime
    argument.

    These last 2 functions must be named timespan_current_{myperiod} and
    timespan_next_{myperiod}, where myperiod matches what you used in the filename.
    
    Here are the example functions for the 'quarter' timespan definition:

      /**
       * Identify our timespan implementation to the timespan.module
       *
       * @return
       *   Array with id and name of the period this file defines
       */
      function timespan_quarter_info() {
        return array('id' => 'quarter',
                     'name' => 'Calendar quarters',
                     );
      }


      /**
       * Given a date string, returns the start and end dates for the quarter that
       * date occurs in.
       *
       * @param $datetime
       *   Datetime string e.g. "2013-06-19" or "2015-07-05 02:18:24"
       *
       * @return
       *   Array with start and end datetimes for the quarter
       */

      function timespan_current_quarter ($datetime = NULL) {
        if ($datetime) {
          $ts = strtotime($datetime);
        }
        else {
          $ts = mktime();
        }
        
        $ts_start = _timespan_current_quarter($ts);
        $ts_end   = _timespan_next_quarter($ts_start) - 1;
        
        return array('start' => date('Y-m-d H:i:s', $ts_start),
                     'end'   => date('Y-m-d H:i:s', $ts_end),
                    );
        
      }
      
      /**
       * Given a date string, returns the start and end dates for the quarter following
       * the quarter that date occurs in.
       *
       * @param $datetime
       *   Datetime string e.g. "2013-06-19" or "2015-07-05 02:18:24"
       *
       * @return
       *   Array with start and end datetimes for the next quarter
       */

      function timespan_next_quarter($datetime = NULL) {
        if ($datetime) {
          $ts = strtotime($datetime);
        }
        else {
          $ts = mktime();
        }
        
        $ts_start = _timespan_next_quarter($ts);
        $ts_end   = _timespan_next_quarter($ts_start) - 1;  // 1 sec before 00:00 on day quarter starts
        
        return array('start' => date('Y-m-d H:i:s', $ts_start),
                     'end'   => date('Y-m-d H:i:s', $ts_end),
                    );
      }      

      Your functions can do any kind of calculations necessary to get the time period
      you're looking for, but they must follow the nomenclature, accept a datetime string
      argument, and return an array with keys 'start' and 'end', and datetime string values.


TODO
----
 1. The /periods subdir of this module should only contain generally useful period
    definitions that ship with the module Ð we should make another location for
    user-defined period .inc files that wonn't be affected by module upgrades.

