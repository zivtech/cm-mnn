<?php
/**
 * @file
 * Printable contract template.
 */

global $base_path;
$logourl = theme_get_setting('logo_path', '');
?>
  <html>
    <head>
      <title>Contract</title>
      <link type="text/css" rel="stylesheet" href="<?php print $base_path ?><?php print drupal_get_path('module', 'reservations_printable_contract'); ?>/contract.css" />
    </head>
    <body>
      <div id="page">
        <div id="header">
        <?php if ($logourl) { ?>
           <img src="<?php print $base_path ?><?php print $logourl ?>">
        <?php } ?>
        <h2><?php print variable_get('site_name', ''); ?> Equipment Rental Contract</h2>
        <?php if (module_exists('token')) {
          print token_replace(variable_get('reservations_contract_header', ''), array('node' => $node));
        }
        else {
          print variable_get('reservations_contract_header','');
        }
        ?>
        <strong>From:</strong>  <?php print $start_date . '<br />'; ?>
        <strong>To:</strong>  <?php print $end_date . '<br />'; ?>
        <strong>Name:</strong>  <?php print $username ?><br />
        <strong>Email:</strong>  <?php print $email ?><br />
        <strong>Phone:</strong>  <?php print $phone ?><br />

     </div>
        <table id="cost">
          <thead>
            <tr>
              <th>Item</th>
            </tr>
          </thead>
          <tbody>
          <?php

  $even_odd = 'even';

  foreach ($items as $item) {

    $item_node = node_load($item['reservations_placeholder_nid']);

    //$type            = reservations_load_content_type_settings($item->type);
    //$type            = reservations_load_item_settings($item, $item->type);
    
    if ($item['item_title']) {
      $ttitle = htmlspecialchars($item['item_title']);
    }
    else {
      $ttitle = '<b>SPECIFIC ITEM NOT SELECTED FROM BUCKET</b>';
    }
    if ($item['reservations_item_nid']) {
      $real_item_node = node_load($item['reservations_item_nid']);
      dsm($real_item_node);
      $inv_number = $real_item_node->field_inventory_number[LANGUAGE_NONE][0]['value'];
      $accessories = $real_item_node->field_accessories[LANGUAGE_NONE][0]['value'];
      
    }
    else {
      $inv_number = "";
      $accessories = "";
    }

    ?>
            <tr class="<?php print $even_odd; ?>">
              <td>
                <div><?php print $ttitle."<br>".$inv_number; ?></div>
                <?php
    if (count($item_node->taxonomy) > 0) {

      ?>
                  <ul class="accessories">
                  <?php
      foreach ($item_node->taxonomy as $accessory) {

        ?>
                    <li><?php print $accessory->name; ?></li>
                    <?php
      }
      // foreach

      ?>
                  </ul>
                  <?php
    }
    // if

    ?>
              </td>
           
            </tr>
            <?php
    $even_odd = ($even_odd == 'even') ? 'odd' : 'even';
  }
  // foreach

  ?>
          </tbody>
         
        </table>
        <div id="boilerplate"><?php module_exists('token') ? print(token_replace(variable_get('reservations_contract_boilerplate', ''), array('node' => $node))) : print(variable_get('reservations_contract_boilerplate','')) ?></div>
        <div id="footer"><?php module_exists('token') ? print(token_replace(variable_get('reservations_contract_footer', ''), array('node' => $node))) : print(variable_get('reservations_contract_footer','')) ?></div>
      </div>
    </body>
