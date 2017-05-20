<?php
/*  
 * Copyright (c) 2010 Brian Hiatt <brian@openmediafoundation.org>  
 *  
 *  This file is free software: you may copy, redistribute and/or modify it  
 *  under the terms of the GNU General Public License as published by the  
 *  Free Software Foundation, either version 2 of the License, or (at your  
 *  option) any later version.  
 *  
 *  This file is distributed in the hope that it will be useful, but  
 *  WITHOUT ANY WARRANTY; without even the implied warranty of  
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  
 *  General Public License for more details.  
 *  
 *  You should have received a copy of the GNU General Public License  
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 *  
 * This file incorporates work covered by the following copyright and  
 * permission notice:  
 * 
 * Copyright (c) 2008, Donovan Schönknecht.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Amazon S3 is a trademark of Amazon.com, Inc. or its affiliates.
 */

//TODO: remove the duplicative amzSetHeader 
//TODO: test item deletion
//TODO: test bucket listing

/**
* Archive.org S3 PHP class
*
* Originally based on:
* @link http://undesigned.org.za/2007/10/22/amazon-s3-php-class
* Heavily modified by brian@openmediafoundation.org for:
* @link http://www.drupal.org/project/internet_archive
*/
class S3 {

  private static $__accessKey; // Archive.org Access key
  private static $__secretKey; // Archive.org Secret key
  private static $transfer_id; // Internet Archive Transfer ID

  /**
  * Constructor - if you're not using the class statically
  *
  * @param string $accessKey Access key
  * @param string $secretKey Secret key
  * @return void
  */
  public function __construct($accessKey = null, $secretKey = null) {
    if ($accessKey !== null && $secretKey !== null)
      self::setAuth($accessKey, $secretKey);
  }


  /**
  * Set Archive.org access key and secret key
  *
  * @param string $accessKey Access key
  * @param string $secretKey Secret key
  * @return void
  */
  public static function setAuth($accessKey, $secretKey) {
    self::$__accessKey = $accessKey;
    self::$__secretKey = $secretKey;
  }


  public static function setTransferId($id) {
    self::$transfer_id = $id;
  }

  public static function getTransferId() {
    return self::$transfer_id;
  }

 /**
  * Check connection to S3
  *
  * ADDED BY ARTHUR
  *
  * @param boolean $detailed Returns response if error, false if ok
  * @return array | false
  */
  public static function checkKeys() {
    $rest = new S3Request('GET', '', '');
    $rest = $rest->getResponse();
    if ($rest->error === false && $rest->code !== 200)
      return array('code' => $rest->code, 'message' => 'Unexpected HTTP status');
    if ($rest->error !== false) {
      return $rest->error;
    }
    return false;
  }

  /**
  * Get a list of buckets
  *
  * @param boolean $detailed Returns detailed bucket list when true
  * @return array | false
  */
  public static function listBuckets($detailed = false) {
    $rest = new S3Request('GET', '', '');
    $rest = $rest->getResponse();
    if ($rest->error === false && $rest->code !== 200)
      $rest->error = array('code' => $rest->code, 'message' => 'Unexpected HTTP status');
    if ($rest->error !== false) {
      trigger_error(sprintf("S3::listBuckets(): [%s] %s", $rest->error['code'], $rest->error['message']), E_USER_WARNING);
      return false;
    }
    $results = array();
    if (!isset($rest->body->Buckets)) return $results;

    if ($detailed) {
      if (isset($rest->body->Owner, $rest->body->Owner->ID, $rest->body->Owner->DisplayName))
      $results['owner'] = array(
        'id' => (string)$rest->body->Owner->ID, 'name' => (string)$rest->body->Owner->ID
      );
      $results['buckets'] = array();
      foreach ($rest->body->Buckets->Bucket as $b)
        $results['buckets'][] = array(
          'name' => (string)$b->Name, 'time' => strtotime((string)$b->CreationDate)
        );
    } 
    else
      foreach ($rest->body->Buckets->Bucket as $b) $results[] = (string)$b->Name;

    return $results;
  }


  /*
  * Get contents for a bucket
  *
  * If maxKeys is null this method will loop through truncated result sets
  *
  * @param string $bucket Bucket name
  * @param string $prefix Prefix
  * @param string $marker Marker (last file listed)
  * @param string $maxKeys Max keys (maximum number of keys to return)
  * @param string $delimiter Delimiter
  * @param boolean $returnCommonPrefixes Set to true to return CommonPrefixes
  * @return array | false
  */
  public static function getBucket($bucket, $prefix = null, $marker = null, $maxKeys = null, $delimiter = null, $returnCommonPrefixes = false) {
    $rest = new S3Request('GET', $bucket, '');
    if ($prefix !== null && $prefix !== '') $rest->setParameter('prefix', $prefix);
    if ($marker !== null && $marker !== '') $rest->setParameter('marker', $marker);
    if ($maxKeys !== null && $maxKeys !== '') $rest->setParameter('max-keys', $maxKeys);
    if ($delimiter !== null && $delimiter !== '') $rest->setParameter('delimiter', $delimiter);
    $response = $rest->getResponse();
    if ($response->error === false && $response->code !== 200)
      $response->error = array('code' => $response->code, 'message' => 'Unexpected HTTP status');
    if ($response->error !== false) {
      if (variable_get('internet_archive_debug', FALSE)) {
           watchdog('internet_archive', 'No information found for bucket: ' . $bucket,NULL, WATCHDOG_NOTICE); }
      return false;
    }

    $results = array();

    $nextMarker = null;
    if (isset($response->body, $response->body->Contents))
    foreach ($response->body->Contents as $c) {
      $results[(string)$c->Key] = array(
        'name' => (string)$c->Key,
        'time' => strtotime((string)$c->LastModified),
        'size' => (int)$c->Size,
        'hash' => substr((string)$c->ETag, 1, -1)
      );
      $nextMarker = (string)$c->Key;
    }

    if ($returnCommonPrefixes && isset($response->body, $response->body->CommonPrefixes))
      foreach ($response->body->CommonPrefixes as $c)
        $results[(string)$c->Prefix] = array('prefix' => (string)$c->Prefix);

    if (isset($response->body, $response->body->IsTruncated) &&
    (string)$response->body->IsTruncated == 'false') return $results;

    if (isset($response->body, $response->body->NextMarker))
      $nextMarker = (string)$response->body->NextMarker;

    // Loop through truncated results if maxKeys isn't specified
    if ($maxKeys == null && $nextMarker !== null && (string)$response->body->IsTruncated == 'true')
    do {
      $rest = new S3Request('GET', $bucket, '');
      if ($prefix !== null && $prefix !== '') $rest->setParameter('prefix', $prefix);
      $rest->setParameter('marker', $nextMarker);
      if ($delimiter !== null && $delimiter !== '') $rest->setParameter('delimiter', $delimiter);

      if (($response = $rest->getResponse(true)) == false || $response->code !== 200) break;

      if (isset($response->body, $response->body->Contents))
      foreach ($response->body->Contents as $c) {
        $results[(string)$c->Key] = array(
          'name' => (string)$c->Key,
          'time' => strtotime((string)$c->LastModified),
          'size' => (int)$c->Size,
          'hash' => substr((string)$c->ETag, 1, -1)
        );
        $nextMarker = (string)$c->Key;
      }

      if ($returnCommonPrefixes && isset($response->body, $response->body->CommonPrefixes))
        foreach ($response->body->CommonPrefixes as $c)
          $results[(string)$c->Prefix] = array('prefix' => (string)$c->Prefix);

      if (isset($response->body, $response->body->NextMarker))
        $nextMarker = (string)$response->body->NextMarker;

    } while ($response !== false && (string)$response->body->IsTruncated == 'true');

    return $results;
  }


  /**
  * Put a bucket
  *
  * @param string $bucket Bucket name
  * @param string $location Set as "EU" to create buckets hosted in Europe
  * @return boolean
  */
  public static function putBucket($bucket, $metaHeaders = NULL,
				   $location = false) {
    dsm('S3 putBucket');
    $rest = new S3Request('PUT', $bucket, '');
    dsm($rest, 'S3 rest');
    
    //TODO: remove irrelevant location information
    $dom = new DOMDocument;
    $createBucketConfiguration =
      $dom->createElement('CreateBucketConfiguration');

    dsm($createBucketConfiguration, 'crate bucket config');
    
    $locationConstraint =
      $dom->createElement('LocationConstraint', strtoupper($location));
    
    $createBucketConfiguration->appendChild($locationConstraint);
    $dom->appendChild($createBucketConfiguration);
    $rest->data = $dom->saveXML();
    $rest->size = strlen($rest->data);
    $rest->setHeader('Content-Type', 'application/xml');
    
    //brian@openmediafoundation.org: modification, archive expects
    //content-length
    $rest->setHeader('Content-Length', strlen($rest->data)); 
    if ($metaHeaders) {
      foreach ($metaHeaders as $h => $v) $rest->setAmzHeader($h, $v);
    }
    dsm($rest, 'S3 rest before response');
    $rest = $rest->getResponse();
    dsm($rest, 'S3 rest after response');
    dsm($rest->code, 'rest code');
    switch ($rest->code) {
      case 200:
      case 201:
        if (variable_get('internet_archive_debug', FALSE)) {
           watchdog('internet_archive', 'Item: ' . $bucket .
		    ' created successfully (http 200)',NULL, WATCHDOG_NOTICE);
	}
        if (is_numeric(self::$transfer_id)) {
          $log_entry =
	    array(
		  'tid' => self::$transfer_id,
		  'message' =>
		  'Archive.org Item: ' . $bucket . ' created successfully',
		  'message_data' => array(
					  'bucket' => $bucket,
					  'metaheaders' => $metaHeaders,
					  'response' => $rest,
					  ),
		  'type' => 3,
		  );
          internet_archive_log($log_entry);
        }
        break;
      case 400:
        watchdog('internet_archive', 'Item: ' . $bucket .
		 ' could not be created, (http 400). Bad request can ' .
		 'suggest a problem with the metadata being attached to ' .
		 'the item.',NULL, WATCHDOG_ERROR);

        if (is_numeric(self::$transfer_id)) {
          $log_entry =
	    array(
		  'tid' => self::$transfer_id,
		  'message' => 'Item: ' . $bucket . ' could not be created. '.
		  'Error returned was Bad Request. This can suggest a ' .
		  'problem with the metadata being attached to the item.',
		  'type' => 1,
		  'message_data' => array(
					  'bucket' => $bucket,
					  'metaheaders' => $metaHeaders,
					  'response' => $rest,
					  ),
		  
		  );
          internet_archive_log($log_entry);
        }
        return false;
        break;    
      case 409:
        //TODO: add support to check if item is owned with S3 credentials,
	//if not append number to item name and try again.
        if (variable_get('internet_archive_debug', FALSE)) {
	  watchdog('internet_archive', 'Item: ' . $bucket .
		   ' already exists, unable to create (http 409). This is '.
		   'only a problem if the existing bucket is not writable '.
		   'by this account.',NULL, WATCHDOG_WARNING); }
        if (is_numeric(self::$transfer_id)) {
          $log_entry =
	    array(
		  'tid' => self::$transfer_id,
		  'message' => 'Item: ' . $bucket . ' already exists, ' .
		  'unable to create. This probably means an item by this '.
		  'name already existed at Archive.org and is only a ' .
		  'problem if the item is not writable by your account',
		  'message_data' => array(               
					  'item' => $bucket,
					  'metaheaders' => $metaHeaders,
					  'response' => $rest,
							 ),
		  'type' => 2,
		  );
          internet_archive_log($log_entry);
        }
        break;
      case 403:
        watchdog('internet_archive', 'Item: ' . $bucket .
		 ' could not be created, (http 403). Access denied can ' .
		 'suggest a problem with your S3 credentials or metadata '.
		 'being attached to the item, or you may have specified a '.
		 'collection you do not have access to.',NULL, WATCHDOG_ERROR);
        if (is_numeric(self::$transfer_id)) {
          $log_entry =
	    array(
		  'tid' => self::$transfer_id,            
		  'message' => 'Item: ' . $bucket . ' could not be created. '.
		  'Error returned was Access Denied. This can suggest a '.
		  'problem with your S3 credentials or metadata being ' .
		  'attached to the item, or you may have specified a ' .
		  'collection you do not have access to',
		  'message_data' => array(               
					  'item' => $bucket,  
					  'metaheaders' => $metaHeaders,
					  'response' => $rest,
							 ),
		  'type' => 1,
		  ); 
          internet_archive_log($log_entry);
        }
        return false;
        break;    
      case 500:
        watchdog('internet_archive', 'Item: ' . $bucket .
		 ' could not be created, (http 500). This error can suggest '.
		 'there is something wrong at the Internet Archive. Check '.
		 'their blog or twitter feed to see if they have posted '.
		 'about any maintenance updates.', NULL, WATCHDOG_ERROR);
        if (is_numeric(self::$transfer_id)) {
          $log_entry =
	    array(
		  'tid' => self::$transfer_id,
		  'message' => 'Item: ' . $bucket . ' could not be created. '.
		  'The error returned suggests there may be something wrong '.
		  'at the Internet Archive. Check their blog or twitter ' .
		  'feed to see if they have posted about any maintenance '.
		  'updates recently',
		  'message_data' => array(
					  'item' => $bucket,            
					  'metaheaders' => $metaHeaders,
					  'response' => $rest,
					  ),
		  'type' => 1,
		  );
          internet_archive_log($log_entry);
        }
        return false;
        break;
    }
    
    return true;
  }

  /**
  * Create input info array for putObject()
  *
  * @param string $file Input file
  * @param mixed $md5sum Use MD5 hash (supply a string if you want to use your own)
  * @return array | false
  */
  public static function inputFile($file, $md5sum = true) {
    if (!file_exists($file) || !is_file($file) || !is_readable($file)) {
      trigger_error('S3::inputFile(): Unable to open input file: '.$file, E_USER_WARNING);
      return false;
    }
    return array('file' => $file, 'size' => internet_archive_filesize($file),
    'md5sum' => $md5sum !== false ? (is_string($md5sum) ? $md5sum :
    base64_encode(md5_file($file, true))) : '');
  }


  /**
  * Create input array info for putObject() with a resource
  *
  * @param string $resource Input resource to read from
  * @param integer $bufferSize Input byte size
  * @param string $md5sum MD5 hash to send (optional)
  * @return array | false
  */
  public static function inputResource(&$resource, $bufferSize, $md5sum = '') {
    if (!is_resource($resource) || $bufferSize < 0) {
      trigger_error('S3::inputResource(): Invalid resource or buffer size', E_USER_WARNING);
      return false;
    }
    $input = array('size' => $bufferSize, 'md5sum' => $md5sum);
    $input['fp'] =& $resource;
    return $input;
  }


  /**
  * Put an object
  *
  * @param mixed $input Input data
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @param array $metaHeaders Array of x--meta-* headers
  * @param array $requestHeaders Array of request headers or content type as a string
  * @return boolean
  */
  public static function putObject($input, $bucket, $uri, $metaHeaders = array(), $requestHeaders = array()) {
    if ($input === false) return false;
    $rest = new S3Request('PUT', $bucket, $uri);

    if (is_string($input)) $input = array(
      'data' => $input, 'size' => strlen($input),
      'md5sum' => base64_encode(md5($input, true))
    );

    // Data
    if (isset($input['fp']))
      $rest->fp =& $input['fp'];
    elseif (isset($input['file']))
      $rest->fp = @fopen($input['file'], 'rb');
    elseif (isset($input['data']))
      $rest->data = $input['data'];

    // Content-Length (required)
    if (isset($input['size']) && $input['size'] >= 0)
      $rest->size = $input['size'];
    else {
      if (isset($input['file']))
        $rest->size = internet_archive_filesize($input['file']);
      elseif (isset($input['data']))
        $rest->size = strlen($input['data']);
    }

    // Custom request headers (Content-Type, Content-Disposition, Content-Encoding)
    if (is_array($requestHeaders))
      foreach ($requestHeaders as $h => $v) $rest->setHeader($h, $v);
    elseif (is_string($requestHeaders)) // Support for legacy contentType parameter
      $input['type'] = $requestHeaders;

    // Content-Type
    if (!isset($input['type'])) {
      if (isset($requestHeaders['Content-Type']))
        $input['type'] =& $requestHeaders['Content-Type'];
      elseif (isset($input['file']))
        $input['type'] = self::__getMimeType($input['file']);
      else
        $input['type'] = 'application/octet-stream';
    }

    // We need to post with Content-Length and Content-Type, MD5 is optional
    if ($rest->size >= 0 && ($rest->fp !== false || $rest->data !== false)) {
      $rest->setHeader('Content-Type', $input['type']);
      if (isset($input['md5sum'])) $rest->setHeader('Content-MD5', $input['md5sum']);
      //BRIAN changed metadata tag to archives version
      foreach ($metaHeaders as $h => $v) $rest->setAmzHeader($h, $v);
      $rest = $rest->getResponse();
    } 
    else {
      watchdog('internet_archive', 'Unable to put file: '.$uri.', missing input parameters.',NULL, WATCHDOG_ERROR); 
      if(!$rest->size >= 0) {
        $message = 'Unable to post file to item '.$bucket.' on Archive.org due to : '.$rest->size;
      }elseif($rest->fp !== false || $rest->data !== false) {
        $message = 'Unable to post file to item '.$bucket.' on Archive.org due to a problem accessing the file.';
      }
      $message_data = array(
        'item' => $bucket,
        'uri' => $uri,
        'metaheaders' => $metaHeaders,
        'requestheaders' => $requestHeaders,
        'request_data' => $rest,
      );
      $log_entry = array(
        'tid' => self::$transfer_id,
        'message' => $message,
        'message_data' => $message_data,
        'type' => 1, 
      );
      internet_archive_log($log_entry);
      return false;
    }
     
    switch ($rest->code) {
      case 200:
      case 201:
        $log_entry = array(
          'tid' => self::$transfer_id,
          'message' => 'File: ' . $uri . ' uploaded successfully',
          'message_data' => array(
            'item' => $bucket,
            'uri' => $uri,
            'metaheaders' => $metaHeaders,
            'requestheaders' => $requestHeaders,
            'response' => $rest,
          ),
          'type' => 3,
        );
        internet_archive_log($log_entry);

        if (variable_get('internet_archive_debug', FALSE)) {
           watchdog('internet_archive', 'File: ' . $uri . ' created successfully (http 200)',NULL, WATCHDOG_NOTICE); }
        break;
      case 400:
        watchdog('internet_archive', 'File: ' . $uri . ' could not be created, (http 400). Bad request can suggest a problem with the metadata being attached to the file.',NULL, WATCHDOG_ERROR);
        $log_entry = array(
          'tid' => self::$transfer_id,
          'message' => 'File: ' . $uri . ' could not be created. Error returned was Bad Request. This can suggest a problem with the metadata being attached to the file',
          'message_data' => array(
            'item' => $bucket,
            'uri' => $uri,
            'metaheaders' => $metaHeaders,
            'requestheaders' => $requestHeaders,
            'response' => $rest,
          ),
          'type' => 1,
        );
        internet_archive_log($log_entry);
        return false;
        break;
      case 409:
        //TODO: add support to check if item is owned with S3 credentials, if not append number to item name and try again.
        watchdog('internet_archive', 'File: ' . $uri . ' already exists, unable to create (http 409).',NULL, WATCHDOG_ERROR);
        $log_entry = array(
          'tid' => self::$transfer_id,
          'message' => 'File: ' . $uri . ' already exists, unable to create. Error returned was http 409',
          'message_data' => array(
            'item' => $bucket,
            'uri' => $uri,
            'metaheaders' => $metaHeaders,
            'requestheaders' => $requestHeaders,
            'response' => $rest,
          ),
          'type' => 1,
        );
        internet_archive_log($log_entry);
        return false;
        break;
      case 403:
        watchdog('internet_archive', 'File: ' . $uri . ' could not be created, (http 403). Access denied can suggest a problem with your S3 credentials or metadata being attached to the file.',NULL, WATCHDOG_ERROR);
        $log_entry = array(
          'tid' => self::$transfer_id,
          'message' => 'File: ' . $uri . ' could not be created. Error return was http 403, Access Denied. This suggests a problem with your S3 credentials or metadata being attached to the file',
          'message_data' => array(
            'item' => $bucket,
            'uri' => $uri,
            'metaheaders' => $metaHeaders,
            'requestheaders' => $requestHeaders,
            'response' => $rest,
          ),
          'type' => 1,
        );
        internet_archive_log($log_entry);
        return false;
        break;
    }

    return true;
  }


  /**
  * Put an object from a file (legacy function)
  *
  * @param string $file Input file path
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @param array $metaHeaders Array of x-amz-meta-* headers
  * @param string $contentType Content type
  * @return boolean
  */
  public static function putObjectFile($file, $bucket, $uri, $metaHeaders = array(), $contentType = null) {
    return self::putObject(self::inputFile($file), $bucket, $uri, $metaHeaders, $contentType);
  }


  /**
  * Put an object from a string (legacy function)
  *
  * @param string $string Input data
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @param array $metaHeaders Array of x-amz-meta-* headers
  * @param string $contentType Content type
  * @return boolean
  */
  public static function putObjectString($string, $bucket, $uri, $metaHeaders = array(), $contentType = 'text/plain') {
    return self::putObject($string, $bucket, $uri, $metaHeaders, $contentType);
  }


  /**
  * Get an object
  *
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @param mixed $saveTo Filename or resource to write to
  * @return mixed
  */
  public static function getObject($bucket, $uri, $saveTo = false) {
    $rest = new S3Request('GET', $bucket, $uri);
    if ($saveTo !== false) {
      if (is_resource($saveTo))
        $rest->fp =& $saveTo;
      else
        if (($rest->fp = @fopen($saveTo, 'wb')) !== false)
          $rest->file = realpath($saveTo);
        else
          $rest->response->error = array('code' => 0, 'message' => 'Unable to open save file for writing: '.$saveTo);
    }
    if ($rest->response->error === false) $rest->getResponse();

    if ($rest->response->error === false && $rest->response->code !== 200)
      $rest->response->error = array('code' => $rest->response->code, 'message' => 'Unexpected HTTP status');
    if ($rest->response->error !== false) {
      trigger_error(sprintf("S3::getObject({$bucket}, {$uri}): [%s] %s",
      $rest->response->error['code'], $rest->response->error['message']), E_USER_WARNING);
      return false;
    }
    return $rest->response;
  }


  /**
  * Get object information
  *
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @param boolean $returnInfo Return response information
  * @return mixed | false
  */
  public static function getObjectInfo($bucket, $uri, $returnInfo = true) {
    $rest = new S3Request('HEAD', $bucket, $uri);
    $rest = $rest->getResponse();
    if ($rest->error === false && ($rest->code !== 200 && $rest->code !== 404))
      $rest->error = array('code' => $rest->code, 'message' => 'Unexpected HTTP status');
    if ($rest->error !== false) {
      trigger_error(sprintf("S3::getObjectInfo({$bucket}, {$uri}): [%s] %s",
      $rest->error['code'], $rest->error['message']), E_USER_WARNING);
      return false;
    }
    return $rest->code == 200 ? $returnInfo ? $rest->headers : true : false;
  }

  /**
  * Delete an object
  *
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @return boolean
  */
  public static function deleteObject($bucket, $uri) {
    $rest = new S3Request('DELETE', $bucket, $uri);
    $rest = $rest->getResponse();
    if ($rest->error === false && $rest->code !== 204)
      $rest->error = array('code' => $rest->code, 'message' => 'Unexpected HTTP status');
    if ($rest->error !== false) {
      trigger_error(sprintf("S3::deleteObject(): [%s] %s",
      $rest->error['code'], $rest->error['message']), E_USER_WARNING);
      return false;
    }
    return true;
  }

  /**
  * Delete all files related to an object. 
  *
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @return boolean
  */
  public static function deleteFiles($bucket, $uri) {
    $rest = new S3Request('DELETE', $bucket, $uri);
    $rest->setHeader('x-archive-cascade-delete', '1');
    
    $rest = $rest->getResponse();
 
    if ($rest->error === false && $rest->code !== 204)
      $rest->error = array('code' => $rest->code, 'message' => 'Unexpected HTTP status');
    if ($rest->error !== false) {
      trigger_error(sprintf("S3::deleteObject(): [%s] %s",
      $rest->error['code'], $rest->error['message']), E_USER_WARNING);
      return false;
    }
    return true;
  }

  /**
  * Get a query string authenticated URL
  *
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @param integer $lifetime Lifetime in seconds
  * @param boolean $hostBucket Use the bucket name as the hostname
  * @return string
  */
  public static function getAuthenticatedURL($bucket, $uri, $lifetime, $hostBucket = false) {
    $expires = time() + $lifetime;
    $uri = str_replace('%2F', '/', rawurlencode($uri)); // URI should be encoded (thanks Sean O'Dea)
    $domain = ($hostBucket ? "$bucket.s3.us.archive.org" : "s3.us.archive.org/$bucket");
    return sprintf(($https ? 'https' : 'http').'://%s/%s?AWSAccessKeyId=%s&Expires=%u&Signature=%s',
    $domain, $uri, self::$__accessKey, $expires,
    urlencode(self::__getHash("GET\n\n\n{$expires}\n/{$bucket}/{$uri}")));
  }

  /**
  * Get MIME type for file
  *
  * @internal Used to get mime types
  * @param string &$file File path
  * @return string
  */
  public static function __getMimeType(&$file) {
    $type = false;
    // Fileinfo documentation says fileinfo_open() will use the
    // MAGIC env var for the magic file
    if (extension_loaded('fileinfo') && isset($_ENV['MAGIC']) &&
    ($finfo = finfo_open(FILEINFO_MIME, $_ENV['MAGIC'])) !== false) {
      if (($type = finfo_file($finfo, $file)) !== false) {
        // Remove the charset and grab the last content-type
        $type = explode(' ', str_replace('; charset=', ';charset=', $type));
        $type = array_pop($type);
        $type = explode(';', $type);
        $type = trim(array_shift($type));
      }
      finfo_close($finfo);

    // If anyone is still using mime_content_type()
    } 
    elseif (function_exists('mime_content_type'))
      $type = trim(mime_content_type($file));

    if ($type !== false && strlen($type) > 0) return $type;

    // Otherwise do it the old fashioned way
    static $exts = array(
      'jpg' => 'image/jpeg', 'gif' => 'image/gif', 'png' => 'image/png',
      'tif' => 'image/tiff', 'tiff' => 'image/tiff', 'ico' => 'image/x-icon',
      'swf' => 'application/x-shockwave-flash', 'pdf' => 'application/pdf',
      'zip' => 'application/zip', 'gz' => 'application/x-gzip',
      'tar' => 'application/x-tar', 'bz' => 'application/x-bzip',
      'bz2' => 'application/x-bzip2', 'txt' => 'text/plain',
      'asc' => 'text/plain', 'htm' => 'text/html', 'html' => 'text/html',
      'css' => 'text/css', 'js' => 'text/javascript',
      'xml' => 'text/xml', 'xsl' => 'application/xsl+xml',
      'ogg' => 'application/ogg', 'mp3' => 'audio/mpeg', 'wav' => 'audio/x-wav',
      'avi' => 'video/x-msvideo', 'mpg' => 'video/mpeg', 'mpeg' => 'video/mpeg',
      'mov' => 'video/quicktime', 'flv' => 'video/x-flv', 'php' => 'text/x-php'
    );
    $ext = strtolower(pathInfo($file, PATHINFO_EXTENSION));
    return isset($exts[$ext]) ? $exts[$ext] : 'application/octet-stream';
  }


  /**
  * Generate the auth string: "AWS AccessKey:Signature"
  *
  * @internal Used by S3Request::getResponse()
  * @param string $string String to sign
  * @return string
  */
  public static function __getSignature($string) {
    return 'AWS '.self::$__accessKey.':'.self::__getHash($string);
  }


  /**
  * Creates a HMAC-SHA1 hash
  *
  * This uses the hash extension if loaded
  *
  * @internal Used by __getSignature()
  * @param string $string String to sign
  * @return string
  */
  private static function __getHash($string) {
    return base64_encode(extension_loaded('hash') ?
    hash_hmac('sha1', $string, self::$__secretKey, true) : pack('H*', sha1(
    (str_pad(self::$__secretKey, 64, chr(0x00)) ^ (str_repeat(chr(0x5c), 64))) .
    pack('H*', sha1((str_pad(self::$__secretKey, 64, chr(0x00)) ^
    (str_repeat(chr(0x36), 64))) . $string)))));
  }

}

final class S3Request {
  private $verb, $bucket, $uri, $resource = '', $parameters = array(),
  $amzHeaders = array(), $headers = array(
    'Host' => '', 'Date' => '', 'Content-MD5' => '', 'Content-Type' => ''
  );
  public $fp = false, $size = 0, $data = false, $response;


  /**
  * Constructor
  *
  * @param string $verb Verb
  * @param string $bucket Bucket name
  * @param string $uri Object URI
  * @return mixed
  */
  function __construct($verb, $bucket = '', $uri = '', $defaultHost = 's3.us.archive.org') {
    $this->verb = $verb;
    $this->bucket = strtolower($bucket);
    $this->uri = $uri !== '' ? '/'.str_replace('%2F', '/', rawurlencode($uri)) : '/';

    if ($this->bucket !== '') {
      $this->headers['Host'] = $this->bucket.'.'.$defaultHost;
      $this->resource = '/'.$this->bucket.$this->uri;
    } 
    else {
      $this->headers['Host'] = $defaultHost;
      //$this->resource = strlen($this->uri) > 1 ? '/'.$this->bucket.$this->uri : $this->uri;
      $this->resource = $this->uri;
    }
    $this->headers['Date'] = gmdate('D, d M Y H:i:s T');

    $this->response = new STDClass;
    $this->response->error = false;
  }


  /**
  * Set request parameter
  *
  * @param string $key Key
  * @param string $value Value
  * @return void
  */
  public function setParameter($key, $value) {
    $this->parameters[$key] = $value;
  }


  /**
  * Set request header
  *
  * @param string $key Key
  * @param string $value Value
  * @return void
  */
  public function setHeader($key, $value) {
    $this->headers[$key] = $value;
  }


  /**
  * Set x-amz-meta-* header
  *
  * @param string $key Key
  * @param string $value Value
  * @return void
  */
  public function setAmzHeader($key, $value) {
    $this->amzHeaders[$key] = $value;
  }


  /**
  * Get the S3 response
  *
  * @return object | false
  */
  public function getResponse() {
    $query = '';
    if (sizeof($this->parameters) > 0) {
      $query = substr($this->uri, -1) !== '?' ? '?' : '&';
      foreach ($this->parameters as $var => $value)
        if ($value == null || $value == '') $query .= $var.'&';
        // Parameters should be encoded (thanks Sean O'Dea)
        else $query .= $var.'='.rawurlencode($value).'&';
      $query = substr($query, 0, -1);
      $this->uri .= $query;

      if (array_key_exists('location', $this->parameters) ||
      array_key_exists('torrent', $this->parameters) ||
      array_key_exists('logging', $this->parameters))
        $this->resource .= $query;
    }
    
    //BRIAN CHANGED LINE BELOW
    $url = 'http://'.$this->headers['Host'].$this->uri;
    //var_dump($this->bucket, $this->uri, $this->resource, $url);

    // Basic setup
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_USERAGENT, 'S3/php');

    curl_setopt($curl, CURLOPT_URL, $url);
    if (variable_get('internet_archive_debug', FALSE)) {
       watchdog('internet_archive', 'CURL: Archive S3 URL target  ' . $url,NULL, WATCHDOG_NOTICE); }
    if (variable_get('internet_archive_debug', FALSE)) {
       watchdog('internet_archive', 'CURL: S3 Object  <pre>' . print_r($this, TRUE) . '</pre>',NULL, WATCHDOG_NOTICE); }
    // Headers
    $headers = array(); $amz = array();
    foreach ($this->amzHeaders as $header => $value)
      if (strlen($value) > 0) $headers[] = $header.': '.$value;
    foreach ($this->headers as $header => $value)
      if (strlen($value) > 0) $headers[] = $header.': '.$value;

    // Collect AMZ headers for signature
    foreach ($this->amzHeaders as $header => $value)
      if (strlen($value) > 0) $amz[] = strtolower($header).':'.$value;

    // AMZ headers must be sorted
    if (sizeof($amz) > 0) {
      sort($amz);
      $amz = "\n".implode("\n", $amz);
    } 
    else $amz = '';

    // Authorization string (CloudFront stringToSign should only contain a date)
    $headers[] = 'Authorization: ' . S3::__getSignature(
      $this->headers['Host'] == 'cloudfront.amazonaws.com' ? $this->headers['Date'] :
      $this->verb."\n".$this->headers['Content-MD5']."\n".
      $this->headers['Content-Type']."\n".$this->headers['Date'].$amz."\n".$this->resource
    );
    if (variable_get('internet_archive_debug', FALSE)) {
       watchdog('internet_archive', 'CURL: Headers <pre>' . print_r($headers, TRUE) . '</pre>',NULL, WATCHDOG_NOTICE); }
    
    curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($curl, CURLOPT_HEADER, false);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, false);
    curl_setopt($curl, CURLOPT_WRITEFUNCTION, array(&$this, '__responseWriteCallback'));
    curl_setopt($curl, CURLOPT_HEADERFUNCTION, array(&$this, '__responseHeaderCallback'));
    curl_setopt($curl, CURLOPT_FOLLOWLOCATION, true);
    curl_setopt($curl, CURLOPT_FRESH_CONNECT, 1);
    curl_setopt($curl, CURLOPT_FORBID_REUSE, 1);
    
    //log all curl transactions against transfer
    $log_entry = array(
      'tid' => S3::getTransferId(),
      'message' => 'S3 '.$this->verb.' transaction executing',
      'message_data' => array(
        'target' => $url,
        'headers' => $headers,
        's3_data' => array(
          'item' => $this->bucket,
          'uri' => $this->uri,
          'headers' => $this->headers,
          'size' => $this->size,
          'data' => $this->data,
          'verb' => $this->verb,
          'parameters' => $this->parameters,
          'resource' => $this->resource,
        ),
      ),
      'type' => 3,
    );
    internet_archive_log($log_entry);

    // Request types
    switch ($this->verb) {
      case 'GET': break;
      case 'PUT':
        if ($this->fp !== false) {
          curl_setopt($curl, CURLOPT_PUT, true);
          curl_setopt($curl, CURLOPT_INFILE, $this->fp);
          if ($this->size >= 0)
            curl_setopt($curl, CURLOPT_INFILESIZE, $this->size);
        } 
        elseif ($this->data !== false) {
          curl_setopt($curl, CURLOPT_CUSTOMREQUEST, $this->verb);
          curl_setopt($curl, CURLOPT_POSTFIELDS, $this->data);
          if ($this->size >= 0)
            curl_setopt($curl, CURLOPT_BUFFERSIZE, $this->size);
        } 
        else
          curl_setopt($curl, CURLOPT_CUSTOMREQUEST, $this->verb);
      break;
      case 'HEAD':
        curl_setopt($curl, CURLOPT_CUSTOMREQUEST, 'HEAD');
        curl_setopt($curl, CURLOPT_NOBODY, true);
      break;
      case 'DELETE':
        curl_setopt($curl, CURLOPT_CUSTOMREQUEST, 'DELETE');
      break;
      default: break;
    }

    // Execute, grab errors
    if (curl_exec($curl))
      $this->response->code = curl_getinfo($curl, CURLINFO_HTTP_CODE);
    else
      $this->response->error = array(
        'code' => curl_errno($curl),
        'message' => curl_error($curl),
        'resource' => $this->resource
      );
    
    //log all curl transactions against transfer
    $log_entry = array(
      'tid' => S3::getTransferId(),
      'message' => 'S3 '.$this->verb.' transaction completed',
      'message_data' => array(
        'target' => $url,
        'headers' => $headers,
        's3_data' => array(
          'item' => $this->bucket,
          'uri' => $this->uri,
          'headers' => $this->headers,
          'size' => $this->size,
          'data' => $this->data,
          'verb' => $this->verb,
          'parameters' => $this->parameters,
          'resource' => $this->resource,
        ), 
        'curl_response' => curl_getinfo($curl),
      ),
      'type' => 3,
    );
    internet_archive_log($log_entry);

    if (variable_get('internet_archive_debug', FALSE)) {
       watchdog('internet_archive', 'CURL: Response <pre>'.print_r(curl_getinfo($curl), TRUE).'</pre>',NULL, WATCHDOG_NOTICE); }
    @curl_close($curl);

    // Parse body into XML
    if ($this->response->error === false && isset($this->response->headers['type']) &&
    $this->response->headers['type'] == 'application/xml' && isset($this->response->body)) {
      $this->response->body = simplexml_load_string($this->response->body);

      // Grab S3 errors
      if (!in_array($this->response->code, array(200, 204)) &&
      isset($this->response->body->Code, $this->response->body->Message)) {
        $this->response->error = array(
          'code' => (string)$this->response->body->Code,
          'message' => (string)$this->response->body->Message
        );
        if (isset($this->response->body->Resource))
          $this->response->error['resource'] = (string)$this->response->body->Resource;
        unset($this->response->body);
      }
    }

    // Clean up file resources
    if ($this->fp !== false && is_resource($this->fp)) fclose($this->fp);

    return $this->response;
  }


  /**
  * CURL write callback
  *
  * @param resource &$curl CURL resource
  * @param string &$data Data
  * @return integer
  */
  private function __responseWriteCallback(&$curl, &$data) {
    if (!isset($this->response->body)) {
      $this->response->body = '';
    }
    if ($this->response->code == 200 && $this->fp !== false) {
      return fwrite($this->fp, $data);
    }
    else {
      $this->response->body .= $data;
    }
    return strlen($data);
  }


  /**
  * CURL header callback
  *
  * @param resource &$curl CURL resource
  * @param string &$data Data
  * @return integer
  */
  private function __responseHeaderCallback(&$curl, &$data) {
    if (($strlen = strlen($data)) <= 2) return $strlen;
    if (substr($data, 0, 4) == 'HTTP')
      $this->response->code = (int)substr($data, 9, 3);
    else {
      list($header, $value) = explode(': ', trim($data), 2);
      if ($header == 'Last-Modified')
        $this->response->headers['time'] = strtotime($value);
      elseif ($header == 'Content-Length')
        $this->response->headers['size'] = (int)$value;
      elseif ($header == 'Content-Type')
        $this->response->headers['type'] = $value;
      elseif ($header == 'ETag')
        $this->response->headers['hash'] = $value{0} == '"' ? substr($value, 1, -1) : $value;
      elseif (preg_match('/^x-amz-meta-.*$/', $header))
        $this->response->headers[$header] = is_numeric($value) ? (int)$value : $value;
    }
    return $strlen;
  }

}

class remoteFile {
  var $url,
      $transfer_id,
      $destinationPath,
      $localPath;

  function download() {
    $fp = fopen($this->destinationPath, 'w'); 
    if(!$fp) {
      $log_entry = array(
        'tid' => $this->transfer_id,
        'message' => 'Download failed due to inability to open file '.$this->destinationPath.' for writing. Please check your file system permissions.',
        'message_data' => array(
          'tid' => $this->transfer_id,
          'destinationPath' => $this->destinationPath,
          'localPath' => $this->localPath,
        ),
        'type' => 1,
      );
      internet_archive_log($log_entry);
  
      return FALSE;
    }
   
    $ch = curl_init($this->url);
    curl_setopt($ch, CURLOPT_FILE, $fp);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    
    if (curl_exec($ch))
      $this->response->code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    else
      $this->response->error = array(
        'code' => curl_errno($ch),
        'message' => curl_error($ch),
        'resource' => $this->resource
      );
    
    curl_close($ch);
    fclose($fp);

    $this->localPath = $this->destinationPath;
    return TRUE;
  }
}
