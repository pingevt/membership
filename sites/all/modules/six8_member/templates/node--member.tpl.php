<pre>
<?php
//print_r($node);
//print_r($content);
?>
</pre>

<div class="image" style="float: left">
  <?php print render($content['field_mem_image']); ?>
</div>

<div id="profile" style="padding-left: 240px">
  <h3><?php print render($content['field_name']); ?></h3>
  <?php print render($content['field_birthdate']); ?>
  <?php print render($content['field_emails']); ?>
  <?php print render($content['field_phone_numbers']); ?>
  <?php print render($content['field_addresses']); ?>
  <?php print render($content['field_mem_spouse']); ?>
  <?php print render($content['field_mem_children']); ?>
  <?php print render($content['field_volunteer_groups']); ?>
  <?php print render($content['field_mem_cg']); ?>
  
  <?php print render($content); ?>
</div>