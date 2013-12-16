<?php
/**
 * @file
 * Drush functions for the New Relic monitoring integration module.
 */

/**
 * Implements hook_drush_command().
 */
function monitoring_newrelic_drush_command() {
  $items = array();

  $items['monitoring-newrelic'] = array(
    'callback' => 'monitoring_newrelic_drush_callback',
    'options' => array(
      'get-config' => 'Get newrelic plugin configuration options.',
    ),
    'description' => 'Run newrelic plugins.',
    'examples' => array(
      'drush monitoring-newrelic' => 'Runs all sensors and sends results to New Relic.',
    ),
    'drupal dependencies' => array('monitoring_newrelic'),
  );

  return $items;
}

/**
 * Drush callback for the main "monitoring-newrelic" drush command.
 */
function monitoring_newrelic_drush_callback() {
  // We are requested to print munin configuration.
  if (drush_get_option('get-config', FALSE)) {
    $output = '';
    foreach (monitoring_newrelic_config() as $category => $graph) {
      $output .= "multigraph {$category}\n";
      foreach ($graph as $key => $val) {
        $output .= "$key $val\n";
      }
      $output .= "\n";
    }

    drush_print($output);
  }
  // We are requested to print values.
  else {
    drush_print(_monitoring_newrelic_result());
  }
}
