<?php
/**
 * @file
 * Administration functions for the New Relic monitoring integration module.
 */

/**
 * Page callback for the settings form for the module.
 */
function monitoring_newrelic_settings_form($form, &$form_state) {
  $form = array();

  $form['monitoring_newrelic_license_key'] = array(
    '#type'   => 'textfield',
    '#title'  => t('New Relic license key'),
    '#description'  => t('This is the license key for the account. You can discover this by choosing "account settings" from the drop-down menu at the top right of the New Relic interface.'),
    '#default_value'  => variable_get('monitoring_newrelic_license_key', ''),
  );

  // Add a setting field for each of the available sensors.
  foreach (monitoring_sensor_info_instance() as $sensor_name => $sensor_info) {
    // Skip any settings for sensors that are disabled.
    if (!$sensor_info->isEnabled()) {
      continue;
    }

    // Get existing settings for this sensor and New Relic. Provide default if
    // no existing settings.
    $newrelic_settings = $sensor_info->getSetting('newrelic', array(
      'newrelic_enabled' => FALSE,
    ));

    $form[$sensor_name] = array(
      '#type' => 'fieldset',
      '#title' => $sensor_info->getLabel(),
      '#description' => $sensor_info->getDescription(),
      '#tree' => TRUE,
    );
    $form[$sensor_name]['newrelic_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => $newrelic_settings['newrelic_enabled'],
      '#attributes' => array('id' => $sensor_name . '_enabled'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submission handler for monitoring_newrelic_settings_form().
 */
function monitoring_newrelic_settings_form_submit($form, &$form_state) {
  // Save the license key field.
  $fields = array('license_key');

  foreach ($fields as $field) {
    variable_set(
      'monitoring_newrelic_' . $field,
      $form_state['values']['monitoring_newrelic_' . $field]
    );
  }

  foreach (monitoring_sensor_info_instance() as $sensor_name => $sensor_info) {
    // Save settings for enabled sensors that have values in the submitted form.
    if ($sensor_info->isEnabled() && !empty($form_state['values'][$sensor_name])) {
      // Load the existing settings and just merge the new settings in before
      // saving them back again.
      $settings = monitoring_sensor_settings_get($sensor_name);
      $settings['newrelic'] = $form_state['values'][$sensor_name];
      monitoring_sensor_settings_save($sensor_name, $settings);
    }
  }

}
