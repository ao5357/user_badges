<?php

/**
 * @file
 * This defines views hooks for the User_badges module. It will be loaded automatically as needed by the Views module.
 *
 * @author Heine Deelstra (Heine), http://drupal.org/user/17943
 * @author Richard Skinner (Likeless), http://drupal.org/user/310635
 *
 */

/**
 * Implements hook_views_handlers().
 */
function user_badges_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'user_badges') .'/views',
    ),
    'handlers' => array(
      'views_handler_field_user_badges_badges_badge' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_user_badges_user_uid' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_filter_user_badges_user_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function user_badges_views_data() {
  $data['user_badges_badges']['table']['group'] = t('User Badges');
  $data['user_badges_user']['table']['group']  = t('User Badges');

  // Fields
  $data['user_badges_badges']['bid'] = array(
    'title' => t('Bid'),
    'help' => t('The badge ID of the badge.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'Badge ID',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['user_badges_badges']['name'] = array(
    'title' => t('Badge Name'),
    'help' => t('The name of the user badge.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Badge Name',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['user_badges_badges']['image'] = array(
    'title' => t('Image URL'),
    'help' => t('The badge images URL.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Image URL',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['user_badges_badges']['href'] = array(
    'title' => t('Badge info page'),
    'help' => t('The link URL for the badge.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Badge info page',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['user_badges_badges']['weight'] = array(
    'title' => t('Badge Weight'),
    'help' => t('The weight of the badge, as defined on the user badges settings page.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['user_badges_badges']['tid'] = array(
    'title' => t('Associated term'),
    'help' => t('An individual taxonomy term tied to a badge.'),
    'field' => array(
      'handler' => 'views_handler_field_taxonomy',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_term_node_tid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_term_node_tid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['user_badges_user']['ubid'] = array(
    'title' => t('ubid'),
    'help' => t('The ID of the user badge.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'User Badge ID',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['user_badges_user']['earned'] = array(
    'title' => t('Date/Time earned'),
    'help' => t('A timestamp when the user badge was earned.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'name field' => 'Earned Timestamp',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['user_badges_user']['type'] = array(
    'title' => t('Type'),
    'help' => t('Whether the badge was set as part of the role, or individually assigned.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_badges_user_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Joins
  $data['user_badges_user']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );
  $data['user_badges_badges']['table']['join']['users'] = array(
    'left_table' => 'user_badges_user',
    'left_field' => 'bid',
    'field' => 'bid',
  );
  $data['user_badges_badges']['table']['join']['term_data'] = array(
    'left_field' => 'tid',
    'field' => 'tid',
  );

  // Handled fields
  $data['user_badges_badges']['badge'] = array(
    'title' => t('Badge'),
    'real field' => 'bid',
    'help' => t('An individual badge.'),
    'field' => array(
      'handler' => 'views_handler_field_user_badges_badges_badge',
    ),
  );

  $data['users']['user_badges_html'] = array(
    'real field' => 'uid',
    'title' => t('Grouped User Badges'),
    'help' => t("All the user's user badges."),
    'group' => t('User Badges'),
    'field' => array(
      'handler' => 'views_handler_field_user_badges_user_uid',
    ),
  );

  return $data;
}
