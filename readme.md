#User Badges Fork
This is a fork of the drupal [user_badges](http://drupal.org/project/user_badges) project. It adds a number of notable features and cleans up the API of the 6.x branch a bit.

##Distinguishing Features

 * Ability for a user to earn the same badge more than once
 * CCK Field for badges
 * Comprehensive Rules support
 * Tons of sane Views support
 * More tokens
 * Badges keep track of when they're earned
 * Stats module to keep track of total badges earned and weekly badges earned

##Why a Fork?
All of this functionality was needed for a Drupal 6 site. In order to get it into the official 6.x release, it should really be implemented in 7.x and backported. Also, the API changes might mean a minor version change for the module, which seems weird to do with a long-term maintenance release. On top of all this, the module appears to be falling out of regular maintenance.