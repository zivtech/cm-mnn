<?php

/**
 * @file
 * Feeds parser class for MediaRSS
 */

/**
 * Class definition for MediaRSS Parser.
 *
 * Parses RSS or Atom feeds returned from YouTube API like.
 */
class FeedsMediaRSSParser extends FeedsParser {

  /**
   * Parse the extra mapping sources provided by this parser.
   *
   * @param $fetcher_result FeedsFetcherResult
   * @param $source FeedsSource
   *
   * @see FeedsParser::parse()
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    $mediarss_feed = $fetcher_result->getRaw();
    
    $result = new FeedsParserResult();

    /**
     * @see common_syndication_parser_parse()
     */
    if (!defined('LIBXML_VERSION') || (version_compare(phpversion(), '5.1.0', '<'))) {
      @$sxml = simplexml_load_string($mediarss_feed, NULL);
    }
    else {
      @$sxml = simplexml_load_string($mediarss_feed, NULL, LIBXML_NOERROR | LIBXML_NOWARNING | LIBXML_NOCDATA);
    }

    // Got a malformed XML.
    if ($sxml === FALSE || is_null($sxml)) {
      throw new Exception(t('FeedsMediaRSSParser: Malformed XML source.'));
    }

    // Run parsing if the feed is Atom or RSS
    if ($this->isAtomFeed($sxml)) {
      $result = $this->parseAtom($sxml, $source, $fetcher_result);
			
    }
    elseif ($this->isRssFeed($sxml)) {
      $result = $this->parseRss20($sxml, $source, $fetcher_result);
			
    }
    else {
      throw new Exception(t('FeedsMediaRSSParser: Unknown type of feed.'));
    }
    return $result;
  }

  /**
   * Check if given feed object is an Atom feed.
   *
   * @param SimpleXMLElement $sxml
   *
   * @return boolen
   *   TRUE if given SimpleXML object is Atom feed or FALSE
   */
  protected function isAtomFeed(SimpleXMLElement $sxml) {
    return $sxml->getName() == 'feed';
  }

  /**
   * Check if given feed object is a RSS feed.
   *
   * @param SimpleXMLElement $sxml
   *
   * @return boolen
   *   TRUE if given SimpleXML object is RSS feed or FALSE
   */
  protected function isRssFeed(SimpleXMLElement $sxml) {
    return $sxml->getName() == 'rss';
  }

  /**
   * Add the extra mapping sources provided by this parser.
   */
  public function getMappingSources() {
    return parent::getMappingSources() + array(
      'guid' => array(
        'name' => t('GUID'),
      ),
      'link' => array(
        'name' => t('Link'),
      ),
      'title' => array(
        'name' => t('Title'),
        'description' => t('Title.'),
      ),
      'author' => array(
        'name' => t('Author'),
        'description' => t('Author or uploader of the video.'),
      ),
      'published' => array(
        'name' => t('Published'),
      ),
      'description' => array(
        'name' => t('Description'),
      ),
      'tags' => array(
        'name' => t('Tags'),
        'description' => t('This can be imported directly with Taxonomy "tags" vocabularies.'),
      ),
      
      
      // http://www.rssboard.org/media-rss
      'media_content' => array(
        'name' => t('Media Content'),
      ),
      
      //http://www.rssboard.org/media-rss#media-content
      // url="http://www.foo.com/movie.mov"
      // fileSize="12216320"
      // type="video/quicktime"
      // medium="video"
      // isDefault="true"
      // expression="full"
      // bitrate="128"
      // framerate="25"
      // samplingrate="44.1"
      // channels="2"
      // duration="185"
      // height="200"
      // width="300"
      // lang="en"
      'media_content_url' => array(
        'name' => t('Media Content: URL'),
      ), 
      'media_content_filesize' => array(
        'name' => t('Media Content: Filesize'),
      ),
      'media_content_type' => array(
        'name' => t('Media Content: Type'),
      ),    
      'media_content_medium' => array(
        'name' => t('Media Content: Media'),
      ),  
      'media_content_isdefult' => array(
        'name' => t('Media Content: is Default'),
      ),     
      'media_content_expression' => array(
        'name' => t('Media Content: Expression'),
      ),      
      'media_content_bitrate' => array(
        'name' => t('Media Content: Bitrate'),
      ),   
      'media_content_framerate' => array(
        'name' => t('Media Content: Framerate'),
      ),   
      'media_content_samplingrate' => array(
        'name' => t('Media Content: Sampling Rate'),
      ),
      'media_content_channels' => array(
        'name' => t('Media Content: Channels'),
      ),
      'media_content_duration' => array(
        'name' => t('Media Content: Duration'),
      ),
      'media_content_height' => array(
        'name' => t('Media Content: Height'),
      ),
      'media_content_width' => array(
        'name' => t('Media Content: Width'),
      ),
      'media_content_lang' => array(
        'name' => t('Media Content: Language'),
      ),
      
      'media_adult' => array(  //5.1
        'name' => t('Media Adult'),
      ),
      'media_rating' => array(  //5.2
        'name' => t('Media Rating'),
      ),
      'media_title' => array(  //5.3
        'name' => t('Media Title'),
      ),
      'media_description' => array(  //5.4
        'name' => t('Media Description'),
      ),
      'media_keywords_csv' => array(  //5.5
        'name' => t('Media Keywords (comma separated)'),
      ),
      'media_thumbnail_url' => array(  //5.6
        'name' => t('Media Thumbnail (URL)'),
      ),
      //music/artist/album/song
      'media_categories_csv' => array(  //5.7
        'name' => t('Media Categories (comma separated)'),
      ),
      'media_hash' => array(  //5.8
        'name' => t('Media Hash'),
      ),
      'media_player_url' => array(  //5.9
        'name' => t('Media Player (URL)'),
      ),
      'media_credit' => array(  //5.10
        'name' => t('Media Credit'),
      ),
      'media_copyright' => array(  //5.11
        'name' => t('Media Copyright'),
      ),
      //Allows the inclusion of a text transcript
      'media_text' => array(  //5.12
        'name' => t('Media Text'),
      ),
      'media_restriction' => array(  //5.13
        'name' => t('Media Restriction'),
      ),
      //starRating, statistics, tags
      'media_community' => array(  //5.14
        'name' => t('Media Community'),
      ),
      'media_comments_csv' => array(  //5.15
        'name' => t('Media Comments (comma separated)'),
      ),
      'media_embed_html' => array(  //5.16
        'name' => t('Media Embed (HTML)'),
      ),
      'media_responses_csv' => array(  //5.17
        'name' => t('Media Responses (comma separated)'),
      ),
      'media_backlinks_csv' => array(  //5.18
        'name' => t('Media BackLinks (comma separated)'),
      ),
      'media_status' => array(  //5.19
        'name' => t('Media Status'),
      ),
      'media_price' => array(  //5.20
        'name' => t('Media Price'),
      ),
      'media_license_text' => array(  //5.21
        'name' => t('Media License (text)'),
      ),
      'media_license_url' => array(  //5.21
        'name' => t('Media License (URL)'),
      ),
      'media_subtitle_url' => array(  //5.22
        'name' => t('Media subTitle (URL)'),
      ),
      'media_peerlink_url' => array(  //5.23
        'name' => t('Media peerLink (URL)'),
      ),
      'media_rights_csv' => array(  //5.24
        'name' => t('Media Rights (comma separated)'),
      ),
      'media_scenes_csv' => array(  //5.25
        'name' => t('Media Scenes (comma separated)'),
      ),
    );
  }

 /**
   * Parse Atom feed
   *
   * @param SimpleXMLElement $sxml
   * @param FeedsFetcherResult $fetcher_result
   * @param FeedsSource $source
   */
  private function parseAtom(SimpleXMLElement $sxml, FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    // XML was parsed successfully, so we can begin to process items
    $result = new FeedsParserResult();
    $fetcher_result->title = (string) $sxml->channel->title;
    $fetcher_result->description = (string) $sxml->channel->description;
    $fetcher_result->link = (string) $sxml->channel->link;
    $feed_title = (string) $sxml->channel->title;
    
    foreach ($sxml->entry as $entry) {
 
      // Get nodes in media: namespace for media information
      $media = $entry->children('http://search.yahoo.com/mrss/');
      
      // Get video player URL
      $attrs = $media->group->player->attributes();
      $player = (string) $attrs['url'];

      // Get video thumbnail
      $attrs = $media->group->thumbnail[0]->attributes();
      $thumbnail = (string) $attrs['url'];
      
      $item = array(
        'guid' => (string) $entry->guid,
        'link' => (string) $entry->link,
        'title' => html_entity_decode((string) $entry->title),
        'author' => (string) $entry->author,
        'description' => html_entity_decode((string) $entry->description),
        'tags' => explode(',', (string) $media->group->keywords),
        'published' => strtotime($entry->pubDate),
        
        // http://www.rssboard.org/media-rss
        'media_content' => NULL,     
        'media_adult' => NULL,
        'media_rating' => NULL,
        'media_title' => NULL,  
        'media_description' => NULL,
        'media_keywords_csv' => NULL,
        'media_thumbnail_url' => $thumbnail,
        'media_categories_csv' => NULL,
        'media_hash' => NULL,
        'media_player_url' => $player,
        'media_credit' => NULL,
        'media_copyright' => NULL,
        'media_text' => NULL,      
        'media_restriction' => NULL,
        'media_community' => NULL,     
        'media_comments_csv' => NULL,     
        'media_embed_html' => NULL,
        'media_responses_csv' => NULL,     
        'media_backlinks_csv' => NULL,    
        'media_status' => NULL, 
        'media_price' => NULL,  
        'media_license' =>  NULL,  
        'media_subtitle_url' => NULL,   
        'media_peerlink_url' => NULL, 
        'media_rights_csv' => NULL,    
        'media_scenes_csv' => NULL,
        
      );

      // Populate the FeedsFetcherResult object with the parsed results.
      $result->items[] = $item;
    }
   
    return $result;
  }
   
  /**
   * Parse RSS 2.0 feed
   *
   * @param SimpleXMLElement $sxml
   * @param FeedsFetcherResult $fetcher_result
   * @param FeedsSource $source
   */
  private function parseRss20(SimpleXMLElement $sxml, FeedsSource $source, FeedsFetcherResult $fetcher_result) {
  
    // XML was parsed successfully, so we can begin to process items
    $result = new FeedsParserResult();
    $fetcher_result->title = (string) $sxml->channel->title;
    $fetcher_result->description = (string) $sxml->channel->description;
    $fetcher_result->link = (string) $sxml->channel->link;
    $feed_title = (string) $sxml->channel->title;

    
    foreach ($sxml->xpath('//item') as $entry) {

      // Get nodes in media: namespace for media information
      // Some cms (like wp) define namespace without trailing slash, therefore
      // if http://search.yahoo.com/mrss/ doesn't have result, try
      // http://search.yahoo.com/mrss  
      $media = $entry->children('http://search.yahoo.com/mrss/') ? $entry->children('http://search.yahoo.com/mrss/') : $entry->children('http://search.yahoo.com/mrss');
      // First level child items require @count instead of isset... see #2118461 
      
      // Get video player URL

      // check to see if player and thumbnail are inside of content
      // not sure if that follows the mrss spec, but Vimeo does it.
      if (isset($media->content->player)) {
        $player = $media->content->player->attributes();
      }
      elseif (@count($media->player)) {
        $player = $media->player->attributes();
      }

      // Get video thumbnail

      // check to see if player and thumbnail are inside of content
      // not sure if that follows the mrss spec, but Vimeo does it.
      if (isset($media->content->thumbnail)) {
        $thumbnail = $media->content->thumbnail->attributes();
      }
      elseif (@count($media->thumbnail)) {
        $thumbnail = $media->thumbnail[0]->attributes();
      }
      
      // Get license
      
      if (isset($media->content->license)) {
        $license = $media->content->licence->attributes();
      }
            
      if (@count($media->license)) {
        $license = $media->license[0]->attributes();
      }

      //get media:content elements from media:group
      if (isset($media->content->group)) {
        $group = $media->content->group;
      }

      elseif (@count($media->group)) {
        $group = $media->group;
      }
      else {
        $group = $media;
      }

      $content_attrs = $group->attributes();

      $item = array(
        'guid' => (string) $entry->guid,
        'link' => (string) $entry->link,
        'title' => html_entity_decode((string) $entry->title),
        'author' => (string) $entry->author,
        'description' => html_entity_decode((string) $entry->description),
        'tags' => explode(',', (string) $media->group->keywords),
        'published' => strtotime($entry->pubDate),
        
        // http://www.rssboard.org/media-rss
        'media_content' => (string) $group->content[0],
        'media_content_url' => (string) $content_attrs['url'],
        'media_content_filesize' => (string) $content_attrs['filesize'],
        'media_content_type' => (string) $content_attrs['type'],
        'media_content_medium' => (string) $content_attrs['medium'],
        'media_content_isdefault' => (string) $content_attrs['isdefault'],
        'media_content_expression' => (string) $content_attrs['expression'],
        'media_content_bitrate' => (string) $content_attrs['bitrate'],
        'media_content_framerate' => (string) $content_attrs['framerate'],
        'media_content_samplingrate' => (string) $content_attrs['samplingrate'],
        'media_content_channels' => (string) $content_attrs['channels'],
        'media_content_duration' => (string) $content_attrs['duration'],
        'media_content_height' => (string) $content_attrs['height'],
        'media_content_width' => (string) $content_attrs['width'],
        'media_content_lang' => (string) $content_attrs['lang'],
        'media_adult' => NULL,
        'media_rating' => NULL,
        'media_title' => NULL,  
        'media_description' => NULL,
        'media_keywords_csv' => NULL,
        'media_thumbnail_url' => isset($thumbnail) ? (string) $thumbnail['url'] : NULL,
        'media_categories_csv' => NULL,
        'media_hash' => NULL,
        'media_player_url' => isset($player) ? (string) $player['url'] : NULL,
        'media_credit' => NULL,
        'media_copyright' => NULL,
        'media_text' => NULL,      
        'media_restriction' => NULL,
        'media_community' => NULL,     
        'media_comments_csv' => NULL,     
        'media_embed_html' => NULL,
        'media_responses_csv' => NULL,     
        'media_backlinks_csv' => NULL,    
        'media_status' => NULL, 
        'media_price' => NULL,  
        'media_license_text' =>  isset($license) ? (string) $license : NULL,
        'media_license_url' => isset($license) ? (string) $license['url'] : NULL,
        'media_subtitle_url' => NULL,   
        'media_peerlink_url' => NULL, 
        'media_rights_csv' => NULL,    
        'media_scenes_csv' => NULL,
        
      );

      // Populate the FeedsFetcherResult object with the parsed results.
      $result->items[] = $item;
    }
   
    return $result;
  }
    
  /**
   *  Display seconds as HH:MM:SS, with leading 0's.
   *
   *  @param $seconds
   *    The number of seconds to display.
   */
  public function secsToTime($seconds) {
    // Number of seconds in an hour.
    $unith = 3600;
    // Number of seconds in a minute.
    $unitm = 60;

    // '/' given value by num sec in hour... output = HOURS
    $hh = intval($seconds / $unith);

    // Multiply number of hours by seconds, then subtract from given value.
    // Output = REMAINING seconds.
    $ss_remaining = ($seconds - ($hh * 3600));

    // Take remaining seconds and divide by seconds in a min... output = MINS.
    $mm = intval($ss_remaining / $unitm);
    // Multiply number of mins by seconds, then subtract from remaining seconds.
    // Output = REMAINING seconds.
    $ss = ($ss_remaining - ($mm * 60));

    $output = '';

    // If we have any hours, then prepend that to our output.
    if ($hh) {
      $output .= "$hh:";
    }

    // Create a safe-for-output MM:SS.
    $output .= sprintf($hh ? "%02d:%02d" : "%d:%02d", $mm, $ss);

    return $output;
  }
}


