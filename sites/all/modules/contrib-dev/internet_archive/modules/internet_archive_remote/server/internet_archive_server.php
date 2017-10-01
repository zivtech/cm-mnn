<?php 

//THIS TOKEN MUST MATCH THE ONE AT admin/settings/internet_archive/remote 
define("TOKEN", "your-token-here");

//Logging class for saving log entries to file
require_once('logging.inc');

/**
 * Authenticates SOAP request with the Token defined above.
 */
function internet_archive_server_authenticate() {
  $xml = file_get_contents('php://input');
  if(strpos($xml, md5(TOKEN)) === false) {
    return FALSE;
  }else{
    return TRUE;
  }
}


/**
 * Check some minimum requirements and print to screen if fail.
 */
function internet_archive_server_check_requirements() {
  // Check for CURL
  if (extension_loaded('curl') &&
      !@dl(PHP_SHLIB_SUFFIX == 'so' ? 'curl.so' : 'php_curl.dll')) {
  }else{
    print 'No CURL support found, please make sure CURL is installed ' .
      'properly.<br />';
    exit();
  }

  if (version_compare(phpversion(), '5') >= 0) {
  }
  else{
    print 'PHP version on remote server is too old. This module requires ' .
      'at least PHP5, the server is currently running '. phpversion();
    exit();
  }
}

/**
 * SOAP function, allows clients to test connection & authentication
 */
function testConnection() {
  $log = new Logging();
  $log->lwrite('==============================================');
  if(internet_archive_server_authenticate()) {
    $log->lwrite('Connection Test Successful');
    return 'archive-server-authenticated';
  }
  else{
    $log->lwrite('Connection Test Failed Authentication');
    return 'Authentication failed, invalid token.';
  }
}

/**
 * SOAP function, executs putObject via the internet archive s3 library
 */
function putObject($request) {
  $log = new Logging();
  $log->lwrite('==============================================');

  if(internet_archive_server_authenticate()) {
    $request = unserialize($request);
    $filepath = $request['filepath'];
    $bucket = $request['bucket'];
    $uri = $request['uri'];
    $headers = $request['headers'];
    $mimetype = $request['mimetype'];
    
    $log->lwrite('Filepath:'.$filepath);
    $log->lwrite('Bucket:'.$bucket);
    $log->lwrite('URI:'.$uri);
    $log->lwrite('Headers:'.print_r($headers, TRUE));
    $log->lwrite('Mimetype:'.$mimetype);

    //include our Archive.org S3 class
    require_once 'archive_remote.php';

    $s3 = new S3($request['key'], $request['skey']);
    $response = $s3->putObjectFile($filepath, $bucket, $uri, $headers,
				   $mimetype);

    return $response;
  }
  else{
    $log->lwrite('Failed Authentication, quitting');
    return;
  }
}

//check basic requirements, exit if failure.
internet_archive_server_check_requirements();

//disable caching while in development
ini_set("soap.wsdl_cache_enabled", "0"); // disabling WSDL cache

//initiate SoapServer using the WSDL file
$server = new SoapServer("internet_archive_server.wsdl"); 
$server->addFunction("putObject"); 
$server->addFunction("testConnection"); 
$server->handle(); 

?>
