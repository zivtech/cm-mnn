NOTE: THESE INSTRUCTIONS ARE OUT OF DATE -- for the latest documentation please install the advanced help module. After enabling it, documentation links (when available) will appear at the top of the various configuration and management pages.

This is the internet archive module.

---------------
INSTALLATION
---------------
1. Make sure you have CURL installed on your server
2. Install the internet archive module directory under sites/all/modules or 
   sites/yoursite/modules
3. Enable the module at admin > build > modules

---------------
BASIC CONFIGURATION
---------------
1. If you do not already have an account on archive.org, create one
2. If you do not already have an S3 key on archive.org, visit 
   http://www.archive.org/account/s3.php while logged in.
3. Visit the internet archive settings page at: 
   admin > site configuration > internet archive
4. Choose a unique default item name to test your connection with (it can be 
   changed at any time, and can be overridden using the "Use node title for item name" 
   or with a module implementing hook_metadata. Item names must consist only of 
   alphanumeric characters (dashes are ok as well). For testing purposes, it is 
   important that this item be unique -- you can check if an item exists by 
   visiting http://www.archive.org/details/itemnamehere
5. Install the internet archive module directory under sites/all/modules or 
   sites/yoursite/modules
6. Enter your S3 Access & Secret keys from step #2
7. Save the configuration, you should get a "tested successfully" message, if 
   not please check that your account credentials are correct. If the problem 
   persists, check the "Enable Debug Mode" box on the settings form and then 
   check the watchdog for any obvious issues / where applicable report issues 
   to the issue queue at http://drupal.org/project/internet_archive

---------------
USING WITH A CONTENT TYPE / FIELD
---------------
1. Add a filefield, textfield or custom emfield to your content type if not 
   already in place. If using a textfield or emfield, it must contain a local 
   path to the files you wish to send to archive.org, 
   example: sites/default/files/botp-cc.mov, unless you have configured the
   internet_archive_remote module
2. Go to admin > site configuration > internet archive
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
