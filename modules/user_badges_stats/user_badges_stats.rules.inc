<?php
/**
 * @file
 * Provide integration with Rules module
 *
 * @author Brad Czerniak (ao5357), http://drupal.org/user/313975
 */

/**
 * Implementation of hook_rules_event_info().
 */
function user_badges_stats_rules_event_info() {
  return array(
    'user_badges_stats_top_weekly' => array(
      'label' => t('Top badge earner determined at end of week'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('Top earning User')),
        'number' => array('type' => 'number', 'label' => t('Badges earned in week')),
      ),
      'module' => 'User Stats',
    ),
  );
}

/**
 * Implementation of hook_rules_condition_info().
 */
function user_badges_stats_rules_condition_info() {
  return array(
    'user_badges_stats_badges_week' => array(
      'label' => t('User badge count for week'),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
        'number' => array('type' => 'number', 'label' => t('Number')),
      ),
      'module' => 'User Badges',
    ),
  );
}

/**
 * Configuration form for user_badges_stats_badges_week condition
 */
function user_badges_stats_badges_week_form($settings = array(), &$form) {
  $form['settings']['operation'] = array(
    '#type' => 'select',
    '#title' => t('User\'s number of badges so far this week is'),
    '#options' => array('greater' => t('Greater than'), 'equal' => t('Equal to'), 'less' => t('Less than')),
    '#default_value' => isset($settings['operation']) ? $settings['operation'] : 'equal',
  );
  $form['settings']['number']['#weight'] = 2;
}

/**
 * Test the user_badges_stats_badges_week condition
 */
function user_badges_stats_badge_count($account, $number, $settings) {
  $badges_week = user_badges_stats_get_stats('badges_week', $account->uid);
  switch ($settings['operation']) {
    case 'greater':
      return ($badges_week > $number);
    case 'equal':
      return ($badges_week == $number);
    case 'less':
      return ($badges_week < $number);
  }
}
