------------------------------------------------------------------------------
  User Badges Module for Drupal
    developed by: Jeff Robbins | jjeff | robbins (att) jjeff (d0t) com
------------------------------------------------------------------------------

Dependencies
============
- upload

Current maintainer: Heine Deelstra (http://drupal.org/user/17943)

The User Badges module allows each user to be assigned 'badges', displayed as
a series of iconic images. A common use will probably be to display the badges
along with the user's information within forums, comments, or node postings.
These badges can be assigned indivicually by the administrator or attached to
a role so that, for example, all users in the 'admin' role will show the
'Administrator' badge which could display as any graphic.

User Badges can be used as a way to establish trust (in the same way as eBay's
star graphics) or as an incentive for users. They can also be a quick way to
identify moderators, administrators, or anyone with a given role.

Use:

Once the module is activated, select 'user badges' from the 'settings' section 
of 'administer'. Here you'll find three tabbed sections for User Badges. 
'edit user badges' allows you to add and edit badges.
The 'image' tab is where you can upload or delete images to associate with each
badge. And the 'roles' tab is where you can associate a badge with a given role
so that all users with that role will display the selected badge.

Users with the 'manage badges' access permission will be shown a 'badges' tab
when visiting user profiles. This is the place to manually assign badges to 
users.

To display user badges in your theme use :

<?php print user_badges_for_uid($uid); ?>