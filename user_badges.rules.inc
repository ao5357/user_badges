<?php
/**
 * @file
 * Provide integration with Rules module
 */

/**
 * Implementation of hook_rules_data_type_info().
 */
function user_badges_rules_data_type_info() {
  return array(
    'userbadge' => array(
      'label' => t('User Badge'),
      'class' => 'rules_data_type_userbadge',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'uses_input_form' => FALSE,
      'module' => 'User Badges',
    ),
  );
}

/**
 * Defines the rules userbadge data type.
 */
class rules_data_type_userbadge extends rules_data_type {
  function load($ubid) {
    return user_badges_get_user_badge($ubid);
  }

  function get_identifier() {
    $badge = &$this->get();
    return $badge->ubid;
  }
}

/**
 * Implementation of hook_rules_event_info().
 */
function user_badges_rules_event_info() {
  return array(
    'user_badges_badge_given' => array(
      'label' => t('User was given a badge'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('User before receiving badge')),
        'badge' => array('type' => 'userbadge', 'label' => t('The given Badge')),
      ),
      'module' => 'User Badges',
    ),
    'user_badges_badge_removed' => array(
      'label' => t('User was removed a badge'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('User before removing badge')),
        'badge' => array('type' => 'userbadge', 'label' => t('The removed Badge')),
      ),
      'module' => 'User Badges',
    ),
  );
}

/**
 * Implementation of hook_rules_condition_info().
 */
function user_badges_rules_condition_info() {
  return array(
    'user_badges_has_badge' => array(
      'label' => t('User has badge'),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'module' => 'User Badges',
    ),
    'user_badges_receives_badge' => array(
      'label' => t('Badge user is receiving'),
      'arguments' => array(
        'badge' => array('type' => 'userbadge', 'label' => t('The given Badge')),
      ),
      'module' => 'User Badges',
    ),
    'user_badges_badge_count' => array(
      'label' => t('User badge count'),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
        'number' => array('type' => 'number', 'label' => t('Number')),
        'since' => array('type' => 'date', 'label' => t('Date to begin counting')),
      ),
      'module' => 'User Badges',
    ),
    'user_badges_has_badge_by_name' => array(
      'label' => t('User has badge by name'),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
        'badgename' => array('type' => 'string', 'label' => t('Badge Name')),
      ),
      'module' => 'User Badges',
    ),
    'user_badges_node_has_badge_term' => array(
      'label' => t('Content has badge term'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('The Node')),
      ),
      'module' => 'User Badges',
    ),
  );
}

/**
 * Configuration form for user_badges_has_badge condition
 */
function user_badges_has_badge_form($settings = array(), &$form) {
  $form['badges'] = array('#tree' => TRUE);
  $badges = user_badges_get_badges('all');
  $settings['badges'] = is_array($settings['badges']) ? $settings['badges'] : array();

  $form['settings']['operator'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#options' => array(
      'any' => t('Has one of'),
      'all' => t('Has all of'),
    ),
    '#default_value' => $settings['operator'],
  );
  foreach ($badges as $badge) {
    $form['settings']['badges'][$badge->bid] = array(
      '#type' => 'checkbox',
      '#title' => theme('user_badge', $badge),
      '#return_value' => 1,
      '#default_value' => $settings['badges'][$badge->bid],
      '#description' => check_plain($badge->name),
    );
  }
  return $form;
}

/**
 * Test the user_badges_has_badge condition
 */
function user_badges_has_badge($account, $settings) {
  $badges = user_badges_get_user_badges($account->uid, array('nolimit' => TRUE));
  $result = ($settings['operator'] == 'all');
  foreach ($settings['badges'] as $bid => $on) {
    if (!$on) {
      continue;
    }
    $has_badge = _user_badges_bid_in_badges($bid, $badges);
    if ($settings['operator'] == 'all') {
      $result = $result && $has_badge;
    }
    else {
      $result = $result || $has_badge;
    }
  }
  return $result;
}

/**
 * Configuration form for user_badges_receives_badge condition
 */
function user_badges_receives_badge_form($settings = array(), &$form) {
  $form['badges'] = array('#tree' => TRUE);
  $badges = user_badges_get_badges('all');
  $settings['badges'] = is_array($settings['badges']) ? $settings['badges'] : array();

  foreach ($badges as $badge) {
    $form['settings']['badges'][$badge->bid] = array(
      '#type' => 'checkbox',
      '#title' => theme('user_badge', $badge),
      '#return_value' => 1,
      '#default_value' => $settings['badges'][$badge->bid],
      '#description' => check_plain($badge->name),
    );
  }
  return $form;
}

/**
 * Test the user_badges_receives_badge condition
 */
function user_badges_receives_badge($badge, $settings) {
  $badge_bool = false;
  foreach ($settings['badges'] as $bid => $on) {
    if ($bid == $badge->bid && $on == 1) {
      $badge_bool = true;
      break;
    }
  }
  return $badge_bool;
}

/**
 * Configuration form for user_badges_badge_count condition
 */
function user_badges_badge_count_form($settings = array(), &$form) {
  $form['settings']['operation'] = array(
    '#type' => 'select',
    '#title' => t('User\'s number of badges is'),
    '#options' => array('greater' => t('Greater than'), 'equal' => t('Equal to'), 'less' => t('Less than')),
    '#default_value' => isset($settings['operation']) ? $settings['operation'] : 'equal',
    '#weight' => 1,
  );
  $form['settings']['daterange'] = array(
    '#type' => 'select',
    '#title' => t('Count badges before or after the given date'),
    '#description' => t('Leave "Date to begin counting" blank if you don\'t care when the badges were earned.'),
    '#options' => array('after' => t('After'), 'before' => t('Before')),
    '#default_value' => isset($settings['daterange']) ? $settings['daterange'] : 'after',
    '#weight' => 0,
  );
  $form['settings']['number']['#weight'] = 2;
  $form['settings']['since']['#required'] = FALSE;
}

/**
 * Test the user_badges_badge_count condition
 */
function user_badges_badge_count($account, $number, $since, $settings) {
  $badges = user_badges_get_user_badges($account->uid, array('nolimit' => TRUE));
  $badges_count = 0;
  $since = ($since !== '1970-01-01 00:00:00') ? strtotime($since) : 'is_not_numeric';

  if (!is_numeric($since)) {
    $badges_count = count($badges);
  }
  else {
    foreach ($badges as $ubid => $badge) {
      if (($settings['daterange'] == 'after' && $badge->earned > $since) || ($settings['daterange'] == 'before' && $badge->earned < $since)) {
        $badges_count++;
      }
    }
  }

  switch ($settings['operation']) {
    case 'greater':
      return ($badges_count > $number);
    case 'equal':
      return ($badges_count == $number);
    case 'less':
      return ($badges_count < $number);
  }
}

/**
 * Test the user_badges_has_badge_by_name condition
 */
function user_badges_has_badge_by_name($account, $badgename, $settings) {
  $has_badge = FALSE;
  $user_badges = user_badges_get_user_badges($account->uid, array('nolimit' => TRUE));
  if (count($user_badges)) {
    foreach ($user_badges as $badge) {
      if ($badgename == $badge->name) {
        $has_badge = TRUE;
        break;
      }
    }
  }
  return $has_badge;
}

/**
 * Configuration form for user_badges_node_has_badge_term condition
 */
function user_badges_node_has_badge_term_form($settings = array(), &$form) {
  $badges = user_badges_get_badges('all');
  $terms = array();
  $form['settings']['operator'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#options' => array(
      'any' => t('Has one of'),
      'all' => t('Has all of'),
    ),
    '#default_value' => $settings['operator'],
  );
  foreach ($badges as $bid => $badge) {
    if ($badge->tid) {
      $terms[$badge->tid] = taxonomy_get_term($badge->tid);
      $form['settings']['badge-terms'][$badge->tid] = array(
        '#type' => 'checkbox',
        '#title' => $terms[$badge->tid]->name,
        '#return_value' => 1,
        '#default_value' => $settings['badge-terms'][$badge->tid],
        '#description' => t('@term is the associated term for the @badge badge.',array('@badge' => $badge->name, '@term' => $terms[$badge->tid]->name)),
      );
    }
  }
  return $form;
}

/**
 * Test the user_badges_node_has_badge_term condition
 */
function user_badges_node_has_badge_term($node, $settings) {
  $result = ($settings['operator'] == 'all');
  foreach ($settings['badge-terms'] as $tid => $on) {
    if (!$on) {
      continue;
    }
    $has_term = (array_key_exists($tid, $node->taxonomy));
    if ($settings['operator'] == 'all') {
      $result = $result && $has_term;
    }
    else {
      $result = $result || $has_term;
    }
  }
  return $result;
}

/**
 * Implements hook_rules_action_info().
 */
function user_badges_rules_action_info(){
  return array(
    'user_badges_action_add_badge_by_name' => array(
      'label' => t('Add badge by name'),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Acting User')),
      ),
      'module' => 'User',
      'eval input' => array('badge'),
    ),
    'user_badges_action_remove_badge_by_name' => array(
      'label' => t('Remove badge by name'),
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('Acting User')),
      ),
      'module' => 'User',
      'eval input' => array('badge'),
    ),
    'user_badges_action_add_badges_by_node_terms' => array(
      'label' => t('Add badges by associated terms on content'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Created content')),
      ),
      'module' => 'User',
    ),
  );
}

/**
 * Implements a Rules action.
 * Adds a badge to the current user, by the textual name of the badge.
 */
function user_badges_action_add_badge_by_name($account, $settings){
  if (isset($account->uid)) {
    $uid = $account->uid;
  }
  else {
    global $user;
    $uid = $user->uid;
  }

  $badges = user_badges_get_badges('all');
  foreach ($badges as $bid => $badge) {
    if ($settings['badge'] == $badge->name) {
      user_badges_user_add_badge($uid, $bid, 'user');
      break;
    }
  }
}

function user_badges_action_add_badge_by_name_form($settings, &$form){
  $settings += array('badge' => '');
  $form['settings']['badge'] = array(
    '#type' => 'textfield',
    '#title' => t('Badge name'),
    '#default_value' => $settings['badge'],
    '#description' => t('The textual name of the badge as configured on the user badges admin page.'),
  );
}

/**
 * Implements a Rules action.
 * Removes a badge from the current user, by the textual name of the badge.
 */
function user_badges_action_remove_badge_by_name($account, $settings){
  if (isset($account->uid)) {
    $uid = $account->uid;
  }
  else {
    global $user;
    $uid = $user->uid;
  }

  $badges = user_badges_get_user_badges($uid, array('nolimit' => TRUE));
  foreach ($badges as $ubid => $badge) {
    if ($settings['badge'] == $badge->name) {
      user_badges_user_remove_badge($ubid);
      break;
    }
  }
}

function user_badges_action_remove_badge_by_name_form($settings, &$form){
  $settings += array('badge' => '');
  $form['settings']['badge'] = array(
    '#type' => 'textfield',
    '#title' => t('Badge name'),
    '#default_value' => $settings['badge'],
    '#description' => t('The textual name of the badge as configured on the user badges admin page.'),
  );
}

/**
 * Implements a Rules action.
 * Adds a badge to the current user, by the badge-associated terms attached to created content.
 */
function user_badges_action_add_badges_by_node_terms($node, $settings) {
  if (isset($node->uid)) {
    $uid = $node->uid;
  }
  else {
    global $user;
    $uid = $user->uid;
  }

  $badges = user_badges_get_badges('all');
  $vid = variable_get('user_badges_vid', 0);
  if ($vid) {
    foreach ($badges as $bid => $badge) {
      if (array_key_exists($badge->tid, $node->taxonomy)) {
        user_badges_user_add_badge($uid, $bid, 'user');
      }
    }
  }
}
