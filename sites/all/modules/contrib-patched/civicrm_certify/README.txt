Introduction
------------
CiviCRM Certify

This module automatically assigns contacts to groups in CiviCRM. This certification process happens in two ways: when an event organizer or instructor updates the participant status to the status defined in the certification rule; and by using the sync option in the certification rule to certify all participants of a particular CiviEvent type.

Installation
------------
Once you have activated the module it sets up two entities. The certification rule entity is managed at Administration > Structure > Certification Rules and the certification entity is managed at Administration > Structure > Certifications.


Using CiviCRM Certify (CC)
---------------------

Participant Status Change Method
When a user signs up for a CiviEvent generally they receive a status that is setup by the Administrator. When the event is over an instructor or authorized user will typically change the status for each participant to reflect their actual work completed or attendance in the workshop. This is when CiviCRM Certify (CC) automatically queries for certification rules that are related to the workshop. If any are found they will be checked for a matching participant status. If a match is found, the user will be added to a group and a certification record will be created. The participant status change method is enabled as soon as a certification rule is created. 

Sync Method
The sync method will certify all participants of all workshops having the specified type and participant status. This allows the certification rule to quickly certify all participants regardless of when the workshop took place. 

