-- Jaraxxus
-- Felflame infernal
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=34815;
DELETE FROM `creature_ai_scripts` WHERE `id`=3481501;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3481501,34815,0,0,100,31,7000,10000,8000,10000,11,67047,0,0,0,0,0,0,0,0,0,0, 'Felflame infernal - cast Fel Inferno');
DELETE FROM `creature_ai_scripts` WHERE `id`=3481502;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3481502,34815,0,0,100,31,7000,10000,8000,10000,11,66519,4,0,0,0,0,0,0,0,0,0, 'Felflame infernal - cast Fel Steak');
-- Mistress of pain
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=34826;
DELETE FROM `creature_ai_scripts` WHERE `id`=3482601;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3482601,34826,0,0,100,31,7000,10000,5000,8000,11,66378,0,0,0,0,0,0,0,0,0,0, 'Mistress of pain - cast Shivan slash');
DELETE FROM `creature_ai_scripts` WHERE `id`=3482602;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3482602,34826,0,0,100,31,7000,10000,5000,8000,11,66283,4,0,0,0,0,0,0,0,0,0, 'Mistress of pain - cast Spinning pain spike');
DELETE FROM `creature_ai_scripts` WHERE `id`=3482603;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3482603,34826,0,0,100,25,13000,15000,12000,16000,11,66425,0,0,0,0,0,0,0,0,0,0, 'Mistress of pain - cast Mistress Kiss');
-- Legion flame
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=34784;
DELETE FROM `creature_ai_scripts` WHERE `id`=3478401;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3478401,34784,11,0,100,30,0,0,0,0,11,66201,0,0,0,0,0,0,0,0,0,0, 'Legion Flame - cast Legion Flame');

-- Beasts
-- Firebomb
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=34854;
DELETE FROM `creature_ai_scripts` WHERE `id`=3485401;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3485401,34854,11,0,100,30,0,0,0,0,11,66318,0,0,0,0,0,0,0,0,0,0, 'Firebomb - cast Firebomb aura');
-- Snobold
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=34800;
DELETE FROM `creature_ai_scripts` WHERE `id`=3480001;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3480001,34800,0,0,100,31,3000,5000,4000,6000,11,66408,1,0,0,0,0,0,0,0,0,0, 'Snobold vassal - cast Batter');
DELETE FROM `creature_ai_scripts` WHERE `id`=3480002;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3480002,34800,0,0,100,31,5000,8000,7000,9000,11,66407,1,0,0,0,0,0,0,0,0,0, 'Snobold vassal - cast Headcrack');
DELETE FROM `creature_ai_scripts` WHERE `id`=3480003;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3480003,34800,0,0,100,31,10000,15000,10000,15000,11,66313,0,0,0,0,0,0,0,0,0,0, 'Snobold vassal - cast Firebomb');
