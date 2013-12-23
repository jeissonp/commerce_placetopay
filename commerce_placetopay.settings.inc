<?php
function commerce_placetopay_settings($form, &$form_state) {
  $url = url('commerce_placetopay/cron/' . commerce_placetopay_get_token(), array('absolute' => TRUE));

  $form['url'] = array(
    '#type' => 'item',
    '#title' => t('Url Cron'),
    '#markup' => l($url, $url),
    '#description' => t('To run cron'),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Regenerate Token'),
  );
  
  return $form;
}

function commerce_placetopay_settings_submit($form, &$form_state) {
  $cron_key = drupal_hash_base64(drupal_random_bytes(55));
  variable_set('commerce_placetopay_cron_key', $cron_key);
  drupal_set_message(t('Key regenerated'));
}