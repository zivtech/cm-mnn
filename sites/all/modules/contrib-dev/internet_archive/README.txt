This is the internet archive module hosted as part of the Community Media Advanced (CMA) Gitub Project. For more info about CMA go see http://cmadvanced.org/

To see the CMA Internet Archive repository go to https://github.com/cm-advanced/internet_archive.

This module allows drupal fields to be linked to Archive.org Bucket Items. Video can be uploaded from the local server or a remote server. Once uploaded, thummbnails and embedded video can be displayed in views using the provided custom view field formatters. Cron tasks and admin tools allow files to be "harvested" and added to the upload queue. Subsequent cron jobs will pass each file upload through its various workflow statuses until its validated, i.e. successfully uploaded and synched. This harvesting can happen on a local or remote server. Only one remote serverlocation can be used per site.

This module originates on Drupal.org. The last working version was for Drupal Version 6. You can see the original project here https://www.drupal.org/project/internet_archive.

Please note: taking advantage of advanced features of this module --like custom metadata and harvesting files for a queue via cron both locally and remotely -- sometimes require creation of new views or even sometimes code changes. Code changes will be done by changing code in an included template module -- internet/archive/modules/custom_internet_archive -- or creating a new  new custom module using the internet_archive/modules/custom_internet_archive module as a template/example. 

Please note #2: This module has only been tested on fields on nodes. Fields on other entities may or may not work. Also, this module has not been tested with the media module. Having both modules turned on could potentially cause issues, or not. Just beware.

---------------
INSTALLATION
---------------
1. Make sure you have CURL installed on your server
2. Install the internet archive module directory under sites/all/modules or 
   sites/yoursite/modules
3. Enable the module via drush or via the module admin page

---------------
BASIC CONFIGURATION
---------------
1. If you do not already have an account on archive.org, create one
2. If you do not already have an S3 key on archive.org, visit 
   http://www.archive.org/account/s3.php while logged in.
3. Choose a unique default bucket item name to test your connection. When saved, a machine friendly version of the bucket item title will be created by swapping all non alphanumeric characters with underscores. This machine friendly version must be unique on Archive.org. You can check if the machine friendly bucket item title is available by entering the following URL with the "my_bucket_item_name" part replaced by your desired bucket item name
    http://www.archive.org/details/my_bucket_item_name    
4. Go to the config page, <your site>/admin/config/internet_archive/default   
6. Enter your S3 Access & Secret keys from step #2
7. Enter your "Default Archive.org Bucket Item" name form step #3
8. Save the configuration, you should get a "tested successfully" message, if 
   not please check that your account credentials are correct. If the problem 
   persists, check the "Enable Debug Mode" box on the settings form and then 
   check the watchdog for any obvious issues / where applicable report issues 
   to the issue queue at https://github.com/cm-advanced/internet_archive/issues


---------------
USING WITH A CONTENT TYPE / FIELD
---------------
1. Decide what field(s) you would like to push files from. Either add a new filefield or a textfield to your content type or choose an already exiting field. If using a textfield for a locally hosted file, it must contain a local path to the files you wish to send to archive.org, example: sites/default/files/botp-cc.mov. Remote files will rely on remote configurations.
2. Go to <your site>/admin/config/internet_archive/default   
3. Under "Field Integration" select the fields you wish to be able to integrate 
   with archive.org.
4. To test, visit a node containing one of the fields you enabled -- you should 
   now see a "Transfer to Archive.org" option beneath the file path listing 
   (if you have custom theming or have removed the field from display this 
   option will not be available. For testing purposes I suggest you change the 
   theme on your admin account temporarily to garland / enable display of the 
   field). Clicking this link will send the associated file to your default item 
   on archive.org (unless you have implemented hook_metadata or checked 
   "Use node title for item name", in which case it will go to a new item).
5. If you are storing video files, you can configure your field to display as
   an archive.org video embed in CCK/views


---------------
USING THE AUTOMATED QUEUE
---------------
1. Download/configure/enable http://www.drupal.org/project/drupal_queue 
   following the included README.txt
2. Download/enable http://www.drupal.org/project/views
3. Create a view that includes file path as one of the fields.
4. Visit admin > site configuration > internet archive
5. Click on the Queue Settings tab
6. Select your view/s, add any time configuration settings
7. You can test out your new queue by visiting the queue monitor page at
   admin/settings/internet_archive/queue-monitor
7. By default, drupal_queue will harvest and transfer files on cron run.If you 
   would prefer to separate the transfer process from Drupal's standard cron, 
   set the variable 'drupal_queue_on_cron' to FALSE. You can then choose to 
   either run drupal_queue's cron via the file and instructions included in that 
   module, or using Drush. If Drush is installed, files can be 
   harvested/transferred via the "drush ia-transfer" command. In all cases the 
   module will only queue up unique files based on their origin path to prevent 
   duplicates.


---------------
IMPLEMENTING hook_internet_archive_metadata
---------------
Take a look at the included internet_archive_om_metadata submodule.

---------------
TRANSFERRING FILES STORED ON A REMOTE SERVER
---------------
See the INSTALL.txt file inside of the internet_archive_remote submodule.
