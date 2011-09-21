-- Anub'Arak
DELETE FROM script_texts WHERE entry BETWEEN -1649074 AND -1649071;
INSERT INTO `script_texts` VALUES ('-1649071', '%s burrows into the ground!', null, null, null, null, null, null, null, null, '0', '3', '0', '0', 'anubarak EMOTE_BURROW');
INSERT INTO `script_texts` VALUES ('-1649072', '%s\'s spikes pursue $N!', null, null, null, null, null, null, null, null, '0', '3', '0', '0', 'anubarak EMOTE_PURSUE');
INSERT INTO `script_texts` VALUES ('-1649073', '%s emerges from the ground!', null, null, null, null, null, null, null, null, '0', '3', '0', '0', 'anubarak EMOTE_EMERGE');
INSERT INTO `script_texts` VALUES ('-1649074', '%s unleashes a Leeching Swarm to heal himself!', null, null, null, null, null, null, null, null, '0', '3', '0', '0', 'anubarak EMOTE_LEECHING_SWARM');

-- The Lich King
UPDATE `script_texts` SET emote='11' WHERE entry='-1649036';

-- Wilfred Fizzlebang Emote bow
UPDATE `script_texts` SET emote='2' WHERE entry='-1649010';
